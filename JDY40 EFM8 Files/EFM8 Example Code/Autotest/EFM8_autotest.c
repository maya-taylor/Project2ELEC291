// EFM8_autotest.c: Test the EFM8 board for short circuits and open circuits.
// The board's pins must be connected in pairs as follows:
//
// P0_0 and P0_1
// P0_2 and P0_3
// P3_7 and P0_6
// P3_3 and P0_7
// P3_2 and P1_0
// P3_1 and P1_1
// P3_0 and P1_2
// P2_6 and P1_3
// P2_5 and P1_4
// P2_4 and P1_5
// P2_3 and P1_6
// P2_2 and P1_7
// P2_1 and P2_0
//
// By:  Jesus Calvino-Fraga (c) 2008-2017
//

#include <EFM8LB1.h>
#include <stdio.h>

#define SYSCLK      48000000L  // SYSCLK frequency in Hz
#define BAUDRATE      115200L  // Baud rate of UART in bps

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

	P0MDOUT |= 0x10; // Enable UART0 TX as push-pull output
	XBR0     = 0x01; // Enable UART0 on P0.4(TX) and P0.5(RX)                     
	XBR1     = 0X00;
	XBR2     = 0x40; // Enable crossbar and weak pull-ups

	#if (((SYSCLK/BAUDRATE)/2L)>0xFFL)
		#error Timer 0 reload value is incorrect because (SYSCLK/BAUDRATE) > 0xFF
	#endif
	// Configure Uart 0
	SCON0 = 0x10;
	CKCON0 |= 0b_0000_1000 ; // Timer 1 uses the system clock.
	TH1 = 0x100-((SYSCLK/BAUDRATE)/2L);
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

unsigned char countzero(void)
{
	unsigned char j;
	
	j=0;
	
	if (P0_0==0) j++;
	if (P0_1==0) j++;
	if (P0_2==0) j++;
	if (P0_3==0) j++;
	//if (P0_4==0) j++; // TX pin not in test
	//if (P0_5==0) j++; // RX pin not in test
	if (P0_6==0) j++;
	if (P0_7==0) j++;

	if (P1_0==0) j++;
	if (P1_1==0) j++;
	if (P1_2==0) j++;
	if (P1_3==0) j++;
	if (P1_4==0) j++;
	if (P1_5==0) j++;
	if (P1_6==0) j++;
	if (P1_7==0) j++;

	if (P2_0==0) j++;
	if (P2_1==0) j++;
	if (P2_2==0) j++;
	if (P2_3==0) j++;
	if (P2_4==0) j++;
	if (P2_5==0) j++;
	if (P2_6==0) j++;

	if (P3_0==0) j++;
	if (P3_1==0) j++;
	if (P3_2==0) j++;
	if (P3_3==0) j++;
	if (P3_7==0) j++;

	return j;
}

void Set_Pin_Output (unsigned char pin)
{
	unsigned char mask;
	
	mask=(1<<(pin&0x7));
	switch(pin/0x10)
	{
		case 0: P0MDOUT |= mask; break;
		case 1: P1MDOUT |= mask; break;
		case 2: P2MDOUT |= mask; break; 
		case 3: P3MDOUT |= mask; break; 
	}	
}

void Set_Pin_Input (unsigned char pin)
{
	unsigned char mask;
	
	mask=(1<<(pin&0x7));
	mask=~mask;
	switch(pin/0x10)
	{
		case 0: P0MDOUT &= mask; break;
		case 1: P1MDOUT &= mask; break;
		case 2: P2MDOUT &= mask; break; 
		case 3: P3MDOUT &= mask; break; 
	}	
}

void Set_Pin_One (unsigned char pin)
{
	unsigned char mask;
	
	mask=(1<<(pin&0x7));
	switch(pin/0x10)
	{
		case 0: P0 |= mask; break;
		case 1: P1 |= mask; break;
		case 2: P2 |= mask; break; 
		case 3: P3 |= mask; break; 
	}	
}

void Set_Pin_Zero (unsigned char pin)
{
	unsigned char mask;
	
	mask=(1<<(pin&0x7));
	mask=~mask;
	switch(pin/0x10)
	{
		case 0: P0 &= mask; break;
		case 1: P1 &= mask; break;
		case 2: P2 &= mask; break; 
		case 3: P3 &= mask; break; 
	}	
}

