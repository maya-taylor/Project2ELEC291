// Clon_EFM8LB1.c: This program copies the entire flash memory from one EFM8LB1 to another
//
// (c) 2008-2018, Jesus Calvino-Fraga
//
// Pin connections (LQFP32 packages):
//    Source EFM8LB1:    Connects to:
//    P1.0   ->  RST' of target EFM8LB1 (this is C2CK)
//    P1.1   ->  P3.7 of target EFM8LB1 (this is C2D)
//    P3.7   ->  The go push-button.  Already connected to BOOT push button. Press to start clonning!
//    P1.3   ->  To 330 ohms + LED + base of pnp.  Emmiter to 5Vo, collector to 5Vi of target EFM8LB1
//    P1.4   ->  Green LED + 330 ohms to 5V
//    P1.5   ->  Red LED + 330 ohms to 5V
//    GND    ->  GND
//
// The next line clears the "C51 command line options:" field when compiling with CrossIDE
//  ~C51~  
  
#include <EFM8LB1.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYSCLK      48000000L  // SYSCLK frequency in Hz
#define BAUDRATE      115200L  // Baud rate of UART in bps

// FLASH and device information
#define  FLASH_SIZE        0x10000L          // FLASH size
#define  NUM_PAGES         FLASH_SIZE/0x200L // Number of 512-byte FLASH pages
#define  BLOCK_SIZE        64                // The size of the read and write buffers in idata
// 1024 bytes (0x400, 2 pages) from 0xFC00 to 0xFFFF are not available for program storage.
#define  NUM_BLOCKS        ((FLASH_SIZE-0x400)/BLOCK_SIZE)
// Device ID should be 0x34 for EFM8LB1
#define EFM8LB1_ID       0x34

// C2 status return codes
#define INVALID_COMMAND   0x00
#define COMMAND_FAILED    0x02
#define COMMAND_OK        0x0D

// From AN127 Table 3.1. C2 Flash Programming Commands
#define BLOCK_READ        0x06
#define BLOCK_WRITE       0x07
#define PAGE_ERASE        0x08
#define DEVICE_ERASE      0x03
#define GET_VERSION       0x01
#define GET_DERIVATIVE    0x02
#define DIRECT_READ       0x09
#define DIRECT_WRITE      0x0A
#define INDIRECT_READ     0x0B
#define INDIRECT_WRITE    0x0C

// C2 Registers
#define  FPDAT             0xB4 // For the EFM8LB1 only
#define  FPCTL             0x02
#define  DEVICEID          0x00
#define  REVID             0x01

// C2 Signals
#define C2CK  P1_0
#define C2D   P1_1

// Push buttons and LEDS
#define FGO   P3_7 // Go push button
#define PWR   P1_3 // Power on/off for target controller 0:on
#define GLED  P1_4
#define YLED  P2_1
#define RLED  P1_5

// Misc constants
#define  LOW               0
#define  HIGH              1
#define  LEDON			   0
#define  LEDOFF            1
#define  PWRON             0
#define  PWROFF            1

// Program MACROS
#define  Poll_OutReady     while(!(C2_ReadAR()&0x01)) 
#define  Poll_InBusy       while((C2_ReadAR()&0x02))
#define  StrobeC2CK        C2CK = LOW; C2CK = HIGH
#define  C2D_DriverOn      P1MDOUT|=0x02; // Configures C2D pin as push-pull output
#define  C2D_DriverOff     P1MDOUT &= ~(0x02);P1 |= 0x02  // Configures C2D pin as open-drain input

unsigned char NUM_BYTES;
unsigned int  FLASH_ADDR;
unsigned char idata * C2_PTR;
unsigned char idata R_BUF[BLOCK_SIZE];
unsigned char idata W_BUF[BLOCK_SIZE];

// Utility functions
void Timer3us(unsigned char);
void waitms(unsigned int);

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

