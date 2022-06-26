#pragma once

#include "KMeans.h"

#include "View.h"

#include "polygon.h"

#include <SFML/Graphics.hpp>
#include <vector>

class ClustersView : public View {
private:
    sf::RenderTarget& window;
    std::vector<sf::CircleShape*> circles;

    KMeans& kmeans;
    bool reached_stop_criteria = false;
    int curr_iter = 0;

    void updateCircles();
public:
    ClustersView(sf::RenderTarget& window_, KMeans& kmeans);

    virtual void refresh();
    virtual void draw();
};