bit Read_Pin (unsigned char pin)
{
	unsigned char mask, result;
	
	mask=(1<<(pin&0x7));
	switch(pin/0x10)
	{
		default:
		case 0: result = P0 & mask; break;
		case 1: result = P1 & mask; break;
		case 2: result = P2 & mask; break; 
		case 3: result = P3 & mask; break; 
	}
	return (result?1:0);
}

void dofail(unsigned char pin)
{
	printf("P%d.%d FAILED (OPEN)\n", pin/0x10, pin&7);
	while(1);
}

void dofailb(unsigned char pin)
{
	printf("P%d.%d FAILED (SHORT)\n", pin/0x10, pin&7);
	while(1);
}

void dopass(unsigned char pin)
{
	printf("P%d.%d, ", pin/0x10, pin&7);
}

void Test_Pair (unsigned char pin1, unsigned char pin2)
{
	Set_Pin_Output(pin1);
	Set_Pin_Input(pin2);
	Set_Pin_Zero(pin1);
	WaitXms(2);
	if(Read_Pin(pin2)==1) dofail(pin2);
	if (countzero()!=2) dofailb(pin2);
	dopass(pin2);

	Set_Pin_Output(pin2);
	Set_Pin_Input(pin1);
	Set_Pin_Zero(pin2);
	WaitXms(2);
	if(Read_Pin(pin1)==1) dofail(pin1);
	if (countzero()!=2) dofailb(pin1);
	dopass(pin1);
	
	Set_Pin_One(pin1);
	Set_Pin_One(pin2);
	Set_Pin_Input(pin1);
	Set_Pin_Input(pin2);
}

void Check_Pin_Zero (unsigned char pin)
{
	if(Read_Pin(pin)==0)
	{
		printf("P%d.%d is connected to ground\n", pin/0x10, pin&7);
	}
}

void Initial_Check (void)
{
	if (countzero()!=0)
	{
		Check_Pin_Zero(0x00);
		Check_Pin_Zero(0x01);
		Check_Pin_Zero(0x02);
		Check_Pin_Zero(0x03);
		//Check_Pin_Zero(0x04);
		//Check_Pin_Zero(0x05);
		Check_Pin_Zero(0x06);
		Check_Pin_Zero(0x07);
		Check_Pin_Zero(0x10);
		Check_Pin_Zero(0x11);
		Check_Pin_Zero(0x12);
		Check_Pin_Zero(0x13);
		Check_Pin_Zero(0x14);
		Check_Pin_Zero(0x15);
		Check_Pin_Zero(0x16);
		Check_Pin_Zero(0x17);
		Check_Pin_Zero(0x20);
		Check_Pin_Zero(0x21);
		Check_Pin_Zero(0x22);
		Check_Pin_Zero(0x23);
		Check_Pin_Zero(0x24);
		Check_Pin_Zero(0x25);
		Check_Pin_Zero(0x26);
		Check_Pin_Zero(0x30);
		Check_Pin_Zero(0x31);
		Check_Pin_Zero(0x32);
		Check_Pin_Zero(0x33);
		Check_Pin_Zero(0x37);
		while(1);
	}
}

void main(void)
{
	#define TOUT 500

	WaitXms(TOUT); // Give PuTTY a chance to start
	
	printf("\n\nEFM8 board autotest\n");
	Initial_Check();

	Test_Pair(0x00, 0x01); // P0.0 and P0.1
	Test_Pair(0x02, 0x03); // P0.2 and P0.3
	Test_Pair(0x37, 0x06); // P3.7 and P0.6
	Test_Pair(0x33, 0x07); // etc.
	printf("\n");
	Test_Pair(0x32, 0x10);
	Test_Pair(0x31, 0x11);
	Test_Pair(0x30, 0x12);
	Test_Pair(0x26, 0x13);
	printf("\n");
	Test_Pair(0x25, 0x14);
	Test_Pair(0x24, 0x15);
	Test_Pair(0x23, 0x16);
	Test_Pair(0x22, 0x17);
	printf("\n");
	Test_Pair(0x21, 0x20);
	
	printf("\n\nSuccess!\n");
	
	Set_Pin_Output(0x21);
	while(1)
	{
		P2_1=0;
		WaitXms(TOUT);
		P2_1=1;
		WaitXms(TOUT);
	}
}
