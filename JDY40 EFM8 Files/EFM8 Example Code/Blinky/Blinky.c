#include <EFM8LB1.h>
#include <stdio.h>

char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key

	SFRPAGE = 0x10;
	PFE0CN  = 0x20; // SYSCLK < 75 MHz.
		
	// Transition to 24.5 MHz first
	SFRPAGE = 0x00;
	CLKSEL  = 0x00;
	CLKSEL  = 0x00;
	while ((CLKSEL & 0x80) == 0);
	
	// Now switch to 72 MHz
	CLKSEL = 0x03;
	CLKSEL = 0x03;
	while ((CLKSEL & 0x80) == 0);
	
	// Configure the pin used for LED output: P2.1 (pin 16 of LQFP32 package)
	P2MDOUT = 0b_0000_0010;

	XBR0 = 0X00;
	XBR1 = 0X00;
	XBR2 = 0x40; // Enable crossbar and weak pull-ups
	
	return 0;
}

void delay (unsigned int x)
{
	unsigned char j;
	while(--x)
	{
		for(j=0; j<100; j++);
	}
}

void main (void)
{
	while(1)
	{
		P2_1=!P2_1;
		delay(50000);
	}
}
