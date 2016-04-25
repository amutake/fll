// utility producers

#pragma once

#include "fll.h"

// -------
// Sources
// -------
// `Source` is a 0 input producer

// yields given button sequence exactly once
class OnetimeSource : public Producer
{
private:
    int i;
    button_t* button_seq;
    int size;
public:
    OnetimeSource(button_t *bs, int s);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

// 0 input and infinitely yields given button sequence
class RepeaterSource : public Producer
{
private:
    int i;
    button_t* button_seq;
    int size;
public:
    RepeaterSource(button_t *bs, int s);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

typedef struct _button_time {
    button_t button;
    float sec;
} button_time; // TODO: rename

button_time single(button_t btn);
button_time hold(button_t btn, float sec);
button_time space(float sec);

// human friendly source
class EasySource : public Producer
{
private:
    button_time* bt_seq;
    int bt_size;
    int index;
    int frame_i;
    bool spacing;
    int space_frame;
public:
    EasySource(button_time* seq, int size);
    EasySource(button_time* seq, int size, int spacer); // spacer is the number of frame
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

// -----
// Flows
// -----
// `Flow` is a N inputs producer

// abstract class for N inputs producer
class FoldFlow : public Producer
{
private:
    Producer** sources;
    int sources_size;
public:
    FoldFlow(Producer **srcs, int srcs_size);
    virtual button_t fold(button_t *bs, int bs_size) = 0;
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

// merge N streams
// Example:
//   [ Up, Right, Down, ... ]
//       +--- MergeFlow ---> [ Up|Circle, Right, Down|Triangle, Up, Right|Circle, ... ]
//   [ Circle, none, Triangle, none, ... ]
class MergeFlow : public FoldFlow
{
public:
    MergeFlow(Producer **srcs, int srcs_size) : FoldFlow(srcs, srcs_size) {}
    virtual button_t fold(button_t *bs, int bs_size);
};

// 1 input, 1 output
// delay input source
class DelayFlow : public Producer
{
private:
    Producer* source;
    float delay; // sec
    float passing;
    bool end;
public:
    DelayFlow(Producer *src, float d);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

// use sources sequentially
class SequentialFlow : public Producer
{
private:
    Producer** sources;
    int sources_size;
    int index;
public:
    SequentialFlow(Producer **srcs, int srcs_size);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

// 1 input, 1 output
// repeats upstream
class RepeaterFlow : public Producer
{
private:
    Producer* source;
public:
    RepeaterFlow(Producer *src);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};

// 1 input, 1 output
// expands upstream N times
class ExpanderFlow : public Producer
{
private:
    Producer* source;
    int n_times;
    int counter;
    button_t now;
    bool finished;
public:
    ExpanderFlow(Producer* src, int n);
    virtual button_t await();
    virtual bool is_finished();
    virtual void reset();
};