// EFM8LB1_Receiver.c:  This program implements a simple serial port
// communication protocol to program, verify, and read SPI flash memories.  Since
// the program was developed to store wav audio files, it also allows 
// for the playback of said audio.  It is assumed that the wav sampling rate is
// 22050Hz, 8-bit, mono.
// ~C51~ 

#include <stdio.h>
#include <stdlib.h>
#include <EFM8LB1.h>
#include <math.h>
#include <string.h>

#define SYSCLK 72000000L
#define BAUDRATE 115200L
#define TIMER_2_FREQ 22050L  // Must match the frequency of the wav file store in external SPI flash
#define F_SCK_MAX 20000000L  // Max SPI SCK freq (Hz) 
#define CS P0_3

// Flash memory commands
#define WRITE_ENABLE     0x06  // Address:0 Dummy:0 Num:0 fMax: 25MHz
#define WRITE_DISABLE    0x04  // Address:0 Dummy:0 Num:0 fMax: 25MHz
#define READ_STATUS      0x05  // Address:0 Dummy:0 Num:1 to infinite fMax: 32MHz
#define READ_BYTES       0x03  // Address:3 Dummy:0 Num:1 to infinite fMax: 20MHz
#define READ_SILICON_ID  0xab  // Address:0 Dummy:3 Num:1 to infinite fMax: 32MHz
#define FAST_READ        0x0b  // Address:3 Dummy:1 Num:1 to infinite fMax: 40MHz
#define WRITE_STATUS     0x01  // Address:0 Dummy:0 Num:1 fMax: 25MHz
#define WRITE_BYTES      0x02  // Address:3 Dummy:0 Num:1 to 256 fMax: 25MHz
#define ERASE_ALL        0xc7  // Address:0 Dummy:0 Num:0 fMax: 25MHz
#define ERASE_BLOCK      0xd8  // Address:3 Dummy:0 Num:0 fMax: 25MHz
#define READ_DEVICE_ID   0x9f  // Address:0 Dummy:2 Num:1 to infinite fMax: 25MHz

// SPI Flash Memory connections:
// 	P0.0: SCK  connected to pin 6
// 	P0.1: MISO connected to pin 2
// 	P0.2: MOSI connected to pin 5
//  P0.3: CS*  connected to pin 1
//  3.3V: connected to pins 3, 7, and 8
//  GND:  connected to pin 4

volatile unsigned long int playcnt=0;
volatile unsigned char play_flag=0; // If '1' the timer interrupt plays the sound (in the 8051 it could be a 'bit', but trying to make this as portable as possible)

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
	
	P0MDOUT  = 0b_0001_1101;//SCK, MOSI, P0.3, TX0 are puspull, all others open-drain
	XBR0     = 0b_0000_0011;//SPI0E=1, URT0E=1
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
  	DAC0=0x80<<4;

	SFRPAGE = 0x00; 

	// SPI inititialization
	SPI0CKR = (SYSCLK/(2*F_SCK_MAX))-1;
	SPI0CFG = 0b_0100_0000; //SPI in master mode
	SPI0CN0 = 0b_0000_0001; //SPI enabled and in three wire mode
	CS=1;

	// Initialize timer 2 for periodic interrupts
	TMR2CN0=0x00;   // Stop Timer2; Clear TF2;
	CKCON0|=0b_0001_0000; // Timer 2 uses the system clock
	TMR2RL=(0x10000L-(SYSCLK/TIMER_2_FREQ)); // Initialize reload value
	TMR2=0xffff;   // Set to reload immediately
	ET2=1;         // Enable Timer2 interrupts
	TR2=1;         // Start Timer2 (TMR2CN is bit addressable)

	EA=1; // Enable interrupts
  		
	return 0;
}

unsigned char uart_getc (void)
{
	char c;
	while (!RI);
	RI=0;
	c=SBUF;
	return c;
}

void uart_putc (unsigned char c)
{
	while (!TI);
	TI=0;
	SBUF=c;
}

unsigned char SPIWrite (unsigned char x)
{
   SPI0DAT=x;
   while(!SPIF);
   SPIF=0;
   return SPI0DAT;
}

