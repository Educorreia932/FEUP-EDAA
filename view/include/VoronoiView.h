#include "View.h"
#include "VoronoiDiagram.h"

#include <SFML/Graphics.hpp>
#include <vector>

class VoronoiView : public View {
    private:
        sf::RenderTarget &window;
        VoronoiDiagram diagram;
        std::vector<sf::Vertex> lines;
        std::vector<sf::CircleShape*> circles;
        double convertX(double x);
        double convertY(double y);
    public:
        VoronoiView(sf::RenderTarget &window_, VoronoiDiagram diagram);
        ~VoronoiView();

        virtual void refresh();
        virtual void draw();
};
