// EFM8_I2C_24C02.c: Shows how to access the 24C02 EEPROM
// By:  Jesus Calvino-Fraga (c) 2010-2018
//
//  ~C51~  

#include <EFM8LB1.h>
#include <stdio.h>
#include <stdlib.h>

#define  SYSCLK         72000000L // System clock frequency in Hz
#define  BAUDRATE       115200L
#define  SMB_FREQUENCY  1000000L   // I2C SCL clock rate (1MHz at 5V)

#define WriteDeviceAddress 0xa0
#define ReadDeviceAddress  0xa1

unsigned int ErrCnt;

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


	P0MDOUT |= 0x10; // Enable UART0 TX as push-pull output
	XBR0 = 0b_0000_0101; // Enable SMBus pins and UART pins P0.4(TX) and P0.5(RX)
	XBR1 = 0X00;
	XBR2 = 0x40; // Enable crossbar and weak pull-ups

	// Configure Timer 0 as the I2C clock source
	CKCON0 |= 0b_0000_0100; // Timer0 clock source = SYSCLK
	TMOD &= 0xf0;  // Mask out timer 1 bits
	TMOD |= 0x02;  // Timer0 in 8-bit auto-reload mode
	// Timer 0 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
	TL0 = TH0 = 256-(SYSCLK/SMB_FREQUENCY/3);
	TR0 = 1; // Enable timer 0
	
	// Configure and enable SMBus
	SMB0CF = 0b_0101_1100; //INH | EXTHOLD | SMBTOE | SMBFTE ;
	SMB0CF |= 0b_1000_0000;  // Enable SMBus

	return 0;
}

// Uses Timer4 to delay <ms> mili-seconds. 
void Timer4ms(unsigned int ms)
{
	unsigned int i;// usec counter
	unsigned char k;
	
	k=SFRPAGE;
	SFRPAGE=0x10;
	
	TMR4RL = 65536-(SYSCLK/(1000L*12L)); // Set Timer4 to overflow in 1 ms.
	TMR4 = TMR4RL;                 // Initialize Timer4 for first overflow
	
	TF4H=0; // Clear overflow flag
	TR4=1;  // Start Timer4
	for (i = 0; i < ms; i++)       // Count <ms> overflows
	{
		while (!TF4H);  // Wait for overflow
		TF4H=0;         // Clear overflow indicator
	}
	TR4=0; // Stop Timer4
	SFRPAGE=k;	
}

void I2C_write (unsigned char output_data)
{
	ACK = 1;
	STA = 0;
	STO = 0;
	SMB0DAT = output_data; // Put data into buffer
	SI = 0;
	while (!SI); // Wait until done with send
}

unsigned char I2C_read (void)
{
	unsigned char input_data;

	ACK = 1;
	STA = 0;
	STO = 0;
	SI = 0;
	while (!SI); // Wait until we have data to read
	input_data = SMB0DAT; // Read the data

	return input_data;
}

void I2C_start (void)
{
	ACK = 1;
	STA = 1;     // Send I2C start
	STO = 0;
	SI = 0;
	while (!SI); // Wait until start sent
	STA = 0;     // Reset I2C start
}

void I2C_stop(void)
{
	ACK = 0;
	STA = 0;
	STO = 1;  	// Perform I2C stop
	SI = 0;	// Clear SI
	Timer4ms(1);
	//while (!SI);	   // Wait until stop complete (Doesn't work???)
}

void Display (void)
{
	int j;
	unsigned char val;
	
	printf("\r\nMemory contents:\r\n");
	
	I2C_start();
	I2C_write(WriteDeviceAddress);
	I2C_write(0);

	I2C_start();
	I2C_write(ReadDeviceAddress);
	
	for(j=0; j<256; j++)
	{
		if((j&0xf)==0) printf("\n%02x: ", j);
		val=I2C_read();
		printf("%02x ", val);
	}
	I2C_stop();
	
	printf("\n");
}

void Write24c02(unsigned char *Wdata, unsigned char RomAddress, unsigned char number)
{
	I2C_start();
	I2C_write(WriteDeviceAddress);
	I2C_write(RomAddress);

	for(;number!=0;number--) 
	{
		I2C_write(*Wdata);
		Wdata++;
	}
	I2C_stop();
}

