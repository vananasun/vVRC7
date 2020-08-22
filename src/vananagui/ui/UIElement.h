#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>



class Mouse;
class UIElement {
public:
    virtual void render() {};
    virtual void handleMouseButton(Mouse* mouse, int btn, bool state) {};
    virtual void handleMouseMotion(Mouse* mouse, long xRel = 0, long yRel = 0) {};
    virtual UIElement* setScale(glm::vec2 scale) {
        this->scale = scale;
        this->calculateBounds();
        return this;
    };
    virtual UIElement* setPos(glm::vec2 pos) {
        this->pos = pos;
        this->calculateBounds();
        return this;
    };
    virtual void calculateBounds() {};

    glm::vec2 pos;
    glm::vec2 scale = { 1.0, 1.0 };
    glm::ivec4 bounds;
};
