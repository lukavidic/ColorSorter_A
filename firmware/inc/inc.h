/**
 * @file inc.h
 *
 * This file includes needed libraries
 * and defines needed macros
 */

#ifndef INC
#define INC

#include "xc.h"
#define FCY 16000000UL
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#define FCY 16000000UL
#define VEML3328_SLAVE_ADD 0x10
#define CONF 0x00
#define C_DATA 0x04
#define R_DATA 0x05
#define G_DATA 0x06
#define B_DATA 0x07
#define IR_DATA 0x08
#define PRESCALER 256
#define NUM_SAMPLES 5 
#define NUM_COLORS (sizeof(colors) / sizeof(colors[0]))

#include <libpic30.h>
#include <stdint.h>
#include <string.h>

#endif