// This bootloader was obtained using the 'dump.c' program from a working EFM8LB1.
const unsigned char Bootloader[]={
/*fa00*/ 0x90, 0x00, 0x00, 0xe4, 0x93, 0xf4, 0x60, 0x18, 0xe5, 0xef, 0xb4, 0x10, 0x08, 0xe8, 0x64, 0xa5,
/*fa10*/ 0x60, 0x0e, 0x02, 0x00, 0x00, 0x54, 0x03, 0x60, 0xf9, 0x78, 0x16, 0x20, 0xb7, 0xf4, 0xd8, 0xfb,
/*fa20*/ 0x75, 0x81, 0x10, 0x02, 0xfa, 0x9c, 0x71, 0xd1, 0x8e, 0x08, 0x8f, 0x09, 0xad, 0x10, 0x71, 0x38,
/*fa30*/ 0x50, 0x23, 0xe5, 0x0d, 0xb4, 0x02, 0x06, 0xaf, 0x09, 0xae, 0x08, 0x71, 0x2a, 0xe5, 0x10, 0x60,
/*fa40*/ 0x17, 0x71, 0xc6, 0xad, 0x07, 0xaf, 0x09, 0xae, 0x08, 0x71, 0x31, 0x05, 0x09, 0xe5, 0x09, 0x70,
/*fa50*/ 0xec, 0x05, 0x08, 0x80, 0xe8, 0x75, 0x0c, 0x41, 0x22, 0x71, 0xd1, 0x8e, 0x08, 0x8f, 0x09, 0x71,
/*fa60*/ 0xd1, 0x75, 0xa7, 0x20, 0x43, 0xce, 0x08, 0xd3, 0xe5, 0x09, 0x9f, 0xe5, 0x08, 0x9e, 0x50, 0x14,
/*fa70*/ 0x85, 0x09, 0x82, 0x85, 0x08, 0x83, 0xe4, 0x93, 0xf5, 0xca, 0x05, 0x09, 0xe5, 0x09, 0x70, 0xe7,
/*fa80*/ 0x05, 0x08, 0x80, 0xe3, 0x71, 0xd1, 0xc0, 0x06, 0xc0, 0x07, 0x71, 0x50, 0xd0, 0x05, 0xd0, 0x04,
/*fa90*/ 0xef, 0x6d, 0x70, 0x02, 0xee, 0x6c, 0x60, 0x03, 0x75, 0x0c, 0x43, 0x22, 0x71, 0x5f, 0xe4, 0xf5,
/*faa0*/ 0x0e, 0xf5, 0x0f, 0x71, 0xab, 0x71, 0xc6, 0xef, 0x24, 0xd0, 0xf5, 0x0d, 0x75, 0x0c, 0x40, 0xb4,
/*fab0*/ 0x07, 0x00, 0x50, 0x5a, 0x90, 0xfa, 0xba, 0x25, 0xe0, 0x73, 0x41, 0xc8, 0x41, 0xd9, 0x41, 0xe3,
/*fac0*/ 0x41, 0xe3, 0x41, 0xe7, 0x41, 0xeb, 0x61, 0x01, 0x71, 0xd1, 0xef, 0x64, 0x42, 0x70, 0x03, 0xee,
/*fad0*/ 0x64, 0x34, 0x60, 0x3d, 0x75, 0x0c, 0x42, 0x80, 0x38, 0x71, 0xc6, 0x8f, 0x0e, 0x71, 0xc6, 0x8f,
/*fae0*/ 0x0f, 0x80, 0x2e, 0x51, 0x26, 0x80, 0x2a, 0x51, 0x59, 0x80, 0x26, 0x71, 0xc6, 0xad, 0x07, 0x7e,
/*faf0*/ 0xfb, 0x7f, 0xfe, 0x71, 0x31, 0x71, 0xc6, 0xad, 0x07, 0x7e, 0xfb, 0x7f, 0xff, 0x71, 0x31, 0x80,
/*fb00*/ 0x10, 0x7f, 0x40, 0x71, 0xde, 0x78, 0x00, 0xe4, 0xf6, 0x75, 0xef, 0x12, 0x80, 0x03, 0x75, 0x0c,
/*fb10*/ 0x90, 0xaf, 0x0c, 0x71, 0xde, 0x80, 0x8c, 0x8f, 0x82, 0x8e, 0x83, 0x85, 0x0e, 0xb7, 0x85, 0x0f,
/*fb20*/ 0xb7, 0x43, 0x8f, 0x01, 0xed, 0xf0, 0x53, 0x8f, 0xfc, 0x22, 0x43, 0x8f, 0x02, 0xe4, 0xfd, 0x61,
/*fb30*/ 0x17, 0xed, 0xf4, 0x60, 0x02, 0x71, 0x17, 0x22, 0xc3, 0xee, 0x94, 0xfa, 0x50, 0x10, 0xed, 0x2f,
/*fb40*/ 0xff, 0xe4, 0x3e, 0xfe, 0xd3, 0xef, 0x94, 0x00, 0xee, 0x94, 0xfa, 0x50, 0x01, 0x22, 0xc3, 0x22,
/*fb50*/ 0x43, 0xce, 0x01, 0xaf, 0xcb, 0xef, 0xfe, 0xad, 0xcb, 0xed, 0xff, 0xe4, 0xf5, 0xa7, 0x22, 0x75,
/*fb60*/ 0x97, 0xde, 0x75, 0x97, 0xad, 0x43, 0xff, 0x80, 0x75, 0xef, 0x02, 0xe4, 0xf5, 0xa9, 0x75, 0xa4,
/*fb70*/ 0x10, 0x75, 0xe1, 0x01, 0x75, 0xe3, 0x40, 0x75, 0xe4, 0xd0, 0x75, 0x8e, 0x08, 0x75, 0x89, 0x90,
/*fb80*/ 0xd2, 0x8e, 0x20, 0x85, 0xfd, 0x30, 0x85, 0xfd, 0xc2, 0x8e, 0xaf, 0x8d, 0xef, 0xfe, 0xad, 0x8b,
/*fb90*/ 0xed, 0xff, 0xee, 0xc3, 0x13, 0xef, 0x13, 0xf4, 0x04, 0xf5, 0x8d, 0x75, 0x89, 0x20, 0xd2, 0x8e,
/*fba0*/ 0xd2, 0x9c, 0x22, 0x30, 0x98, 0xfd, 0xc2, 0x98, 0xaf, 0x99, 0x22, 0x71, 0xa3, 0xbf, 0x24, 0xfb,
/*fbb0*/ 0x71, 0xa3, 0xae, 0x07, 0x8e, 0x10, 0xee, 0x60, 0x0c, 0x71, 0xa3, 0xf5, 0x82, 0x75, 0x83, 0x00,
/*fbc0*/ 0xef, 0xf0, 0x1e, 0x80, 0xf1, 0x22, 0x85, 0x10, 0x82, 0x75, 0x83, 0x00, 0xe0, 0xff, 0x15, 0x10,
/*fbd0*/ 0x22, 0x71, 0xc6, 0x8f, 0x0a, 0x71, 0xc6, 0x8f, 0x0b, 0xae, 0x0a, 0xaf, 0x0b, 0x22, 0xc2, 0x99,
/*fbe0*/ 0x8f, 0x99, 0x30, 0x99, 0xfd, 0x22, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
/*fbf0*/ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x90, 0xa5, 0xff };

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
	P1MDOUT |= 0x01; // C2CK as push-pull output
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

