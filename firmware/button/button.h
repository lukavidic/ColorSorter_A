#ifndef BUTTON_H

#define BUTTON_H

#include "../inc/inc.h"
extern volatile int press_counter;

extern int reset_counter;

void button_init();
void initPrekidi();
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void);

#endif
