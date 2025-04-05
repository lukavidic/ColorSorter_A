#include "wifi_module.h"

volatile char buffer[BUFF_SIZE];
volatile unsigned head = 0, tail = 0;
unsigned char counter = 0;


void wifi_init() {
	/* Set RB12(RX) and RB13(TX) pins to input and output, respectively */
	TRISBbits.TRISB12 = 1;
	TRISBbits.TRISB13 = 0;

	/* TRISBbits.TRISB5 = 1; // FOR DEBUGGIN */
	/* TRISBbits.TRISB7 = 0; // FOR DEBUGGIN */
	
	/* Remap RB12 and RB13 pins to UART RX and TX */
	OSCCONbits.IOLOCK = 0; // Enable remapping

	RPINR18bits.U1RXR = 12; // Remapping RX pin to RB12
	/* RPINR18bits.U1RXR = 5; // Remapping RX pin to RB5 FOR DEBUGGIN UART */

	RPOR6bits.RP13R = 3; // Remapping TX pin to RB13
	/* RPOR3bits.RP7R = 3; // Remapping TX pin to RB7 FOR DEBUGGIN UART */

	OSCCONbits.IOLOCK = 1; // Disable remapping
	
	/* U1MODE register setup */
	U1MODEbits.BRGH = 1; // Baud rate high speed mode
	U1MODEbits.USIDL = 0; // Continue operation even in  idle mode
	/* U1MODEbits.IREN = 0; // irDA encoder/decoder disabled */
	U1MODEbits.UEN = 0b00; // U1RX and U1TX enabled and used; CTS, RTS, BCLK controlled by port latches
	U1MODEbits.LPBACK = 0; // Loopback mode disabled
	U1MODEbits.ABAUD = 0; // Baud rate measurement disabled
	U1MODEbits.URXINV = 0; // Idle state of RX is '1'
	U1MODEbits.PDSEL = 0b00; // 8-bit data, no parity
	U1MODEbits.STSEL = 0; // 1 stop bit (1 for 2 stop bits)
	U1MODEbits.UARTEN = 1; // Enable UART1
	
	/* U1BRG register baud rate setup */
	U1BRG = 34; // calculated by the formula from documentation for baud rate 115200
	
	/* UART1 status and control register setup */
	U1STAbits.UTXISEL1 = 0b01; // Interrupt is generated when the last transmission is over and all transmit operatins are completed
	U1STAbits.UTXINV = 0; // Idle state of U1TX is '1'
	/* U1STAbits.UTXBRK = 0; // Sync break transmission is disabled or completed */
	U1STAbits.UTXEN = 1; // Transmitting enabled
	U1STAbits.URXEN = 1; // Receiving enabled
	U1STAbits.URXISEL = 0b00; // Interrupt flag bit is set when character is received
	U1STAbits.ADDEN = 0; // Address detect mode disabled
	
	/* Configure interrupts */
	__delay_ms(100);
	
	IFS0bits.U1RXIF = 0; // Set flag status bit to 0 for receive
	IFS0bits.U1TXIF = 0; // Set flag status bit to 0 for transmit
	IEC0bits.U1RXIE = 1; // Enable interrupt for receive
	IEC0bits.U1TXIE = 1; // Enable interrupt for transmit
	IPC2bits.U1RXIP = 4; // Set receive interrupt priority to 4
	IPC3bits.U1TXIP = 4; // Set transmit interrupt priority to 4

	IFS4bits.U1ERIF = 0; // Set flag status bit to 0 for receive error
	IEC4bits.U1ERIE = 1; // Enable interrupt error receive
	IPC16bits.U1ERIP = 4; // Set priority of receive error interrupt to 4
	U1STAbits.URXEN = 1; // Enable Receiving

	/* Writing startup commands to wifi module(UART and network configuration) */
	/* wifi_send_string("AT\r\n"); */
	/* __delay_ms(5000); */
	/* wifi_send_string("AT+UART_CUR=115200, 8, 1, 0, 0\r\n"); // UART configuration on wifi: 115200 - baud rate, 8 bit mode, 1 stop bit, 0 parity bits, 0 - disable flow control */
	/* __delay_ms(5000); */
	/* wifi_send_string("AT+CWMODE_CUR=1\r\n"); // Sets wifi module to station mode */
	/* __delay_ms(5000); */
	/* wifi_send_string("AT+CWJAP_CUR=\"etfbl.net\",\"\"\r\n"); // Connect to etfbl.net network */
	/* __delay_ms(5000); */
}

