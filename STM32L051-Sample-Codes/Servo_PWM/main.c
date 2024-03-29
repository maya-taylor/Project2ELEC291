#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../Common/Include/serial.h"

#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick
//from motor.#include <stdio.h> 
#include <stdlib.h>

#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 0
#define MAX_PWM 100.0
#define RIGHT_MOTOR_ADJUST 1.0000 //what does this constant do? -> Adjustment factors incase motors dont match
#define F_CPU 32000000L

// Global variables	
volatile int PWM_Counter = 0;
volatile unsigned char pwm1=100, pwm2=100;

// Global timer variables (for tb only)
volatile int timerCount_10us = 0;
volatile int timerCount_ms = 0;
volatile int timerCount_s = 0;

void wait_1ms(void)
{
	// For SysTick info check the STM32l0xxx Cortex-M0 programming manual.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void delayms(int len)
{
	while(len--) wait_1ms();
}
// following chunk of code from motor testbench
float LeftMotorAdjust_angle(float refAngle) {
    // https://components101.com/motors/servo-motor-basics-pinout-datasheet
    // PWM signal probably adjusts motor speed
    // direction of motor spin based on voltages applied to MOSFETs in the Hbridge
    float angle;
    
    angle = refAngle * 180.0 / PI;
    float PWM_adjust = 1;

    if (angle > 0 & angle <= 90) { 
        PWM_adjust = 1;
    }
    else if (angle > 90 & angle <= 180) {
        PWM_adjust = -cos(2*refAngle); // this is just math
    }
    else if (angle <= 0 & angle > -90) {
        PWM_adjust = cos(2*refAngle); // this is just math
    }
    else {
        PWM_adjust = -1;
    }

    return PWM_adjust;
}

float RightMotorAdjust_angle(float refAngle) { 
    float angle = refAngle * 180 / PI;
    float PWM_adjust = 1;

    if (angle > 0 & angle <= 90) { 
        PWM_adjust = -cos(2*refAngle); // this is just math
    }
    else if (angle > 90 & angle <= 180) {
        PWM_adjust = 1;
    }
    else if (angle <= 0 & angle > -90) {
        PWM_adjust = -1;
    }
    else {
        PWM_adjust = cos(2*refAngle); // this is just math
    }
    
    return PWM_adjust;
}

/*  
 *  Function: Returns a PWM value between 65 - 100 based on joy stick position
 *  Note: Joystick already returns values between 65 - 100
 *  Param: velocity -- 
 *  Param: PWM_adjust -- value adjustment to PWM based on angle, between -1 and 1
 */
float get_PWM (int velocity, float PWM_adjust) {
    float PWM;
    float adjust = velocity * fabsf(PWM_adjust) / MAX_VELOCITY; // returns a value between 0 and 1
    
    // map 0 to 1 to MIN_PWM - 100
    PWM = MIN_PWM + (100.00 - MIN_PWM) * adjust; // linear mapping function from x,y to PWM value
    return PWM;
}

/*
*   Function: Turns on and off transistors based on power adjustment value
*   Param: PWM - between 
*   Return: High/Low value to turn off or on transistors in the h bridge 
*/
void turnLeftMotor (int PWM, float PWM_adjust) {
    // controlling outputs on PA2 (left motor), and PA3(right motor)
    // is done by bitmasking the ODR register, this sets those pins
    // to be 1 or 0 which then controlls CW or CCW rotation
    // Clock wise rotation
    if (PWM_adjust < 0) {
        pwm1 = PWM;
        GPIOA->ODR &= ~BIT2;    // set left motor to 0 when CW
    }
    else { 
        pwm1 = 100-PWM;  		// Inversed by 100 - PWM, turns when `pwm1` is is LOW
        GPIOA->ODR |= BIT2;     // set left motor to 1 when CCW
    }
}

int turnRightMotor (int PWM, float PWM_adjust) {
    if (PWM_adjust < 0) {
        pwm2 = PWM;
        GPIOA->ODR &= ~BIT4;    // set right motor to 0 when CW
    }
    else {
        pwm2 = 100-PWM;         // Inversed by 100 - PWM, turns when `pwm2` is LOW
        GPIOA->ODR |= BIT4;     // set right motor to 1 when CCW
    }
}

// Motor control loop
// Function: edits global variables pwm1 and pwm2

void motorControlLoop (int x, int y) {
	// calculate angle
	float angle = atan2f(y,x);
	int velocity =  sqrt(pow(x,2) + pow(y,2));

	// left motor control
	float left_adjust = LeftMotorAdjust_angle(angle); 
	int left_PWM = get_PWM(velocity, left_adjust);
	turnLeftMotor(left_PWM, left_adjust); // set PWM1

	// right motor control
	float right_adjust = RightMotorAdjust_angle(angle); 
	int right_PWM = get_PWM(velocity, right_adjust);
	turnRightMotor(right_PWM, right_adjust); // set PWM2

	// Stop detection
	// this turns both motors off
	if (abs(x) < 5 & abs(y) < 5) {
		pwm1 = 0;
		pwm2 = 0;
	}

	// Debug Log/
	// printf("x:%d   y:%d left_PWM:%d right_PWM:%d  l_adj:%f, r_adj:%f\r\n", x,y, pwm1, pwm2, left_adjust, right_adjust); 
	printf("x=%d, y=%d, left_PWM = %d, right_PWM = %d, PWM1=%d, PWM2=%d, l_adj=%.2f, r_adj=%.2f\r\n", x, y, left_PWM, right_PWM, pwm1, pwm2, left_adjust, right_adjust); 

}
// seconds - number of seconds to run the test case
void motor_testHandler(int x, int y, int seconds) {
	timerCount_s = 0;
	
	// runs the motor control loop for a certain amount of seconds
	while (timerCount_s < seconds) {
		motorControlLoop(x,y); 
	}
}

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 100kHz. 
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	TIM2->SR &= ~BIT0; // clear update interrupt flag
	PWM_Counter++;
	timerCount_10us++; // Global time elapsed counter (for tb only)

	// update global timer (for tb only)
	if (timerCount_10us >= 100) {
		timerCount_10us = 0;
		timerCount_ms++;
	}
	if (timerCount_ms >= 1000) {
		timerCount_ms = 0;
		timerCount_s++;
	}
	
	if(pwm1>PWM_Counter)
	{
		GPIOA->ODR |= BIT3;
	}
	else
	{
		GPIOA->ODR &= ~BIT3;
	}
	
	if(pwm2>PWM_Counter)
	{
		GPIOA->ODR |= BIT5;
	}
	else
	{
		GPIOA->ODR &= ~BIT5;
	}
	
	if (PWM_Counter > 100) // THe period is 1ms
	{
		PWM_Counter=0;
		GPIOA->ODR |= (BIT3|BIT5);
	}   
}

