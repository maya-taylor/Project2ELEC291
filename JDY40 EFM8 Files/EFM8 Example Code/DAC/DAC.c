// DAC.c:  Shows how to use the 12-bit DACs of the EFM8LB1.
// (c) 2018, Jesus Calvino-Fraga
//

#include <stdio.h>
#include <stdlib.h>
#include <EFM8LB1.h>
#include <math.h>

// ~C51~  

#define SYSCLK 72000000L
#define BAUDRATE 115200L

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

	// Initialize the pin used by DAC0 (P3.0 in the LQFP32 package)
	// 1. Clear the bit associated with the pin in the PnMDIN register to 0. This selects analog mode for the pin.
    // 2. Set the bit associated with the pin in the Pn register to 1.
    // 3. Skip the bit associated with the pin in the PnSKIP register to ensure the crossbar does not attempt to assign a function to the pin.
	P3MDIN&=0b_1111_1110;
	P3|=0b_0000_0001;
	//P3SKIP|=0b_0000_0001; // P3 Pins Not Available on Crossbar
	
	P0MDOUT |= 0x10; // Enable UART0 TX as push-pull output
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00;
	XBR2     = 0x40; // Enable crossbar and weak pull-ups

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
	
  	// Initialize DAC
	SFRPAGE = 0x30; 
  	DACGCF0=0b_1000_1000; // 1:D23REFSL(VCC) 1:D3AMEN(NORMAL) 2:D3SRC(DAC3H:DAC3L) 1:D01REFSL(VCC) 1:D1AMEN(NORMAL) 1:D1SRC(DAC1H:DAC1L)
  	DACGCF1=0b_0000_0000;
  	DACGCF2=0b_0010_0010; // Reference buffer gain 1/3 for all channels
  	DAC0CF0=0b_1000_0000; // Enable DAC 0
  	DAC0CF1=0b_0000_0010; // DAC gain is 3.  Therefore the overall gain is 1.
  	DAC0=0;
	SFRPAGE = 0x00; 
  		
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

void main (void)
{
    char wavetype='s';
    unsigned char j=0;
    xdata unsigned int WaveTable[256];
    bit init=1;
    
	printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	
	printf ("DAC test program\n"
	        "File: %s\n"
	        "Compiled: %s, %s\n\n",
	        __FILE__, __DATE__, __TIME__);

    printf("Type 'r' for ramp, 't' for triangle, 'q' for square, or 's' for sine.\n");
      
	while(1)
	{
	    if (RI | init)
	    {
	        wavetype=init?'s':getchar();
	        init=0;
	        switch (wavetype)
	        {
				case 'r': // Ramp
					for (j=1, WaveTable[0]=0; j!=0; j++)
					{
						WaveTable[j]=j*16; 
					}
				break;
				case 's': // Sine
					for (j=1, WaveTable[0]=2047; j!=0; j++)
					{
						WaveTable[j]=2047*(1.0+sinf(j*2.0*PI/256.0));
					}
				break;
				case 't': // Triangle
					for (j=0; j<128; j++)
					{
						WaveTable[j]=j*16*2; 
					}
					for (j=0; j<128; j++)
					{
						WaveTable[j+128]=(127-j)*16*2; 
					}
				break;
				case 'q': // Square
					for (j=0; j<128; j++)
					{
						WaveTable[j]=4095; 
					}
					for (j=0; j<128; j++)
					{
						WaveTable[j+128]=0; 
					}
				break;
				default:
				break;
            }
	    }
	    SFRPAGE = 0x30; // Just want to make clear that SFRPAGE must be 0x30 before writing to DAC0
        DAC0=WaveTable[j++];
	    SFRPAGE = 0x00;
    }  
}	

