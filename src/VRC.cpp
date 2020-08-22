#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <functional>
#include "VRC.h"
#include "VRCEditor.h"
#include "emu2413.h"
#include "Channel.h"
#include "Instrument.h"
#include "VSTParams.h"
#include "common.h"



AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {
    #ifdef DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    #endif
    return new VRC(audioMaster);
}


VRC::VRC(audioMasterCallback audioMaster) : AudioEffectX(audioMaster, 1, N_VSTPARAMS) {
    setNumInputs(0);
    setNumOutputs(2);
    setUniqueID('mvdV');
    canProcessReplacing();
    isSynth(true);

    // initialize OPLL chip with the standard MSX clock
    #define MSX_CLK 3579545
    this->opll = OPLL_new(MSX_CLK, this->sampleRate);
    OPLL_reset(this->opll);

    // initialize instrument and channels
    this->instrument = new Instrument(this->opll);
    this->channels = static_cast<Channel**>(malloc(9 * sizeof(Channel)));
    for (int i = 0; i < 9; i++)
        this->channels[i] = new Channel(this->opll, this->instrument, i);

    // initialize the GUI
    this->editor = new VRCEditor(this);
    setEditor(this->editor);
}

VRC::~VRC() {
    for (int i = 0; i < 9; i++)
        SAFE_DELETE_PTR(this->channels[i]);
    SAFE_DELETE_PTR(this->channels);
    SAFE_DELETE_PTR(this->instrument);
    OPLL_delete(opll);
}

VstInt32 VRC::canDo(char* text) {
    if (!strcmp(text, "receiveVstEvents"))
        return 1;
    if (!strcmp(text, "receiveVstMidiEvent"))
        return 1;
    return -1; // explicitly can't do; 0 => don't know
}

VstInt32 VRC::getNumMidiInputChannels() {
    return 0; // or up to 16 if you prefer
}

VstInt32 VRC::getNumMidiOutputChannels() {
    return 0; // no MIDI output back to host
}

void VRC::setSampleRate(float sampleRate) {
    OPLL_setRate(this->opll, sampleRate);
}

void VRC::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) {
    float* out1 = outputs[0];
    float* out2 = outputs[1];

    while (--sampleFrames >= 0) {
        float sample = static_cast<float>(OPLL_calc(this->opll)) / (32768.0 / 2.0);
        (*out1++) = sample;
        (*out2++) = sample;
    }
}

/**
 * Process VST MIDI events.
 *
 * @param VstEvents* events
 *
 * @return VstInt32
 */
VstInt32 VRC::processEvents(VstEvents* events) {
    for (VstInt32 i = 0; i < events->numEvents; i++) {

        if ((events->events[i])->type != kVstMidiType)
            continue;
        VstMidiEvent* event = (VstMidiEvent*)events->events[i];
        VstInt32 status = event->midiData[0] & 0xF0; // ignore MIDI channel

        switch (status) {
        case 0x90: // note on
        case 0x80:
        {
            VstInt32 note = event->midiData[1] & 0x7F;
            VstInt32 velocity = event->midiData[2] & 0x7F;
            if (0x80 == status || 0 == velocity) {
                // note off
                int channelIndex = -1;
                for (int i = 0; i < 9; i++) {
                    if (this->channels[i]->isPlaying && this->channels[i]->currentNote == note)
                        channelIndex = i;
                }
                if (-1 != channelIndex)
                    this->channels[channelIndex]->release();

            } else {

                // note on
                #ifdef DEBUG
                velocity = 127;
                #endif

                int channelIndex = 0; // @TODO: abs(last - 1 mod 12) <-incorrect
                for (int i = 0; i < 9; i++) {
                    if (!this->channels[i]->isPlaying)
                        channelIndex = i;
                }
                this->channels[channelIndex]->trigger(note, velocity);

            }
        } break;

        case 0xE0: // pitch bend
            break;
        case 0xB0: // controller
            break;
        }

    }

    return 1;
}



