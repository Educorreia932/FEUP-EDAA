#include "MapViewer.h"

#define COORDMULT               50000       // Multiply coordinates to get integer positions

MapViewer::MapViewer(window_t w_, window_t h_, coord_t min_, coord_t max_):
    GraphViewer(), w(w_), h(h_), min(min_), max(max_){
    GraphViewer::Node::setDefaultSize(0);
    
    coord_t mean = (min+max)/2;
    coord_t size(double(h)/COORDMULT, double(w)/COORDMULT);
    nw_corner = mean + coord_t(size.getLat()/2, -size.getLon()/2);

    GraphViewer::setEnabledNodes(false);
    GraphViewer::setEnabledEdgesText(false);
}

void MapViewer::addNode(node_t i, coord_t c){
    float x = float(+(c.getLon() - nw_corner.getLon())*COORDMULT);
    float y = float(-(c.getLat() - nw_corner.getLat())*COORDMULT);
    GraphViewer::addNode(i, sf::Vector2f(x, y));
}

void MapViewer::addNode(node_t i, coord_t c, const sf::Color &color, window_t w){
    window_t x = (window_t)(double)(+(c.getLon() - nw_corner.getLon())*COORDMULT);
    window_t y = (window_t)(double)(-(c.getLat() - nw_corner.getLat())*COORDMULT);
    GraphViewer::Node &node = GraphViewer::addNode(i, sf::Vector2f(x, y));
    node.setColor(color);
    node.setSize(w);
}

void MapViewer::addEdge(edge_t i, node_t u, node_t v, GraphViewer::Edge::EdgeType edge_type, const sf::Color &color, window_t w, bool dashed){
    GraphViewer::Node &uNode = getNode(u);
    GraphViewer::Node &vNode = getNode(v);
    
    GraphViewer::Edge &edge = GraphViewer::addEdge((int)i, uNode, vNode, edge_type);
    edge.setColor(color);
    edge.setThickness(w);
    edge.setDashed(dashed);
}

void MapViewer::createWindow(){
    GraphViewer::setZipEdges(true);
    GraphViewer::createWindow(w, h);
}

void MapViewer::join(){
    GraphViewer::join();
}
