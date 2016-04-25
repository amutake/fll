#include "mbed.h"
#include "rtos.h"
#include "fll.h"
#include "fllaux.h"
#include "taiko.h"

int main(void)
{
    note don1 = { Don, 1 };
    note don05 = { Don, 0.5 };
    note don025 = { Don, 0.25 };
    note un1 = { Un, 1 };
    note un05 = { Un, 0.5 };
    note ka1 = { Ka, 1 };
    note ka05 = { Ka, 0.5 };
    note ka025 = { Ka, 0.25 };
    note renda2 = { Renda, 2 };
    note dondai05 = { DonDai, 0.5 };
    note dondai1 = { DonDai, 1 };
    note kadai05 = { KaDai, 0.5 };
    note kadai1 = { KaDai, 1 };

    button_t go_bs[] = {
        CIRCLE, CIRCLE,
    };

    Producer* go = new OnetimeSource(go_bs, sizeof(go_bs) / sizeof(button_t));

    // Koibumi2000
    note* note_seq[] = {
        &un1, &un1, &un1, &un1,
        &un1, &un1, &un05, &dondai05, &un1,

        &don05, &don05, &un1, &un1, &un1,
        &un1, &un1, &un1, &un1,

        &don05, &don05, &un1, &un1, &un1,
        &dondai1, &un1, &dondai1, &un1,

        //
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don1,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don1,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don1,
        &don05, &don05, &ka05, &ka05, &don05, &don05, &ka1,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don1,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don1,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don1,
        &don05, &don05, &ka05, &ka05, &don05, &don05, &ka1,

        //
        &dondai1, &un1, &don025, &don025, &don025, &don025, &don1, //

        //
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &don05, &ka05, &ka05, &don05, &don05, &ka1,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &kadai05, &un1,

        //
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &kadai05, &kadai05, &kadai05,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don025, &don025, &don025, &don025, &don1,

        //
        &don025, &don025, &don05, &un05, &don025, &don025, &don05, &un05, &dondai1,
        &don025, &don025, &don05, &un05, &don025, &don025, &don05, &un05, &dondai1,

        &don025, &don025, &don05, &un05, &don025, &don025, &don05, &un05, &kadai1,
        &don025, &don025, &don05, &un05, &don025, &don025, &don05, &un05, &kadai1,

        &don025, &don025, &don05, &un05, &ka025, &ka025, &ka05, &un05, &dondai1,
        &don025, &don025, &don05, &un05, &ka025, &ka025, &ka05, &un05, &dondai1,

        &don025, &don025, &don05, &un05, &don025, &don025, &don05, &un05, &kadai1,
        &don025, &don025, &don05, &un05, &don025, &don025, &don05, &un05, &kadai1,

        //
        &don05, &don05, &don1, &don05, &don025, &don025, &don05, &don05,
        &don05, &don05, &don1, &don05, &don025, &don025, &don05, &don05,

        &don05, &don05, &don1, &don05, &don025, &don025, &don05, &don05,
        &don05, &don025, &don025, &don05, &don05, &don05, &don025, &don025, &don05, &don05,

        &don05, &ka05, &don05, &ka05, &don05, &don025, &don025, &don05, &don05,
        &don05, &ka05, &don05, &ka05, &don05, &don025, &don025, &don05, &don05,

        &don05, &ka05, &don05, &ka05, &don05, &don025, &don025, &don05, &ka05,
        &don05, &don025, &don025, &don05, &ka05, &don05, &ka025, &ka025, &ka05, &ka05,

        //
        &don025, &don025, &don05, &don025, &don025, &don05, &don05, &don05, &don05, &don05,
        &don025, &don025, &don05, &don025, &don025, &don05, &don05, &don05, &don05, &don05,

        &don025, &don025, &don05, &don025, &don025, &don05, &don05, &don05, &don05, &don05,
        &don05, &don025, &don025, &don025, &don025, &don05, &don05, &don025, &don025, &don025, &don025,&don05,

        &don025, &don025, &don05, &ka05, &ka05, &don025, &don025, &don05, &ka05, &ka05,
        &don025, &don025, &don05, &ka05, &ka05, &don025, &don025, &don05, &ka05, &ka05,

        &don025, &don025, &don05, &ka05, &ka05, &don025, &don025, &don05, &ka05, &ka05,
        &don025, &don025, &don05, &ka025, &ka025, &ka05, &don025, &don025, &don05, &dondai1,

        // irregular time
        &don05, &don1, &don1, &don1, &don05, &un05, &don05, &don05, &don05, &don1, &don05, &don05,
        &un05, &don1, &don1, &don1, &don05, &don05, &don05, &don1, // ?

        &don05, &don1, &don1, &don05, &don05, &don05, &un05, &ka025, &ka025, &ka05, &ka025, &ka025, &ka1, &don05, &don05,
        &un05, &don1, &don05, &don05, &don05, &un05, &ka025, &ka025, &ka05, &ka025, &ka025, &ka05, &un05,

        &don05, &don025, &don025, &don05, &don1, &don1, &ka025, &ka025, &ka05, &ka025, &ka025, &ka05, &don025, &don025, &don1, &don05, &don1, &don1, &don1, &ka025, &ka025, &ka05, &ka025, &ka025, &ka05, &don05, &don05, &don05,

        // normal time
        &don1, &ka1, &don1, &ka1, &don05, &don05, &ka05, &ka05, &don05, &don05, &ka1,
        &don025, &don025, &don05, &ka025, &ka025, &ka05, &don025, &don025, &don05, &ka025, &ka025, &ka05, &don1, &un1, &un05, &don025, &don025, &don05, &don05,

        &don1, &ka1, &don1, &ka1, &don05, &don05, &ka05, &ka05, &don05, &don05, &ka1,
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05, &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05, &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05, &don05, &don05, &don05, &don05,

        &don1, &ka1, &don1, &ka1, &dondai1, &kadai1, &dondai1, &kadai1,
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka025, &ka025,
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka025, &ka025,
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka025, &ka025,
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05,

        &renda2, &un05, &don025, &don025, &don1,

        // switch (tatsujin)
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,

        &don05, &ka05, &don05, &ka05, &don05, &ka05, &don05, &ka05,
        &don05, &ka05, &don05, &ka05, &don05, &kadai05, &un05, &kadai05,

        //
        &don025, &don025, &don05, &don025, &don025, &don05, &don05, &don05, &don05, &don05,
        &don025, &don025, &don05, &don025, &don025, &don05, &don05, &don05, &don05, &don05,

        &don025, &don025, &don05, &don025, &don025, &don05, &don05, &don05, &don05, &don05,
        &don05, &don05, &don05, &don05, &don05, &don025, &don025, &don025, &don025, &don05,
        //
        &don025, &don025, &don05, &ka025, &ka025, &ka05, &don025, &don025, &don05, &ka025, &ka025, &ka05,
        &don025, &don025, &don05, &ka025, &ka025, &ka05, &don025, &don025, &don05, &ka025, &ka025, &ka05,

        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka025, &ka025, &don025, &don025, &don025, &don025, &ka025, &ka025, &ka025, &ka025,
        &don025, &don025, &don05, &ka05, &ka05, &don025, &don025, &don025, &don025, &don05, &ka05,
        //
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05, &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05,
        &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05, &don025, &don025, &don025, &don025, &ka025, &ka025, &ka05,

        &don025, &don025, &ka025, &ka025, &don025, &don025, &ka025, &ka025, &don025, &don025, &ka025, &ka025, &don025, &don025, &ka025, &ka025,
        &don05, &don05, &don05, &don05, &don025, &don025, &don025, &don025, &don025, &don025, &don05,

        // irregular time
        &don05, &don05, &un05, &don05, &un05, &don05, &un05, &don05, &un05, &don05, &don05, &don05, &don1,
        &don05, &don05, &un05, &don05, &un05, &don05, &un05, &don05, &un05, &don05, &don05, &don05, &don1,

        // last
        &dondai05, &dondai05
    };

    Producer* taiko = new TaikoSource(note_seq, sizeof(note_seq)/sizeof(note*), 200);

    Producer* delayed_taiko = new DelayFlow(taiko, 9.068);

    button_time menu_bs[] = {
        space(40), single(CIRCLE), space(3), single(CIRCLE), space(4)
    };

    Producer* back = new EasySource(menu_bs, sizeof(menu_bs) / sizeof(menu_bs[0]));

    Producer* src_seq[] = { go, delayed_taiko, back };

    Producer* oneset = new SequentialFlow(src_seq, sizeof(src_seq) / sizeof(src_seq[0]));

    Producer* source = new RepeaterFlow(oneset);

    FLL fll(source);
    fll.run();
}
