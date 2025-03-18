#ifndef INC
#define INC

#pragma config FNOSC = FRC    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

#include "xc.h"
#define FCY 4000000UL
#include <libpic30.h>
#define PRESCALER 256

#include <libpic30.h>

#endif
