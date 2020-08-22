#pragma once
#include "vananagui/ui/UIElement.h"

class UICheckbox : public UIElement {
public:
    UICheckbox(int paramID);
    void render();
    void handleMouseButton(Mouse* mouse, int btn, bool state);
    void handleMouseMotion(Mouse* mouse, long xRel = 0, long yRel = 0);
    UICheckbox* setSize(glm::vec2 size);
    void calculateBounds();


    float value;
    bool hovered = false, checked = false;

    int paramID;
    glm::vec2 size;
};
