#include "graphviewer.h"

#include "coord.h"
#include <unordered_map>

class MapViewer : public GraphViewer {
public:
    typedef int window_t;
    typedef long long node_t;
    typedef long long edge_t;
private:
    const int w, h;
    const coord_t min, max;
    coord_t nw_corner;
public:
    /**
     * @brief Construct a new Map Viewer object
     * 
     * @param w window width
     * @param h window height
     * @param min 
     * @param max 
     */
    MapViewer(window_t w, window_t h, coord_t min, coord_t max);
    void addNode(node_t i, coord_t c);
    void addNode(node_t i, coord_t c, const sf::Color &color, window_t width);
    void addEdge(edge_t i, node_t u, node_t v, GraphViewer::Edge::EdgeType edge_type, const sf::Color &color, window_t width, bool dashed = false);
    void createWindow();
    void join();
};
