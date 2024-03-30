//  all_timers.c: Uses timer 0 to 5 overflow interrupts to generate a square
//  wave at pins P2.0, P1_7, P1_6, P1_5, P1_4, and P1_3.  Also use the PCA with interrupts to
//  generate square signals at pins P1.2, P1.1, P1.0, P0.7, and P0.6
//
//  Copyright (c) 2010-2018 Jesus Calvino-Fraga
//
//  ~C51~

#include <EFM8LB1.h>
#include <stdlib.h>
#include <stdio.h>

#define SYSCLK 72000000L // SYSCLK frequency in Hz
#define TIMER_0_FREQ 1000L
#define TIMER_1_FREQ 2000L
#define TIMER_2_FREQ 3000L
#define TIMER_3_FREQ 4000L
#define TIMER_4_FREQ 5000L
#define TIMER_5_FREQ 6000L
#define PCA_0_FREQ 7000L
#define PCA_1_FREQ 8000L
#define PCA_2_FREQ 9000L
#define PCA_3_FREQ 10000L
#define PCA_4_FREQ 11000L

#define TIMER_OUT_0 P2_0
#define TIMER_OUT_1 P1_7
#define TIMER_OUT_2 P1_6
#define TIMER_OUT_3 P1_5
#define TIMER_OUT_4 P1_4
#define TIMER_OUT_5 P1_3
#define MAIN_OUT    P0_1 // Updated in the main program
#define PCA_OUT_0   P1_2
#define PCA_OUT_1   P1_1
#define PCA_OUT_2   P1_0
#define PCA_OUT_3   P0_7
#define PCA_OUT_4   P0_6

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

	// Configure the pins used for square output
	P0MDOUT|=0b_1100_0010;
	P1MDOUT|=0b_1111_1111;
	P2MDOUT|=0b_0000_0001;
	
	XBR0     = 0x00;                     
	XBR1     = 0X00;
	XBR2     = 0x40; // Enable crossbar and weak pull-ups
	
	// Initialize timer 0 for periodic interrupts
	TR0=0;
	TF0=0;
	CKCON0|=0b_0000_0100; // Timer 0 uses the system clock
	TMOD&=0xf0;
	TMOD|=0x01; // Timer 0 in mode 1: 16-bit timer
	// Initialize reload value
	TMR0=65536L-(SYSCLK/(2*TIMER_0_FREQ));
	ET0=1;     // Enable Timer0 interrupts
	TR0=1;     // Start Timer0

	// Initialize timer 1 for periodic interrupts
	TR1=0;
	TF1=0;
	CKCON0|=0b_0000_1000; // Timer 1 uses the system clock
	TMOD&=0x0f;
	TMOD|=0x10; // Timer 1 in mode 1: 16-bit timer
	// Initialize reload value
	TMR1=65536L-(SYSCLK/(2*TIMER_1_FREQ));
	ET1=1;     // Enable Timer1 interrupts
	TR1=1;     // Start Timer1

	// Initialize timer 2 for periodic interrupts
	TMR2CN0=0x00;   // Stop Timer2; Clear TF2;
	CKCON0|=0b_0001_0000; // Timer 2 uses the system clock
	TMR2RL=(0x10000L-(SYSCLK/(2*TIMER_2_FREQ))); // Initialize reload value
	TMR2=0xffff;   // Set to reload immediately
	ET2=1;         // Enable Timer2 interrupts
	TR2=1;         // Start Timer2 (TMR2CN is bit addressable)

	// Initialize timer 3 for periodic interrupts
	TMR3CN0=0x00;   // Stop Timer3; Clear TF3;
	CKCON0|=0b_0100_0000; // Timer 3 uses the system clock
	TMR3RL=(0x10000L-(SYSCLK/(2*TIMER_3_FREQ))); // Initialize reload value
	TMR3=0xffff;   // Set to reload immediately
	EIE1|=0b_1000_0000;     // Enable Timer3 interrupts
	TMR3CN0|=0b_0000_0100;  // Start Timer3 (TMR3CN0 is not bit addressable)

	// Initialize timer 4 for periodic interrupts
	SFRPAGE=0x10;
	TMR4CN0=0x00;   // Stop Timer4; Clear TF4; WARNING: lives in SFR page 0x10
	CKCON1|=0b_0000_0001; // Timer 4 uses the system clock
	TMR4RL=(0x10000L-(SYSCLK/(2*TIMER_4_FREQ))); // Initialize reload value
	TMR4=0xffff;   // Set to reload immediately
	EIE2|=0b_0000_0100;     // Enable Timer4 interrupts
	TR4=1;

	// Initialize timer 5 for periodic interrupts
	SFRPAGE=0x10;
	TMR5CN0=0x00;   // Stop Timer5; Clear TF5; WARNING: lives in SFR page 0x10
	CKCON1|=0b_0000_0100; // Timer 5 uses the system clock
	TMR5RL=(0x10000L-(SYSCLK/(2*TIMER_5_FREQ))); // Initialize reload value
	TMR5=0xffff;   // Set to reload immediately
	EIE2|=0b_0000_1000; // Enable Timer5 interrupts
	TR5=1;         // Start Timer5 (TMR5CN0 is bit addressable)

	// Initialize the Prgramable Counter Array to generate the requested frequencies
	SFRPAGE=0x0;
	PCA0MD=0x00; // Disable and clear everything in the PCA
	PCA0L=0; // Initialize the PCA counter to zero
	PCA0H=0;
	PCA0MD=0b_0000_1000; // Configure PCA.  System CLK is the frequency input for the PCA
	// Enable all PCS modules comparators and to generate interrupts
	PCA0CPM0=PCA0CPM1=PCA0CPM2=PCA0CPM3=PCA0CPM4=0b_0100_1001; // ECOM|MAT|ECCF;
	// The frequency for PCA channel 0
	PCA0CPL0=(SYSCLK/(2*PCA_0_FREQ))%0x100; //Always write low byte first!
	PCA0CPH0=(SYSCLK/(2*PCA_0_FREQ))/0x100;
	// The frequency for PCA channel 1
	PCA0CPL1=(SYSCLK/(2*PCA_1_FREQ))%0x100; //Always write low byte first!
	PCA0CPH1=(SYSCLK/(2*PCA_1_FREQ))/0x100;
	// The frequency for PCA channel 2
	PCA0CPL2=(SYSCLK/(2*PCA_2_FREQ))%0x100; //Always write low byte first!
	PCA0CPH2=(SYSCLK/(2*PCA_2_FREQ))/0x100;
	// The frequency for PCA channel 3
	PCA0CPL3=(SYSCLK/(2*PCA_3_FREQ))%0x100; //Always write low byte first!
	PCA0CPH3=(SYSCLK/(2*PCA_3_FREQ))/0x100;
	// The frequency for PCA channel 4
	PCA0CPL4=(SYSCLK/(2*PCA_4_FREQ))%0x100; //Always write low byte first!
	PCA0CPH4=(SYSCLK/(2*PCA_4_FREQ))/0x100;
	CR=1; // Enable PCA counter
	EIE1|=0b_0001_0000; // Enable PCA interrupts
	
	EA=1; // Enable interrupts
	
	return 0;
}