// C2 access routines

// Performs a C2 Address register read and returns the 8-bit register content
unsigned char C2_ReadAR(void)
{
   unsigned char i;
   unsigned char addr;

   // START field
   StrobeC2CK; // Strobe C2CK with C2D driver disabled
   
   // INS field (10b, LSB first)
   C2D = LOW;                       
   C2D_DriverOn; // Enable C2D driver (output)
   StrobeC2CK;
   C2D = HIGH;
   StrobeC2CK;

   C2D_DriverOff; // Disable C2D driver (input)

   // ADDRESS field
   addr = 0;
   for (i=0;i<8;i++) // Shift in 8 bit ADDRESS field LSB-first
   {
      addr >>= 1;                   
      StrobeC2CK;     
      if (C2D)
         addr |= 0x80;
   }              
   
   // STOP field
   StrobeC2CK; // Strobe C2CK with C2D driver disabled
   
   return addr; // Return Address register read value
}

// Performs a C2 Address register write (writes the <addr> input to Address register)
void C2_WriteAR(unsigned char addr)
{
   unsigned char i;
   
   // START field
   StrobeC2CK; // Strobe C2CK with C2D driver disabled

   // INS field (11b, LSB first)
   C2D = HIGH;             
   C2D_DriverOn;
   StrobeC2CK; 
   C2D = HIGH;
   StrobeC2CK;
   
   // Shift out 8-bit ADDRESS field
   for(i=0;i<8;i++)
   {
      C2D = (addr & 0x01);
      StrobeC2CK;
      addr >>= 1;
   }

   // STOP field
   C2D_DriverOff;
   StrobeC2CK;

   return;
}