// LQFP32 pinout
//		             ----------
//		       VDD -|1       32|- VSS
//		      PC14 -|2       31|- BOOT0
//		      PC15 -|3       30|- PB7
//		      NRST -|4       29|- PB6
//		      VDDA -|5       28|- PB5
//		       PA0 -|6       27|- PB4
//		       PA1 -|7       26|- PB3
//  (L Motor)  PA2 -|8       25|- PA15
//  (pwm1)     PA3 -|9       24|- PA14
//	(R Motor)  PA4 -|10      23|- PA13
//	(pwm2)     PA5 -|11      22|- PA12 
//		       PA6 -|12      21|- PA11 
//		       PA7 -|13      20|- PA10 (Reserved for RXD)
//		       PB0 -|14      19|- PA9  (Reserved for TXD)
//		       PB1 -|15      18|- PA8
//		       VSS -|16      17|- VDD
//		             ----------

void Hardware_Init(void)
{
	// Set up output pins
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER = (GPIOA->MODER & ~(BIT4|BIT5)) | BIT4; 	// Make pin PA2 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT2; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT6|BIT7)) | BIT6; 	// Make pin PA3 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT3; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT8|BIT9)) | BIT8; 	// Make pin PA4 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT4; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT10|BIT11)) | BIT10; // Make pin PA5 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT5; // Push-pull

	// Set up timer
	RCC->APB1ENR |= BIT0;  // turn on clock for timer2 (UM: page 177)
	TIM2->ARR = F_CPU/DEF_F-1;
	NVIC->ISER[0] |= BIT15; // enable timer 2 interrupts in the NVIC
	TIM2->CR1 |= BIT4;      // Downcounting    
	TIM2->CR1 |= BIT7;      // ARPE enable    
	TIM2->DIER |= BIT0;     // enable update event (reload event) interrupt 
	TIM2->CR1 |= BIT0;      // enable counting    
	
	__enable_irq();
}

int main(void)
{
    char buf[32];
    int npwm;
	int x, y;
	
	Hardware_Init();
	
	delayms(500); // Give putty a chance to start before we send characters with printf()
	
    printf("Run Servo Motor\r\n");
    printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
    printf("Based on Servo PWM Code in Samples\r\n\r\n");

	// // Parsing string to an integer
	// char str[] = "10,20,30,40,50";
	// char *token = strtok(str, ",");
	// parse for x and y data
	

	timerCount_10us = 0; // increments by 1 every 10us (for tb only)
	timerCount_ms = 0;   // (for tb only)

	/* Test Cases
	* 1. Stop initially 3s
	* 2. Forward max speed for 10s
	* 3. CW rotation on one spot
	* 4. smooth right turn for 20s
	* 5. sharp right turn for 10s
	* 6. backwards max speed for 10s
	* 7. CCW rotation on one spot
	* 8. smooth left turn for 20s
	* 9. sharp left turn for 10s
	* 10. Stop for 5s
	*/

	// Stop for 3s
	while (1) {
		x = 0;
		y = 0;
		motor_testHandler(x,y,3); 

		// Forward max speed for 10s
		x = 0;
		y = 50;
		motor_testHandler(x,y,10);

		// clockwise rotation on one spot for 10s
		x = 50;
		y = 0;
		motor_testHandler(x,y,10);

		// more aggressive right turn for 10s
		x = 40;
		y = 30;
		motor_testHandler(x,y,20);

		// large right turn for 20s
		x = 16;
		y = 47.371;
		motor_testHandler(x,y,20);


		// backwards max speed for 10s
		x = 0;
		y = -50;
		motor_testHandler(x,y,10);

		// counter-clockwise rotation on one spot for 10s
		x = -50;
		y = 0;
		motor_testHandler(x,y,10);

		// sharp left turn for 20s
		x = -40;
		y = 30;
		motor_testHandler(x,y,10);

		// large left turn for 20s
		x = -16;
		y = 47.371;
		motor_testHandler(x,y,20);

		x = 0;
		y = 0;
		// Stop to 5s
		motor_testHandler(x,y,10);
	}
}