void wifi_init2() {
	/* Set RB5(RX) and RB13(TX) pins to input and output, respectively */

	TRISBbits.TRISB5 = 1; // FOR DEBUGGIN
	TRISBbits.TRISB7 = 0; // FOR DEBUGGIN
	
	/* Remap RB5 and RB7 pins to UART RX and TX */
	OSCCONbits.IOLOCK = 0; // Enable remapping

	RPINR19bits.U2RXR = 5; // Remapping RX pin to RB5 FOR DEBUGGIN UART

	RPOR3bits.RP7R = 5; // Remapping TX pin to RB7 FOR DEBUGGIN UART

	OSCCONbits.IOLOCK = 1; // Disable remapping
	
	/* U1MODE register setup */
	U2MODEbits.UARTEN = 1; // Enable UART1
	U2MODEbits.USIDL = 0; // Continue operation even in  idle mode
	U2MODEbits.IREN = 0; // irDA encoder/decoder disabled
	U2MODEbits.UEN = 0b00; // U1RX and U1TX enabled and used; CTS, RTS, BCLK controlled by port latches
	U2MODEbits.LPBACK = 0; // Loopback mode disabled
	U2MODEbits.ABAUD = 0; // Baud rate measurement disabled
	U2MODEbits.URXINV = 0; // Idle state of RX is '1'
	U2MODEbits.BRGH = 1; // Baud rate high speed mode
	U2MODEbits.PDSEL = 0b00; // 8-bit data, no parity
	U2MODEbits.STSEL = 0; // 1 stop bit (1 for 2 stop bits)
	
	/* U1BRG register baud rate setup */
	U2BRG = 34; // calculated by the formula from documentation for baud rate 115200
	
	/* UART1 status and control register setup */
	U2STAbits.UTXISEL1 = 0b01; // Interrupt is generated when the last transmission is over and all transmit operatins are completed
	U2STAbits.UTXINV = 0; // Idle state of U1TX is '1'
	U2STAbits.UTXBRK = 0; // Sync break transmission is disabled or completed
	U2STAbits.UTXEN = 1; // Transmitting disabled initialy
	U2STAbits.URXEN = 1; // Transmitting disabled initialy
	U2STAbits.URXISEL = 0b00; // Interrupt flag bit is set when character is received
	U2STAbits.ADDEN = 0; // Address detect mode disabled
	
	/* Configure interrupts */
	__delay_ms(100);
	
	IFS1bits.U2RXIF = 0; // Set flag status bit to 0 for receive
	IFS1bits.U2TXIF = 0; // Set flag status bit to 0 for transmit
	IEC1bits.U2RXIE = 1; // Enable interrupt for receive
	IEC1bits.U2TXIE = 1; // Enable interrupt for transmit
	IPC7bits.U2RXIP = 4; // Set receive interrupt priority to 4
	IPC7bits.U2TXIP = 4; // Set transmit interrupt priority to 4

	IFS4bits.U2ERIF = 0; // Set flag status bit to 0 for receive error
	IEC4bits.U2ERIE = 1; // Enable interrupt error receive
	IPC16bits.U2ERIP = 4; // Set priority of receive error interrupt to 4
	U2STAbits.URXEN = 1; // Enable Receiving

	/* Writing startup commands to wifi module(UART and network configuration) */
	/* wifi_send_string("AT\r\n"); */
	/* __delay_ms(5000); */
	/* wifi_send_string("AT+UART_CUR=115200, 8, 1, 0, 0\r\n"); // UART configuration on wifi: 115200 - baud rate, 8 bit mode, 1 stop bit, 0 parity bits, 0 - disable flow control */
	/* __delay_ms(5000); */
	/* wifi_send_string("AT+CWMODE_CUR=1\r\n"); // Sets wifi module to station mode */
	/* __delay_ms(5000); */
	/* wifi_send_string("AT+CWJAP_CUR=\"etfbl.net\",\"\"\r\n"); // Connect to etfbl.net network */
	/* __delay_ms(5000); */
}
void wifi_send_character(uint8_t character) {
	while(U1STAbits.UTXBF);
	U1TXREG = character;
	while(!U1STAbits.TRMT);
}
void wifi_send_character2(uint8_t character) {
	while(U2STAbits.UTXBF);
	U2TXREG = character;
	while(!U2STAbits.TRMT);
}

void wifi_send_string(uint8_t* string) {
	while(*string) {
		wifi_send_character(*string++);
	}
}
void wifi_send_string2(uint8_t* string) {
	while(*string) {
		wifi_send_character2(*string++);
	}
}

void __attribute__((interrupt(auto_psv))) _U1ErrInterrupt(void) {
	IFS4bits.U1ERIF = 0;
	U1STAbits.OERR = 0;
}
void __attribute__((interrupt(auto_psv))) _U2ErrInterrupt(void) {
	IFS4bits.U2ERIF = 0;
	U2STAbits.OERR = 0;
}

void __attribute__((interrupt(auto_psv))) _U1RXInterrupt(void) {
	IFS0bits.U1RXIF = 0;
	buffer[head] = U1RXREG;

	// TEMPORARY, JUST FOR TESTING:
	/* for(int i=tail;i<=head;i++) */
	/* 	wifi_send_character(buffer[i]); */
	/* tail = head; */
	head = (head + 1) % BUFF_SIZE;
}
void __attribute__((interrupt(auto_psv))) _U2RXInterrupt(void) {
	IFS1bits.U2RXIF = 0;
	buffer[head] = U2RXREG;

	// TEMPORARY, JUST FOR TESTING:
	/* for(int i=tail;i<=head;i++) */
	/* 	wifi_send_character(buffer[i]); */
	/* tail = head; */
	head = (head + 1) % BUFF_SIZE;
}

void __attribute__((interrupt(auto_psv))) _U1TXInterrupt(void) {
	IFS0bits.U1TXIF = 0;
}
void __attribute__((interrupt(auto_psv))) _U2TXInterrupt(void) {
	IFS1bits.U2TXIF = 0;
}

void wifi_sendpls() {
	for(int i=0;i<BUFF_SIZE;i++)
		wifi_send_character(buffer[i]);
}
void wifi_sendpls2() {
	for(int i=0;i<BUFF_SIZE;i++)
		wifi_send_character2(buffer[i]);
}