// Performs a C2 Data register read and returns the 8-bit register content
unsigned char C2_ReadDR(void)
{
   unsigned char i;
   unsigned char dat; // Data register read content

   // START field
   StrobeC2CK; // Strobe C2CK with C2D driver disabled

   // INS field (00b, LSB first)
   C2D = LOW;
   C2D_DriverOn;
   StrobeC2CK;
   C2D = LOW;
   StrobeC2CK;

   // LENGTH field (00b -> 1 byte)
   C2D = LOW;
   StrobeC2CK;
   C2D = LOW;
   StrobeC2CK;

   // WAIT field
   C2D_DriverOff;
   do
   {
      StrobeC2CK;
   }
   while (!C2D); // Strobe C2CK until target transmits a '1'

   // DATA field
   dat = 0;
   for (i=0;i<8;i++) // Shift in 8-bit DATA field LSB-first
   {
      dat >>= 1;
      StrobeC2CK;
      if (C2D)
         dat  |= 0x80;
   }

   // STOP field
   StrobeC2CK;
   
   return dat;
}

// Performs a C2 Data register write (writes <dat> input to data register)
void C2_WriteDR(unsigned char dat)
{
   unsigned char i;
   
   // START field
   StrobeC2CK; // Strobe C2CK with C2D driver disabled

   // INS field (01b, LSB first)
   C2D = HIGH;
   C2D_DriverOn;
   StrobeC2CK;
   C2D = LOW;
   StrobeC2CK;
   
   // LENGTH field (00b -> 1 byte)
   C2D = LOW;
   StrobeC2CK;
   C2D = LOW;
   StrobeC2CK;
   
   // DATA field
   for (i=0;i<8;i++) // Shift out 8-bit DATA field LSB-first
   {
      C2D = (dat & 0x01);
      StrobeC2CK;
      dat >>= 1;
   }

   // WAIT field
   C2D_DriverOff; // Disable C2D driver for input
   do
   {
      StrobeC2CK; // Strobe C2CK until target transmits a '1'
   }
   while (!C2D);

   // STOP field
   StrobeC2CK; // Strobe C2CK with C2D driver disabled
   
   return;
}                    
                  
// Performs a target device reset by pulling the C2CK pin low for >20us
void C2_Reset(void)
{
   C2CK = LOW;   // Put target device in reset state by pulling
   Timer3us(20); // C2CK low for >20us
   C2CK = HIGH;  // Release target device from reset
}

// FLASH Programming Routines (High Level):
//
// These high-level routines perform the FLASH Programming Interface (FPI)
// command sequences.

void C2_WriteSFR (unsigned char sfraddress, unsigned char sfrdata)
{
   C2_WriteAR (sfraddress);
   C2_WriteDR (sfrdata);
   Poll_InBusy; // Wait for input acknowledge
}

unsigned char C2_ReadSFR (unsigned char sfraddress)
{
   unsigned char j;
   C2_WriteAR (sfraddress);
   j=C2_ReadDR ();
   Poll_InBusy; // Wait for input acknowledge
   return j;
}

// C2_Init: Initializes the C2 Interface for FLASH programming
void C2_Init(void)
{
   C2_Reset();    // Reset the target device
   Timer3us(2);   // Delay for at least 2us
 
   // FPCTL: Flash Programming Control Register.
   // This register is used to enable Flash programming via the C2 interface. To enable C2
   // Flash programming, the following codes must be written in order: 0x02, 0x01. Note
   // that once C2 Flash programming is enabled, a system reset must be issued to
   // resume normal operation.
   
   C2_WriteAR(FPCTL); // Target the C2 FLASH Programming Control register (FPCTL)
   C2_WriteDR(0x02);  // Write the first key code to enable C2 FLASH programming
   C2_WriteDR(0x01);  // Write the second key code to enable C2 FLASH programming
   waitms(20);        // Delay for at least 20ms to ensure the target is ready for C2 FLASH programming
   
   // EFM8LB1 Device-Specific Programming Sequences.  This comes from
   // Silabs' Application Note AN127:
   // VDD Monitor Initialization:
   C2_WriteSFR(0xFF, 0x80);
   Timer3us(5); //Delay 5 us
   C2_WriteSFR(0xEF, 0x02);
   // Oscillator Initialization:
   C2_WriteSFR(0xA9, 0x00);
}

