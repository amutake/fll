// Frame Level Language
// implementation

#include "mbed.h"
#include "rtos.h"
#include "fll.h"

#include <stdint.h>

void invoke_sinkrun(const void *p);

// if you want to debug by `printf`, uncomment next line and put `pc.printf("...", ...);` into certain position.
// Serial pc(USBTX, USBRX);

// ----
// Sink
// ----
Sink::Sink(Producer* src, rtos::Mail<button_t, MAIL_BOX_SIZE>* box, Mutex* mut)
{
    source = src;
    mail_box = box;
    mutex = mut;
    paused = true;
    temporary_size = 0;

}

void Sink::run()
{
    button_t *btn;
    while(1) {
        if (paused) {
            continue;
        }
        mutex->lock();
        btn = mail_box->alloc();
        if(!btn) {
            mutex->unlock();
            continue;
        }
        *btn = source->await();
        mail_box->put(btn);
        mutex->unlock();
    }
}

void Sink::resume()
{
    if (!paused) {
        return;
    }
    button_t *btn;
    mutex->lock();
    for (int i = 0; i < temporary_size; i++) {
        btn = mail_box->alloc();
        *btn = temporary[i];
        mail_box->put(btn);
    }
    mutex->unlock();
    paused = false;
}

void Sink::pause()
{
    if (paused) {
        return;
    }
    paused = true;
    mutex->lock();
    osEvent e;
    int i = 0;
    do {
        e = mail_box->get(0);
        if(e.status == osEventMail) {
            mail_box->free((button_t*)e.value.p);
            temporary[i++] = *(button_t*)(e.value.p);
        }
    } while (e.status == osEventMail);
    temporary_size = i;
    mutex->unlock();
}

void Sink::restart()
{
    mutex->lock();
    osEvent e;
    do {
        e = mail_box->get(0);
        if(e.status == osEventMail) {
            mail_box->free((button_t*)e.value.p);
        }
    } while (e.status == osEventMail);
    // reset source
    source->reset();
    temporary_size = 0;
    mutex->unlock();
}

void Sink::reset(Producer* src)
{
    mutex->lock();
    // consume current (and actually old) buffer
    osEvent e;
    do {
        e = mail_box->get(0);
        if(e.status == osEventMail) {
            mail_box->free((button_t*)e.value.p);
        }
    } while (e.status == osEventMail);
    // update source
    source = src;
    mutex->unlock();
}

// ------
// Output
// ------
Output::Output(rtos::Mail<button_t, MAIL_BOX_SIZE>* box, FLL *f)
{
    mail_box = box;
    fll = f;
}

void Output::run()
{
    osEvent e = mail_box->get(0);
    if (e.status == osEventMail) { // getting is success
        button_t b = *(button_t*)(e.value.p);
        mail_box->free((button_t*)e.value.p);
        fll->press(b);
    } else {
        fll->press(0); // if mail box is empty
    }
}

FLL::FLL(Producer* p)
{
    producer = p;

    // button_no -> pin (R1 to L1)
    PinName ps[] = {
        p5, p6, p7, p8, p9,
        p10, p11, p12, p13, p14, p15,
        p16, p17, p18, p19,
    };

    for(int i = 0; i < BUTTON_NUM; i++) {
        off[i] = 1;
        pin[i] = new DigitalOut(ps[i], off[i]);
    }
}

FLL::~FLL()
{
    for(int i = 0; i < BUTTON_NUM; i++) {
        delete pin[i];
    }
}

void FLL::press(button_t btn)
{
    for(int i = 0; i < BUTTON_NUM; i++) {
        if ((btn >> i) & 0x1) {
            pin[i]->write(!off[i]);
        } else {
            pin[i]->write(off[i]);
        }
    }
}

void FLL::run()
{

    Mail<button_t, MAIL_BOX_SIZE>* mail_box = new Mail<button_t, MAIL_BOX_SIZE>();
    Mutex* mutex = new Mutex();
    Sink* sink = new Sink(producer, mail_box, mutex);
    Output* output = new Output(mail_box, this);

    InterruptIn reset_pin(p21);
    InterruptIn resume_pin(p22);
    InterruptIn pause_pin(p23);
    reset_pin.rise(sink, &Sink::restart);
    resume_pin.rise(sink, &Sink::resume);
    pause_pin.rise(sink, &Sink::pause);

    Thread th(invoke_sinkrun, (void *)sink);

    Ticker ticker;
    ticker.attach(output, &Output::run, FRAME);

    Thread::wait(osWaitForever);
}

static void invoke_sinkrun(const void *p)
{
    ((Sink*)p)->run();
}