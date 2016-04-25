// Toolkit for Taiko no Tatsujin

#pragma once

#include "fll.h"
#include "fllaux.h"

enum Taiko {
    Un = 0,
    Don = 1,
    Ka = 2,
    Renda = 3,
    DonDai = 4,
    KaDai = 5,
};

typedef struct _note {
    Taiko taiko;
    float length;
} note;

class TaikoSource : public Producer
{
private:
    note** note_seq;
    int index;
    int size;
    float bpm;
    int frame_i;
    float lag; // < FRAME
public:
    TaikoSource(note** ns, int s, float b);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};