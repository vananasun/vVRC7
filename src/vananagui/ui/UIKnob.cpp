#include <public.sdk/source/vst2.x/audioeffect.h>
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include "vananagui/renderer/Texture.h"
#include "vananagui/platform/Mouse.h"
#include "vananagui/ui/UIKnob.h"
#include "vananagui/Vanme.h"
#include "common.h"
#define M_PI 3.14159265358979323846264338327950288



UIKnob::UIKnob(int paramID, const char* image) {
    this->paramID = paramID;
    this->tex = new Texture(image);
}

void UIKnob::setOrigin(glm::vec2 origin) {
    this->origin = origin;
}

void UIKnob::calculateBounds() {
    this->bounds = {
        static_cast<int>(this->pos.x),
        static_cast<int>(this->pos.y),
        static_cast<int>(this->pos.x + this->tex->width  * this->scale.x),
        static_cast<int>(this->pos.y + this->tex->height * this->scale.y)
    };
}

void UIKnob::render() {
    Van::me().spriteRenderer->render(*this->tex, this->pos, this->scale, (this->value * 270) * ((M_PI * 2) / 360), this->origin);
}

void UIKnob::handleMouseButton(Mouse* mouse, int btn, bool state) {
    if (btn != MOUSE_BUTTON_LEFT) return;

    this->grabbed = (state && mouse->inRect(this->bounds));
}

void UIKnob::handleMouseMotion(Mouse* mouse, long xRel, long yRel) {
    if (!this->grabbed) {
        if (mouse->inRect(this->bounds)) {
            // @TODO: set cursor to hand grab
        }
        return;
    }

    // @TODO: set cursor to hand grab
    this->value = MAX(MIN(this->value - (yRel * 0.8 / 270.0), 1.0), 0.0);
    Van::me().getEditor()->getEffect()->setParameter(this->paramID, this->value);
}
