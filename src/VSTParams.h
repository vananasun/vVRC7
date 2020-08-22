#pragma once
#include <functional>



typedef struct {
    float value;
    const char* name;
    const char* label;
    std::function<void(float)> setter;
} VSTPARAM;



class VSTParams {

private:
    VSTPARAM* params;

public:
    /**
     * @constructor
     *
     * @param int amount - The number of parameters to allocate.
     */
    VSTParams(int amount);

    /**
     * @destructor
     */
    ~VSTParams();

    /**
     * Access an individual VST parameter.
     */
    VSTPARAM& operator [] (int i) { return this->params[i]; }

};




/*
 * Parameters
 */
#define N_VSTPARAMS 24
#define PARAM_CARRIER_AM             0
#define PARAM_CARRIER_VIBRATO        1
#define PARAM_CARRIER_SUSTAINED      2
#define PARAM_CARRIER_RATE           3
#define PARAM_CARRIER_LEVEL          4
#define PARAM_CARRIER_RECT           5
#define PARAM_CARRIER_MULT           6
#define PARAM_CARRIER_ATTACK         7
#define PARAM_CARRIER_DECAY          8
#define PARAM_CARRIER_SUSTAIN        9
#define PARAM_CARRIER_RELEASE       10
#define PARAM_MODULATOR_AM          11
#define PARAM_MODULATOR_VIBRATO     12
#define PARAM_MODULATOR_SUSTAINED   13
#define PARAM_MODULATOR_RATE        14
#define PARAM_MODULATOR_LEVEL       15
#define PARAM_MODULATOR_RECT        16
#define PARAM_MODULATOR_MULT        17
#define PARAM_MODULATOR_ATTACK      18
#define PARAM_MODULATOR_DECAY       19
#define PARAM_MODULATOR_SUSTAIN     20
#define PARAM_MODULATOR_RELEASE     21
#define PARAM_MODULATION            22
#define PARAM_FEEDBACK              23