// C2_GetDevID:  Reads the target Device ID register
unsigned char C2_GetDevID(void)
{
   C2_WriteAR(DEVICEID); // Select DeviceID register for C2 Data register accesses
   return C2_ReadDR();   // Read and return the DeviceID register
}

// C2_GetRevID: Reads Revision ID register
unsigned char C2_GetRevID(void)
{
   C2_WriteAR(REVID);   // Select REVID regsiter for C2 Data register accesses
   return C2_ReadDR();  // Read and return the DeviceID register
}

// C2_BlockRead:
// - Reads a block of FLASH memory starting at <FLASH_ADDR>
// - The size of the block is defined by <NUM_BYTES>
// - Stores the read data at the location targeted by the pointer <C2_PTR>
// - Assumes that FLASH accesses via C2 have been enabled prior to the function call
// - Function call returns a '1' if successful; returns a '0' if unsuccessful
//
char C2_BlockRead(void)
{
   unsigned char i;        // Counter
   unsigned char status;   // FPI status information holder

   C2_WriteAR(FPDAT);      // Select the FLASH Programming Data register for C2 Data register accesses
   C2_WriteDR(BLOCK_READ); // Send FLASH block read command
   Poll_InBusy;            // Wait for input acknowledge

   // Check status before starting FLASH access sequence
   Poll_OutReady;                      // Wait for status information
   status = C2_ReadDR();               // Read FLASH programming interface status
   if (status != COMMAND_OK) return 0; // Exit and indicate error
   
   C2_WriteDR(FLASH_ADDR >> 8);        // Send address high byte to FPDAT
   Poll_InBusy;                        // Wait for input acknowledge
   C2_WriteDR(FLASH_ADDR & 0x00FF);    // Send address low byte to FPDAT
   Poll_InBusy;                        // Wait for input acknowledge
   C2_WriteDR(NUM_BYTES);              // Send block size
   Poll_InBusy;                        // Wait for input acknowledge

   // Check status before reading FLASH block
   Poll_OutReady;                      // Wait for status information
   status = C2_ReadDR();               // Read FLASH programming interface status
   if (status != COMMAND_OK)
      return 0;                        // Exit and indicate error
   
   // Read FLASH block
   for (i=0;i<NUM_BYTES;i++)
   {
      Poll_OutReady;                   // Wait for data ready indicator
      *C2_PTR++ = C2_ReadDR();         // Read data from the FPDAT register
   }
   return 1;                           // Exit and indicate success
}

// C2_BlockWrite:
// - Writes a block of FLASH memory starting at <FLASH_ADDR>
// - The size of the block is defined by <NUM_BYTES>
// - Writes the block stored at the location targetted by <C2_PTR>
// - Assumes that FLASH accesses via C2 have been enabled prior to the function call
// - Function call returns a '1' if successful; returns a '0' if unsuccessful
//
char C2_BlockWrite(void)
{
   unsigned char i;                    // Counter
   unsigned char status;               // FPI status information holder

   C2_WriteAR(FPDAT);                  // Select the FLASH Programming Data register 
                                       // for C2 Data register accesses
   C2_WriteDR(BLOCK_WRITE);            // Send FLASH block write command
   Poll_InBusy;                        // Wait for input acknowledge

   // Check status before starting FLASH access sequence
   Poll_OutReady;                      // Wait for status information
   status = C2_ReadDR();               // Read FLASH programming interface status
   if (status != COMMAND_OK)
      return 0;                        // Exit and indicate error

   C2_WriteDR(FLASH_ADDR >> 8);        // Send address high byte to FPDAT
   Poll_InBusy;                        // Wait for input acknowledge
   C2_WriteDR(FLASH_ADDR & 0x00FF);    // Send address low byte to FPDAT
   Poll_InBusy;                        // Wait for input acknowledge
   C2_WriteDR(NUM_BYTES);              // Send block size
   Poll_InBusy;                        // Wait for input acknolwedge

   // Check status before writing FLASH block
   Poll_OutReady;                      // Wait for status information
   status = C2_ReadDR();               // Read FLASH programming interface status
   if (status != COMMAND_OK)
      return 0;                        // Exit and indicate error

   // Write FLASH block
   for (i=0;i<NUM_BYTES;i++)
   {
      C2_WriteDR(*C2_PTR++);           // Write data to the FPDAT register
      Poll_InBusy;                     // Wait for input acknowledge
   }   

   Poll_OutReady;                      // Wait for last FLASH write to complete
   return 1;                           // Exit and indicate success
}

