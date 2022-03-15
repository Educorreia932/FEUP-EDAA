#include <bits/stdc++.h>
#include <filesystem>
namespace fs = std::filesystem;

#include "rapidxml.hpp"
#include "EdgeType.h"
#include "coord.h"
#include "encoded_string.h"
#include "DWGraph.h"
#include "point.h"

using namespace std;
using namespace rapidxml;

xml_node<> *find_tag(xml_node<> *p, const string &k) {
    xml_node<> *ret;
    for (ret = p->first_node("tag"); ret; ret = ret->next_sibling()) {
        if (k == ret->first_attribute("k")->value()) break;
    }
    return ret;
}

typedef int speed_t;

class way_t : public list<DWGraph::node_t> {
public:
    enum dir_t {
        Front = 1,
        Both = 0,
        Back = -1
    };
private:
    void get_way(xml_node<> *it){
        for(auto j = it->first_node("nd"); string(j->name()) == "nd"; j = j->next_sibling()){
            this->push_back(atoll(j->first_attribute("ref")->value()));
        }
    }

    dir_t get_dir(xml_node<> *it){
        // Oneway
        auto poneway = find_tag(it, "oneway");
        if(poneway != nullptr){
            string oneway = poneway->first_attribute("v")->value();
            if(oneway == "yes") return dir_t::Front;
            if(oneway == "no") return dir_t::Both;
            if(oneway == "reversible") return dir_t::Both;
        }
        // Roundabouts
        auto pjunction = find_tag(it, "junction");
        if(pjunction != nullptr){
            string junction = pjunction->first_attribute("v")->value();
            if(junction == "roundabout" || 
               junction == "circular") return dir_t::Front;
        }
        // Return
        return dir_t::Both;
    }

    speed_t get_speed(xml_node<> *it){
        auto p = find_tag(it, "maxspeed");
        if(!p) return -1;
        speed_t s;
        if(sscanf(p->first_attribute("v")->value(), "%d", &s) != 1) return -1;
        return s;
    }
public:
    dir_t dir;
    speed_t speed;
    edge_type_t edgeType;
    way_t(xml_node<> *it, edge_type_t eType): edgeType(eType) {
        get_way(it);
        dir = get_dir(it);
        speed = get_speed(it);
        if(dir == dir_t::Back){
            reverse();
            dir = dir_t::Front;
        }
    }
    size_t getNumWays() const{
        switch(dir){
            case dir_t::Front: return 1;
            case dir_t::Back: return 1;
            case dir_t::Both: return 2;
            default: throw invalid_argument("");
        }
    }
    coord_t get_mean_coord(const std::unordered_map<DWGraph::node_t, coord_t> &nodes) const{
        coord_t ret(0,0);
        for(auto it = begin(); it != end(); ++it){
            ret = ret + nodes.at(*it);
        }
        return ret/double(size());
    }
};

ostream& operator<<(ostream &os, const way_t &w){
    if(w.dir == way_t::dir_t::Front || w.dir == way_t::dir_t::Both){
        os << char(w.edgeType) << " " << w.speed << " " << w.size();
        for(auto it = w.begin(); it != w.end(); ++it){
            os << "\n" << *it;
        }
    }
    if(w.dir == way_t::dir_t::Back || w.dir == way_t::dir_t::Both){
        os << "\n" << char(w.edgeType) << " " << w.speed << " " << w.size();
        for(auto it = w.rbegin(); it != w.rend(); ++it){
            os << "\n" << *it;
        }
    }
        
    return os;
}

unordered_map<string, edge_type_t> edge_accept = {
    {"motorway"         , edge_type_t::MOTORWAY         },
    {"motorway_link"    , edge_type_t::MOTORWAY_LINK    },
    {"trunk"            , edge_type_t::TRUNK            },
    {"trunk_link"       , edge_type_t::TRUNK_LINK       },
    {"primary"          , edge_type_t::PRIMARY          },
    {"primary_link"     , edge_type_t::PRIMARY_LINK     },
    {"secondary"        , edge_type_t::SECONDARY        },
    {"secondary_link"   , edge_type_t::SECONDARY_LINK   },
    {"tertiary"         , edge_type_t::TERTIARY         },
    {"tertiary_link"    , edge_type_t::TERTIARY_LINK    },
    {"unclassified"     , edge_type_t::UNCLASSIFIED     },
    {"residential"      , edge_type_t::RESIDENTIAL      },
    {"living_street"    , edge_type_t::LIVING_STREET    },
    {"road"             , edge_type_t::UNCLASSIFIED     },
    {"bus_stop"         , edge_type_t::SERVICE          },
    {"track"            , edge_type_t::SERVICE          }
};
unordered_set<string> edge_reject = {
    "steps",        "pedestrian", "footway",   "cycleway",
    "construction", "path",       "bridleway", "platform", "raceway",
    "elevator",     "proposed",   "planned",   "bus_stop", "services"};
unordered_set<string> service_accept = {"driveway", "parking_aisle", "alley"};
unordered_set<string> service_reject = {"campground", "emergency_access",
                                        "drive-through"};
