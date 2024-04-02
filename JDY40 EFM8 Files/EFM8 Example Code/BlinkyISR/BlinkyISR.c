#include <EFM8LB1.h>

#define SYSCLK 72000000L // SYSCLK frequency in Hz
#define TIMER_0_FREQ 2000L // For a 0.5ms tick

volatile unsigned int TickCount=0;

char _c51_external_startup (void)
{
	// Disable Watchdog with 2-byte key sequence
	SFRPAGE = 0x00;
	WDTCN = 0xDE; //First key
	WDTCN = 0xAD; //Second key

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

	// Configure the pin used for LED output: P2.1 (pin 16 of LQFP32 package)
	P2MDOUT = 0b_0000_0010;

	XBR0 = 0X00;
	XBR1 = 0X00;
	XBR2 = 0x40; // Enable crossbar and weak pull-ups

	// Initialize timer 0 for periodic interrupts
	TR0=0;
	TF0=0;
	CKCON0|=0b_0000_0100; // Timer 0 uses the system clock
	TMOD&=0xf0;
	TMOD|=0x01; // Timer 0 in mode 1: 16-bit timer
	
	#if (SYSCLK/(TIMER_0_FREQ)>0xFFFFL)
		#error Timer 0 reload value is incorrect because SYSCLK/(TIMER_0_FREQ) > 0xFFFFL
	#endif
	TMR0=0x10000L-(SYSCLK/(TIMER_0_FREQ)); // Initialize reload value
	ET0=1; // Enable Timer0 interrupts
	TR0=1; // Start Timer0
	EA=1;  // Enable global interrupts
	
	return 0;
}

void Timer0_ISR (void) interrupt INTERRUPT_TIMER0
{
	SFRPAGE=0x0;
	// Timer 0 in 16-bit mode doesn't have auto reload, so reload here
	TMR0=0x10000L-(SYSCLK/(TIMER_0_FREQ));
	
	TickCount++;
	if(TickCount==1000)
	{
		TickCount=0;
		P2_1=!P2_1; // Toggle P2_1 (where the LED is)
	}
}

void main (void)
{
	while(1)
	{
	}
}
