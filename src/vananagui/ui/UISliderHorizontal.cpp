#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include "vananagui/ui/UISliderHorizontal.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/renderer/ShaderProgram.h"
#include "vananagui/renderer/Texture.h"
#include "vananagui/Vanme.h"
#include "common.h"

#define MOISTITY 0.75f

UISliderHorizontal::UISliderHorizontal(int paramID, const char* image) {
    this->paramID = paramID;

    this->texFilled = new Texture(image);
}

void UISliderHorizontal::calculateBounds() {
    this->bounds = {
        static_cast<int>(this->pos.x),
        static_cast<int>(this->pos.y),
        static_cast<int>(this->pos.x + this->texFilled->width  * this->scale.x),
        static_cast<int>(this->pos.y + this->texFilled->height * this->scale.y)
    };
}

void UISliderHorizontal::render() {
    float v = Van::me().getEditor()->getEffect()->getParameter(this->paramID);
    // this->texFilled->drawBarHorizontally(this->pos, this->scale, v);
    Van::me().spriteRenderer->renderHorizontalBar(*this->texFilled, this->pos, this->scale, v);
}

void UISliderHorizontal::handleMouseButton(Mouse* mouse, int btn, bool state) {
    if (btn != MOUSE_BUTTON_LEFT) return;

    this->grabbed = (state && mouse->inRect(this->bounds));
}

void UISliderHorizontal::handleMouseMotion(Mouse* mouse, long xRel, long yRel) {
    if (!this->grabbed) {
        if (mouse->inRect(this->bounds)) {
            // @TODO: set cursor to hand grab
        }
        return;
    }

    // @TODO: set cursor to hand grab
    this->value = MAX(MIN(this->value + (xRel * MOISTITY / this->texFilled->width), 1.0), 0.0);
    Van::me().getEditor()->getEffect()->setParameter(this->paramID, this->value);
}
