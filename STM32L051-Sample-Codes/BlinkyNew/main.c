#include "../Common/Include/stm32l051xx.h"
#include "../Common/Include/serial.h"
#include "../Common/Include/bits.h"
#include <stdio.h>

#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 65.00
#define RIGHT_MOTOR_ADJUST 1.0000
#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick

//global constants
volatile int PWM_Counter = 0;
volatile unsigned char pwm1=100, pwm2=100;

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
//       PB1 -|15      18|- PA8  (LED+1k)
//       VSS -|16      17|- VDD
//             ----------


volatile int x;

void delay(int dly)
{
	while( dly--) x++;
}

void main(void)
{
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER = (GPIOA->MODER & ~(BIT17|BIT16)) | BIT16; // Make pin PA8 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0))

	eputs("Attach LED to PA8 (pin 18)\r\n");
	
	while(1)
	{
		GPIOA->ODR ^= BIT8; // Toggle PA8
		delay(500000);
	}
}