// C2_PageErase:
// - Erases a 512-byte FLASH page
// - Targets the FLASH page containing the address <FLASH_ADDR>
// - Assumes that FLASH accesses via C2 have been enabled prior to the function call
// - Function call returns a '1' if successful; returns a '0' if unsuccessful
char C2_PageErase(void)
{
   unsigned char page;                 // Target FLASH page
   unsigned char status;               // FPI status information holder

   page=(unsigned char)(FLASH_ADDR>>9);// <page> is the 512-byte sector containing the target <FLASH_ADDR>.

   if (page >= (NUM_PAGES-2))          // Check that target page is within range (NUM_PAGES minus 2 for reserved area)
      return 0;                        // Indicate error if out of range
   C2_WriteAR(FPDAT);                  // Select the FLASH Programming Data register for C2 Data register accesses
   C2_WriteDR(PAGE_ERASE);             // Send FLASH page erase command Wait for input acknowledge

   // Check status before starting FLASH access sequence
   Poll_OutReady;                      // Wait for status information
   status = C2_ReadDR();               // Read FLASH programming interface status
   if (status != COMMAND_OK)
      return 0;                        // Exit and indicate error
   
   C2_WriteDR(page);                   // Send FLASH page number
   Poll_InBusy;                        // Wait for input acknowledge

   Poll_OutReady;                      // Wait for ready indicator
   status = C2_ReadDR();               // Read FLASH programming interface status
   if (status != COMMAND_OK)
      return 0;                        // Exit and indicate error
   
   C2_WriteDR(0x00);                   // Dummy write to initiate erase
   Poll_InBusy;                        // Wait for input acknowledge

   Poll_OutReady;                      // Wait for erase operation to complete
   return 1;                           // Exit and indicate success
}

// C2_Device_Erase:
// - Erases the entire FLASH memory space
// - Assumes that FLASH accesses via C2 have been enabled prior to the function call
// - Function call returns a '1' if successful; returns a '0' if unsuccessful
char C2_DeviceErase(void)
{
   unsigned char status;       // FPI status information holder

   C2_WriteAR(FPDAT);          // Select the FLASH Programming Data register for C2 Data register accesses
   C2_WriteDR(DEVICE_ERASE);   // Send Device Erase command
   Poll_InBusy;                // Wait for input acknowledge

   // Check status before starting FLASH access sequence
   Poll_OutReady;               // Wait for status information
   status = C2_ReadDR();        // Read FLASH programming interface status
   if (status != COMMAND_OK)
      return 0;                 // Exit and indicate error

   // Send a three-byte arming sequence to enable the device erase. If the sequence
   // is not received correctly, the command will be ignored.
   // Sequence: 0xDE, 0xAD, 0xA5.
   C2_WriteDR(0xDE);  // Arming sequence command 1
   Poll_InBusy;       // Wait for input acknowledge

   C2_WriteDR(0xAD);  // Arming sequence command 2
   Poll_InBusy;       // Wait for input acknowledge

   C2_WriteDR(0xA5);  // Arming sequence command 3
   Poll_InBusy;       // Wait for input acknowledge

   Poll_OutReady;     // Wait for erase operation to complete

   return 1;          // Exit and indicate success
}