void VRC::setParameter(VstInt32 index, float value) {
    // printf("%i %f\n",index,value);
    switch (index) {
    case PARAM_CARRIER_AM: this->instrument->setCarrierAM(value); break;
    case PARAM_CARRIER_VIBRATO: this->instrument->setCarrierVibrato(value); break;
    case PARAM_CARRIER_SUSTAINED: this->instrument->setCarrierSustained(value); break;
    case PARAM_CARRIER_LEVEL: this->instrument->setCarrierLevel(value * 3); break;
    case PARAM_CARRIER_RATE: this->instrument->setCarrierRate(value); break;
    case PARAM_CARRIER_RECT: this->instrument->setCarrierRect(value); break;
    case PARAM_CARRIER_MULT: this->instrument->setCarrierMultFactor(value * 15); break;
    case PARAM_CARRIER_ATTACK: this->instrument->setCarrierAttack(value * 15); break;
    case PARAM_CARRIER_DECAY: this->instrument->setCarrierDecay(value * 15); break;
    case PARAM_CARRIER_SUSTAIN: this->instrument->setCarrierSustain(value * 15); break;
    case PARAM_CARRIER_RELEASE: this->instrument->setCarrierRelease(value * 15); break;
    case PARAM_MODULATOR_AM: this->instrument->setModulatorAM(value); break;
    case PARAM_MODULATOR_VIBRATO: this->instrument->setModulatorVibrato(value); break;
    case PARAM_MODULATOR_SUSTAINED: this->instrument->setModulatorSustained(value); break;
    case PARAM_MODULATOR_LEVEL: this->instrument->setModulatorLevel(value * 3); break;
    case PARAM_MODULATOR_RATE: this->instrument->setCarrierRate(value); break;
    case PARAM_MODULATOR_RECT: this->instrument->setModulatorRect(value); break;
    case PARAM_MODULATOR_MULT: this->instrument->setModulatorMultFactor(value * 15); break;
    case PARAM_MODULATOR_ATTACK: this->instrument->setModulatorAttack(value * 15); break;
    case PARAM_MODULATOR_DECAY: this->instrument->setModulatorDecay(value * 15); break;
    case PARAM_MODULATOR_SUSTAIN: this->instrument->setModulatorSustain(value * 15); break;
    case PARAM_MODULATOR_RELEASE: this->instrument->setModulatorRelease(value * 15); break;
    case PARAM_MODULATION: this->instrument->setModulation(value * 63); break;
    case PARAM_FEEDBACK: this->instrument->setFeedback(value * 7); break;
    default: return;
    }
    this->instrument->applyInstrumentData();
}

float VRC::getParameter(VstInt32 index) {
    switch (index) {
    case PARAM_CARRIER_AM: return static_cast<float>(this->instrument->getCarrierAM());
    case PARAM_CARRIER_VIBRATO: return static_cast<float>(this->instrument->getCarrierVibrato());
    case PARAM_CARRIER_SUSTAINED: return static_cast<float>(this->instrument->getCarrierSustained());
    case PARAM_CARRIER_LEVEL: return static_cast<float>(this->instrument->getCarrierLevel() / 3.0);
    case PARAM_CARRIER_RATE: return static_cast<float>(this->instrument->getCarrierRate());
    case PARAM_CARRIER_RECT: return static_cast<float>(this->instrument->getCarrierRect());
    case PARAM_CARRIER_MULT: return static_cast<float>(this->instrument->getCarrierMultFactor() / 15.0);
    case PARAM_CARRIER_ATTACK: return static_cast<float>(this->instrument->getCarrierAttack() / 15.0);
    case PARAM_CARRIER_DECAY: return static_cast<float>(this->instrument->getCarrierDecay() / 15.0);
    case PARAM_CARRIER_SUSTAIN: return static_cast<float>(this->instrument->getCarrierSustain() / 15.0);
    case PARAM_CARRIER_RELEASE: return static_cast<float>(this->instrument->getCarrierRelease() / 15.0);
    case PARAM_MODULATOR_AM: return static_cast<float>(this->instrument->getModulatorAM());
    case PARAM_MODULATOR_VIBRATO: return static_cast<float>(this->instrument->getModulatorVibrato());
    case PARAM_MODULATOR_SUSTAINED: return static_cast<float>(this->instrument->getModulatorSustained());
    case PARAM_MODULATOR_LEVEL: return static_cast<float>(this->instrument->getModulatorLevel() / 3.0);
    case PARAM_MODULATOR_RATE: return static_cast<float>(this->instrument->getCarrierRate());
    case PARAM_MODULATOR_RECT: return static_cast<float>(this->instrument->getModulatorRect());
    case PARAM_MODULATOR_MULT: return static_cast<float>(this->instrument->getModulatorMultFactor() / 15.0);
    case PARAM_MODULATOR_ATTACK: return static_cast<float>(this->instrument->getModulatorAttack() / 15.0);
    case PARAM_MODULATOR_DECAY: return static_cast<float>(this->instrument->getModulatorDecay() / 15.0);
    case PARAM_MODULATOR_SUSTAIN: return static_cast<float>(this->instrument->getModulatorSustain() / 15.0);
    case PARAM_MODULATOR_RELEASE: return static_cast<float>(this->instrument->getModulatorRelease() / 15.0);
    case PARAM_MODULATION: return static_cast<float>(this->instrument->getModulation() / 63.0);
    case PARAM_FEEDBACK: return static_cast<float>(this->instrument->getFeedback() / 7.0);
    }
    return -1.0f;
}


