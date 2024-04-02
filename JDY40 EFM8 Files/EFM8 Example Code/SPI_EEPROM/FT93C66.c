// FT93C66A SPI EEPROM test program
// by Jesus Calvino-Fraga (c) 2018

#include <EFM8LB1.h>

#define SPI_MISO P0_1
#define FT93C66_CE P0_3

void SmallDelay (void)
{
	_asm
		nop
		nop
		nop 
		nop 
		nop 
		nop 
	_endasm;
}

// Notice: in the EFM8LB1 each write to SPI0DAT must be followed
// by a read from SPI0DAT, otherwise the receive FIFO keeps the
// received value and will be reported next time SPI0DAT is read.
// Also notice that the FT93C66 is sensitive to the falling edge
// of the clock, therefore select CKPHA=1 while setting the SPI.
unsigned char SPIWrite (unsigned char x)
{
   SPI0DAT=x;
   while(!SPIF);
   SPIF=0;
   return SPI0DAT;
}

void FT93C66_Poll(void)
{
	unsigned char j;
	unsigned char mscount=0;
	
	SmallDelay();
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	while(SPI_MISO==0)
	{
		for(j=0; j<250; j++)
		{
			SmallDelay();
		}
		mscount++;
		if(mscount==200) break;
	}
	FT93C66_CE=0; // De-activate the EEPROM.
}

void FT93C66_Write_Enable(void)
{
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	SPIWrite(0b_1001); // Send start bit, op code
	SPIWrite(0b_1000_0000); // Send enable bits.
	FT93C66_CE=0; // De-activate the EEPROM.
	FT93C66_Poll();
}

void FT93C66_Write_Disable(void)
{
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	SPIWrite(0b_1000); // Send start bit, op code
	SPIWrite(0b_0000_0000); // Send disabble bits.
	FT93C66_CE=0; // De-activate the EEPROM.
	FT93C66_Poll();
}

unsigned char FT93C66_Read(unsigned int add)
{
	unsigned char val;
	FT93C66_CE=1; // Activate the EEPROM.
	SPIWrite(0b_1100|(add/0x100)); // Send start bit, op code, A8
	SPIWrite(add%0x100); // Send A7 down to A0.
	val=SPIWrite(0x00); // Read 8 bits from the memory location
	FT93C66_CE=0; // De-activate the EEPROM.
	return val;
}

void FT93C66_Erase(unsigned int add)
{
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	SPIWrite(0b_1110|(add/0x100)); // Send start bit, op code, A8
	SPIWrite(add%0x100); // Send A7 down to A0.
	FT93C66_CE=0; // De-activate the EEPROM.
	FT93C66_Poll();
}

void FT93C66_Erase_All(void)
{
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	SPIWrite(0b_1001); // Send start bit, op code.
	SPIWrite(0b_0000_0000);
	FT93C66_CE=0; // De-activate the EEPROM.
	FT93C66_Poll();
}

void FT93C66_Write(unsigned int add, unsigned char val)
{
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	SPIWrite(0b_1010|(add/0x100)); // Send start bit, op code, A8
	SPIWrite(add%0x100); // Send A7 down to A0.
	SPIWrite(val); // Send value to write.
	FT93C66_CE=0; // De-activate the EEPROM.
	FT93C66_Poll();
}

void FT93C66_Write_All(unsigned char val)
{
	FT93C66_CE=1; // Activate the EEPROM.
	SmallDelay();
	SPIWrite(0b_1000); // Send start bit, op code.
	SPIWrite(0b_1000_0000);
	SPIWrite(val); // Send value to write.
	FT93C66_CE=0; // De-activate the EEPROM.
	FT93C66_Poll();
}

void FT93C66_Init(void)
{
	FT93C66_CE=0;
}
