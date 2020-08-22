#include "Channel.h"
#include "Instrument.h"
#include "emu2413.h"



Channel::Channel(OPLL* opll, Instrument* instrument, int channel) {
    this->opll = opll;
    this->instrument = instrument;
    this->channel = channel;
}


void Channel::release() {
    this->isPlaying = false;

    this->reg2x &= ~0x10; // key off
    OPLL_writeReg(this->opll, 0x20 + this->channel, this->reg2x);
}


void Channel::trigger(int note, int velocity) {
    this->reg2x = 0x30; // key and sustain on;

    // compute frequency and octave
    uint16_t fnumber;
    uint8_t octaveBits = (note / 12) << 1;
    if (octaveBits > 0x0F) { // limit note
        fnumber = 242;
        this->reg2x |= 0x0F;
    } else {
        int fnumbers[12] = { 172,181,192,204,216,229,242,257,272,288,305,323 };
        fnumber = static_cast<uint8_t>(fnumbers[note % 12]);
        this->reg2x |= ((fnumber >> 7) ^ 1) | octaveBits;
    }

    OPLL_writeReg(this->opll, 0x10 + this->channel, static_cast<uint8_t>(fnumber));
    OPLL_writeReg(this->opll, 0x20 + this->channel, this->reg2x);
    this->applyVolume(15 - (velocity * 15 / 127));
    this->instrument->applyInstrumentData();

    this->currentNote = note;
    this->isPlaying = true;

}

void Channel::applyVolume(uint8_t volume) {
    this->reg3x = volume & 0x0F;
    this->reg3x |= 0x00 << 4;
    OPLL_writeReg(this->opll, 0x30 + this->channel, this->reg3x);
}
