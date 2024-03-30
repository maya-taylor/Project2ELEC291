// FT93C66A SPI EEPROM test program
// by Jesus Calvino-Fraga (c) 2018

#include <stdio.h>
#include <stdlib.h>
#include <EFM8LB1.h>
#include "FT93C66.h"

#define SYSCLK 72000000L    // Internal oscillator frequency in Hz
#define BAUDRATE 115200L
#define F_SCK_MAX 2000000L  // FT93C66A Max SCK freq (Hz) 

// Pins used by the SPI interface via the crossbar:
// 	P0.0: SCK  connected to pin 2 of FT93C66A
// 	P0.1: MISO connected to pin 4 of FT93C66A
// 	P0.2: MOSI connected to pin 3 of FT93C66A
//  Also, the FT93C66A CE (pin 1, active high) is connected to P0.3

unsigned int ErrCnt, SeedCnt;
	
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
	SPI0CFG = 0b_0110_0000; //SPI in master mode, CKPHA=1
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

void Test (unsigned char testval)
{
	unsigned char j;
	unsigned int k;
	unsigned int cnt=0;
	
	FT93C66_Write_All(testval);

	for(k=0; k<0x200; k++)
	{
		j=FT93C66_Read(k);
		if(j!=testval)
		{
			if(cnt==0) printf("\n0x%02x failed at:", testval);
			ErrCnt++;
			if( ((cnt&0x0f)==0) && (cnt>0) ) printf("\n               ");
			cnt++;
			printf(" %03x", k);
		}
	}
}

void main (void)
{
	volatile unsigned char i=0, j=0;
	volatile unsigned int k, m;
	
	code unsigned char pattern[]={
		 0x00,  0xff,  0x55,  0xaa,  0x0f,  0xf0,  0x5a,  0xa5,
		 0x01,  0x02,  0x04,  0x08,  0x10,  0x20,  0x40,  0x80,
		~0x01, ~0x02, ~0x04, ~0x08, ~0x10, ~0x20, ~0x40, ~0x80 };
	
	waitms(1000);
	
	while(1)
	{
		FT93C66_Init();
	
		printf(
			"\n\nEFM8LB1 SPI EEPROM test program.\n"
			"\nSelect option:\n"
			"   1) Pattern test\n"
			"   2) Random test\n"
			"   3) Write Memory location\n"
			"   4) Read Memory location\n"
			"   5) Memory Location test\n"
			"   6) Display Memory\n"
			"   7) Fill Memory\n"
			"   8) Erase Memory\n"
			"Option: ");
		
		while(RI==0)
		{
			SeedCnt++;
		}

		switch(getchar())
		{
			case '1':
				FT93C66_Write_Enable();
			
				ErrCnt=0;		
				printf("\nPattern testing all memory locations.\n");
			
				for(i=0; i<sizeof(pattern); i++) Test(pattern[i]);
				if(ErrCnt>0)
				{
					printf("\nThere were %d ERROR(s).\n", ErrCnt);
				}
				else
				{
					printf("\nNo errors.  Memory works fine!\n");
				}
		
				FT93C66_Write_Disable();
				break;
			
			case '2':
				FT93C66_Write_Enable();
				printf("\nRandom testing all memory locations.\n");
				srand(SeedCnt);
				for(k=0; k<0x200; k++)
				{
					i=rand()&0xff;
					FT93C66_Write(k, i);
					j=FT93C66_Read(k);
					if((k&0xf)==0)
					{
						printf("\n%03x: ", k);
					}
					printf(" %02x", j);			
					if(j!=i) break;
				}
				if(j!=i)
				{
					printf("\nERROR at location %03x.  Wrote %02x but read %02x\n", k, i, j);
				}
				else
				{
					printf("\nTest pass\n");
				}
				FT93C66_Write_Disable();
				break;
			
			case '3':
				printf("\nAddress: ");
				scanf("%x", &k);
				waitms(10);
				printf("\nValue: ");
				scanf("%x", &m);
				
				FT93C66_Write_Enable();
				FT93C66_Write(k, m);
				j=FT93C66_Read(k);
				FT93C66_Write_Disable();

				printf("\n[%03x]: %02x\n", k, j);			

				break;

			case '4':
				printf("\nAddress: ");
				scanf("%x", &k);
				j=FT93C66_Read(k);
				printf("\n[%03x]: %02x\n", k, j);			
				break;
				
			case '5':
				FT93C66_Write_Enable();
				printf("\nLocation to test: ");
				scanf("%x", &k);

				i=0;
				ErrCnt=0;
				do
				{
					FT93C66_Write(k, i);
					j=FT93C66_Read(k);
					if((i&0xf)==0)
					{
						printf("\n%03x: ", i);
					}
					printf(" %02x", j);			
					if(j!=i) ErrCnt++;
					i++;
				} while (i!=0);
				printf("\nThere were %d error(s).\n", ErrCnt);
				
				FT93C66_Write_Disable();
				break;
			
			case '6':
				printf("\nMemory contains:\n");
				for(k=0; k<0x200; k++)
				{
					j=FT93C66_Read(k);
					if((k&0xf)==0)
					{
						printf("\n%03x: ", k);
					}
					printf(" %02x", j);			
				}
				printf("\n");
				break;

			case '7':
				FT93C66_Write_Enable();
				printf("\nValue: ");
				scanf("%x", &m);
				FT93C66_Write_All(m);
				FT93C66_Write_Disable();
				break;

			case '8':
				FT93C66_Write_Enable();
				FT93C66_Erase_All();
				FT93C66_Write_Disable();
				break;
			
			default:
				break;
		}
	}
}
