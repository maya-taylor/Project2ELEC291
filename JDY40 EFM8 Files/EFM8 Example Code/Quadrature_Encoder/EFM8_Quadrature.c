// EFM quadrature encoder: This program tests a quadrature encoder.
// The center pin of the encoder is connected to ground.  The other
// two pins of the encoder are connected to P2.0 and P1.6.
//  
//  Copyright (c) 2004-2018 Jesus Calvino-Fraga
//  ~C51~

#include <EFM8LB1.h>
#include <stdlib.h>
#include <stdio.h>

// ~C51~

#define SYSCLK 72000000L // SYSCLK frequency in Hz
#define BAUDRATE 115200L
#define RELOAD_1MS (0x10000L-(SYSCLK/(12L*1000L)))

#define PHASEA P2_0
#define PHASEB P1_6

//Remember to make your variables volatile if they are going to
//be used both in an ISR and the main program.
volatile unsigned char mycounter0;
volatile unsigned char count_ms;

char _c51_external_startup (void)
{
	// Disable Watchdog with key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key
  
	VDM0CN |= 0x80;
	RSTSRC = 0x02;

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
	
	P0MDOUT |= 0x10; // Enable UART0 TX as push-pull output
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X10; // Enable T0 on P0.0
	XBR2     = 0x40; // Enable crossbar and weak pull-ups

	#if (((SYSCLK/BAUDRATE)/(2L*12L))>0xFFL)
		#error Timer 0 reload value is incorrect because (SYSCLK/BAUDRATE)/(2L*12L) > 0xFF
	#endif
	// Configure Uart 0
	SCON0 = 0x10;
	CKCON0 |= 0b_0000_0000 ; // Timer 1 uses the system clock divided by 12.
	TH1 = 0x100-((SYSCLK/BAUDRATE)/(2L*12L));
	TL1 = TH1;      // Init Timer1
	TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	TMOD |=  0x20;                       
	TR1 = 1; // START Timer1
	TI = 1;  // Indicate TX0 ready

	// Initialize timer 5 for periodic interrupts
	SFRPAGE=0x10;
	TMR5CN0=0x00;
	TMR5RL=RELOAD_1MS;
	TMR5=0xffff; // Set to reload immediately
	EIE2|=0b_0000_1000; // Enable Timer5 interrupts
	TR5=1; // Start Timer5 (TMR5CN0 is bit addressable)
	
	EA=1;
	
	SFRPAGE=0x00;
	
	return 0;
}

//Use register bank 1 for this interrupt
void Timer5_ISR (void) interrupt INTERRUPT_TIMER5 using 1
{
	static unsigned char state=0;
	unsigned char phase;

	SFRPAGE=0x10;
	TF5H = 0; // Clear Timer5 interrupt flag
	
	phase=0;
	if(PHASEA) phase=1;
	if(PHASEB) phase|=2;
	switch (state)
	{
		case 0:
			if(phase==2) 
			{
				mycounter0--;
				state=3;
			}
			else if(phase==1)
			{
				mycounter0++;
				state=1;
			}
		break;
		case 1:
			if(phase==0)
			{
				mycounter0--;
				state=0;
			}
			else if(phase==3)
			{
				mycounter0++;
				state=2;
			}
		break;
		case 2:
			if(phase==1)
			{
				mycounter0--;
				state=1;
			}
			else if(phase==2)
			{
				mycounter0++;
				state=3;
			}
		break;
		case 3:
			if(phase==3)
			{
				mycounter0--;
				state=2;
			}
			else if(phase==0)
			{
				mycounter0++;
				state=0;
			}
		break;
		default:
			state=0;
		break;
	}
	
	count_ms++;
}

void waitms (unsigned int x)
{
	unsigned char j;
	
	for(j=x/250 ; j!=0; j--)
	{
		count_ms=0;
		while(count_ms!=250);
	}
	j=x%250;
	count_ms=0;
	while(count_ms!=j);
}

void main (void)
{
	int smycounter0=10;
	
	waitms(1000);
	
	printf("EFM8 quadrature encoder test\n\n");
	printf("Counter is:\n");
	
	while(1)
	{
		if ( (smycounter0!=mycounter0)  )
		{
			smycounter0=mycounter0;
			printf("%3d\r", mycounter0);
		}
		if(P3_7==0) mycounter0=0; //A clear button is always handy!
	}
}
    