edge_type_t get_edge_type(xml_node<> *it) {
    auto parea = find_tag(it, "area");
    if (parea != nullptr && string(parea->first_attribute("v")->value()) == "yes") return edge_type_t::NO;
    
    /* highway */
    auto phighway = find_tag(it, "highway");{
        if (phighway == nullptr) return edge_type_t::NO;
        string highway = phighway->first_attribute("v")->value();
        if (edge_reject.find(highway) != edge_reject.end()) return edge_type_t::NO;
        if (edge_accept.find(highway) != edge_accept.end()) return edge_accept[highway];
        if (highway == "service"){
            auto paccess  = find_tag(it, "access");
            if(paccess == nullptr) return edge_type_t::SERVICE;
            string access = paccess->first_attribute("v")->value();
            if (access == "private" || access == "no" ) return edge_type_t::NO;
            return edge_type_t::SERVICE;
        }
    }
    

    cerr << it->first_attribute("id")->value() << " rejected "
         << phighway->first_attribute("v")->value() << "\n";
    return edge_type_t::NO;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    // Create destination directory
    {
        string s = string(argv[1]);
        long i = long(s.size()-1) - long(find(s.rbegin(), s.rend(), '/') - s.rbegin());
        if(i >= 0) {
            s = s.substr(0, i);
            if(fs::exists(s)){
                cout << "Directory already exists: " + s + "; proceeding" << endl;
            } else {
                cout << "Creating directory " + s << endl;
                assert(fs::create_directories(s));
            }
        }
    }
    // Check if files already exist
    {
        if(fs::exists(string(argv[1]) + ".nodes")){
            cout << "Files already exist, exiting" << endl;
            return 0;
        }
    }
    //
    char *text = nullptr; {
        string all = "";
        string buf;
        while (getline(cin, buf)) {
            all += buf + "\n";
        }
        text = new char[all.size() + 1];
        strcpy(text, all.c_str());
    }
    xml_document<> doc;
    doc.parse<0>(text);

    unordered_map<long long, coord_t> nodes, nodes_all;

    list<way_t> ways;
    
    {
        for (auto it = doc.first_node()->first_node("way"); string(it->name()) == "way"; it = it->next_sibling()) {
            edge_type_t t = get_edge_type(it);
            if (t == edge_type_t::NO) continue;
            way_t way(it, t); ways.push_back(way);
            for(const DWGraph::node_t &u: way) nodes[u];
        }
    }

    /** Insert missing coordinates in nodes */ {
        for (auto it = doc.first_node()->first_node("node");
          string(it->name()) == "node";
          it = it->next_sibling()) {
            DWGraph::node_t u = atoll(it->first_attribute("id")->value());
            coord_t coord(atof(it->first_attribute("lat")->value()),
                          atof(it->first_attribute("lon")->value()));
            nodes_all[u] = coord;
            if(nodes.count(u)) nodes[u] = coord;
        }
    }

    // Print nodes
    {
        ofstream os;
        os.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            os.open(string(argv[1]) + ".nodes");
        } catch(std::ofstream::failure e) {
            cerr << "ERROR: Failed to open file " << string(argv[1]) + ".nodes" << endl;
            return 1;
        }
        os << nodes.size() << "\n";
        for(const std::pair<DWGraph::node_t, coord_t> &u: nodes) os << u.first << " " << u.second << "\n";
    }
    // Print ways/edges
    {
        ofstream os;
        os.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            os.open(string(argv[1]) + ".edges");
        } catch(std::ofstream::failure e) {
            cerr << "ERROR: Failed to open file " << string(argv[1]) + ".edges" << endl;
            return 1;
        }
        size_t sz = 0;
        for(const way_t &w: ways){
            sz += w.getNumWays();
        }
        os << sz << "\n";
        for(const way_t &w: ways){
            os << w << "\n";
        }
    }
    
    list<point_t> points; {
        // Nodes
        for (auto it = doc.first_node()->first_node("node"); string(it->name()) == "node"; it = it->next_sibling()) {
            auto pname = find_tag(it, "name");
            if(pname != NULL){
                point_t p;
                p.setName(pname->first_attribute("v")->value());
                p.setCoord(coord_t(atof(it->first_attribute("lat")->value()),
                                   atof(it->first_attribute("lon")->value())));
                points.push_back(p);
            }
        }
        // Ways
        for (auto it = doc.first_node()->first_node("way"); string(it->name()) == "way"; it = it->next_sibling()) {
            auto pname = find_tag(it, "name");
            if(pname != NULL){
                point_t p;
                p.setName(pname->first_attribute("v")->value());
                way_t way(it, edge_type_t::NO);
                p.setCoord(way.get_mean_coord(nodes_all));
                if(p.getName() == "Porto - Campanhã") points.push_front(p);
                else points.push_back(p);
            }
        }
    }
    // Print points of interest
    {
        ofstream os;
        os.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            os.open(string(argv[1]) + ".points");
        } catch(std::ofstream::failure e) {
            cerr << "ERROR: Failed to open file " << string(argv[1]) + ".points" << endl;
            return 1;
        }
        os << points.size() << "\n";
        for(const auto &p: points){
            os << p << "\n";
        }
    }
    return 0;
}
