// Metal detector code based on period for STM by Jesus (our fave)
// Anticipated frequency without coin ~167kHz
	

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\STM32L051-Sample-Codes\Common\Include\stm32l051xx.h"

#define F_CPU 32000000L

// AL
#define statquo_freq_upper_bound 177600 // approximate
#define max_detectable 179000 



void delay(int dly)
{
	while( dly--);
}

void wait_1ms(void)
{
	// For SysTick info check the STM32L0xxx Cortex-M0 programming manual page 85.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void waitms(int len)
{
	while(len--) wait_1ms();
}

#define PIN_PERIOD (GPIOA->IDR&BIT6)

// GetPeriod() seems to work fine for frequencies between 300Hz and 600kHz - EDIT THIS FUNC IF FREQ IS OUTSIDE RANGE
// 'n' is used to measure the time of 'n' periods; this increases accuracy.
long int GetPeriod (int n)
{
	int i;
	unsigned int saved_TCNT1a, saved_TCNT1b;
	
	SysTick->LOAD = 0xffffff;  // 24-bit counter set to check for signal present
	SysTick->VAL = 0xffffff; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while (PIN_PERIOD!=0) // Wait for square wave to be 0
	{
		if(SysTick->CTRL & BIT16) return 0;
	}
	SysTick->CTRL = 0x00; // Disable Systick counter

	SysTick->LOAD = 0xffffff;  // 24-bit counter set to check for signal present
	SysTick->VAL = 0xffffff; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while (PIN_PERIOD==0) // Wait for square wave to be 1
	{
		if(SysTick->CTRL & BIT16) return 0;
	}
	SysTick->CTRL = 0x00; // Disable Systick counter
	
	SysTick->LOAD = 0xffffff;  // 24-bit counter reset
	SysTick->VAL = 0xffffff; // load the SysTick counter to initial value
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	for(i=0; i<n; i++) // Measure the time of 'n' periods
	{
		while (PIN_PERIOD!=0) // Wait for square wave to be 0
		{
			if(SysTick->CTRL & BIT16) return 0;
		}
		while (PIN_PERIOD==0) // Wait for square wave to be 1
		{
			if(SysTick->CTRL & BIT16) return 0;
		}
	}
	SysTick->CTRL = 0x00; // Disable Systick counter

	return 0xffffff-SysTick->VAL;
}

// LQFP32 pinout
//             ----------
//       VDD -|1       32|- VSS
//      PC14 -|2       31|- BOOT0
//      PC15 -|3       30|- PB7
//      NRST -|4       29|- PB6
//      VDDA -|5       28|- PB5
//       PA0 -|6       27|- PB4
//       PA1 -|7       26|- PB3
//       PA2 -|8       25|- PA15
//       PA3 -|9       24|- PA14
//       PA4 -|10      23|- PA13
//       PA5 -|11      22|- PA12
//       PA6 -|12      21|- PA11
//       PA7 -|13      20|- PA10 (Reserved for RXD)
//       PB0 -|14      19|- PA9  (Reserved for TXD)
//       PB1 -|15      18|- PA8  (Measure the period at this pin)
//       VSS -|16      17|- VDD
//             ----------
float mapped(float x, float in_min, float in_max, float out_min, float out_max)
{
	float tempa, tempb, tempc, tempd, tempe, tempf;
	
	tempa = (x - in_min);
	printf("%f\r\n", tempa);
	tempb = (out_max - out_min);
	printf("%f\r\n", tempb);
	tempc = tempa * tempb;
	printf("%f\r\n", tempc);
	tempd = (in_max - in_min);
	printf("%f\r\n", tempd);
	tempe = (float)tempc/(float)tempd;
	printf("%f\r\n", tempe);
	tempf = tempe+ out_min;
	printf("%f\r\n", tempf);
	return tempf;
}
int metal_strength(int frequency) {
	int strength = 11;
	float difference = 0;

	if (frequency > statquo_freq_upper_bound) {
		printf("higher than normal\r\n");
		if (frequency > max_detectable) {
			printf("max strength detected\r\n");
			strength = 10;
		}
		else {
			difference = abs(frequency - statquo_freq_upper_bound);
			strength = mapped (difference, statquo_freq_upper_bound, max_detectable, 0.0, 10.0);
		}
	}
	else {
		printf("error. finding metal strength but no metal should be detected.\r\n");
	}

	return strength;
}

void main(void)
{
	long int count;
	float T, f;
	int metal_strength_val = 0;
	
	RCC->IOPENR |= 0x00000001; // peripheral clock enable for port A
	
	GPIOA->MODER &= ~(BIT12 | BIT13); // Make pin PA6 input
	// Activate pull up for pin PA6:
	GPIOA->PUPDR |= BIT12; 
	GPIOA->PUPDR &= ~(BIT13); 

	waitms(500); // Wait for putty to start.
	printf("Period measurement using the Systick free running counter.\r\n"
	      "Connect signal to PA6 (pin 12).\r\n");

	//Need a function to translate from period to whether we detect a coin
	while(1)
	{
		count=GetPeriod(100);
		if(count>0)
		{
			T=count/(F_CPU*100.0); // Since we have the time of 100 periods, we need to divide by 100
			f=1.0/T;
			printf("f=%.2fHz, count=%d            \r", f, count);

			if (f > statquo_freq_upper_bound) {
				//printf("Mine some metal!\r\n");
				// Determine the strength of the metal detected - map function to 1-10
				metal_strength_val = metal_strength(f);
				if (metal_strength_val > 10) {
					printf("error in function\r\n");
				}
				else {
					printf("Metal Strength (1-10): %i\r\n", metal_strength_val);
				}
				
			}
			else {
				printf("Nothing Detected.\r\n"); 
				metal_strength_val = 0;
			}

		}
		else
		{
			printf("NO SIGNAL                     \r");
		}
		fflush(stdout); // GCC printf wants a \n in order to send something.  If \n is not present, we fflush(stdout)
		waitms(200);

	
	}
}

