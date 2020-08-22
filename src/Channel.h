#pragma once
#include <stdint.h>
#include "emu2413.h"
#include "Instrument.h"



class Channel {
public:

    bool isPlaying = false;
    int currentNote = 0;
    int channel = 0;
    OPLL* opll = nullptr;
    Instrument* instrument;
    uint8_t reg2x = 0x00;
    uint8_t reg3x = 0x00;

    /**
     * @constructor
     *
     * @param OPLL* opll
     * @param Instrument* instrument
     * @param int channel
     */
    Channel(OPLL* opll, Instrument* instrument, int channel);

    /**
     * Triggers a note on the channel.
     *
     * @param int note - MIDI note number
     * @param int velocity - MIDI note volume.
     */
    void trigger(int note, int velocity);

    /**
     * Stop playing the note.
     */
    void release();

    /**
     * Set instrument volume.
     *
     * @param uint8_t volume - 4 bit integer, max=15, min=0.
     */
    void applyVolume(uint8_t volume);

};
