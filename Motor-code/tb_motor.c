#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "..\STM32L051-Sample-Codes\Common\Include\stm32l051xx.h"
#include "..\STM32L051-Sample-Codes\Common\Include\serial.h"
//#include "motor.h" //this header file contains all the functions we need

#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 65.00
#define RIGHT_MOTOR_ADJUST 1.0000
#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick

//global constants
volatile int PWM_Counter = 0;
volatile unsigned char pwm1=100, pwm2=100;

#define min_pwm_position 20.0
#define max_pwm_position 50.0



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
//       PA3 -|9       24|- PA14
//       PA4 -|10      23|- PA13
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
	RCC->APB1ENR |= BIT0;   // turn on clock for timer2 (UM: page 177)
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
float get_PWM (float velocity, float PWM_adjust) {
    float PWM;
    float adjust = velocity * fabsf(PWM_adjust) / MAX_VELOCITY; // returns a value between 0 and 1
    
    // map 0 to 1 to MIN_PWM - 100
    PWM = MIN_PWM + (100.00 - MIN_PWM) * adjust; // linear mapping function
    return PWM;
}

/*
*   Function: Turns on and off transistors based on power adjustment value
*   Param: PWM - between 
*   Return: High/Low value to turn off or on transistors in the h bridge 
*/
void turnLeftMotor (float PWM, float PWM_adjust) {
    // controlling outputs on PA14 (left motor), and PA13(right motor)
    // is done by bitmasking the ODR register, this sets those pins
    // to be 1 or 0 which then controlls CW or CCW rotation
    // Clock wise rotation
    if (PWM_adjust >= 0) {
        pwm1 = PWM_adjust;
        GPIOA->ODR &= ~(BIT14);  // set left motor to 0 when CW
    }
    else { 
        pwm1 = 100-PWM_adjust;  // Inversed by 100 - PWM, turns when `pwm1` is is LOW
        GPIOA->ODR |= (BIT14);  // set left motor to 1 when CCW
    }
}

int turnRightMotor (float PWM, float PWM_adjust) {
    if (PWM_adjust >= 0) {
        pwm2 = PWM;
        GPIOA->ODR &= ~(BIT13); // set rigth motor to 0 when CW
    }
    else {
        pwm2 = 100-PWM;          // Inversed by 100 - PWM, turns when `pwm2` is LOW
        GPIOA->ODR |= (BIT13);  // set right motor to 1 when CCW
    }
}

// Receives x and y and runs PWM to motors for a certain amount of time
void testing (float x, float y, float delay) {

    int npwm;
    float angle;    
    float velocity;
    float t = 0;
    float left_PWM;
    float right_PWM;
    float left_adjust;
    float right_adjust;

    while (t < 2* PI) {
        angle = atan2f(y,x); 
        velocity =  sqrt(pow(x,2)+pow(y,2)); // set velocity as the magnitude of the joystick cartesian coords

        left_adjust = LeftMotorAdjust_angle  ((float)angle);
        right_adjust = RightMotorAdjust_angle((float)angle);
        
        left_PWM = get_PWM(velocity, left_adjust);
        right_PWM = get_PWM(velocity, right_adjust) * RIGHT_MOTOR_ADJUST;

        // restricting error values to be between MIN_PWM and 100 
        
        turnLeftMotor (left_PWM, left_adjust);
        turnRightMotor(right_PWM, right_adjust);
        // printf("Right:%f\n, Left:%f\r\n",left_PWM, right_PWM);
        t += 1;

        /* Debug Log*/

        printf("x:%f   y:%f  velocity:%f \nleft:%f right%f\n", x,y,velocity, left_PWM, right_PWM);
        
    }

    // controller loop process: 
    // take x and y and used to create PWM values for each motor


}
// only runs the motor until t = 5 for each x,y value
void short_testing (float x, float y) {

    int npwm;
    float angle;    
    float velocity;
    float t = 0;
    float left_PWM;
    float right_PWM;
    float left_adjust;
    float right_adjust;

    while (t < 5) {
        angle = atan2f(y,x); 
        velocity =  sqrt(pow((float)x,2)+pow((float)y,2)); // set velocity as the magnitude of the joystick cartesian coords

        left_adjust = LeftMotorAdjust_angle(angle);
        left_PWM = get_PWM(velocity, left_adjust);
        turnLeftMotor (left_PWM, left_adjust);
        
        right_adjust = RightMotorAdjust_angle(angle);
        right_PWM = get_PWM(velocity, right_adjust) * RIGHT_MOTOR_ADJUST;
        turnRightMotor(right_PWM, right_adjust);

        // printf("%f, %f, %f, %f, %f, %f\n", t, x, y, angle, left_adjust, right_adjust);
        printf("Right:%f\n, Left:%f\r\n",left_PWM, right_PWM);
        t += 1;
    }

    // controller loop process: 
    // take x and y and used to create PWM values for each motor


}

