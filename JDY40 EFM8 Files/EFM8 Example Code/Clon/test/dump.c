#include <EFM8LB1.h>
#include <stdio.h>

#define SYSCLK      72000000L  // SYSCLK frequency in Hz
#define BAUDRATE      115200L  // Baud rate of UART in bps

struct _id {
   unsigned char device_id;
   unsigned char derivative_id;
   char * description; // The Flash size is embedded in the description string: F16, F32, or F64
};

const struct _id id[]=
{
	{0x34, 0x41, "EFM8LB12F64E_QFN32"},
	{0x34, 0x42, "EFM8LB12F64E_QFP32"},
	{0x34, 0x43, "EFM8LB12F64E_QSOP24"},
	{0x34, 0x44, "EFM8LB12F64E_QFN24"},
	{0x34, 0x45, "EFM8LB12F32E_QFN32"},
	{0x34, 0x46, "EFM8LB12F32E_QFP32"},
	{0x34, 0x47, "EFM8LB12F32E_QSOP24"},
	{0x34, 0x48, "EFM8LB12F32E_QFN24"},
	{0x34, 0x49, "EFM8LB11F32E_QFN32"},
	{0x34, 0x4A, "EFM8LB11F32E_QFP32"},
	{0x34, 0x4B, "EFM8LB11F32E_QSOP24"},
	{0x34, 0x4C, "EFM8LB11F32E_QFN24"},
	{0x34, 0x4D, "EFM8LB11F16E_QFN32"},
	{0x34, 0x4E, "EFM8LB11F16E_QFP32"},
	{0x34, 0x4F, "EFM8LB11F16E_QSOP24"},
	{0x34, 0x50, "EFM8LB11F16E_QFN24"},
	{0x34, 0x51, "EFM8LB10F16E_QFN32"},
	{0x34, 0x52, "EFM8LB10F16E_QFP32"},
	{0x34, 0x53, "EFM8LB10F16E_QSOP24"},
	{0x34, 0x54, "EFM8LB10F16E_QFN24"},
	{0x34, 0x61, "EFM8LB12F64ES0_QFN32"},
	{0x34, 0x64, "EFM8LB12F64ES0_QFN24"},
	{0x34, 0x65, "EFM8LB12F32ES0_QFN32"},
	{0x34, 0x68, "EFM8LB12F32ES0_QFN24"},
	{0x34, 0x69, "EFM8LB11F32ES0_QFN32"},
	{0x34, 0x6C, "EFM8LB11F32ES0_QFN24"},
	{0x34, 0x6D, "EFM8LB11F16ES0_QFN32"},
	{0x34, 0x70, "EFM8LB11F16ES0_QFN24"},
	{0x34, 0x71, "EFM8LB10F16ES0_QFN32"},
	{0x34, 0x74, "EFM8LB10F16ES0_QFN24"},
	{0x00, 0x00, ""}
};

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

	#if ( ((SYSCLK/BAUDRATE)/(12L*2L)) > 0x100)
		#error Can not configure baudrate using timer 1 
	#endif
	
	P0MDOUT |= 0x10; // Enable UART0 TX as push-pull output
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00;
	XBR2     = 0x40; // Enable crossbar and weak pull-ups
	
	// Configure Uart 0
	SCON0 = 0x10;
	TH1 = 0x100-((SYSCLK/BAUDRATE)/(12L*2L));
	TL1 = TH1;      // Init Timer1
	TMOD &= ~0xf0;  // TMOD: timer 1 in 8-bit auto-reload
	TMOD |=  0x20;                       
	TR1 = 1; // START Timer1
	TI = 1;  // Indicate TX0 ready
	
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

void WaitXms (unsigned int ms)
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

void main (void) 
{
    code unsigned char * uuid_ptr;
    unsigned int j;
    
	WaitXms(500); // Give PuTTY a chance to start.
	printf( "This EFM8LB1 universally unique identifier (UUID) is: \r\n" );
	uuid_ptr=(code unsigned char *)0xffc0;
	for(j=0; j<16; j++)
	{
	    printf("%02x ", *uuid_ptr);
	    uuid_ptr++;
	}
	printf("\n");
	
	if(DEVICEID==0x34)
	{
		for(j=0; id[j].device_id!=0; j++)
		{
			if (id[j].derivative_id==DERIVID) 
			{
				printf("Derivative Identification=0x%02x. Description: %s\n", DERIVID, id[j].description);
				break;
			}
		}
	}
	
	printf("const unsigned char Bootloader[]={");
	uuid_ptr=(code unsigned char *)0xfa00;
	for(j=0; j<0x200; j++)
	{
		if(j%16==0)
		{
		    printf("\n/*%04x*/ ", j+0xfa00);
		}
		printf("0x%02x, ", *uuid_ptr);
		uuid_ptr++;
	}
	printf("};\n");
}
