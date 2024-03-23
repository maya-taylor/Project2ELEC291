 /* File Function: Funcitons which map joystick inputs to servo outputs
 * Parameter: x -- x velocity vector
 * Parameter: y -- y velocity vector
 * Return:    PWM -- duty cycle to control motor spin speed
 * Return:    directionArray -- array of booleans to determine motor spin direction
 */

//Pseudo/half-completed code for the motor, update and test once 
//we have the rest of the board for the car assembled --HY

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 65.00
#define RIGHT_MOTOR_ADJUST 1.0000

//#define LEFT_MOTOR [pinout]
//#define RIGHT_MOTOR [pinout]

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
    // Clock wise rotation
    if (PWM_adjust >= 0) {
        // LEFT_MOTOR = PWM; // turns when `LEFT_MOTOR` is HIGH
        // LEFT_GROUND = 0;  // set to 0 when CW
    }
    // One pin is set to 1
    // CCW rotation 
    else { 
        // LEFT_MOTOR = 100-PWM; // Inversed by 100 - PWM, turns when `LEFT_MOTOR` is LOW
        // LEFT_GROUND = 1;      // set to 1 when CCW 
    }
}

int turnRightMotor (float PWM, float PWM_adjust) {
    if (PWM_adjust >= 0) {
        // RIGHT_MOTOR = PWM;     // turns when `RIGHT_MOTOR` is HIGH
        // LEFT_GROUND = 0;       // set to 0 when CW
    }
    // One pin is set to 1
    // CW rotation
    else {
        // RIGHT_MOTOR = 100-PWM    // Inversed by 100 - PWM, turns when `RIGHT_MOTOR` is LOW
        // RIGHT_GROUND = 1;        // set to 1 when CCW
    }
}

int main (void) {
    // need to add in buffer for the midpoint values
    double x, y;

    double angle;    
    double velocity;

    /* Motor Spin Simulation - GL
     * Param: 0 < t < 2PI
     * Out: left_adjust  -- adjustment and direction of left motor
     * Out: right_adjust -- adjustment and direction of right motor
     * Out: left_PWM     -- PWM on left motor
     * Out: right_PWM    -- PWM on right motor
     */
    float t = 0;
    while (t < 2*PI) {
        x = 50.0 * cos (t); // setting imaginary x joystick input
        y = 50.0 * sin (t); // setting imaginary y joystick input

        angle = atan2f(y,x); 
        velocity =  sqrt(pow((double)x,2)+pow((double)y,2)); // set velocity as the magnitude of the joystick cartesian coords

        t += 0.01;
        float left_adjust = LeftMotorAdjust_angle(angle);
        float right_adjust = RightMotorAdjust_angle(angle);
        
        float left_PWM = get_PWM(velocity, left_adjust);
        float right_PWM = get_PWM(velocity, right_adjust) * RIGHT_MOTOR_ADJUST;

        turnLeftMotor (left_PWM, left_adjust);
        turnRightMotor(right_PWM, right_adjust);
        
        // printf("%f, %f, %f, %f, %f, %f\n", t, x, y, angle, left_adjust, right_adjust);
        // printf("%f, %f\n",left_PWM, right_PWM);
    }
}
