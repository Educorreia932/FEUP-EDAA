#include "WindowView.h"

#include <cmath>

using namespace sf;

WindowView::WindowView(Vector2f center_) : center(center_) {
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    window = new RenderWindow(VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), "Window Title", Style::Default, settings);
    
    view = new sf::View(window->getDefaultView());

    debugView = new DebugView(window, fpsMonitor);
}

void WindowView::setView(View *view){
    v = view;
}

void WindowView::draw(){
    window->setView(*view);
    window->clear(background_color);
    v->draw();
    fpsMonitor.count();
    if(debug_mode) debugView->draw();
}

void WindowView::onResize(){
    recalculateView();
}

void WindowView::onScroll(float delta){
    scale *= pow(SCALE_DELTA, -delta);
    recalculateView();
}

void WindowView::recalculateView(){
    Vector2f size((float) window->getSize().x, (float) window->getSize().y);
    *view = sf::View(center, size*scale);
    debugView->recalculateView();
}

RenderWindow *WindowView::getWindow(){
    return window;
}
bool WindowView::getDebugMode() const {
    return debug_mode;
}
void WindowView::setDebugMode(bool b) {
    debug_mode = b;
}

void WindowView::onLeftPress(const Event &event){
    isLeftClickPressed = true;
    centerInitial = center;
    posMouseInitial = Vector2f(
        (float) event.mouseButton.x,
        (float) event.mouseButton.y
    );
}

void WindowView::onLeftRelease(){
    isLeftClickPressed = false;
}

void WindowView::onMouseMoved(const Event &event){
    if(isLeftClickPressed){
        Vector2f mouse_pos(
            (float) event.mouseMove.x,
            (float) event.mouseMove.y
        );
        center = centerInitial - (mouse_pos - posMouseInitial)*scale;
        recalculateView();
    }
}
