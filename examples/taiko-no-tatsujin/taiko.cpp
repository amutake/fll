// Toolkit for Taiko no Tatsujin

#include "fll.h"
#include "taiko.h"

button_t taiko2button(Taiko t)
{

    switch (t) {
        case Un:
            return 0;
        case Don:
            return CIRCLE;
        case Ka:
            return R1;
        case Renda:
            return CIRCLE;
        case DonDai:
            return CIRCLE|LEFT;
        case KaDai:
            return L1|R1;
    }
    return 0;
}

TaikoSource::TaikoSource(note** ns, int s, float b)
{
    note_seq = ns;
    size = s;
    bpm = b;
    index = 0;
    frame_i = 0;
    lag = 0;
}

button_t TaikoSource::await()
{
    if (index >= size) {
        return 0;
    }

    note* n = note_seq[index];
    float len = n->length * (60 / bpm); // note length (sec)
    float passing = frame_i * FRAME; // passing time since the begining of this note (sec)

    frame_i++;

    button_t btn = 0;

    if (n->taiko == Renda) {
        if (passing + FRAME >= len - lag) {
            frame_i = 0;
            index++;
            lag = passing + FRAME - (len - lag);
        } else {
            if (frame_i % 2 == 0) {
                btn = CIRCLE; // TODO
            } else {
                btn = LEFT; // TODO
            }
        }
    } else {
        if (frame_i < 3) { // in the former of this note
            btn = taiko2button(n->taiko);
        } else if (passing + FRAME >= len - lag) { // final frame of this note
            frame_i = 0;
            index++;
            lag = passing + FRAME - (len - lag);
        }
    }
    return btn;
}

bool TaikoSource::is_finished()
{
    return index >= size;
}

void TaikoSource::reset()
{
    index = 0;
    frame_i = 0;
    lag = 0;
}