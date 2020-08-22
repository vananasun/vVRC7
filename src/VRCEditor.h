#pragma once
#include <public.sdk/source/vst2.x/aeffeditor.h>
#include "vananagui/VananaGUI.h"

class VRCEditor : public AEffEditor {
private:
    ERect rect;
    AudioEffect* effect;
    VananaGUI* vananagui;
public:
    VRCEditor(AudioEffect* effect);
    ~VRCEditor();

    bool getRect(ERect** rect);
    bool isOpen();
    bool open(void* ptr);
    void close();
    void idle();

    bool onWheel(float distance);

    AudioEffect* getEffect() { return this->effect; }
};
