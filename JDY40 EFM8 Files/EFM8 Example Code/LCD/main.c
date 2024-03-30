#include <EFM8LB1.h>
#include <stdio.h>
#include "lcd.h"

int getsn (char * buff, int len)
{
	int j;
	char c;
	
	for(j=0; j<(len-1); j++)
	{
		c=getchar();
		if ( (c=='\n') || (c=='\r') )
		{
			buff[j]=0;
			return j;
		}
		else
		{
			buff[j]=c;
		}
	}
	buff[j]=0;
	return len;
}

void main (void)
{
	char buff[17];
	// Configure the LCD
	LCD_4BIT();
	
   	// Display something in the LCD
	LCDprint("LCD 4-bit test:", 1, 1);
	LCDprint("Hello, World!", 2, 1);
	while(1)
	{
		printf("Type what you want to display in line 2 (16 char max): ");
		getsn(buff, sizeof(buff));
		printf("\n");
		LCDprint(buff, 2, 1);
	}
}
