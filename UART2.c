#include "../Common/Include/stm32l051xx.h"
#include "UART2.h"

// Serial comms routine for the stm32l051 microcontroller.
// Makes use of UART2.  Pins PA9 and PA10 are used for transmission/reception.
// Defines a new version of puts: e(mbedded)puts and egets
// Similar to puts and gets in standard C however egets checks the size
// of the input buffer.  This could be extended to include a timeout quite easily.
// Written by Frank Duignan and modified by Jesus Calvino-Fraga
// 

// define the size of the communications buffer (adjust to suit)
#define MAXBUFFER   64
typedef struct tagComBuffer{
    unsigned char Buffer[MAXBUFFER];
    unsigned Head,Tail;
    unsigned Count;
} ComBuffer2;

ComBuffer2 ComRXBuffer2, ComTXBuffer2;

int PutBuf2(ComBuffer2  *Buf,unsigned char Data);
unsigned char GetBuf2(ComBuffer2  *Buf);
unsigned GetBufCount2(ComBuffer2  *Buf);
int ReadCom2(int Max,unsigned char *Buffer);
int WriteCom2(int Count,unsigned char *Buffer);

void usart_tx2 (void);
void usart_rx2 (void);
unsigned ComOpen2;
unsigned ComError2;
unsigned ComBusy2;

int ReadCom2(int Max,unsigned char *Buffer)
{
	// Read up to Max bytes from the communications buffer
	// into Buffer.  Return number of bytes read
	unsigned i;
  	if (!ComOpen2)
    	return (-1);
	i=0;
	while ((i < Max-1) && (GetBufCount2(&ComRXBuffer2)))
		Buffer[i++] = GetBuf2(&ComRXBuffer2);
	if (i>0)
	{
		Buffer[i]=0;
		return(i);
	}
	else {
		return(0);
	}	
}

int WriteCom2(int Count, unsigned char *Buffer)
{
	// Writes Count bytes from Buffer into the the communications TX buffer
	// returns -1 if the port is not open (configured)
	// returns -2 if the message is too big to be sent
	// If the transmitter is idle it will initiate interrupts by 
	// writing the first character to the hardware transmit buffer
	unsigned i;
	if (!ComOpen2) return (-1);
		
	// Wait for transmission to complete before sending more
	if(Count<MAXBUFFER)
	{
		while ( (MAXBUFFER - GetBufCount2(&ComTXBuffer2)) < Count );
	}
	else
	{
		return (-2);
	}
		
	for(i=0; i<Count; i++) PutBuf2(&ComTXBuffer2,Buffer[i]);
	
	if ( (USART2->CR1 & BIT3)==0) // transmitter was idle, turn it on and force out first character
	{ 
		USART2->CR1 |= BIT3;
		USART2->TDR = GetBuf2(&ComTXBuffer2);		
	} 
	return 0;
}

void initUART2(int BaudRate)
{
	int BaudRateDivisor;
	//int j;
	
	 __disable_irq();
	ComRXBuffer2.Head = ComRXBuffer2.Tail = ComRXBuffer2.Count = 0;
	ComTXBuffer2.Head = ComTXBuffer2.Tail = ComTXBuffer2.Count = 0;
	ComOpen2 = 1;
	ComError2 = 0;
	
	// Turn on the clock for GPIOA (usart 2 uses it)
	RCC->IOPENR |= BIT0;
	
	BaudRateDivisor = 32000000; // assuming 32MHz clock 
	BaudRateDivisor = BaudRateDivisor / (long) BaudRate;

	//Configure PA14 (TXD for USART2, pin 24 in LQFP32 package)
	GPIOA->OSPEEDR  |= BIT28; // MEDIUM SPEED
	GPIOA->OTYPER   &= ~BIT14; // Push-pull
	GPIOA->MODER    = (GPIOA->MODER & ~(BIT28)) | BIT29; // AF-Mode
	GPIOA->AFR[1]   |= BIT26 ; // AF4 selected
	
	//Configure PA15 (RXD for USART2, pin 25 in LQFP32 package)
	GPIOA->MODER    = (GPIOA->MODER & ~(BIT30)) | BIT31; // AF-Mode
	GPIOA->AFR[1]   |= BIT30;  // AF4 selected
	
	RCC->APB1ENR    |= BIT17; // Turn on the clock for the USART2 peripheral

	USART2->CR1 |= (BIT2 | BIT3 | BIT5 | BIT6); // Enable Transmitter, Receiver, Transmit & Receive interrupts.
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;           
	USART2->BRR = BaudRateDivisor;
	USART2->CR1 |= BIT0; // Enable Usart 1
	
	NVIC->ISER[0] |= BIT28; // Enable USART2 interrupts in NVIC	
	__enable_irq();
}

