#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../Common/Include/serial.h"
#include "UART2.h"

#define F_CPU 32000000L
// #define DEF_F 100000L // 10us tick
// #define DEF_F 100000000L // a tick of an unknown period 10ms tick???
#define DEF_F 10000L // a tick of an unknown period
#define SYSCLK 32000000L

#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 0
#define MAX_PWM 100.0
#define MOTOR_ADJUST  0.93 //value between 0 and 1, multiply PWM of faster motor by this value 
#define FORWARD_VELOCITY 		20.0  // cm/s -- measured value for x = 0, y = 50
#define CW_VELOCITY      		100.0 // angular velocity in degrees/s CW -- measured value for x = 50, y = 0
#define CCW_VELOCITY     		100.0 // angular velocity in degrees/s CCW -- measured value for x = -50, y = 0
#define WIDE_LEFT_TURN_ANGLE_F 	atan2f(47,-16)
#define BACKWARD_CCW_ANGLE 		atan2f(-47,16)
#define WIDE_RIGHT_TURN_ANGLE_F atan2f(47,16)
#define BACKWARD_CW_ANGLE 		atan2f(-47,-16)
#define F_CPU 32000000L

// Global variables	
volatile int PWM_Counter = 0;
volatile unsigned char pwm1 = 0, pwm2 = 0;
volatile int leftDirection = 0;   // 0 if CW, 1 if CCW
volatile int rightDirection = 0;  // 0 if CW, 1 if CCW

// Global timer variables
volatile int timerCount_10us = 0;
volatile int timerCount_ms = 0;
volatile int timerCount_s = 0;


/* 
* Function: adjusts motor 
* Output: Set 4 pins to either `0, 1, or LEFT_PWM, right_PWM
*/

/* Control Scheme based on joystick position
* Diagram roughly to scale
* 
*       Right Wheel Power            Left wheel Power
*       0      1      1              1      1      0
*        \     |     /                \     |     /
*          \   |   /                    \   |   /
*            \ | /                        \ | /
*  -1  ------------------ 1       1 ------------------ -1
*            / | \                        / | \
*          /   |   \                    /   |   \
*        /     |     \                /     |     \     
*     -1      -1      0             0      -1      -1 
* 
* negative values = CCW motion
* scale duty cycle based on power
*/
float RightMotorAdjust_angle(float refAngle) {
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

		// hard coded for wide turns
		if (angle < 50 && angle > 40)
			PWM_adjust = -cos(2*WIDE_LEFT_TURN_ANGLE_F);
		if (angle < 140 && angle > 130)
			PWM_adjust = -cos(2*WIDE_RIGHT_TURN_ANGLE_F);
    }
    else if (angle <= 0 & angle > -90) {
        PWM_adjust = cos(2*refAngle); // this is just math

		if (angle > -50 & angle < -40)
			PWM_adjust = cos(2*BACKWARD_CCW_ANGLE);
		if (angle > -140 & angle < -130)
			PWM_adjust = cos(2*BACKWARD_CW_ANGLE);
    }
    else {
        PWM_adjust = -1;
    }


    return PWM_adjust;
}

