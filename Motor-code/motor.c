 /* File Function: Funcitons which map joystick inputs to servo outputs
 * Parameter: x -- x velocity vector
 * Parameter: y -- y velocity vector
 * Return:    PWM -- duty cycle to control motor spin speed
 * Return:    directionArray -- array of booleans to determine motor spin direction
 */

//HY -- update 3/24/2024 -- integrating servo code with this code
//include statements
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "..\STM32L051-Sample-Codes\Common\Include\stm32l051xx.h"
#include "..\STM32L051-Sample-Codes\Common\Include\serial.h"
#include "motor.h"

//defines 
#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 65.00
#define PWM_THRESHOLD 0.3 // armound joystick is deflected from neutral position before motor turns on, between 0 and 1
#define SLOW_MOTOR_ADJUST 1.0000 // which ever is the slower motor, divide by this value
#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick

//global constants
volatile int PWM_Counter = 0;
volatile unsigned char pwm1=100, pwm2=100;


//#define LEFT_MOTOR [pinout]
//#define RIGHT_MOTOR [pinout]


void wait_1ms(void)
{
	// For SysTick info check the STM32l0xxx Cortex-M0 programming manual.
	SysTick->LOAD = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SysTick->VAL = 0; // load the SysTick counter
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // Enable SysTick IRQ and SysTick Timer */
	while((SysTick->CTRL & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SysTick->CTRL = 0x00; // Disable Systick counter
}

void delayms(int len)
{
	while(len--) wait_1ms();
}


// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 1kHz.
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	TIM2->SR &= ~BIT0; // clear update interrupt flag
	PWM_Counter++;
	
	if(pwm1>PWM_Counter)
	{
		GPIOA->ODR |= BIT11;
	}
	else
	{
		GPIOA->ODR &= ~BIT11;
	}

	if(pwm2>PWM_Counter)
	{
		GPIOA->ODR |= BIT12;
	}
	else
	{
		GPIOA->ODR &= ~BIT12;
	}
	
	if (PWM_Counter > 2000) // THe period is 20ms
	{
		PWM_Counter=0;
		GPIOA->ODR |= (BIT11|BIT12);

		// P1_1
		// P1_2 = P1_1
		// P1_1 = 0
	}   
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
//       PA3 -|9       24|- PA14 (left motor control pin)
//       PA4 -|10      23|- PA13 (right motor control pin)
//       PA5 -|11      22|- PA12 (pwm2)
//       PA6 -|12      21|- PA11 (pwm1)
//       PA7 -|13      20|- PA10 (Reserved for RXD)
//       PB0 -|14      19|- PA9  (Reserved for TXD)
//       PB1 -|15      18|- PA8
//       VSS -|16      17|- VDD
//             ----------

void Hardware_Init(void)
{
	// Set up output pins
	RCC->IOPENR |= BIT0; // peripheral clock enable for port A
    GPIOA->MODER = (GPIOA->MODER & ~(BIT22|BIT23)) | BIT22; // Make pin PA11 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT11; // Push-pull
    GPIOA->MODER = (GPIOA->MODER & ~(BIT24|BIT25)) | BIT24; // Make pin PA12 output (page 200 of RM0451, two bits used to configure: bit0=1, bit1=0)
	GPIOA->OTYPER &= ~BIT12; // Push-pull

	// Set up timer
	RCC->APB1ENR |= BIT0;  // turn on clock for timer2 (UM: page 177)
	TIM2->ARR = F_CPU/DEF_F-1;
	NVIC->ISER[0] |= BIT15; // enable timer 2 interrupts in the NVIC
	TIM2->CR1 |= BIT4;      // Downcounting    
	TIM2->CR1 |= BIT7;      // ARPE enable    
	TIM2->DIER |= BIT0;     // enable update event (reload event) interrupt 
	TIM2->CR1 |= BIT0;      // enable counting    
	
    // Configuring pins in push-pull configuration
    // Initialize pins PA14 (24) and PA13 (23) as outputs in push-pull configuration
    GPIOA->MODER = (GPIOA->MODER & ~(BIT27|BIT26)) | BIT26; //REPLACE BIITS WITH PPIN SPECIFIC
    GPIOA->MODER = (GPIOA->MODER & ~(BIT29|BIT28)) | BIT28; //REPLACE BIITS WITH PPIN SPECIFIC
    
	__enable_irq();
}



/* TESTED ON SOFTWARE - GL
* Function: adjusts motor 
* Output: Set 4 pins to either `0, 1, or LEFT_PWM, right_PWM
*/

/* Control Scheme based on joystick position
* Diagram roughly to scale
* 
*       Left Wheel Power            Right wheel Power
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
 *
 *  Mapping Functions Present
 *     PWM_adjust -- threshold and then cosines between transistion regions, between -1 and 1
 *     velocity/MAX_VELOCITY -- linear mapping, 0 to 1 based on joystick position
 * 
 *  Alternative nonlinear / piecewise mapping functions
 *     Set motors to 0 when not pressed, but quickly set values to be between MIN_PWM and 100
 * 
 */
float get_PWM (float velocity, float PWM_adjust) {
    float PWM;
    float adjust = velocity * fabsf(PWM_adjust) / MAX_VELOCITY; // returns a value between 0 and 1
    
    PWM = MIN_PWM + (100.00 - MIN_PWM) * adjust; // linear mapping function, returns a value between MIN_PWM and 100

    // // Threshold mapping
    // if (adjust < 0.2)
    //     return PWM = 0;
    // else {
    //     // map 0 to 1 to MIN_PWM - 100
    //     PWM = MIN_PWM + (100.00 - MIN_PWM) * adjust; // linear mapping function, returns a value between MIN_PWM and 100
    //     return PWM;
    // }

    // Sigmoid mapping

    // Cosine mapping

}

/*
*   Function: Turns on and off transistors based on power adjustment value
*   Param: PWM - between MIN_PWM and 100
*   Return: High/Low value to turn off or on transistors in the h bridge 
*/
void turnLeftMotor (float PWM, float PWM_adjust) {
    // controlling outputs on PA14 (left motor), and PA13(right motor)
    // is done by bitmasking the ODR register, this sets those pins
    // to be 1 or 0 which then controlls CW or CCW rotation
    // Clock wise rotation
    if (PWM_adjust >= 0) {
        pwm1 = PWM_adjust;
        GPIOA->ODR &= ~BIT14;  // set left motor to 0 when CW
    }
    else { 
        pwm1 = 100-PWM_adjust;  // Inversed by 100 - PWM, turns when `pwm1` is is LOW
        GPIOA->ODR |= BIT14;  // set left motor to 1 when CCW
    }
}

int turnRightMotor (float PWM, float PWM_adjust) {
    if (PWM_adjust >= 0) {
        pwm2 = PWM;
        GPIOA->ODR &= ~BIT13; // set rigth motor to 0 when CW
    }
    else {
        pwm2 = 100-PWM;          // Inversed by 100 - PWM, turns when `pwm2` is LOW
        GPIOA->ODR |= BIT13;  // set right motor to 1 when CCW
    }
}

void controllerLoop(float x, float y) {
    while (1) {
        float angle = atan2f(y,x);  // controller angle
        float velocity =  sqrt(pow((double)x,2)+pow((double)y,2)); // set velocity as the magnitude of the joystick cartesian coords

        float left_adjust = LeftMotorAdjust_angle(angle); // calculate PWM adjustment based on angle, allows control bewteen CW and CCW
        float left_PWM = get_PWM(velocity, left_adjust);  // mapping PWM based on input joystick velocity and direction
        turnLeftMotor (left_PWM, left_adjust);            // setting pwm1 to turn motor either CW or CCW 
        
        float right_adjust = RightMotorAdjust_angle(angle);
        float right_PWM = get_PWM(velocity, right_adjust) / SLOW_MOTOR_ADJUST;
        turnRightMotor(right_PWM, right_adjust);
    }
}
/*
int main (void) {
    char buf[32];
    int npwm;
    
    // need to add in buffer for the midpoint values
    double x, y;

    double angle;    
    double velocity;

    Motor Spin Simulation - GL
     * Param: 0 < t < 2PI
     * Out: left_adjust  -- adjustment and direction of left motor
     * Out: right_adjust -- adjustment and direction of right motor
     * Out: left_PWM     -- PWM on left motor
     * Out: right_PWM    -- PWM on right motor
     
    float t = 0;
    float left_PWM;
    float right_PWM;
    float left_adjust;
    float right_adjust;
    Hardware_Init();
    delayms(500); // Give putty a chance to start before we send characters with printf()
    printf("Servo signal generatioin using the STM32L051 using TIM2\r\n");
    printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
    printf("By Jesus Calvino-Fraga (c) 2018-2023.\r\n\r\n");
	
    while (t < 2*PI) {

        
        x = 50.0 * cos (t); // setting imaginary x joystick input
        y = 50.0 * sin (t); // setting imaginary y joystick input

        angle = atan2f(y,x); 
        velocity =  sqrt(pow((double)x,2)+pow((double)y,2)); // set velocity as the magnitude of the joystick cartesian coords

        t += 0.01;
        float left_adjust = LeftMotorAdjust_angle(angle);
        float right_adjust = RightMotorAdjust_angle(angle);
        
        float left_PWM = get_PWM(velocity, left_adjust);
        float right_PWM = get_PWM(velocity, right_adjust) * SLOW_MOTOR_ADJUST;

        turnLeftMotor (left_PWM, left_adjust);
        turnRightMotor(right_PWM, right_adjust);
        
        //integrated from /Servo_PWM/main.c
        printf("PWM1 (60 to 255): ");
    	fflush(stdout);
    	egets_echo(buf, 31); // wait here until data is received
  		printf("\r\n");
	    npwm=atoi(buf);
        left_PWM = 100;
        right_PWM = 70;
	    if(left_PWM!=0)
	    {
		    if(left_PWM>100) left_PWM=100;
		    if(left_PWM<65) left_PWM=65;
		    pwm1=left_PWM;
	    }
	    
	    if(right_PWM!=0)
	    {
		    if(right_PWM>100) right_PWM=100;
		    if(right_PWM<65) right_PWM=65;
		    pwm2=right_PWM;
        }


        //printf("%f, %f, %f, %f, %f, %f\n", t, x, y, angle, left_adjust, right_adjust);
        printf("Right:%f\n, Left:%f\r\n",left_PWM, right_PWM);
    }

    // controller loop process: 
    // take x and y and used to create PWM values for each motor

    angle = atan2f(y,x); 
    velocity =  sqrt(pow((double)x,2)+pow((double)y,2)); // set velocity as the magnitude of the joystick cartesian coords

    left_adjust = LeftMotorAdjust_angle(angle);
    right_adjust = RightMotorAdjust_angle(angle);
    
    left_PWM = get_PWM(velocity, left_adjust);
    right_PWM = get_PWM(velocity, right_adjust) * SLOW_MOTOR_ADJUST;

    // restricting error values to be between MIN_PWM and 100 
    

    turnLeftMotor (left_PWM, left_adjust);
    turnRightMotor(right_PWM, right_adjust);

}*/