void Timer0_ISR (void) interrupt INTERRUPT_TIMER0
{
	SFRPAGE=0x0;
	// Timer 0 in 16-bit mode doesn't have auto reload
	TMR0=0x10000L-(SYSCLK/(2*TIMER_0_FREQ));
	TIMER_OUT_0=!TIMER_OUT_0;
}

void Timer1_ISR (void) interrupt INTERRUPT_TIMER1
{
	SFRPAGE=0x0;
	// Timer 1 in 16-bit mode doesn't have auto reload
	TMR1=0x10000L-(SYSCLK/(2*TIMER_1_FREQ));
	TIMER_OUT_1=!TIMER_OUT_1;
}

void Timer2_ISR (void) interrupt INTERRUPT_TIMER2
{
	SFRPAGE=0x0;
	TF2H = 0; // Clear Timer2 interrupt flag
	TIMER_OUT_2=!TIMER_OUT_2;
}

void Timer3_ISR (void) interrupt INTERRUPT_TIMER3
{
	SFRPAGE=0x0;
	TMR3CN0&=0b_0011_1111; // Clear Timer3 interrupt flags
	TIMER_OUT_3=!TIMER_OUT_3;
}

void Timer4_ISR (void) interrupt INTERRUPT_TIMER4
{
	SFRPAGE=0x10;
	TF4H = 0; // Clear Timer4 interrupt flag
	TIMER_OUT_4=!TIMER_OUT_4;
}

void Timer5_ISR (void) interrupt INTERRUPT_TIMER5
{
	SFRPAGE=0x10;
	TF5H = 0; // Clear Timer5 interrupt flag
	TIMER_OUT_5=!TIMER_OUT_5;
}

void PCA_ISR (void) interrupt INTERRUPT_PCA0
{
	unsigned int j;
	
	SFRPAGE=0x0;
	
	if (CCF0)
	{
		j=(PCA0CPH0*0x100+PCA0CPL0)+(SYSCLK/(2L*PCA_0_FREQ));
		PCA0CPL0=j%0x100; //Always write low byte first!
		PCA0CPH0=j/0x100;
		CCF0=0;
		PCA_OUT_0=!PCA_OUT_0;
	}
	if (CCF1)
	{
		j=(PCA0CPH1*0x100+PCA0CPL1)+(SYSCLK/(2L*PCA_1_FREQ));
		PCA0CPL1=j%0x100; //Always write low byte first!
		PCA0CPH1=j/0x100;
		CCF1=0;
		PCA_OUT_1=!PCA_OUT_1;
	}
	if (CCF2)
	{
		j=(PCA0CPH2*0x100+PCA0CPL2)+(SYSCLK/(2L*PCA_2_FREQ));
		PCA0CPL2=j%0x100; //Always write low byte first!
		PCA0CPH2=j/0x100;
		CCF2=0;
		PCA_OUT_2=!PCA_OUT_2;
	}
	if (CCF3)
	{
		j=(PCA0CPH3*0x100+PCA0CPL3)+(SYSCLK/(2L*PCA_3_FREQ));
		PCA0CPL3=j%0x100; //Always write low byte first!
		PCA0CPH3=j/0x100;
		CCF3=0;
		PCA_OUT_3=!PCA_OUT_3;
	}
	if (CCF4)
	{
		j=(PCA0CPH4*0x100+PCA0CPL4)+(SYSCLK/(2L*PCA_4_FREQ));
		PCA0CPL4=j%0x100; //Always write low byte first!
		PCA0CPH4=j/0x100;
		CCF4=0;
		PCA_OUT_4=!PCA_OUT_4;
	}

	CF=0;
}

void main (void)
{
    unsigned int j;
	while(1)
	{
	    // Everybody is interrupting, so it is imposible to obtain a perfect square wave
		MAIN_OUT=!MAIN_OUT;
		for (j=0; j<1000; j++);
	}
}
