#ifndef BUTTON_H
#define BUTTON_H

#include "../inc/inc.h"

void button_init();
void initPrekidi();

void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void);

#endif