void Fill24c02 (unsigned char val)
{
	unsigned int i;

	for(i=0; i<256; i++) 
	{
		if ((i&0x07)==0) // 8 bytes per page only
		{
			if (i>0)
			{
				I2C_stop();
				Timer4ms(10);
			}
			I2C_start();
			I2C_write(WriteDeviceAddress);
			I2C_write(i);
		}
		I2C_write(val);
	}
	I2C_stop();
}

void Read24c02(unsigned char *RamAddress, unsigned char RomAddress, unsigned char bytes)
{
	I2C_start();
	I2C_write(WriteDeviceAddress);
	I2C_write(RomAddress);

	I2C_start();
	I2C_write(ReadDeviceAddress);

	while(bytes!=1)
	{
		*RamAddress=I2C_read();
		RamAddress++;
		bytes--;
	}
	*RamAddress=I2C_read();
	I2C_stop();
}

void modify_byte (void)
{
	unsigned char addr, val;
	int x;
	
	printf("Location: ");
	scanf("%x", &x);
	addr=x;
	printf("\nValue: ");
	scanf("%x", &x);
	val=x;
	Write24c02(&val,addr,1);
}

void Test (unsigned char testval)
{
	unsigned int k;
	unsigned int cnt=0;
    xdata unsigned char inbuf[0x100];
	
	Fill24c02(testval);
	Read24c02(inbuf, 0, 0x100);

	for(k=0; k<0x100; k++)
	{
		if(inbuf[k]!=testval)
		{
			if(cnt==0) printf("\n0x%02x failed at:", testval);
			ErrCnt++;
			if( ((cnt&0x0f)==0) && (cnt>0) ) printf("\n               ");
			cnt++;
			printf(" %02x", k);
		}
	}
}

void main_menu (void)
{
	printf("\nMenu:\n"
	        "  1) Display memory content.\n"
	        "  2) Modify byte.\n"
	        "  3) Fill memory.\n"
	        "  4) Erase memory.\n"
	        "  5) Pattern test.\n"
	        "  6) Ramdom test.\n"
	        "\nAction: ");
}

void main (void)
{
	char c;
	unsigned int i, k;
	unsigned char valin, valout;
	
	code unsigned char pattern[]={
		 0x00,  0xff,  0x55,  0xaa,  0x0f,  0xf0,  0x5a,  0xa5,
		 0x01,  0x02,  0x04,  0x08,  0x10,  0x20,  0x40,  0x80,
		~0x01, ~0x02, ~0x04, ~0x08, ~0x10, ~0x20, ~0x40, ~0x80 };
	
	Timer4ms(1000);
	printf("\x1b[2J\x1b[1;1H"); // Clear screen using ANSI escape sequence.
	printf("\nEFM8LB1 24C02 I2C EEPROM Test.\n\n");

	main_menu();
		
	while(1)
	{
		if(RI)
		{
			Timer4ms(50);
			c=getchar();
			putchar('\n');
			switch (c)
			{
				case 'd': case 'D': case '1':
				    Display();
					break;
				case 'm': case 'M': case '2':
					modify_byte();
					break;
			    case 'f': case 'F': case '3':
					printf("Value: ");
					scanf("%x", &i);
			        Fill24c02(i);
			        break;
			    case 'e': case 'E': case '4':
			        Fill24c02(0xff);
			        break;			        
			    case 'p': case 'P': case '5':
				    ErrCnt=0;		
					printf("\nPattern testing all memory locations:\n");
				
					for(i=0; i<sizeof(pattern); i++)
					{
					    Test(pattern[i]);
					    putchar('*');
					}
					if(ErrCnt>0)
					{
						printf("\nThere were %d ERROR(s).\n", ErrCnt);
					}
					else
					{
						printf("\nNo errors.  Memory works fine!\n");
					}
			        break;
			    case 'r': case 'R': case '6':
				    printf("\nRandom testing all memory locations.\n");
					srand(TL0);
					valout=0xff;
					valin=0;
					for(k=0; k<0x100; k++)
					{
						valout=rand()&0xff;
						Write24c02(&valout,k,1);
						Read24c02(&valin,k,1);
						if((k&0xf)==0)
						{
							printf("\n%02x: ", k);
						}
						printf(" %02x", valout);			
						if(valin!=valout) break;
					}
					if(valin!=valout)
					{
						printf("\nERROR at location %02x.  Wrote %02x but read %02x\n", k, valin, valout);
					}
					else
					{
						printf("\nTest pass\n");
					}
			        break;
				default:
					break;
			}
			main_menu();
		}
	}
}
