#include "button.h"
#include "../led/led.h"
#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../inc/inc.h"
//#define BUTTON PORTBbits.RB5
//#define LED LATAbits.LATA2
//int reset_counter=1;        //ako je reset_couter=1 -> zautsvi sistem , reset_counetr=2 -> pokreni sistem
int reset_counter = 1;
volatile int press_counter = 1;

void button_init() {
    
    TRISBbits.TRISB5 = 1;  // RB5 -> Dugme (ulaz)
    initPrekidi();
}
    
void initPrekidi() {
    /*__builtin_write_OSCCONL(OSCCON & ~(1 << 6));    // Otkljucavanje PPS registra
    RPINR0bits.INT1R = 5;                           // Mapiranje RB5 (RP5) na INT1
    __builtin_write_OSCCONL(OSCCON | (1 << 6));     // Zakljucavanje PPS registra*/

    INTCON2bits.INT1EP = 1;                         // 1 = Silazna ivica (prekid pri pritisku dugmeta)
    
    IFS1bits.INT1IF = 0;                            // Ocisti INT1 zastavicu prekida
    IEC1bits.INT1IE = 1;                            // Omoguci INT1 prekid
    IPC5bits.INT1IP = 7;                            // Postavi prioritet INT1 prekida (najvici)

    INTCON2bits.GIE = 1;                            // Omoguci globalne prekide
}

void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0;

    press_counter++;

    if (press_counter % 2 == 0) {
        led_off();
        servo_center();
        WS2812_SetColor(0, 0, 0);
        //i2c_write2ByteRegister(VEML3328_SLAVE_ADD, CONF, 0x8011);
        reset_counter = 0;
    } else {
        led_on();
        WS2812_SetColor(128, 128, 128);
        //i2c_write2ByteRegister(VEML3328_SLAVE_ADD, CONF, 0x0400);
        reset_counter = 1;
    }
    __delay_ms(300); 
}