void Timer2_ISR (void) interrupt INTERRUPT_TIMER2
{
	unsigned char x;
	
	SFRPAGE=0x0;
	TF2H = 0; // Clear Timer2 interrupt flag

	if (play_flag==1)
	{
		if(playcnt==0) // Done playing?
		{
			CS=1; 
			play_flag=0;
		}
		else
		{
			x=SPIWrite(0x55);
			SFRPAGE = 0x30;
			DAC0=x<<4;
			playcnt--;
		}
	}
}

void Start_Playback (unsigned long int address, unsigned long int numb)
{
	play_flag=0;
	CS=1;

    CS=0;
    SPIWrite(READ_BYTES);
    SPIWrite((unsigned char)((address>>16)&0xff));
    SPIWrite((unsigned char)((address>>8)&0xff));
    SPIWrite((unsigned char)(address&0xff));
    playcnt=numb;
    play_flag=1;
}

void Enable_Write (void)
{
    CS=0;
    SPIWrite(WRITE_ENABLE);
    CS=1;
}

void Check_WIP (void)
{
	unsigned char c;
	do
	{
	    CS=0;
	    SPIWrite(READ_STATUS);
	    c=SPIWrite(0x55);
	    CS=1;
	} while (c&0x01);
}

static const unsigned short crc16_ccitt_table[256] = {
    0x0000U, 0x1021U, 0x2042U, 0x3063U, 0x4084U, 0x50A5U, 0x60C6U, 0x70E7U,
    0x8108U, 0x9129U, 0xA14AU, 0xB16BU, 0xC18CU, 0xD1ADU, 0xE1CEU, 0xF1EFU,
    0x1231U, 0x0210U, 0x3273U, 0x2252U, 0x52B5U, 0x4294U, 0x72F7U, 0x62D6U,
    0x9339U, 0x8318U, 0xB37BU, 0xA35AU, 0xD3BDU, 0xC39CU, 0xF3FFU, 0xE3DEU,
    0x2462U, 0x3443U, 0x0420U, 0x1401U, 0x64E6U, 0x74C7U, 0x44A4U, 0x5485U,
    0xA56AU, 0xB54BU, 0x8528U, 0x9509U, 0xE5EEU, 0xF5CFU, 0xC5ACU, 0xD58DU,
    0x3653U, 0x2672U, 0x1611U, 0x0630U, 0x76D7U, 0x66F6U, 0x5695U, 0x46B4U,
    0xB75BU, 0xA77AU, 0x9719U, 0x8738U, 0xF7DFU, 0xE7FEU, 0xD79DU, 0xC7BCU,
    0x48C4U, 0x58E5U, 0x6886U, 0x78A7U, 0x0840U, 0x1861U, 0x2802U, 0x3823U,
    0xC9CCU, 0xD9EDU, 0xE98EU, 0xF9AFU, 0x8948U, 0x9969U, 0xA90AU, 0xB92BU,
    0x5AF5U, 0x4AD4U, 0x7AB7U, 0x6A96U, 0x1A71U, 0x0A50U, 0x3A33U, 0x2A12U,
    0xDBFDU, 0xCBDCU, 0xFBBFU, 0xEB9EU, 0x9B79U, 0x8B58U, 0xBB3BU, 0xAB1AU,
    0x6CA6U, 0x7C87U, 0x4CE4U, 0x5CC5U, 0x2C22U, 0x3C03U, 0x0C60U, 0x1C41U,
    0xEDAEU, 0xFD8FU, 0xCDECU, 0xDDCDU, 0xAD2AU, 0xBD0BU, 0x8D68U, 0x9D49U,
    0x7E97U, 0x6EB6U, 0x5ED5U, 0x4EF4U, 0x3E13U, 0x2E32U, 0x1E51U, 0x0E70U,
    0xFF9FU, 0xEFBEU, 0xDFDDU, 0xCFFCU, 0xBF1BU, 0xAF3AU, 0x9F59U, 0x8F78U,
    0x9188U, 0x81A9U, 0xB1CAU, 0xA1EBU, 0xD10CU, 0xC12DU, 0xF14EU, 0xE16FU,
    0x1080U, 0x00A1U, 0x30C2U, 0x20E3U, 0x5004U, 0x4025U, 0x7046U, 0x6067U,
    0x83B9U, 0x9398U, 0xA3FBU, 0xB3DAU, 0xC33DU, 0xD31CU, 0xE37FU, 0xF35EU,
    0x02B1U, 0x1290U, 0x22F3U, 0x32D2U, 0x4235U, 0x5214U, 0x6277U, 0x7256U,
    0xB5EAU, 0xA5CBU, 0x95A8U, 0x8589U, 0xF56EU, 0xE54FU, 0xD52CU, 0xC50DU,
    0x34E2U, 0x24C3U, 0x14A0U, 0x0481U, 0x7466U, 0x6447U, 0x5424U, 0x4405U,
    0xA7DBU, 0xB7FAU, 0x8799U, 0x97B8U, 0xE75FU, 0xF77EU, 0xC71DU, 0xD73CU,
    0x26D3U, 0x36F2U, 0x0691U, 0x16B0U, 0x6657U, 0x7676U, 0x4615U, 0x5634U,
    0xD94CU, 0xC96DU, 0xF90EU, 0xE92FU, 0x99C8U, 0x89E9U, 0xB98AU, 0xA9ABU,
    0x5844U, 0x4865U, 0x7806U, 0x6827U, 0x18C0U, 0x08E1U, 0x3882U, 0x28A3U,
    0xCB7DU, 0xDB5CU, 0xEB3FU, 0xFB1EU, 0x8BF9U, 0x9BD8U, 0xABBBU, 0xBB9AU,
    0x4A75U, 0x5A54U, 0x6A37U, 0x7A16U, 0x0AF1U, 0x1AD0U, 0x2AB3U, 0x3A92U,
    0xFD2EU, 0xED0FU, 0xDD6CU, 0xCD4DU, 0xBDAAU, 0xAD8BU, 0x9DE8U, 0x8DC9U,
    0x7C26U, 0x6C07U, 0x5C64U, 0x4C45U, 0x3CA2U, 0x2C83U, 0x1CE0U, 0x0CC1U,
    0xEF1FU, 0xFF3EU, 0xCF5DU, 0xDF7CU, 0xAF9BU, 0xBFBAU, 0x8FD9U, 0x9FF8U,
    0x6E17U, 0x7E36U, 0x4E55U, 0x5E74U, 0x2E93U, 0x3EB2U, 0x0ED1U, 0x1EF0U
};

