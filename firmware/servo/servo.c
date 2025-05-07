/**
 * @file servo.c
 * @brief Servo module source file
 *
 * Contains definitions of functions decleared in servo.h
 */

#include "servo.h"
#include "xc.h"
void servo_init() {
	TRISBbits.TRISB10 = 0;
	OSCCONbits.IOLOCK = 0;
	RPOR5bits.RP10R = 13;
	OSCCONbits.IOLOCK = 1;

	OC1CON2bits.SYNCSEL = 0x1f;
	OC1CON2bits.OCTRIG = 0;
	OC1CON1bits.OCTSEL = 0;
	
	OC1CON1bits.OCM = 6;
    
    T2CONbits.TCKPS = 0b01;
	T2CONbits.TON = 1;
	OC1R = 3200;
    PR2 = 40000;
    OC1RS = PR2;
}

void servo_set_angle(int angle) {
    OC1R = angle;
}

void servo_right() {
	OC1R = 1800;
}

void servo_left() {
	OC1R = 4200;
}

void servo_center() {
	OC1R = 3200;
}
