#include <stdio.h>
#include <stdlib.h>
#include <EFM8LB1.h>

#define SYSCLK 72000000L    // Internal oscillator frequency in Hz
#define BAUDRATE 115200L
#define F_SCK_MAX 2000000L  // Max SCK freq (Hz)

// Pins used by the SPI interface:
// 	P0.0: SCK
// 	P0.1: MISO
// 	P0.2: MOSI
// 	P0.3: SS*

#define ADC_CE P0_3
	
char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
  
	VDM0CN=0x80;       // enable VDD monitor
	RSTSRC=0x02|0x04;  // Enable reset on missing clock detector and VDD

	#if (SYSCLK == 48000000L)	
		SFRPAGE = 0x10;
		PFE0CN  = 0x10; // SYSCLK < 50 MHz.
		SFRPAGE = 0x00;
	#elif (SYSCLK == 72000000L)
		SFRPAGE = 0x10;
		PFE0CN  = 0x20; // SYSCLK < 75 MHz.
		SFRPAGE = 0x00;
	#endif
	
	#if (SYSCLK == 12250000L)
		CLKSEL = 0x10;
		CLKSEL = 0x10;
		while ((CLKSEL & 0x80) == 0);
	#elif (SYSCLK == 24500000L)
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0);
	#elif (SYSCLK == 48000000L)	
		// Before setting clock to 48 MHz, must transition to 24.5 MHz first
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0);
		CLKSEL = 0x07;
		CLKSEL = 0x07;
		while ((CLKSEL & 0x80) == 0);
	#elif (SYSCLK == 72000000L)
		// Before setting clock to 72 MHz, must transition to 24.5 MHz first
		CLKSEL = 0x00;
		CLKSEL = 0x00;
		while ((CLKSEL & 0x80) == 0);
		CLKSEL = 0x03;
		CLKSEL = 0x03;
		while ((CLKSEL & 0x80) == 0);
	#else
		#error SYSCLK must be either 12250000L, 24500000L, 48000000L, or 72000000L
	#endif

	P0MDOUT=0b_0001_1101;//SCK, MOSI, P0.3, TX0 are puspull, all others open-drain
	XBR0=0b_0000_0011;//SPI0E=1, URT0E=1
	XBR1=0b_0000_0000;
	XBR2=0b_0100_0000; // Enable crossbar and weak pull-ups

	#if ( ((SYSCLK/BAUDRATE)/(12L*2L)) > 0x100)
		#error Can not configure baudrate using timer 1 
	#endif
	// Configure Uart 0
	SCON0 = 0x10;
	TH1 = 0x100-((SYSCLK/BAUDRATE)/(12L*2L));
	TL1 = TH1;      // Init Timer1
	TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	TMOD |=  0x20;                       
	TR1 = 1; // START Timer1
	TI = 1;  // Indicate TX0 ready

	// SPI inititialization
	SPI0CKR = (SYSCLK/(2*F_SCK_MAX))-1;
	SPI0CFG = 0b_0100_0000; //SPI in master mode
	SPI0CN0 = 0b_0000_0001; //SPI enabled and in three wire mode
	
	return 0;
}

// Uses Timer3 to delay <us> micro-seconds. 
void Timer3us(unsigned char us)
{
	unsigned char i;               // usec counter
	
	// The input for Timer 3 is selected as SYSCLK by setting T3ML (bit 6) of CKCON0:
	CKCON0|=0b_0100_0000;
	
	TMR3RL = (-(SYSCLK)/1000000L); // Set Timer3 to overflow in 1us.
	TMR3 = TMR3RL;                 // Initialize Timer3 for first overflow
	
	TMR3CN0 = 0x04;                 // Sart Timer3 and clear overflow flag
	for (i = 0; i < us; i++)       // Count <us> overflows
	{
		while (!(TMR3CN0 & 0x80));  // Wait for overflow
		TMR3CN0 &= ~(0x80);         // Clear overflow indicator
	}
	TMR3CN0 = 0 ;                   // Stop Timer3 and clear overflow flag
}

void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Timer3us(250);
}

void SPIWrite (unsigned char x)
{
   SPI0DAT=x;
   while(!SPIF);
   SPIF=0;
}

// Read 10 bits from ithe MCP3008 ADC converter using the recomended
// format in the datasheet.
unsigned int volatile GetADC(char channel)
{
	unsigned int adc;

	ADC_CE=0; //Select the MCP3008 converter.
	
	SPIWrite(0x01);//Send the start bit.
	adc=SPI0DAT; // Unlike SPI in the C8051F38C we need to empty the receive buffer.
	SPIWrite((channel*0x10)|0x80);	//Send single/diff* bit,  D2, D1, and D0 bits.
	adc=((SPI0DAT & 0x03)*0x100);//SPI0DAT contains now the high part of the result.
	SPIWrite(0x55);//Dont' care what you send now.  0x55 looks good on the oscilloscope though!
	adc+=SPI0DAT;//SPI0DAT contains now the low part of the result. 
	
	ADC_CE=1; //Deselect the MCP3008 converter.
		
	return adc;
}

// The Voltage reference input to the MCP3008, the '5Vo' pin
// from the EFM8 board, was measured as:
#define VREF 4.8
 
void main (void)
{
	printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	
	printf ("EFM8LB1 SPI/MCP3008 test program\n"
	        "File: %s\n"
	        "Compiled: %s, %s\n\n",
	        __FILE__, __DATE__, __TIME__);
	
	while(1)
	{
		printf("V0=%5.3f, V1=%5.3f\r", (GetADC(0)*VREF)/1023.0, (GetADC(1)*VREF)/1023.0);
		waitms(500);
	}
}

