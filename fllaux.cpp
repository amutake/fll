// utility producers

#include "fll.h"
#include "fllaux.h"

// -------------
// OnetimeSource
// -------------
OnetimeSource::OnetimeSource(button_t *bs, int s)
{
    button_seq = bs;
    size = s;
    i = 0;
}

button_t OnetimeSource::await()
{
    if (size <= i) {
        return 0;
    } else {
        return button_seq[i++];
    }
}

bool OnetimeSource::is_finished()
{
    return (size <= i);
}

void OnetimeSource::reset()
{
    i = 0;
}

// --------------
// RepeaterSource
// --------------
RepeaterSource::RepeaterSource(button_t *bs, int s)
{
    button_seq = bs;
    size = s;
    i = 0;
}

button_t RepeaterSource::await()
{
    if(size == 0) {
        return 0;
    } else if(size <= i) {
        i = 0;
    }
    return button_seq[i++];
}

bool RepeaterSource::is_finished()
{
    return false;
}

void RepeaterSource::reset()
{
    i = 0;
}

// ----------
// EasySource
// ----------
button_time single(button_t btn)
{
    button_time bt = { btn, FRAME * 2 };
    return bt;
}

button_time hold(button_t btn, float sec)
{
    button_time bt = { btn, sec };
    return bt;
}

button_time space(float sec)
{
    button_time bt = { 0, sec };
    return bt;
}

EasySource::EasySource(button_time* seq, int size)
{
    bt_seq = seq;
    bt_size = size;
    index = 0;
    frame_i = 0;
    spacing = false;
    space_frame = 2;
}

EasySource::EasySource(button_time* seq, int size, int spacer)
{
    bt_seq = seq;
    bt_size = size;
    index = 0;
    frame_i = 0;
    spacing = false;
    space_frame = spacer;
}

button_t EasySource::await()
{
    if (index >= bt_size) {
        return 0;
    }

    if (spacing) {
        if (frame_i + 1 >= space_frame) { // final frame of spacer
            spacing = false;
            frame_i = 0;
            index++;
        } else {
            frame_i++;
        }
        return 0;
    } else {
        button_time bt = bt_seq[index];
        float passing = frame_i * FRAME; // passing time since the begining of this note (sec)

        if (passing + FRAME >= bt.sec) { // final frame
            frame_i = 0;
            spacing = true;
        } else {
            frame_i++;
        }
        return bt.button;
    }
}

bool EasySource::is_finished()
{
    return index >= bt_size;
}

void EasySource::reset()
{
    index = 0;
    frame_i = 0;
    spacing = false;
}

// --------
// FoldFlow
// --------
FoldFlow::FoldFlow(Producer **srcs, int srcs_size)
{
    sources = srcs;
    sources_size = srcs_size;
}

button_t FoldFlow::await()
{
    button_t *bs = (button_t*) malloc(sizeof(button_t) * sources_size);
    for (int i = 0; i < sources_size; i++) {
        bs[i] = sources[i]->await();
    }
    button_t b = fold(bs, sources_size);
    free(bs);
    return b;
}

bool FoldFlow::is_finished()
{
    for (int i = 0; i < sources_size; i++) {
        if (!sources[i]->is_finished()) {
            return false;
        }
    }
    return true;
}

void FoldFlow::reset()
{
    for (int i = 0; i < sources_size; i++) {
        sources[i]->reset();
    }
}

// ----------
// MergeFlow
// ----------
button_t MergeFlow::fold(button_t *bs, int bs_size)
{
    button_t b = 0x00;
    for(int i = 0; i < bs_size; i++) {
        b |= bs[i];
    }
    return b;
}

// ---------
// DelayFlow
// ---------
DelayFlow::DelayFlow(Producer *src, float d)
{
    source = src;
    delay = d;
    passing = 0;
    end = false;
}

button_t DelayFlow::await()
{
    if (end) {
        return source->await();
    }

    passing += FRAME;
    if (passing >= delay) {
        end = true;
    }
    return 0;
}

bool DelayFlow::is_finished()
{
    return end && source->is_finished();
}

void DelayFlow::reset()
{
    source->reset();
    passing = 0;
    end = false;
}

// --------------
// SequentialFlow
// --------------
SequentialFlow::SequentialFlow(Producer **srcs, int srcs_size)
{
    sources = srcs;
    sources_size = srcs_size;
    index = 0;
}

button_t SequentialFlow::await()
{
    if (index >= sources_size) {
        return 0;
    }

    // FIXME
    // while(sources[index]->is_finished()) {
    //     index++;
    if (sources[index]->is_finished()) {
        index++;
        if (index >= sources_size) {
            return 0;
        }
    }

    return sources[index]->await();
}

bool SequentialFlow::is_finished()
{
    return index >= sources_size;
}

void SequentialFlow::reset()
{
    for (int i = 0; i < sources_size; i++) {
        sources[i]->reset();
    }
    index = 0;
}

// ------------
// RepeaterFlow
// ------------
RepeaterFlow::RepeaterFlow(Producer* src)
{
    source = src;
}

button_t RepeaterFlow::await()
{
    if (source->is_finished()) {
        source->reset();
    }
    return source->await();
}

bool RepeaterFlow::is_finished()
{
    return false;
}

void RepeaterFlow::reset()
{
    source->reset();
}

// ------------
// ExpanderFlow
// ------------
ExpanderFlow::ExpanderFlow(Producer* src, int n)
{
    source = src;
    n_times = n;
    counter = 0;
    now = 0;
    finished = false;
}

button_t ExpanderFlow::await()
{
    // already finished
    if (finished) {
        return 0;
    }

    // else, switch by counter
    if (counter == 0) {
        finished = source->is_finished();
        if (!finished) {
            now = source->await();
            counter++;
        } else {
            now = 0;
        }
    } else if (counter < n_times) {
        counter++;
    } else { // counter >= n_times
        counter = 0;
    }
    return now;
}

bool ExpanderFlow::is_finished()
{
    return finished;
}

void ExpanderFlow::reset()
{
    counter = 0;
    now = 0;
    source->reset();
    finished = false;
}