/*
 * File:   main.c
 * Author: Korisnik
 *
 * Created on 11. mart 2025., 12.55
 */

#pragma config FNOSC = FRC    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

#include "xc.h"
#define FCY 4000000UL
#include <libpic30.h>
#define PRESCALER 256

#include <libpic30.h>
int counter = 100;
volatile int servo_state = 0;
void initPins()
{
	ANSELB=0X0000;  
    TRISAbits.TRISA2=0; // RA2->RB4 LED - izlaz
    TRISBbits.TRISB5=1; //RB5 - dugme/reset ulaz
    
	//pocetna stanje led
	LATAbits.LATA2=0;
    LATBbits.LATB5 = 0;
	
}

void initPrekidi()
{

    INTCON2bits.INT0EP=1;
    INTCON2bits.AIVTEN =0;
    
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 1;
    IPC5bits.INT1IP = 7;
    
    OSCCONbits.IOLOCK = 0;
    RPINR0bits.INT1R = 5;
    
    //__builtin_write_OSCCONL(OSCCON & 0xBF); // Otklju?avanje PPS registra
    //RPINR0bits.INT1R = 5; // INT1 povezan na pin RB5 (5 = RB5)
    //__builtin_write_OSCCONL(OSCCON | 0x40); // Zaklju?avanje PPS registra
    
}
void __attribute__((__interrupt__, __auto_psv__)) _INT1Interrupt(void) 
{
	IFS0bits.INT0IF=0;	
	//ledState=~ledState;
    LATAbits.LATA2=~LATAbits.LATA2; 
               
}
void PWM_init() {

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
	OC1R = 300;
    PR2 = 9999;
    OC1RS = PR2;
     
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    //T2CONbits.TON = 0; 
    if(servo_state == 0){
        OC1R = 300; 
        servo_state = 1;
        //OC1R = 0;
    } else {
        OC1R = 1100;  
        servo_state = 0;
        //OC1R = 0;
    }
    //T2CONbits.TON = 1;
}

void Timer1_init(){
    
    T1CON = 0;
    T1CONbits.TCKPS = 0b11; 
    PR1 = 15000; 
    TMR1 = 0; 

    IFS0bits.T1IF = 0; 
    IEC0bits.T1IE = 1; 
    T1CONbits.TON = 1;
    
   
}
int main(void) {
    
    ANSB = 0x0000;
    //TRISB = 0;
    PWM_init();
    INTCON2bits.GIE = 1; 
    Timer1_init();
   
   int beg = 100; 
     while(1) {
        
    }   
    return 0;
}