void USART2_Handler(void) 
{
	// check which interrupt happened.
    if (USART2->ISR & BIT7) // is it a TXE interrupt?
		usart_tx2();
	if (USART2->ISR & BIT5) // is it an RXNE interrupt?
		usart_rx2();
}

void usart_rx2 (void)
{
	// Handles serial comms reception
	// simply puts the data into the buffer and sets the ComError flag
	// if the buffer is fullup
	if ( PutBuf2(&ComRXBuffer2, USART2->RDR) )
	{
		ComError2 = 1; // if PutBuf returns a non-zero value then there is an error
	}
}

void usart_tx2 (void)
{
	// Handles serial comms transmission
	// When the transmitter is idle, this is called and the next byte
	// is sent (if there is one)
	if (GetBufCount2(&ComTXBuffer2)) USART2->TDR=GetBuf2(&ComTXBuffer2);
	else
	{
		// No more data, disable the transmitter 
		USART2->CR1 &= ~BIT3;
		if (USART2->ISR & BIT6) USART2->ICR |= BIT6; // Write TCCF to USART_ICR
		if (USART2->ISR & BIT7) USART2->RQR |= BIT4; // Write TXFRQ to USART_RQR
	}
}

int PutBuf2(ComBuffer2 *Buf, unsigned char Data)
{
	//while ( (Buf->Head==Buf->Tail) && (Buf->Count!=0));  /* OverFlow? Wait... */
	if ( (Buf->Head==Buf->Tail) && (Buf->Count!=0)) return(1);  /* OverFlow */
	__disable_irq();
	Buf->Buffer[Buf->Head++] = Data;
	Buf->Count++;
	if (Buf->Head==MAXBUFFER) Buf->Head=0;
	__enable_irq();
	return(0);
}

unsigned char GetBuf2(ComBuffer2 *Buf)
{
    unsigned char Data;
    if ( Buf->Count==0 ) return (0);
    __disable_irq();
    Data = Buf->Buffer[Buf->Tail++];
    if (Buf->Tail == MAXBUFFER) Buf->Tail = 0;
    Buf->Count--;
    __enable_irq();
    return (Data);
}

unsigned int GetBufCount2(ComBuffer2 *Buf)
{
    return Buf->Count;
}

int eputs2(char *s)
{
	// only writes to the comms port at the moment
	if (!ComOpen2) return -1;
	while (*s) WriteCom2(1,s++);
	return 0;
}

void eputc2(char c)
{
	WriteCom2(1,&c);
}

char egetc2(void)
{
	return GetBuf2(&ComRXBuffer2);
}

int egets2(char *s, int Max)
{
	// read from the comms port until end of string
	// or newline is encountered.  Buffer is terminated with null
	// returns number of characters read on success
	// returns 0 or -1 if error occurs
	// Warning: This is a blocking call.
	int Len;
	char c;
	if (!ComOpen2) return -1;
	Len=0; // Until initialization code is put in init.s, we have to do this
	c = 0;
	while ( (Len < Max-1) && (c != LINEFEED) )
	{   
		while (!GetBufCount2(&ComRXBuffer2)); // wait for a character
		c = GetBuf2(&ComRXBuffer2);
		s[Len++] = c;
	}
	if (Len>0)
	{
		s[Len]=0;
	}	
	return Len;
}

int ReceivedBytes2 (void)
{
	return GetBufCount2(&ComRXBuffer2);
}

char egetc_echo2(void)
{
	char c;
	c=egetc2();
	eputc2(c);
	return c;
}

int egets_echo2(char *s, int Max)
{
	// read from the comms port until end of string
	// or newline is encountered.  Buffer is terminated with null
	// returns number of characters read on success
	// returns 0 or -1 if error occurs
	// Warning: This is a blocking call.
	int Len;
	char c;
	if (!ComOpen2) return -1;
	Len=0; // Until initialization code is put in init.s, we have to do this
	c = 0;
	while ( (Len < Max-1) && (c != NEWLINE) )
	{   
		while (!GetBufCount2(&ComRXBuffer2)); // wait for a character
		c = GetBuf2(&ComRXBuffer2);
		eputc2(c);
		s[Len++] = c;
	}
	if (Len>0)
	{
		s[Len]=0;
	}	
	return Len;
}