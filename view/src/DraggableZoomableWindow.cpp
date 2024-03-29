#include "DraggableZoomableWindow.h"

#include <cmath>

DraggableZoomableWindow::DraggableZoomableWindow(sf::Vector2f center_) :
    center(center_)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    create(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), "Window Title", sf::Style::Default, settings);
    
    view = new sf::View(getDefaultView());

    debugView = new DebugView(this, fpsMonitor);
}

void DraggableZoomableWindow::setDrawView(View *view_){
    v = view_;
}

void DraggableZoomableWindow::draw(){
    setView(*view);
    clear(backgroundColor);
    v->draw();
    fpsMonitor.count();
    if(debugMode) debugView->draw();
}

void DraggableZoomableWindow::onResize(){
    recalculateView();
}

void DraggableZoomableWindow::onScroll(float delta){
    scale *= pow(SCALE_DELTA, -delta);
    recalculateView();
}

void DraggableZoomableWindow::recalculateView(){
    sf::Vector2f size((float) getSize().x, (float) getSize().y);
    *view = sf::View(center, size*scale);
    debugView->recalculateView();
}

bool DraggableZoomableWindow::getDebugMode() const {
    return debugMode;
}
void DraggableZoomableWindow::setDebugMode(bool b) {
    debugMode = b;
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
    backgroundColor = color;
}