int main(void) {

    float x, y;
    float both;

    Hardware_Init();
    delayms(500); // Give putty a chance to start before we send characters with printf()
    printf("PWM Motor Control Testbench using the STM32L051 using TIM2\r\n");
    printf("(outputs are PA11 and PA12, pins 21 and 22).\r\n");
	
    x = max_pwm_position;
    y = max_pwm_position;
    testing (x,y,2000);

    // Mapping values to between 60 to 100
        
    // RUN MAIN.C Function with the following parameters
    // 
    
    // /* Test Case 1: left motor on , min PWM */
    // x = -min_pwm_position;
    // y = max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 2: right motor on , min PWM */
    // x = min_pwm_position;
    // y = max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 3: both motors on, max PWM - test smooth transition  */
    // x = 0.0;
    // y = max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 4: right motor on , max PWM */
    // x = max_pwm_position;
    // y = max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 5: left motor on , max PWM */
    // x = -max_pwm_position;
    // y = max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 5: Full stop */
    // x = 0.0;
    // y = 0.0;
    // testing (x,y,2000);
    // /* Test Case 6: Gradual start */

    // for (both = min_pwm_position; both < max_pwm_position; both++) {
    //     short_testing(0.0,both);
    // }
    // /* Test Case 7: Gradual Stop */
    // for (both = max_pwm_position; both > min_pwm_position; both++) {
    //     short_testing(0.0, both);
    // }
    // /* Test Case 8: Figure 8 */
    // // pseudocode - right curve for a full circle until you reach orig position, then left curve for a full circle 
    // x = max_pwm_position;
    // y = max_pwm_position;

    // testing (x,y,2000); // need to test if this is a full circle at this speed

    // x = -max_pwm_position;
    // y = max_pwm_position;

    // testing (x,y, 2000);
    // /* Test Case 9: Square Via Right Turns: test 90 degree turns */
    // for (int i = 0; i < 4; i++) {
    //     // turn
    //     x = max_pwm_position;
    //     y = 0.0;
    //     testing (x,y,2000); // need to figure out time for turn to minimize curve
    //     // move forward
    //     x = min_pwm_position;
    //     y = min_pwm_position;
    //     testing (x,y,2000);
    // }
    // /* Test Case 10: Square Via Left Turns: test 90 degree turns */
    // for (int i = 0; i < 4; i++) {
    //     // turn
    //     x = -max_pwm_position;
    //     y = 0.0;
    //     testing (x,y,2000); // need to figure out time for turn to minimize curve
    //     // move forward
    //     x = min_pwm_position;
    //     y = min_pwm_position;
    //     testing (x,y,2000);
    // }

    //  /* Test Case 11: left motor on REV, min PWM */
    // x = min_pwm_position;
    // y = -max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 12: right motor on REV, min PWM */
    // x = -min_pwm_position;
    // y = -max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 13: both motors on REV, max PWM - test smooth transition  */
    // x = 0.0;
    // y = -max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 14: right motor on REV, max PWM */
    // x = -max_pwm_position;
    // y = -max_pwm_position;
    // testing (x,y,2000);
    // /* Test Case 15: left motor on REV, max PWM */
    // x = max_pwm_position;
    // y = -max_pwm_position;
    // testing (x,y,2000);

    // /* Test Case 16: Square Via Right Turns REV: test 90 degree turns */
    // for (int i = 0; i < 4; i++) {
    //     // turn
    //     x = -max_pwm_position;
    //     y = 0.0;
    //     testing (x,y,2000); // need to figure out time for turn to minimize curve
    //     // move forward
    //     x = -min_pwm_position;
    //     y = -min_pwm_position;
    //     testing (x,y,2000);
    // }
    // /* Test Case 17: Square Via Left Turns REV: test 90 degree turns */
    // for (int i = 0; i < 4; i++) {
    //     // turn
    //     x = max_pwm_position;
    //     y = 0.0;
    //     testing (x,y,2000); // need to figure out time for turn to minimize curve
    //     // move forward
    //     x = -min_pwm_position;
    //     y = -min_pwm_position;
    //     testing (x,y,2000);
    // }

}
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



