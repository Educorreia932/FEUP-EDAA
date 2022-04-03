#include "DebugView.h"

#include "loadFont.h"

using namespace sf;
using namespace std;

const Font DebugView::FONT = loadFont("/../../res/fonts/inconsolata.ttf");

DebugView::DebugView(RenderWindow *window_, const FpsMonitor &fpsMonitor_):
    window(window_),
    v(window->getDefaultView()),
    fpsMonitor(fpsMonitor_),
    debugText("", DebugView::FONT, DebugView::FONT_SIZE)
{
    debugText.setFillColor(Color::Black);
    debugText.setStyle(Text::Bold);
}

void DebugView::recalculateView(){
    Vector2f size((float) window->getSize().x, (float) window->getSize().y);
    v = sf::View(FloatRect(0.0, 0.0, size.x, size.y));
}

void DebugView::draw(){
    window->setView(v);
    
    string debugInfo;
    debugInfo += "FPS: " + to_string(int(fpsMonitor.getFPS())) + "\n";

    if(debugInfo[debugInfo.size()-1] == '\n')
        debugInfo = debugInfo.substr(0, debugInfo.size()-1);
    debugText.setString(debugInfo);
    Vector2f size = Vector2f(window->getSize());
    FloatRect bounds = debugText.getLocalBounds();
    debugText.setOrigin(Vector2f(0.0, bounds.height));
    debugText.setPosition(Vector2f(0.2f*DebugView::FONT_SIZE, size.y-0.7f*DebugView::FONT_SIZE));

    window->draw(debugText);
}