float LeftMotorAdjust_angle(float refAngle) { 
    float angle = refAngle * 180 / PI;
    float PWM_adjust = 1;

    if (angle > 0 & angle <= 90) { 
        PWM_adjust = -cos(2*refAngle); // this is just math

		// hard coded for wide turns
		if (angle < 50 && angle > 40)
			PWM_adjust = -cos(2*WIDE_LEFT_TURN_ANGLE_F);
		if (angle < 140 && angle > 130)
			PWM_adjust = -cos(2*WIDE_RIGHT_TURN_ANGLE_F);
    }
    else if (angle > 90 & angle <= 180) {
        PWM_adjust = 1;
    }
    else if (angle <= 0 & angle > -90) {
        PWM_adjust = -1;

		if (angle > -50 & angle < -40)
			PWM_adjust = cos(2*BACKWARD_CCW_ANGLE);
    }
    else {
        PWM_adjust = cos(2*refAngle); // this is just math

		if (angle > -140 & angle < -130)
			PWM_adjust = cos(2*BACKWARD_CW_ANGLE);
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

//THIS IS ACTUALLY THE RIGHT MOTOR

void turnLeftMotor (int PWM, float PWM_adjust) { 
    // controlling outputs on PA2 (left motor), and PA3(right motor)
    // is done by bitmasking the ODR register, this sets those pins
    // to be 1 or 0 which then controlls CW or CCW rotation
    // Clock wise rotation
    if (PWM_adjust < 0) { // Backwards
        pwm1 = PWM;
        GPIOA->ODR &= ~BIT2;    // set left motor to 0 when CW
		// leftDirection = 0;      // 0 if CW
    } 
    else { // Forwards
        pwm1 = (100-PWM*MOTOR_ADJUST);  		// Inversed by 100 - PWM, turns when `pwm1` is is LOW
        GPIOA->ODR |= BIT2;     // set left motor to 1 when CCW
		// leftDirection = 1;      // 1 if CCW
    }
}

// THIS IS ACTUALLY THE LEFT MOTOR
int turnRightMotor (int PWM, float PWM_adjust) { 
    if (PWM_adjust < 0) {
        pwm2 = PWM*MOTOR_ADJUST;
        GPIOA->ODR &= ~BIT4;     // set right motor to 0 when CCW
		// rightDirection = 0;      // 0 if CCW
    }
    else {
        pwm2 = (100-PWM);         // Inversed by 100 - PWM, turns when `pwm2` is LOW
        GPIOA->ODR |= BIT4;     // set right motor to 1 when CCW
		// rightDirection = 1;      // 1 if CCW
    }
}

// Motor control loop
// Function: edits global variables pwm1 and pwm2 then exits, non-blocking
void motorControlLoop (int x, int y) {

	// calculate angle
	float angle;
	int velocity;
	float left_adjust;
	int left_PWM;
	float right_adjust;
	int right_PWM;

	velocity =  sqrt(pow(x,2) + pow(y,2));

	if (velocity > 50) // error handling if velocity is greater than 50
		velocity = 50; 


	//Added this to stop assigning a undeclared value to angle
	// stop both motors when x and y are zero
	if ((x||y) == 0) {
		pwm1 = 0;    
		GPIOA->ODR &= ~BIT2;
		pwm2 = 0;
		GPIOA->ODR &= ~BIT4;

	}
	else {
		angle = atan2f(y,x);

		// left motor control
		left_adjust = LeftMotorAdjust_angle(angle); 
		left_PWM = get_PWM(velocity, left_adjust);
		turnLeftMotor(left_PWM, left_adjust); // set PWM1

		// right motor control
		right_adjust = RightMotorAdjust_angle(angle); 
		right_PWM = get_PWM(velocity, right_adjust);
		turnRightMotor(right_PWM, right_adjust); // set PWM2
	}


	// Debug Log/
	// printf("x:%d   y:%d left_PWM:%d right_PWM:%d  l_adj:%f, r_adj:%f\r\n", x,y, pwm1, pwm2, left_adjust, right_adjust); 
	//printf("x=%d, y=%d, left_PWM = %d, right_PWM = %d, PWM1=%d, PWM2=%d, l_adj=%.2f, r_adj=%.2f\r\n", x, y, left_PWM, right_PWM, pwm1, pwm2, left_adjust, right_adjust); 

}
// seconds - number of seconds to run the test case
void motor_testHandler(int x, int y, int seconds) {
	timerCount_s = 0;
	
	// runs the motor control loop for a certain amount of seconds
	while (timerCount_s < seconds) {
		motorControlLoop(x,y); 
	}
}

void stop (int seconds) {
	int x = 0;
	int y = 0;
	motor_testHandler(x,y,seconds); 
}

void forward (int seconds) {
	int x = 0;
	int y = 50;
	motor_testHandler(x,y,seconds); 
}

void CW_turn(int seconds) {
	

}

void CCW_turn (int seconds) {
	
}

void testCases (void) {
		// Forward max speed for 10s
		int x = 0;
		int y = 50;
		motor_testHandler(x,y,10);

		 // clockwise rotation on one spot for 10s // counter - all directions opposite
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

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 100kHz. 
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	TIM2->SR &= ~BIT0; // clear update interrupt flag

	PWM_Counter++;
			// -- Debug -- Keep this code commented out so see if this is what is causing the ISR issues
			// timerCount_10us++; // Global time elapsed counter (for tb only)

			// update global timer (for tb only)
			// if (timerCount_10us >= 100) {
			// 	timerCount_10us = 0;
			// 	timerCount_ms++;
			// }
			// if (timerCount_ms >= 1000) {
			// 	timerCount_ms = 0;
			// 	timerCount_s++;
			// }
	
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

// Uses SysTick to delay <us> micro-seconds. 
void Delay_us(unsigned char us)
{
	// For SysTick info check the STM32L0xxx Cortex-M0 programming manual page 85.
	SysTick->LOAD = (F_CPU/(1000000L/us)) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer 
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Delay_us(250);
}

void SendATCommand (char * s)
{
	char buff[40];
	printf("Command: %s", s);
	GPIOA->ODR &= ~(BIT13); // 'set' pin to 0 is 'AT' mode.
	waitms(10);
	eputs2(s);
	egets2(buff, sizeof(buff)-1);
	GPIOA->ODR |= BIT13; // 'set' pin to 1 is normal operation mode.
	waitms(10);
	printf("Response: %s", buff);
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
	RCC->APB1ENR |= BIT0;   // turn on clock for timer2 (UM: page 177)
	TIM2->ARR = F_CPU/DEF_F-1;
	NVIC->ISER[0] |= BIT15; // enable timer 2 interrupts in the NVIC
	TIM2->CR1 |= BIT4;      // Downcounting    
	TIM2->CR1 |= BIT7;      // ARPE enable    
	TIM2->DIER |= BIT0;     // enable update event (reload event) interrupt 
	TIM2->CR1 |= BIT0;      // enable counting    


	__enable_irq();

	//from JDY40
	GPIOA->OSPEEDR=0xffffffff; // All pins of port A configured for very high speed! Page 201 of RM0451

	RCC->IOPENR |= BIT0; // peripheral clock enable for port A

    GPIOA->MODER = (GPIOA->MODER & ~(BIT27|BIT26)) | BIT26; // Make pin PA13 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0))
	GPIOA->ODR |= BIT13; // 'set' pin to 1 is normal operation mode.

	GPIOA->MODER &= ~(BIT12 | BIT13); // Make pin PA8 input
	// Activate pull up for pin PA6:
	GPIOA->PUPDR |= BIT12; 
	GPIOA->PUPDR &= ~(BIT13);
	
}

#define PIN_PERIOD (GPIOA->IDR&BIT6)

// GetPeriod() seems to work fine for frequencies between 300Hz and 600kHz - EDIT THIS FUNC IF FREQ IS OUTSIDE RANGE
// 'n' is used to measure the time of 'n' periods; this increases accuracy.
long int GetPeriod (int n)
{
	__disable_irq();
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
	for(i = 0; i < n; i++) // Measure the time of 'n' periods
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
	__enable_irq();

	return 0xffffff-SysTick->VAL;
}


int main(void)
{
    char buff[80];
    int npwm;
	int x, y;
	long int count;
	float T, f;
	
    RCC->IOPENR |= 0x00000001; // peripheral clock enable for port A Rohan wants to delete this, already declared in hardware init

	GPIOA->MODER &= ~(BIT12 | BIT13); // Make pin PA6 input
	// Activate pull up for pin PA6:
	GPIOA->PUPDR |= BIT12; 
	GPIOA->PUPDR &= ~(BIT13); 

	Hardware_Init();
	initUART2(9600); //from radio
	
	waitms(500); // Give putty a chance to start before we send characters with printf()
	
    printf("Run Servo Motor\r\n");
    printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
    printf("Based on Servo PWM Code in Samples\r\n\r\n");

	// We should select an unique device ID.  The device ID can be a hex
	// number from 0x0000 to 0xFFFF.  In this case is set to 0xABBA
	SendATCommand("AT+DVIDBEEF\r\n");  

	// To check configuration
	SendATCommand("AT+VER\r\n");
	SendATCommand("AT+BAUD\r\n");
	SendATCommand("AT+RFID\r\n");
	SendATCommand("AT+DVID\r\n");
	SendATCommand("AT+RFC\r\n");
	SendATCommand("AT+POWE\r\n");
	SendATCommand("AT+CLSS\r\n");
	
	//printf("\r\nPress and hold a push-button attached to PA8 (pin 18) to transmit.\r\n");

	// // Parsing string to an integer
	// char str[] = "10,20,30,40,50";
	// char *token = strtok(str, ",");
	// parse for x and y data
	

	//timerCount_10us = 0; // increments by 1 every 10us (for tb only)
	//timerCount_ms = 0;   // (for tb only)

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

	//use motorControlLoop(x,y)
	while (1) {
		// insert joystick reading 
		
		if (ReceivedBytes2()>0)
		{
			egets2(buff, sizeof(buff)-1);
			
			printf("RX: %s", buff);
			//printf("len: %d", strlen(buff));
			if (buff[0]=='M' && (strlen(buff)==3 || strlen(buff)==2 || strlen(buff)==1)) {// remote wants metal detector status (&& strlen(buff)==3)

				count=GetPeriod(100);

				
				T= 1.0*count/(F_CPU*100.0); // Since we have the time of 200 periods, we need to divide by 200
				f=1.0/T;
				waitms(1);
				
				printf("count = %d, f=%.2f\r\n",count, f);
				
				if (count==0) {
					eputs2("000000.00\r\n");
				}
				else {
					sprintf(buff,"%.2f\r\n",f);
					eputs2(buff);
				}
			}
			//changing if statement to receive position instead of M being sent
			if (strlen(buff) == 9 || strlen(buff) == 8) // remote sent pos data
			{
				//if (strlen(buff)==2) printf("\r");
				//if (strlen(buff)==1) printf("\r\n");
				x = atoi(buff);
				y = atoi(buff+4);
				printf("xpos = %d, ypos = %d\r\n", x,y);
				motorControlLoop(x, y);
			
			}
			
		}
	}
}