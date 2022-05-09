#ifndef EVLAMP_INPUT_H_
#define EVLAMP_INPUT_H_

#include "common.h"

enum {
    INPUT_BTN_MAIN = 0,
    INPUT_BTN_RESET,
    INPUT_BTN_UP,
    INPUT_BTN_DOWN,

    INPUT_BTN_MAX
};

esp_err_t input_init();

#endif /* EVLAMP_INPUT_H_ */