void int2cstr(VstInt32 value, char *string) { sprintf(string, "%d", value); }


void VRC::getParameterName(VstInt32 index, char* label) {
    switch (index) {
    case PARAM_CARRIER_AM: strcpy(label, "Carrier AM"); break;
    case PARAM_CARRIER_VIBRATO: strcpy(label, "Carrier Vibrato"); break;
    case PARAM_CARRIER_SUSTAINED: strcpy(label, "Carrier Hold"); break;
    case PARAM_CARRIER_LEVEL: strcpy(label, "Carrier Level"); break;
    case PARAM_CARRIER_RATE: strcpy(label, "Carrier Rate"); break;
    case PARAM_CARRIER_RECT: strcpy(label, "Carrier Rect"); break;
    case PARAM_CARRIER_MULT: strcpy(label, "Carrier Mult"); break;
    case PARAM_CARRIER_ATTACK: strcpy(label, "Carrier Attack"); break;
    case PARAM_CARRIER_DECAY: strcpy(label, "Carrier Decay"); break;
    case PARAM_CARRIER_SUSTAIN: strcpy(label, "Carrier Sustain"); break;
    case PARAM_CARRIER_RELEASE: strcpy(label, "Carrier Release"); break;
    case PARAM_MODULATOR_AM: strcpy(label, "Modulator AM"); break;
    case PARAM_MODULATOR_VIBRATO: strcpy(label, "Modulator Vibrato"); break;
    case PARAM_MODULATOR_SUSTAINED: strcpy(label, "Modulator Hold"); break;
    case PARAM_MODULATOR_LEVEL: strcpy(label, "Modulator Level"); break;
    case PARAM_MODULATOR_RATE: strcpy(label, "Modulator Rate"); break;
    case PARAM_MODULATOR_RECT: strcpy(label, "Modulator Rect"); break;
    case PARAM_MODULATOR_MULT: strcpy(label, "Modulator Mult"); break;
    case PARAM_MODULATOR_ATTACK: strcpy(label, "Modulator Attack"); break;
    case PARAM_MODULATOR_DECAY: strcpy(label, "Modulator Decay"); break;
    case PARAM_MODULATOR_SUSTAIN: strcpy(label, "Modulator Sustain"); break;
    case PARAM_MODULATOR_RELEASE: strcpy(label, "Modulator Release"); break;
    case PARAM_MODULATION: strcpy(label, "Modulation"); break;
    case PARAM_FEEDBACK: strcpy(label, "Feedback"); break;
    }
}

// void VRC::getParameterDisplay(VstInt32 index, char* text) {
//     switch (index) {
//     case PARAM_CARRIER_AM: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_VIBRATO: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_SUSTAINED: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_LEVEL: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_RATE: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_RECT: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_MULT: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_ATTACK: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_DECAY: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_SUSTAIN: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_CARRIER_RELEASE: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_AM: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_VIBRATO: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_SUSTAINED: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_LEVEL: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_RATE: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_RECT: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_MULT: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_ATTACK: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_DECAY: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_SUSTAIN: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATOR_RELEASE: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_MODULATION: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     case PARAM_FEEDBACK: int2cstr(static_cast<VstInt32>(this->instrument->get()), text); break;
//     }
// }

// void Floaty::getParameterLabel(VstInt32 index, char* label) {
//     strcpy(label, "%");
// }

// switch (index) {
// case PARAM_CARRIER_AM:
// case PARAM_CARRIER_VIBRATO:
// case PARAM_CARRIER_SUSTAINED:
// case PARAM_CARRIER_LEVEL:
// case PARAM_CARRIER_RATE:
// case PARAM_CARRIER_RECT:
// case PARAM_CARRIER_MULT:
// case PARAM_CARRIER_ATTACK:
// case PARAM_CARRIER_DECAY:
// case PARAM_CARRIER_SUSTAIN:
// case PARAM_CARRIER_RELEASE:
// case PARAM_MODULATOR_AM:
// case PARAM_MODULATOR_VIBRATO:
// case PARAM_MODULATOR_SUSTAINED:
// case PARAM_MODULATOR_LEVEL:
// case PARAM_MODULATOR_RATE:
// case PARAM_MODULATOR_RECT:
// case PARAM_MODULATOR_MULT:
// case PARAM_MODULATOR_ATTACK:
// case PARAM_MODULATOR_DECAY:
// case PARAM_MODULATOR_SUSTAIN:
// case PARAM_MODULATOR_RELEASE:
// case PARAM_MODULATION:
// case PARAM_FEEDBACK:
// }
