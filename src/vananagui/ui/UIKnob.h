#pragma once
#include "vananagui/ui/UIElement.h"
#include "vananagui/renderer/Texture.h"

class UIKnob : public UIElement {
public:
    UIKnob(int paramID, const char* image);
    void render();
    void handleMouseButton(Mouse* mouse, int btn, bool state);
    void handleMouseMotion(Mouse* mouse, long xRel = 0, long yRel = 0);
    void setOrigin(glm::vec2 origin);
    void calculateBounds();



    Texture* tex;
    float value;
    bool hovered = false, grabbed = false;
    glm::vec2 origin = {0,0};

    int paramID;
};
