#pragma once
#include <stdint.h>
#include "emu2413.h"

typedef struct {
    uint8_t _00;
    uint8_t _01;
    uint8_t _02;
    uint8_t _03;
    uint8_t _04;
    uint8_t _05;
    uint8_t _06;
    uint8_t _07;
} INSTRUMENT_REGISTERS;


class Instrument {
public:
    OPLL* opll;
    INSTRUMENT_REGISTERS r;



    /**
     * @constructor
     */
    Instrument(OPLL* opll);

    /**
     * Apply the custom instrument parameters.
     */
    void applyInstrumentData();

    /**
     * @return INSTRUMENT_REGISTERS*
     */
    INSTRUMENT_REGISTERS* getRegisterData();

    /*
     * Mult factor; 4-bit integer, 0 - 15.
     */
    void setCarrierMultFactor(uint8_t mult);
    void setModulatorMultFactor(uint8_t mult);
    uint8_t getCarrierMultFactor();
    uint8_t getModulatorMultFactor();

    /*
     * Amplitude modulation
     */
    void setCarrierAM(bool enabled);
    void setModulatorAM(bool enabled);
    bool getCarrierAM();
    bool getModulatorAM();

    /*
     * Vibrato
     */
    void setCarrierVibrato(bool enabled);
    void setModulatorVibrato(bool enabled);
    bool getCarrierVibrato();
    bool getModulatorVibrato();

    /*
     * Sustained.
     */
    void setCarrierSustained(bool enabled);
    void setModulatorSustained(bool enabled);
    bool getCarrierSustained();
    bool getModulatorSustained();

    /*
     * Level key scale; 2-bit integer, 0 - 3.
     */
    void setCarrierLevel(uint8_t level);
    void setModulatorLevel(uint8_t level);
    uint8_t getCarrierLevel();
    uint8_t getModulatorLevel();

    /*
     * Rate key scale.
     */
    void setCarrierRate(bool enabled);
    void setModulatorRate(bool enabled);
    bool getCarrierRate();
    bool getModulatorRate();

    /*
     * Modulated wave total level; 6-bit integer, 63 - 0.
     */
    void setModulation(uint8_t x);
    uint8_t getModulation();

    /*
     * Feedback; 3-bit integer, 0 - 7.
     */
    void setFeedback(uint8_t x);
    uint8_t getFeedback();

    /*
     * Wave rectifications.
     */
    void setCarrierRect(bool enabled);
    void setModulatorRect(bool enabled);
    bool getCarrierRect();
    bool getModulatorRect();

    /*
     * ADSR; 4-bit integers, 0 - 15.
     */
    void setCarrierAttack(uint8_t x);
    void setCarrierDecay(uint8_t x);
    void setCarrierSustain(uint8_t x);
    void setCarrierRelease(uint8_t x);
    void setModulatorAttack(uint8_t x);
    void setModulatorDecay(uint8_t x);
    void setModulatorSustain(uint8_t x);
    void setModulatorRelease(uint8_t x);
    uint8_t getCarrierAttack();
    uint8_t getCarrierDecay();
    uint8_t getCarrierSustain();
    uint8_t getCarrierRelease();
    uint8_t getModulatorAttack();
    uint8_t getModulatorDecay();
    uint8_t getModulatorSustain();
    uint8_t getModulatorRelease();
};
