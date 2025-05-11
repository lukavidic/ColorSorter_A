/**
 * @file button.c
 * @brief Button module source file
 *
 * File containing definitions of functions decleared in button.h
 */

#include "button.h"
#include "../led/led.h"
#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../inc/inc.h"

int reset_counter = 1;
volatile int press_counter = 1;

void button_init() {
    
    TRISBbits.TRISB5 = 1;  // RB5 ->input
    initPrekidi();
}
    
void initPrekidi() {
    

    INTCON2bits.INT1EP = 1;                         // 1 = Falling edge (interrupt on button press)
    
    IFS1bits.INT1IF = 0;                            
    IEC1bits.INT1IE = 1;                            //Enable INT1 interrupt
    IPC5bits.INT1IP = 7;                            //Set INT1 interrupt priority (highest)
    INTCON2bits.GIE = 1;                            //Enable global interrupts
}

void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0;

    press_counter++;

    if (press_counter % 2 == 0) {
        led_off();
        servo_center();
        WS2812_SetColor(0, 0, 0);
        
        reset_counter = 0;
    } else {
        led_on();
        WS2812_SetColor(128, 128, 128);
        
        reset_counter = 1;
    }
    __delay_ms(300); 
}
