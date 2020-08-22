#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include "vananagui/ui/UISliderHorizontal.h"
#include "vananagui/renderer/RectangleRenderer.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/Vanme.h"
#include "common.h"



UICheckbox::UICheckbox(int paramID) {
    this->paramID = paramID;
}

UICheckbox* UICheckbox::setSize(glm::vec2 size) {
    this->size = size;
    this->calculateBounds();
    return this;
}

void UICheckbox::calculateBounds() {
    this->bounds = {
        (int)this->pos.x - 9,
        (int)this->pos.y - 9,
        (int)this->pos.x + (int)this->size.x + 9,
        (int)this->pos.y + (int)this->size.y + 9,
    };
}

void UICheckbox::render() {
    if (!this->checked) return;

    Van::me().rectangleRenderer->render(
        this->pos * this->scale,
        this->size * this->scale,
        { 0.8f, 0.8f, 0.8823529411764706f, 1.0f }
    );
}

void UICheckbox::handleMouseButton(Mouse* mouse, int btn, bool state) {
    if (btn != MOUSE_BUTTON_LEFT) return;

    if (state && mouse->inRect(this->bounds))
        this->checked ^= true;
    Van::me().getEditor()->getEffect()->setParameter(this->paramID, this->checked);
}

void UICheckbox::handleMouseMotion(Mouse* mouse, long xRel, long yRel) {
    if (!this->checked) {
        if (mouse->inRect(this->bounds)) {
            // @TODO: set cursor to hand grab
        }
        return;
    }
}
