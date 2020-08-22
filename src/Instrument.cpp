#include "Instrument.h"
#include "common.h"

Instrument::Instrument(OPLL* opll) {
    this->opll = opll;
    this->r._00 = 0x13;
    this->r._01 = 0x01;
    this->r._02 = 0x99;
    this->r._03 = 0x00;
    this->r._04 = 0xf2;
    this->r._05 = 0xc4;
    this->r._06 = 0x21;
    this->r._07 = 0x23;
}

void Instrument::applyInstrumentData() {
    OPLL_writeReg(this->opll, 0x00, this->r._00);
    OPLL_writeReg(this->opll, 0x01, this->r._01);
    OPLL_writeReg(this->opll, 0x02, this->r._02);
    OPLL_writeReg(this->opll, 0x03, this->r._03);
    OPLL_writeReg(this->opll, 0x04, this->r._04);
    OPLL_writeReg(this->opll, 0x05, this->r._05);
    OPLL_writeReg(this->opll, 0x06, this->r._06);
    OPLL_writeReg(this->opll, 0x07, this->r._07);
}

INSTRUMENT_REGISTERS* Instrument::getRegisterData() {
    return &this->r;
}

/*
 * Modulator mult
 */
void Instrument::setCarrierMultFactor(uint8_t mult) {
    this->r._00 = (this->r._00 & 0xf0) | mult;
}
void Instrument::setModulatorMultFactor(uint8_t mult) {
    this->r._01 = (this->r._01 & 0xf0) | mult;
}
uint8_t Instrument::getCarrierMultFactor() {
    return this->r._00 & 0x0f;
}
uint8_t Instrument::getModulatorMultFactor() {
    return this->r._01 & 0x0f;
}

/*
 * Amplitude modulation
 */
void Instrument::setCarrierAM(bool enabled) {
    this->r._00 = (this->r._00 & ~0x80) | (enabled << 7);
}
void Instrument::setModulatorAM(bool enabled) {
    this->r._01 = (this->r._01 & ~0x80) | (enabled << 7);
}
bool Instrument::getCarrierAM() {
    return this->r._00 & 0x80;
}
bool Instrument::getModulatorAM() {
    return this->r._01 & 0x80;
}

/*
 * Vibrato
 */
void Instrument::setCarrierVibrato(bool enabled) {
    this->r._00 = (this->r._00 & ~0x40) | (enabled << 6);
}
void Instrument::setModulatorVibrato(bool enabled) {
    this->r._01 = (this->r._01 & ~0x40) | (enabled << 6);
}
bool Instrument::getCarrierVibrato() {
    return this->r._00 & 0x40;
}
bool Instrument::getModulatorVibrato() {
    return this->r._01 & 0x40;
}

/*
 * Sustainance
 */
void Instrument::setCarrierSustained(bool enabled) {
    this->r._00 = (this->r._00 & ~0x20) | (enabled << 5);
}
void Instrument::setModulatorSustained(bool enabled) {
    this->r._01 = (this->r._01 & ~0x20) | (enabled << 5);
}
bool Instrument::getCarrierSustained() {
    return this->r._00 & 0x20;
}
bool Instrument::getModulatorSustained() {
    return this->r._01 & 0x20;
}

/*
 * Rate key scale.
 */
void Instrument::setCarrierRate(bool enabled) {
    this->r._00 = (this->r._00 & ~0x10) | (enabled << 4);
}
void Instrument::setModulatorRate(bool enabled) {
    this->r._01 = (this->r._01 & ~0x10) | (enabled << 4);
}
bool Instrument::getCarrierRate() {
    return (this->r._00 & 0x10) >> 4;
}
bool Instrument::getModulatorRate() {
    return (this->r._01 & 0x10) >> 4;
}

/*
 * Level key scale; 2-bit integer
 */
void Instrument::setCarrierLevel(uint8_t level) {
    this->r._02 = (this->r._02 & ~0xC0) | (level << 6);
}
void Instrument::setModulatorLevel(uint8_t level) {
    this->r._03 = (this->r._03 & ~0xC0) | (level << 6);
}
uint8_t Instrument::getCarrierLevel() {
    return (this->r._02 & 0xC0) >> 6;
}
uint8_t Instrument::getModulatorLevel() {
    return (this->r._03 & 0xC0) >> 6;
}

/*
 * Modulated wave total level; 6-bit integer, 63 - 0.
 */
void Instrument::setModulation(uint8_t x) {
    this->r._02 = (this->r._02 & ~0x3F) | (63 - x);
}
uint8_t Instrument::getModulation() {
    return 63 - (this->r._02 & 0x3F);
}

/*
 * Feedback; 3-bit integer, 0 - 7.
 */
void Instrument::setFeedback(uint8_t x) {
    this->r._03 = (this->r._03 & ~0x07) | x;
}
uint8_t Instrument::getFeedback() {
    return (this->r._03 & 0x07);
}

/*
 * Wave rectifications.
 */
void Instrument::setCarrierRect(bool enabled) {
    this->r._03 = (this->r._03 & ~0x08) | (enabled << 3);
}
void Instrument::setModulatorRect(bool enabled) {
    this->r._03 = (this->r._03 & ~0x10) | (enabled << 4);
}
bool Instrument::getCarrierRect() {
    return (this->r._03 & 0x08);
}
bool Instrument::getModulatorRect() {
    return (this->r._03 & 0x10);
}

/*
 * ADSR; 4-bit integers, 0 - 15.
 */
void Instrument::setCarrierAttack(uint8_t x) {
    this->r._04 = (this->r._04 & ~0xF0) | (x << 4);
}
void Instrument::setCarrierDecay(uint8_t x) {
    this->r._04 = (this->r._04 & ~0x0F) | x;
}
void Instrument::setCarrierSustain(uint8_t x) {
    this->r._06 = (this->r._06 & ~0xF0) | (x << 4);
}
void Instrument::setCarrierRelease(uint8_t x) {
    this->r._06 = (this->r._06 & ~0x0F) | x;
}
void Instrument::setModulatorAttack(uint8_t x) {
    this->r._05 = (this->r._05 & ~0xF0) | (x << 4);
}
void Instrument::setModulatorDecay(uint8_t x) {
    this->r._05 = (this->r._05 & ~0x0F) | x;
}
void Instrument::setModulatorSustain(uint8_t x) {
    this->r._07 = (this->r._07 & ~0xF0) | (x << 4);
}
void Instrument::setModulatorRelease(uint8_t x) {
    this->r._07 = (this->r._07 & ~0x0F) | x;
}

uint8_t Instrument::getCarrierAttack() {
    return (this->r._04 & 0xF0) >> 4;
}
uint8_t Instrument::getCarrierDecay() {
    return (this->r._04 & 0x0F);
}
uint8_t Instrument::getCarrierSustain() {
    return (this->r._06 & 0xF0) >> 4;
}
uint8_t Instrument::getCarrierRelease() {
    return (this->r._06 & 0x0F);
}
uint8_t Instrument::getModulatorAttack() {
    return (this->r._05 & 0xF0) >> 4;
}
uint8_t Instrument::getModulatorDecay() {
    return (this->r._05 & 0x0F);
}
uint8_t Instrument::getModulatorSustain() {
    return (this->r._07 & 0xF0) >> 4;
}
uint8_t Instrument::getModulatorRelease() {
    return (this->r._07 & 0x0F);
}
