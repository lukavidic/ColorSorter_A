#include "wifi_module.h"

volatile char buffer[BUFF_SIZE];
volatile unsigned head = 0, tail = 0;
unsigned char counter = 0;

/* IP ADDRESS OF ESP8266EX ON ETFBL.NET IS: 10.99.146.44 */

const char* CMD_AT = "AT\r\n";
const char* CMD_BAUDR = "AT+UART_DEF=115200,8,1,0,0\r\n";
const char* CMD_RST = "AT+RST\r\n";
const char* CMD_MODE = "AT+CWMODE=1\r\n";
const char* CMD_LIST_AP = "AT+CWLAP\r\n";
const char* CMD_WIFI_CONN = "AT+CWJAP=\"etfbl.net\",\"\"\r\n";
const char* CMD_CONN_TYPE = "AT+CIPMUX=0\r\n";
const char* CMD_START_TCP = "AT+CIPSTART=\"TCP\",\"10.99.171.98\",8084\r\n";
const char* CMD_SEND = "AT+CIPSEND=3\r\n";
const char* CMD_IP = "AT+CIFSR\r\n";

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
	
	__delay_ms(100);
	
	/* Configure interrupts */
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

}

void wifi_setup_connection() {
    clean_buffer();
    wifi_send_string(CMD_AT); 
    __delay_ms(1000);
    
    clean_buffer();
    wifi_send_string(CMD_BAUDR); 
    __delay_ms(1000);
    
    /* clean_buffer(); */
    /* wifi_send_string(CMD_RST); */
    /* __delay_ms(1000); */
    
    clean_buffer();
    wifi_send_string(CMD_MODE);
    __delay_ms(1000);
    
    clean_buffer();
    wifi_send_string(CMD_WIFI_CONN);
    __delay_ms(1000);
    
    clean_buffer();
    wifi_send_string(CMD_CONN_TYPE);
    __delay_ms(1000);
    
	__delay_ms(3000);
    clean_buffer();
    wifi_send_string(CMD_IP);
	__delay_ms(1000);
    clean_buffer();

	wifi_send_string(CMD_START_TCP);
	__delay_ms(1000);
	clean_buffer();
}

void wifi_send_app(char* data) {
    wifi_send_string(CMD_START_TCP);
    __delay_ms(100);
    
    wifi_send_string(CMD_SEND);
    __delay_ms(100); 
    
    wifi_send_string(data);
    __delay_ms(100);
}

void clean_buffer() {
	for(int i=0;i<BUFF_SIZE;i++) {
		buffer[i] = 0;
	}
	head = tail = 0;
}

void wifi_send_character(uint8_t character) {
	while(U1STAbits.UTXBF);
	U1TXREG = character;
	while(!U1STAbits.TRMT);
}

void wifi_send_string(uint8_t* string) {
	while(*string) {
		wifi_send_character(*string++);
	}
}

void __attribute__((interrupt(auto_psv))) _U1ErrInterrupt(void) {
	IFS4bits.U1ERIF = 0;
	U1STAbits.OERR = 0;
}

void __attribute__((interrupt(auto_psv))) _U1RXInterrupt(void) {
    IFS0bits.U1RXIF = 0;  // Reset interrupt flag
    
    if (U1STAbits.URXDA)  // Check if there are data incoming
    { 
        buffer[head] = U1RXREG; // Put data in buffer
		wifi_send_character2(buffer[head]); // FOR DEBUGGIN
        head = (head + 1) % BUFF_SIZE;   // Update pointer
    }  
}

void __attribute__((interrupt(auto_psv))) _U1TXInterrupt(void) {
	IFS0bits.U1TXIF = 0;
}

void wifi_sendpls() {
	for(int i=0;i<BUFF_SIZE;i++)
		wifi_send_character(buffer[i]);
}
