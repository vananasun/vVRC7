#include "VRCEditor.h"
#include "VSTParams.h"
#include "vananagui/VananaGUI.h"
#include "common.h"



VRCEditor::VRCEditor(AudioEffect* effect) : AEffEditor(effect) {
    this->effect = effect;
}

VRCEditor::~VRCEditor() {
    this->effect = 0;
}

bool VRCEditor::getRect(ERect** rect) {
    *rect = &this->rect;
    return false;
}

bool VRCEditor::isOpen() {
    return true; // @TODO: is open
}

bool VRCEditor::open(void* ptr) {
    this->vananagui = new VananaGUI(this, (HWND)ptr, "vVRC7", { 1122, 434 } );
    this->systemWindow = this->vananagui->getWindow().getHwnd();
    this->rect = {
        0,
        0,
        (VstInt16)this->vananagui->getWindow().getHeight(),
        (VstInt16)this->vananagui->getWindow().getWidth()
    };


    // Background
    this->vananagui->insertUIElement(new UIImage("background_x100"));

    // Carrier
    auto carrierAM = new UICheckbox(PARAM_CARRIER_AM);
    carrierAM->setPos({66, 57});
    carrierAM->setSize({16, 16});

    auto carrierVibrato = new UICheckbox(PARAM_CARRIER_VIBRATO);
    carrierVibrato->setPos({246, 57});
    carrierVibrato->setSize({16, 16});

    auto carrierSustained = new UICheckbox(PARAM_CARRIER_SUSTAINED);
    carrierSustained->setPos({66, 118});
    carrierSustained->setSize({16, 16});

    auto carrierRect = new UICheckbox(PARAM_CARRIER_RECT);
    carrierRect->setPos({246, 118});
    carrierRect->setSize({16, 16});

    auto carrierKeyScaling = new UICheckbox(PARAM_CARRIER_RATE);
    carrierKeyScaling->setPos({66, 179});
    carrierKeyScaling->setSize({16, 16});

    auto carrierLevel = new UISliderHorizontal(PARAM_CARRIER_LEVEL, "slider_level");
    carrierLevel->setPos({237 - 6, 172 - 8});

    auto carrierMult = new UISliderHorizontal(PARAM_CARRIER_MULT, "slider_mult");
    carrierMult->setPos({57 - 6, 231 - 6});

    auto carrierAttack = new UIKnob(PARAM_CARRIER_ATTACK, "knob_small_x4");
    carrierAttack->setOrigin({72/2,72/2});
    carrierAttack->setPos({57, 313});
    carrierAttack->setScale({0.25, 0.25});

    auto carrierDecay = new UIKnob(PARAM_CARRIER_DECAY, "knob_small_x4");
    carrierDecay->setOrigin({72/2,72/2});
    carrierDecay->setPos({153, 313});
    carrierDecay->setScale({0.25, 0.25});

    auto carrierSustain = new UIKnob(PARAM_CARRIER_SUSTAIN, "knob_small_x4");
    carrierSustain->setOrigin({72/2,72/2});
    carrierSustain->setPos({249, 313});
    carrierSustain->setScale({0.25, 0.25});

    auto carrierRelease = new UIKnob(PARAM_CARRIER_RELEASE, "knob_small_x4");
    carrierRelease->setOrigin({72/2,72/2});
    carrierRelease->setPos({345, 313});
    carrierRelease->setScale({0.25, 0.25});

    // Modulator
    float modulatorXOffset = 456 - 24;

    auto modulatorAM = new UICheckbox(PARAM_MODULATOR_AM);
    modulatorAM->setPos({modulatorXOffset + 66, 57});
    modulatorAM->setSize({16, 16});

    auto modulatorVibrato = new UICheckbox(PARAM_MODULATOR_VIBRATO);
    modulatorVibrato->setPos({modulatorXOffset + 246, 57});
    modulatorVibrato->setSize({16, 16});

    auto modulatorSustained = new UICheckbox(PARAM_MODULATOR_SUSTAINED);
    modulatorSustained->setPos({modulatorXOffset + 66, 118});
    modulatorSustained->setSize({16, 16});

    auto modulatorRect = new UICheckbox(PARAM_MODULATOR_RECT);
    modulatorRect->setPos({modulatorXOffset + 246, 118});
    modulatorRect->setSize({16, 16});

    auto modulatorKeyScaling = new UICheckbox(PARAM_MODULATOR_RATE);
    modulatorKeyScaling->setPos({modulatorXOffset + 66, 179});
    modulatorKeyScaling->setSize({16, 16});

    auto modulatorLevel = new UISliderHorizontal(PARAM_MODULATOR_LEVEL, "slider_level");
    modulatorLevel->setPos({modulatorXOffset + 237 - 6, 172 - 8});

    auto modulatorMult = new UISliderHorizontal(PARAM_MODULATOR_MULT, "slider_mult");
    modulatorMult->setPos({modulatorXOffset + 57 - 6, 231 - 6});

    auto modulatorAttack = new UIKnob(PARAM_MODULATOR_ATTACK, "knob_small_x4");
    modulatorAttack->setOrigin({72/2,72/2});
    modulatorAttack->setPos({modulatorXOffset + 57, 313});
    modulatorAttack->setScale({0.25, 0.25});

    auto modulatorDecay = new UIKnob(PARAM_MODULATOR_DECAY, "knob_small_x4");
    modulatorDecay->setOrigin({72/2,72/2});
    modulatorDecay->setPos({modulatorXOffset + 153, 313});
    modulatorDecay->setScale({0.25, 0.25});

    auto modulatorSustain = new UIKnob(PARAM_MODULATOR_SUSTAIN, "knob_small_x4");
    modulatorSustain->setOrigin({72/2,72/2});
    modulatorSustain->setPos({modulatorXOffset + 249, 313});
    modulatorSustain->setScale({0.25, 0.25});

    auto modulatorRelease = new UIKnob(PARAM_MODULATOR_RELEASE, "knob_small_x4");
    modulatorRelease->setOrigin({72/2,72/2});
    modulatorRelease->setPos({modulatorXOffset + 345, 313});
    modulatorRelease->setScale({0.25, 0.25});

    // general
    auto modulation = new UIKnob(PARAM_MODULATION, "knob_big_x4");
    modulation->setOrigin({142/2,142/2});
    modulation->setPos({9 + 888 + 25, 2 + 24 + 45});
    modulation->setScale({0.25, 0.25});

    auto feedback = new UIKnob(PARAM_FEEDBACK, "knob_big_x4");
    feedback->setOrigin({142/2,142/2});
    feedback->setPos({9 + 888 + 25, 389 - 142 +   9+6});
    feedback->setScale({0.25, 0.25});


    this->vananagui->insertUIElement(modulation);
    this->vananagui->insertUIElement(feedback);

    this->vananagui->insertUIElement(carrierAM);
    this->vananagui->insertUIElement(carrierVibrato);
    this->vananagui->insertUIElement(carrierSustained);
    this->vananagui->insertUIElement(carrierRect);
    this->vananagui->insertUIElement(carrierKeyScaling);
    this->vananagui->insertUIElement(carrierLevel);
    this->vananagui->insertUIElement(carrierMult);
    this->vananagui->insertUIElement(carrierAttack);
    this->vananagui->insertUIElement(carrierDecay);
    this->vananagui->insertUIElement(carrierSustain);
    this->vananagui->insertUIElement(carrierRelease);

    this->vananagui->insertUIElement(modulatorAM);
    this->vananagui->insertUIElement(modulatorVibrato);
    this->vananagui->insertUIElement(modulatorSustained);
    this->vananagui->insertUIElement(modulatorRect);
    this->vananagui->insertUIElement(modulatorKeyScaling);
    this->vananagui->insertUIElement(modulatorLevel);
    this->vananagui->insertUIElement(modulatorMult);
    this->vananagui->insertUIElement(modulatorAttack);
    this->vananagui->insertUIElement(modulatorDecay);
    this->vananagui->insertUIElement(modulatorSustain);
    this->vananagui->insertUIElement(modulatorRelease);

    this->vananagui->getWindow().show();
    return true;
}

void VRCEditor::close() {
    SAFE_DELETE_PTR(this->vananagui);
}

void VRCEditor::idle() {
    if (this->vananagui->shouldDraw())
        this->vananagui->render();
}

bool VRCEditor::onWheel(float distance) {
    this->vananagui->render();
    return false;
}
