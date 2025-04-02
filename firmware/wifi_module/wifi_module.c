#include "wifi_module.h"

void wifi_init() {
	/* Set RB12(RX) and RB13(TX) pins to input and output, respectively */
	/*TRISBbits.TRISB12 = 0;
	TRISBbits.TRISB13 = 0;
	
	/* Remap RB12 and RB13 pins to UART RX and TX /
	OSCCONbits.IOLOCK = 0; // Enable remapping
	RPINR5bits.U1RXR = 12; // Remapping RX pin to RB12
	RPOR6bits.RP13R = 3; // Remapping TX pin to RB13
	OSCCONbits.IOLOCK = 1; // Disable remapping
	
	/* U1MODE register setup /
	UART1bits.UARTEN = 1; // Enable UART1
	UART1bits.USIDL = 0; // Continue operation even in  idle mode
	UART1bits.IREN = 0; // irDA encoder/decoder disabled
	UART1bits.UEN = 0b00 // U1RX and U1TX enabled and used; CTS, RTS, BCLK controlled by port latches
	UART1bits.LPBACK = 0; // Loopback mode disabled
	UART1bits.ABAUD = 0; // Baud rate measurement disabled
	UART1bits.U1RX = 0; // Idle state of RX is '1'
	UART1bits.BRGH = 1; // Baud rate high speed mode
	UART1bits.PDSEL = 0b00; // 8-bit data, no parity
	UART1bits.STSEL = 0; // 1 stop bit (1 for 2 stop bits)
	
	/* U1BRG register baud rate setup /
	U1BRG = 8; // calculated by the formula from documentation for baud rate 115200
	
	/* UART1 status and control register setup /
	U1STAbits.UT1ISEL = 0b01; // Interrupt is generated when the last transmission is over and all transmit operatins are completed
	U1STAbits.UT1INV = 0; // Idle state of U1TX is '1'
	U1STAbits.UT1BRK = 0; // Sync break transmission is disabled or completed
	U1STAbits.UT1EN = 0; // Transmitting disabled initialy
	U1STAbits.UR1ISEL = 0b11 // Interrupt flag bit is set when character is received
	U1STAbits.ADDEN = 0; // Address detect mode disabled
	
	__delay_ms(100);
	
	/* Configure interrupts /
	IFS0bits.U1RX = 0; // Set flag status bit to 0 for receive
	IFS0bits.U1TX = 0; // Set flag status bit to 0 for transmit
	IEC0bits.U1RX = 1; // Enable interrupt for receive
	IEC0bits.U1TX = 1; // Enable interrupt for transmit
	IPC2bits.U1RX = 4; // Set receive interrupt priority to 4
	IPC3bits.U1TX = 4; // Set transmit interrupt priority to 4

	IFS4bits.U1E = 0; // Set flag status bit to 0 for receive error
	IEC4bits.U1E = 1; // Enable interrupt error receive
	IPC16bits.U1E = 4; // Set priority of receive error interrupt to 4*/
}

void wifi_send_character(uint8_t character) {
	//U1STAbits.UT1EN = 1; // Enable transmitting
}
