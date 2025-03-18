#include "../inc/inc.h"
#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../led/led.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

#pragma config FNOSC = FRC    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

void pins_init();

int main() {
	pins_init();
	servo_init();
	button_init();
	wifi_init();
	led_init();
	color_sensor_init();

	I2C1CONLbits.PEN = 1;
	I2C1CONLbits.SEN = 1;
	while(IFS1bits.MI2C1IF == 0);
	TRISAbits.TRISA2 = 0;
	LATAbits.LATA2 = 1;
	IFS1bits.MI2C1IF = 0;

	I2C1TRN = 0b00010000;
	I2C1TRN = 0x00;
	I2C1TRN = 0x00;
	I2C1TRN = 0x00;

}

void pins_init() {
	ANSB = 0;
	ANSA = 0;
}