unsigned short crc16_ccitt(unsigned char val, unsigned short crc)
{
    unsigned short tmp;

    tmp = (crc >> 8) ^ val;
    crc = ((unsigned short)(crc << 8U)) ^ crc16_ccitt_table[tmp];
    return crc;
}

void main (void)
{
    unsigned char c;
    unsigned int j, n;
    unsigned long start, nbytes;
    unsigned int crc; // For software CRC16 calculation (if used)

	playcnt=0;
	play_flag=0;
	CS=1;
      
	while(1)
	{
		c=uart_getc();

		if(c=='#')
		{
			playcnt=0;
			play_flag=0; // Stop previous playback if any
			CS=1;
					
			c=uart_getc();
			switch(c)
			{
				case '0': // Identify command
				    CS=0;
				    SPIWrite(READ_DEVICE_ID);
				    c=SPIWrite((unsigned char)(0xff));
				    uart_putc(c);
				    c=SPIWrite((unsigned char)(0xff));
				    uart_putc(c);
				    c=SPIWrite((unsigned char)(0xff));
				    uart_putc(c);
				    CS=1;
				break;

				case '1': // Erase whole flash (takes a long time)
					Enable_Write();
				    CS=0;
				    SPIWrite(ERASE_ALL);
				    CS=1;
				    Check_WIP();
				    uart_putc(0x01);
				break;
				
				case '2': // Load flash page (256 bytes or less)
					Enable_Write();
				    CS=0;
				    SPIWrite(WRITE_BYTES);
				    c=uart_getc(); // Address bits 16 to 23
				    SPIWrite(c);
				    c=uart_getc(); // Address bits 8 to 15
				    SPIWrite(c);
				    c=uart_getc(); // Address bits 0 to 7
				    SPIWrite(c);
				    n=uart_getc(); // Number of bytes to write.
				    if(n==0) n=256;
				    for(j=0; j<n; j++)
				    {
				    	c=uart_getc();
				    	SPIWrite(c);
				    }
				    CS=1;
				    Check_WIP();
				    uart_putc(0x01);
				break;
				
				case '3': // Read flash bytes (256 bytes or less)
				    CS=0;
				    SPIWrite(READ_BYTES);
				    c=uart_getc(); // Address bits 16 to 23
				    SPIWrite(c);
				    c=uart_getc(); // Address bits 8 to 15
				    SPIWrite(c);
				    c=uart_getc(); // Address bits 0 to 7
				    SPIWrite(c);
				    n=uart_getc(); // Number of bytes to write
				    if(n==0) n=256;
				    for(j=0; j<n; j++)
				    {
				    	c=SPIWrite(0x55);
				    	uart_putc(c);
				    }
				    CS=1;
				break;
				
				case '4': // Playback a portion of the stored wav file
					// Get the start position
					c=uart_getc();
					start=c;
					c=uart_getc();
					start=start*256L+c;
					c=uart_getc();
					start=start*256L+c;
					
					// Get the number of bytes to playback
					c=uart_getc();
					nbytes=c;
					c=uart_getc();
					nbytes=nbytes*256L+c;
					c=uart_getc();
					nbytes=nbytes*256L+c;
					
					Start_Playback(start, nbytes);
				
				break;

				// This version uses the fast hardware CRC calculator.
				case '5': ; // Calculate and send CRC-16 of ISP flash memory from zero to the 24-bit passed value.
					c=uart_getc();
					nbytes=c;
					c=uart_getc();
					nbytes=nbytes*256L+c;
					c=uart_getc();
					nbytes=nbytes*256L+c;
				
				    CS=0;
				    SPIWrite(READ_BYTES);
				    SPIWrite(0x00); // Address bits 16 to 23
				    SPIWrite(0x00); // Address bits 8 to 1
				    SPIWrite(0x00); // Address bits 0 to 7

					SFRPAGE=0x20;   // UART0, CRC, and SPI can work on this page
					CRC0CN0=0b_0000_1000; // Initialize hardware CRC result to zero;
					for(start=0; start<nbytes; start++)
					{
						CRC0IN=SPIWrite(0x00);// Feed new byte to hardware CRC calculator
					}
					CS=1;

					CRC0CN0=0x01; // Set bit to read hardware CRC high byte
					uart_putc(CRC0DAT);
					CRC0CN0=0x00; // Clear bit to read hardware CRC low byte
					uart_putc(CRC0DAT);
					
					SFRPAGE = 0x00;
				break;

				// Software CRC calculation.  Since the EFM8LB1 has a hardware CRC calculator
				// we use that instead of this one (see above) which is a lot faster.  If the
				// part you are using doesn't have a CRC calculator, you can use this instead.
				// Rename this one to '5' and erase the one above.
				case 'X': ; // Calculate and send CRC-16 of ISP flash memory from zero to the 24-bit passed value.
					c=uart_getc();
					nbytes=c;
					c=uart_getc();
					nbytes=nbytes*256L+c;
					c=uart_getc();
					nbytes=nbytes*256L+c;
				
					crc=0;
				    CS=0; // Enable 25Q32 SPI flash memory.
				    SPIWrite(READ_BYTES);
				    SPIWrite(0x00); // Address bits 16 to 23
				    SPIWrite(0x00); // Address bits 8 to 1
				    SPIWrite(0x00); // Address bits 0 to 7
				    
					for(start=0; start<nbytes; start++)
					{
						c=SPIWrite(0x00);
						crc=crc16_ccitt(c, crc); // Calculate CRC here
					}
				    CS=1; // Disable 25Q32 SPI flash memory

					uart_putc(crc/0x100); // Send high byte of CRC
					uart_putc(crc%0x100); // Send low byte of CRC
				break;

				case '6': // Fill flash page (256 bytes or less).
					Enable_Write();
				    CS=0;
				    SPIWrite(WRITE_BYTES);
				    c=uart_getc(); // Address bits 16 to 23
				    SPIWrite(c);
				    c=uart_getc(); // Address bits 8 to 15
				    SPIWrite(c);
				    c=uart_getc(); // Address bits 0 to 7
				    SPIWrite(c);
				    c=uart_getc(); // byte to copy to page
				    for(j=0; j<256; j++)
				    {
				    	SPIWrite(c);
				    }
				    CS=1;
				    Check_WIP();
				    uart_putc(0x01);
				break;
			}
		}
    }  
}	
