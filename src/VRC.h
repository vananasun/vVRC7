#pragma once
#include <public.sdk/source/vst2.x/audioeffectx.h>
#include "emu2413.h"
#include "Channel.h"
#include "VSTParams.h"
#include "Instrument.h"
#include "VRCEditor.h"



class VRC: public AudioEffectX {
public:
    VRC(audioMasterCallback audioMaster);
    ~VRC();

    virtual void setSampleRate(float sampleRate);
    virtual void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);

    virtual VstInt32 canDo(char* text);
    virtual VstInt32 getNumMidiInputChannels();
    virtual VstInt32 getNumMidiOutputChannels();
    virtual VstInt32 processEvents(VstEvents* events);

    virtual void setParameter(VstInt32 index, float value);
    virtual float getParameter(VstInt32 index);
    virtual void getParameterName(VstInt32 index, char* label);

    OPLL* opll;
    Instrument* instrument;
    Channel** channels;
    VSTParams* vstParams;
    VRCEditor* editor;
};
