#include "DraggableZoomableWindow.h"

#include <cmath>

DraggableZoomableWindow::DraggableZoomableWindow(sf::Vector2f center_) :
    center(center_)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    create(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), "Window Title", sf::Style::Default, settings);
    
    view = new sf::View(window->getDefaultView());

    debugView = new DebugView(window, fpsMonitor);
}

void DraggableZoomableWindow::setView(View *view_){
    v = view_;
}

void DraggableZoomableWindow::draw(){
    window->setView(*view);
    window->clear(background_color);
    v->draw();
    fpsMonitor.count();
    if(debug_mode) debugView->draw();
}

void DraggableZoomableWindow::onResize(){
    recalculateView();
}

void DraggableZoomableWindow::onScroll(float delta){
    scale *= pow(SCALE_DELTA, -delta);
    recalculateView();
}

void DraggableZoomableWindow::recalculateView(){
    sf::Vector2f size((float) window->getSize().x, (float) window->getSize().y);
    *view = sf::View(center, size*scale);
    debugView->recalculateView();
}

bool DraggableZoomableWindow::getDebugMode() const {
    return debug_mode;
}
void DraggableZoomableWindow::setDebugMode(bool b) {
    debug_mode = b;
}

void DraggableZoomableWindow::onLeftPress(const sf::Event &event){
    isLeftClickPressed = true;
    centerInitial = center;
    posMouseInitial = sf::Vector2f(
        (float) event.mouseButton.x,
        (float) event.mouseButton.y
    );
}

void DraggableZoomableWindow::onLeftRelease(){
    isLeftClickPressed = false;
}

void DraggableZoomableWindow::onMouseMoved(const sf::Event &event){
    if(isLeftClickPressed){
        sf::Vector2f mouse_pos(
            (float) event.mouseMove.x,
            (float) event.mouseMove.y
        );
        center = centerInitial - (mouse_pos - posMouseInitial)*scale;
        recalculateView();
    }
}

void DraggableZoomableWindow::setBackgroundColor(sf::Color color){
    background_color = color;
}
