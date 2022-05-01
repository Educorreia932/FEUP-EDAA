#pragma once

class View {
public:
    virtual void refresh() = 0;
    virtual void draw() = 0;
};
