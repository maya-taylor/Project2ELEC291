//  EFM8LB1 with multiplexed 7-segment displays
//  Copyright (c) 2014-2018 Jesus Calvino-Fraga
//  ~C51~

#include <EFM8LB1.h>
#include <stdio.h>

#define SYSCLK 72000000L // SYSCLK frequency in Hz
#define BAUDRATE 115200

#define SEG_A P1_1
#define SEG_B P1_3
#define SEG_C P1_5
#define SEG_D P1_6
#define SEG_E P1_7
#define SEG_G P1_4
#define SEG_F P1_2
#define DP    P2_0

#define CA1   P1_0
#define CA2   P0_6
#define CA3   P0_7

code unsigned char seven_seg[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
                                   0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E };
                                   
volatile unsigned char ISR_state=0;
volatile unsigned char disp3, disp2, disp1;
unsigned char dp_pos=0;

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
	
	// Configure the pins used for square output
	P0MDOUT|=0b_1101_0001; // Enable UART0 TX as push-pull output (P0.4) as well as P0.6 and P0.7
	P1MDOUT|=0b_1111_1111; 
	P2MDOUT|=0b_0000_0001; 
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00; // Enable T0 on P0.0
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

	// Initialize timer 4 for periodic interrupts
	SFRPAGE=0x10;
	TMR4CN0=0x00;
	TMR4RL=0x10000L-(SYSCLK)/(1000L*12L); // 1 miliseconds interrupt
	TMR4=0xffff;   // Set to reload immediately
	EIE2|=0b_0000_0100; // Enable Timer4 interrupts
	TR4=1;         // Start Timer4 (TMR4CN0 is bit addressable)
	
	EA=1;
	
	SFRPAGE=0x00;
	
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
	for(j=ms; j!=0; j--)
	{
		Timer3us(249);
		Timer3us(249);
		Timer3us(249);
		Timer3us(250);
	}
}

void Load_Segments(unsigned char todisp)
{
	ACC=todisp;
	SEG_A=ACC_0;
	SEG_B=ACC_1;
	SEG_C=ACC_2;
	SEG_D=ACC_3;
	SEG_E=ACC_4;
	SEG_F=ACC_5;
	SEG_G=ACC_6;
	DP=ACC_7;
}

void Timer4_ISR (void) interrupt INTERRUPT_TIMER4
{
	SFRPAGE=0x10;
	TF4H = 0; // Clear Timer4 interrupt flag
	
	CA3=1;
	CA2=1;
	CA1=1;
	Load_Segments(0xff);

	switch(ISR_state)
	{
		case 0:
		    Load_Segments(disp3);
			CA3=0;
			ISR_state=1;
		break;
		case 1:
		    Load_Segments(disp2);
			CA2=0;
			ISR_state=2;
		break;
		case 2:
		    Load_Segments(disp1);
			CA1=0;
			ISR_state=0;
		break;
		default:
			ISR_state=0;
		break;
	}
}

void Send_7Seg (unsigned int x)
{
	disp3=seven_seg[x/100];
	disp2=seven_seg[(x/10)%10];
	disp1=seven_seg[x%10];
	
	switch(dp_pos)
	{
		case 0:
			disp1&=0b_0111_1111; // Turn on the decimal point of the first digit
			dp_pos=1;
			break;
		case 1:
			disp2&=0b_0111_1111; // Turn on the decimal point of the second digit
			dp_pos=2;
			break;
		case 2:
			disp3&=0b_0111_1111; // Turn on the decimal point of the third digit
			dp_pos=0;
			break;
		default:
		    dp_pos=0;
		    break;
	}
}

void main (void)
{
	unsigned int j=0;
	
	waitms(500); // Give PuTTY a chance to start
	
	printf("\n\nEFM8 multiplexed 7-segment displays test.\n");

	while(1)
	{
		Send_7Seg(j);
		waitms(200);
		j++;
		if(j>999) j=0;
	}
}
