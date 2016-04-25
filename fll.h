// Frame Level Language
// interface

#pragma once

#include "mbed.h"
#include "rtos.h"
#include <stdint.h>

#define MAIL_BOX_SIZE 16

#define FRAME (1.0 / 59.94) // 1 frame (sec)

#define R1       0x0001
#define TRIANGLE 0x0002
#define CIRCLE   0x0004
#define CROSS    0x0008
#define SQUARE   0x0010
#define R2       0x0020
#define START    0x0040
#define ANALOG   0x0080
#define SELECT   0x0100
#define L2       0x0200
#define UP       0x0400
#define DOWN     0x0800
#define LEFT     0x1000
#define RIGHT    0x2000
#define L1       0x4000
#define BUTTON_NUM   15

typedef uint16_t button_t;

// Base object for stream network
class Producer
{
public:
    virtual button_t await() = 0;
    virtual bool is_finished() = 0;
    virtual void reset() = 0;
};

// Sink is the end of button stream network
// It sends next button to Output object through rtos::Mail
class Sink
{
private:
    Producer* source;
    rtos::Mail<button_t, MAIL_BOX_SIZE>* mail_box;
    Mutex* mutex;
    button_t temporary[MAIL_BOX_SIZE];
    int temporary_size;
    bool paused;
public:
    Sink(Producer* src, rtos::Mail<button_t, MAIL_BOX_SIZE>* box, Mutex* mut);
    void run();
    void resume();
    void pause();
    void restart(); // reset source
    void reset(Producer* src); // reset with new source
};

class FLL
{
public:
    FLL(Producer* p);
    ~FLL();
    void press(button_t btn);
    void run();
private:
    Producer *producer;
    DigitalOut* pin[BUTTON_NUM];
    int off[BUTTON_NUM];
};

// Output reads the next button from mail box and press the button
// It must be run per 1/60 sec
class Output
{
private:
    rtos::Mail<button_t, MAIL_BOX_SIZE>* mail_box;
    FLL *fll;
public:
    Output(rtos::Mail<button_t, MAIL_BOX_SIZE>* box, FLL *f);
    void run();
};