void main (void)
{
	unsigned int i, j; // Misc. counters

	printf("\x1b[2JEFM8LB1 Clonner.\n"
	       "Based on Cygnal application note AN027 and Silicon Labs application note AN127\n"
		   "By Jesus Calvino-Fraga (2008-2018)\n\n");
	
	while(1)
	{
	    printf("Press a key or the 'BOOT' pushbutton to start.\n");
		RLED=LEDOFF;
		GLED=LEDOFF;
		YLED=LEDOFF;
		C2D_DriverOn;
		C2CK=LOW;
		C2D=LOW;
		PWR=PWROFF; // Power-off device
		
		while((FGO==1) && (RI==0)) // Blinking Green LED indicates new device can be inserted
		{
			GLED=(!GLED);
			YLED=(!YLED);
			waitms(300);
		}
		if(RI) getchar();
		GLED=LEDON;
		YLED=LEDON;
		PWR=PWRON; // Power-on device
		waitms(300);
		C2D_DriverOff;
		C2CK=HIGH;
		while(FGO==0);
		GLED=LEDOFF;
		YLED=LEDOFF;
		
		// Read Device ID
		C2_Reset();        // Reset target
		j = C2_GetDevID(); 
   
		printf("Checking for EFM8LB1 microcontroller...");
		if (j != id[1].device_id)
		{
			printf("\nERROR: EFM8LB1 device not present!\n");
			RLED=LEDON;
			goto the_end;
		}
		printf(" Done.\n");
		
		j = C2_ReadSFR(0xAD);
		printf("Checking for %s microcontroller...", id[1].description);
		if (j != id[1].derivative_id)
		{
			printf("\nERROR: %s device not present! (%02x)\n", id[1].description, j);
			RLED=LEDON;
			goto the_end;
		}
		printf(" Done.\n");
		
		// Initiate C2 FLASH Programming
		C2_Reset();  // Start with a target device reset
		C2_Init();   // Enable FLASH programming via C2
		
		printf("Erasing the flash memory...");
		C2_DeviceErase(); // Erase entire code space
		printf(" Done.\n");
		
		printf("Verifying that the flash memory is blank...");
		// Read back entire FLASH memory (should be all '1's)
		for (i=0; i<NUM_BLOCKS; i++) // Perform block reads (0x0000 to 0xFBFF)
		{
			FLASH_ADDR = i*BLOCK_SIZE;    // Set target addresss
			NUM_BYTES = BLOCK_SIZE;       // Set number of bytes to read
			C2_PTR = R_BUF;               // Initialize C2 pointer to the read buffer
			C2_BlockRead();               // Initiate FLASH read            
			for (j=0; j<BLOCK_SIZE; j++)  // Check read data
			{
				if (R_BUF[j] != 0xFF)
				{
					printf("\nERROR: flash memory is not blank. @%04x=%02x\n", FLASH_ADDR+j, R_BUF[j]);
					RLED=LEDON;
					goto the_end;
				}
			}
		}
		printf(" Done.\n");

		printf("Copying from source to target...");
		for (i=0; i<NUM_BLOCKS; i++) // Perform block write/reads
		{
			// Write FLASH block
			FLASH_ADDR = i*BLOCK_SIZE;
			NUM_BYTES = BLOCK_SIZE;
			memcpy(W_BUF, (code unsigned char *)FLASH_ADDR, BLOCK_SIZE);
			// Is the page to write full of 0xff?
			for (j=0; j<BLOCK_SIZE; j++)
			{
				if (W_BUF[j] != 0xFF) break;
			}
			if(j!=BLOCK_SIZE) // At least one byte is not 0xff, so write the block.
			{
				C2_PTR = W_BUF;
				C2_BlockWrite();
			
				// Read back FLASH block
				FLASH_ADDR = i*BLOCK_SIZE;
				NUM_BYTES = BLOCK_SIZE;
				C2_PTR = R_BUF;
				C2_BlockRead();
				
				if (memcmp(R_BUF, W_BUF, BLOCK_SIZE) != 0) // Verify written bytes
				{
					printf("\nERROR: Memory flash failed.\n");
					RLED=LEDON;
					goto the_end;
				}
			}
		}
	
		printf(" Done.\n");
		GLED=LEDON;
		YLED=LEDON;
		C2_Reset();
		printf("The LED connected to the target EFM8LB1 P2.1 should be blinking.\n\n");
				
		the_end:
	    printf("Press a key or the 'BOOT' pushbutton to continue.\n");
		
		//Display the result of the flashing proccess until button pressed
		while((FGO==1)&&(RI==0));
		if(RI) getchar();
		waitms(100); //Debounce...
		while(FGO==0);
		waitms(100); //Debounce...
	}
}   

