// #include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include "../Common/Include/serial.h"

#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick
//from motor.#include <stdio.h> 
#include <stdlib.h>

#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 0
#define MAX_PWM 100
#define RIGHT_MOTOR_ADJUST 1.0000 //what does this constant do?
#define F_CPU 32000000L

// Global variables	
volatile int PWM_Counter = 0;
volatile unsigned char pwm1=100, pwm2=100;

// Global timer variables (for tb only)
volatile int timerCount_10us = 0;
volatile int timerCount_ms = 0;
volatile int timerCount_s = 0;

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
    int PWM;
    float adjust = velocity * fabsf(PWM_adjust) / MAX_VELOCITY; // returns a value between 0 and 1
    
    // map 0 to 1 to MIN_PWM - 100
    PWM = MIN_PWM + (100 - MIN_PWM) * adjust; // linear mapping function from x,y to PWM value
    return PWM;
}

/*
*   Function: Turns on and off transistors based on power adjustment value
*   Param: PWM - between 
*   Return: High/Low value to turn off or on transistors in the h bridge 
*/
void turnLeftMotor (float PWM, float PWM_adjust) {
    // controlling outputs on PA2 (left motor), and PA3(right motor)
    // is done by bitmasking the ODR register, this sets those pins
    // to be 1 or 0 which then controlls CW or CCW rotation
    // Clock wise rotation
    if (PWM_adjust >= 0) {
        pwm1 = PWM;
        // GPIOA->ODR &= ~BIT2;    // set left motor to 0 when CW
    }
    else { 
        pwm1 = 100-PWM_adjust;  // Inversed by 100 - PWM, turns when `pwm1` is is LOW
        // GPIOA->ODR |= BIT2;     // set left motor to 1 when CCW
    }
}

int turnRightMotor (float PWM, float PWM_adjust) {
    if (PWM_adjust >= 0) {
        pwm2 = PWM;
        // GPIOA->ODR &= ~BIT3;    // set rigth motor to 0 when CW
    }
    else {
        pwm2 = 100-PWM;         // Inversed by 100 - PWM, turns when `pwm2` is LOW
        // GPIOA->ODR |= BIT3;     // set right motor to 1 when CCW
    }
}

// Motor control loop
// Function: edits global variables pwm1 and pwm2

void motorControlLoop (int x, int y) {
	// calculate angle
	float angle = atan2(y,x);
	int velocity =  sqrt(pow(x,2) + pow(y,2));

	// left motor control
	float left_adjust = LeftMotorAdjust_angle(angle); 
	int left_PWM = get_PWM(velocity, left_adjust);
	turnLeftMotor(left_PWM, left_adjust);

	// right motor control
	float right_adjust = RightMotorAdjust_angle(angle); 
	int right_PWM = get_PWM(velocity, right_adjust);
	turnRightMotor(right_PWM, right_adjust);

	// Debug Log/
    // __mingw_printf("%f\n", angle/PI*180);
    // __mingw_printf("%d\n", velocity);
	// printf("x:%d   y:%d  velocity:%d   left_PWM:%d   right_PWM:%d\n", x,y,velocity, left_PWM, right_PWM); 
	printf("x=%d, y=%d, left_PWM = %d, right_PWM = %d, PWM1=%d, PWM2=%d, l_adj=%f, r_adj=%f\n", x, y, left_PWM, right_PWM, pwm1, pwm2, left_adjust, right_adjust); 
}
 
int main(void)
{
    char buf[32];
    int npwm;
	int x, y;
    float t = 0;

    while (t < 2*PI) {
        // x = 50*cos(t);
        // y = 50*sin(t);
        x = 0;
        y = 50;
        motorControlLoop(x,y);

        t += 0.05;
    }

}
