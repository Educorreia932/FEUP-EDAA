#include <bits/stdc++.h>
#include <filesystem>
namespace fs = std::filesystem;

#include "rapidxml.hpp"
#include "EdgeType.h"
#include "coord.h"
#include "DWGraph.h"
#include "point.h"
#include "polygon.h"

#include "dir.h"

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

/**
 * @brief A way is a sequence of nodes that are connected by one type of
 * transport infrastructure, like a road or a path.
 * 
 * A way generally describes a single road, where all edges are connected by the
 * same type of road/path (highway, road, path, ...), and the speed limit is the
 * same between consecutive edges in a way.
 * 
 * A way may be one-way if transit can only flow in the direction of the nodes
 * (a way that is one-way with nodes 1,2,3,4 can only be traversed 1->2->3->4,
 * not 4->3->2->1). A way may also be two-way, or both-way, in which case it can
 * be traversed in both directions. A way may also be a roundabout, in which
 * case it is similar in working as one-way.
 * 
 * Note that a way is NOT an edge, but rather a list of edges.
 */
class way_t : public list<DWGraph::node_t> {
public:
    enum dir_t {
        Front = 1,
        Both = 0,
        Back = -1
    };
private:
    void get_way(xml_node<> *it){
        id = atoll(it->first_attribute("id")->value());
        for(auto j = it->first_node("nd"); j != nullptr && string(j->name()) == "nd"; j = j->next_sibling()){
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
    long long id;
    dir_t dir;
    speed_t speed;
    edge_type_t edgeType;
    way_t(){}
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
    coord_t get_mean_coord(const unordered_map<DWGraph::node_t, coord_t> &nodes) const{
        coord_t ret(0,0);
        for(auto it = begin(); it != end(); ++it){
            ret = ret + nodes.at(*it);
        }
        return ret/double(size());
    }
};

ostream& operator<<(ostream &os, const way_t &w){
    if(w.dir == way_t::dir_t::Front || w.dir == way_t::dir_t::Both){
        os << w.id << " " << char(w.edgeType) << " " << w.speed << " " << w.size();
        for(auto it = w.begin(); it != w.end(); ++it){
            os << "\n" << *it;
        }
    }
    if(w.dir == way_t::dir_t::Back || w.dir == way_t::dir_t::Both){
        os << "\n" << w.id + 1000000000000L << " " << char(w.edgeType) << " " << w.speed << " " << w.size();
        for(auto it = w.rbegin(); it != w.rend(); ++it){
            os << "\n" << *it;
        }
    }
        
    return os;
}

/**
 * @brief Way types we are accepting from OpenStreetMap, and how we map each OSM
 * way type into our own edge types.
 */
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
/**
 * @brief Set of way types we are explicitly rejecting.
 * 
 * This is so that new edge types can be printed on screen so the user can judge
 * if they are worth including or not. Anyway, way types not in edge_accept are
 * rejected, except those that are not in edge_reject trigger a warning.
 */
unordered_set<string> edge_reject = {
    "steps",        "pedestrian", "footway",   "cycleway",
    "construction", "path",       "bridleway", "platform", "raceway",
    "elevator",     "proposed",   "planned",   "bus_stop", "services",
    "crossing", "corridor"};
// unordered_set<string> service_accept = {"driveway", "parking_aisle", "alley"};
// unordered_set<string> service_reject = {"campground", "emergency_access",
//                                         "drive-through"};
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
    
    const string prefix = string(argv[1]);

    const string dir = getDirectory(prefix);

    const string nodesFilepath    = prefix + ".nodes";
    const string edgesFilepath    = prefix + ".edges";
    const string pointsFilepath   = prefix + ".points";
    const string polygonsFilepath = prefix + ".polygons";

    // Create destination directory
    if(dir != ""){
        if(fs::exists(dir)){
            cout << "Directory already exists: " + dir << endl;
        } else {
            cout << "Creating directory: " + dir << endl;
            assert(fs::create_directories(dir));
        }
    }

    // Read XML
    char *text = nullptr; {
        stringstream ss;
        string buf;
        while (getline(cin, buf)) {
            ss << buf << "\n";
        }
        text = new char[ss.str().size()+1];
        strcpy(text, ss.str().c_str());
    }
    xml_document<> doc;
    doc.parse<0>(text);

    // Get nodes
    unordered_map<long long, coord_t> nodes;
    for (
        auto it = doc.first_node()->first_node("node");
        string(it->name()) == "node";
        it = it->next_sibling()
    ) {
        DWGraph::node_t u = atoll(it->first_attribute("id")->value());
        coord_t c(
            atof(it->first_attribute("lat")->value()),
            atof(it->first_attribute("lon")->value())
        );
        nodes[u] = c;
    }

    // Get ways
    list<way_t> ways;
    unordered_set<long long> nodesInWays;
    for (
        auto it = doc.first_node()->first_node("way");
        string(it->name()) == "way";
        it = it->next_sibling()
    ) {
        if (find_tag(it, "area") != nullptr) continue;
        edge_type_t t = get_edge_type(it);
        if (t == edge_type_t::NO) continue;
        way_t way(it, t); ways.push_back(way);
        for(const DWGraph::node_t &u: way) nodesInWays.insert(u);
    }

    // Print nodes
    {
        ofstream os;
        os.exceptions(ifstream::failbit | ifstream::badbit);
        os.precision(7);
        os.open(nodesFilepath);
        os << nodesInWays.size() << "\n";
        for(const long long nodeId: nodesInWays){
            coord_t c = nodes[nodeId];
            os << nodeId << " " << c.getLat() << " " << c.getLon() << "\n";
        }
    }

    // Print ways/edges
    {
        ofstream os;
        os.exceptions(ifstream::failbit | ifstream::badbit);
        os.open(edgesFilepath);
        size_t sz = 0;
        for(const way_t &w: ways){
            sz += w.getNumWays();
        }
        os << sz << "\n";
        for(const way_t &w: ways){
            os << w << "\n";
        }
    }
    
    // Get points of interest
    list<point_t> points; {
        // Nodes that may be points of interest
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
        // Ways that may be points of interest (e.g., buildings are described as ways)
        for (auto it = doc.first_node()->first_node("way"); string(it->name()) == "way"; it = it->next_sibling()) {
            auto pname = find_tag(it, "name");
            if(pname != NULL){
                point_t p;
                p.setName(pname->first_attribute("v")->value());
                way_t way(it, edge_type_t::NO);
                p.setCoord(way.get_mean_coord(nodes));
                points.push_back(p);
            }
        }
    }
    // Print points of interest
    {
        ofstream os;
        os.exceptions(ifstream::failbit | ifstream::badbit);
        os << fixed << setprecision(7);
        os.open(pointsFilepath);
        os << points.size() << "\n";
        for(const auto &p: points){
            os << urlencode(p.getName(), " \t\n") << " " << p.getCoord().getLat() << " " << p.getCoord().getLon() << "\n";
        }
    }

    // Get polygons
    list<polygon_t> polygons; {
        unordered_map<long long, list<long long>> membersOfRelations;
        unordered_map<long long, polygon_t::type> typeOfRelations;
        for (
            auto it = doc.first_node()->first_node("relation");
            it != NULL && string(it->name()) == "relation";
            it = it->next_sibling()
        ) {
            auto id = atoll(it->first_attribute("id")->value());

            bool good = false;
            polygon_t::type t;

            if(id == 3870917){
                good = true;
                t = polygon_t::type::LAND;
            } else {
                auto natural = find_tag(it, "natural");
                if(natural != NULL){
                    if(string(natural->first_attribute("v")->value()) == "water"){
                        good = true;
                        t = polygon_t::type::WATER;
                    }
                }
            }

            if(good){
                typeOfRelations[id] = t;
                for(
                    auto it2 = it->first_node("member");
                    it2 != NULL && string(it2->name()) == "member";
                    it2 = it2->next_sibling()
                ){
                    if(t == polygon_t::type::WATER || t == polygon_t::type::LAND){
                        if(
                            string(it2->first_attribute("type")->value()) == "way" &&
                            string(it2->first_attribute("role")->value()) == "outer"    
                        ){
                            membersOfRelations[id].push_back(atoll(it2->first_attribute("ref")->value()));
                        }
                    }
                }
            }
        }

        unordered_map<long long, way_t> ways;
        for (
            auto it = doc.first_node()->first_node("way");
            string(it->name()) == "way";
            it = it->next_sibling()
        ) {
            way_t way(it, edge_type_t::NO);
            ways[way.id] = way;

            auto building = find_tag(it, "building");
            if(building != NULL){
                polygons.push_back(polygon_t());
                polygon_t &polygon = *polygons.rbegin();
                polygon.id = way.id;
                polygon.t = polygon_t::type::BUILDING;
                for(const auto &u: way)
                    polygon.coords.push_back(nodes.at(u));
            }
        }

        for(const auto &p: membersOfRelations){
            long long id = p.first;
            list<long long> members = p.second;

            for(auto it = members.begin(); it != members.end();){
                if(ways.count(*it) == 0) it = members.erase(it);
                else ++it;
            }

            polygon_t::type t = typeOfRelations.at(id);
            
            list<DWGraph::node_t> nodesList;
            
            auto it = members.begin();
            
            nodesList.insert(nodesList.begin(), ways.at(*it).begin(), ways.at(*it).end());
            members.erase(it);

            size_t prevSize = members.size()+1;
            while(members.size() != prevSize){
                prevSize = members.size();
                for(auto it = members.begin(); it != members.end(); ++it){
                    const way_t &w = ways[*it];
                    if(*w. begin() == *nodesList.rbegin()){ nodesList.insert(nodesList.end  (), ++w. begin(), w. end()); members.erase(it); break; }
                    if(*w. begin() == *nodesList. begin()){ nodesList.insert(nodesList.begin(), ++w.rbegin(), w.rend()); members.erase(it); break; }
                    if(*w.rbegin() == *nodesList. begin()){ nodesList.insert(nodesList.begin(), ++w. begin(), w. end()); members.erase(it); break; }
                    if(*w.rbegin() == *nodesList.rbegin()){ nodesList.insert(nodesList.end  (), ++w.rbegin(), w.rend()); members.erase(it); break; }
                }
            }

            polygons.push_back(polygon_t());
            polygon_t &polygon = *polygons.rbegin();
            polygon.id = id;
            polygon.t = t;
            for(const DWGraph::node_t &u: nodesList){
                polygon.coords.push_back(nodes.at(u));
            }

            if(id == 3870917){
                polygon.coords.push_back(coord_t(40.8160979, -8.4718));
                polygon.coords.push_back(coord_t(41.3759964, -8.4718));
            }
        }
    }

    // Print polygons
    {
        ofstream os;
        os.exceptions(ifstream::failbit | ifstream::badbit);
        os << fixed << setprecision(7);
        os.open(polygonsFilepath);
        os << polygons.size() << "\n";
        for(const polygon_t &polygon: polygons){
            os << polygon.id << " " << char(polygon.t) << " " << polygon.coords.size() << "\n";
            for(const coord_t &c: polygon.coords){
                os << c.getLat() << " " << c.getLon() << "\n";
            }
        }
    }

    return 0;
}
