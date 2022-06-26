#pragma once

#include "KMeans.h"

#include "View.h"

#include "polygon.h"

#include <SFML/Graphics.hpp>
#include <vector>

class ClustersView: public View {
private:
    sf::RenderTarget &window;
    std::vector<sf::CircleShape*> circles;
public:
    ClustersView(sf::RenderTarget &window_, std::vector<Cluster> clusters);

    virtual void refresh();
    virtual void draw();
};