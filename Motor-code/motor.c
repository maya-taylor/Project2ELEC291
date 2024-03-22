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

/* UNTESTED - GL
 * Function: adjusts motor 
 * return: float - scaling factor
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
    Function: Returns a PWM value between 65 - 100 based on joy stick position
    Note: Joystick already returns values between 65 - 100
    Param: velocity -- 
    Param: PWM_adjust -- value adjustment to PWM based on angle, between -1 and 1
*/
float get_PWM (float velocity, float PWM_adjust) {
    float PWM;
    float adjust = velocity * fabsf(PWM_adjust) / MAX_VELOCITY; // returns a value between 0 and 1
    
    // printf("adjust = %f, PWM = %f\n", adjust, PWM);
    // printf(", in func = %f\n", PWM_adjust);
    // map 0 to 1 to 65 - 100
    PWM = MIN_PWM + (100.00 - MIN_PWM) * adjust; // linear mapping function
    return PWM;
}

/*
* Function: Turns on and off transistors based on power adjustment value
* Param: PWM - between 
* Return: High/Low value to turn off or on transistors in the h bridge 
*/
int LeftMotorDirection (float PWM, float PWM_adjust) {

    // Clock wise rotation
    if (PWM_adjust >= 0) {
        // LEFT_MOTOR = PWM;
        // LEFT_GROUND = 0;
    }
    else {
        // LEFT_MOTOR = 100-PWM; // Inversed since 
        // LEFT_GROUND = 1;
    }
        return 0;    
}

int RightMotorDirection (float PWM, float PWM_adjust) {
    if (PWM_adjust >= 0)
        return 1; /* ---- Keep/change after testing*/
    else
        return 0;    
}

int main (void) {
    // need to add in buffer for the midpoint values

    // hardcoded x and y values for testing
    double x = 1.1199;
    double y = 1.2288;
    
    // the below voltage values are used as zeros
    double midX = 0.0; // 2.30355 V
    double midY = 0.0; // 2.37534 V
    double angle;    
    double velocity;

    /* used to replace hardcoded values */
    // note that x and y (floats in the reading) must be cast to doubles)
    // calculate the velocity we should be moving at using abs()
    velocity = sqrt(pow((double)x,2)+pow((double)y,2));

    // calculate the direction in which we should be moving (in radians)
    /* angle calculation
        - an if statement to change divide by zero errors
        - checking values of y and x so that domain is correct
        - y positive => 0 to 180
        - y negative => 0 to -180
    */
    angle = atan2f(y,x);

    // tests
    float t = 0;
    while (t < 2*PI) {
        x = 50.0 * cos (t); // setting imaginary x joystick input
        y = 50.0 * sin (t); // setting imaginary y joystick input

        angle = atan2f(y,x); 
        velocity =  sqrt(pow((double)x,2)+pow((double)y,2)); // set velocity as the magnitude of the joystick cartesian coords

        t += 0.01;
        float left_adjust = LeftMotorAdjust_angle(angle);
        float right_adjust = RightMotorAdjust_angle(angle);
        
        // printf("t = %f , ", t);
        // printf("in main: %f", left_adjust);
        float left_PWM = get_PWM(velocity, left_adjust);
        float right_PWM = get_PWM(velocity, right_adjust) * RIGHT_MOTOR_ADJUST;

        
        // printf("%f, %f, %f, %f, %f, %f\n", t, x, y, angle, left_adjust, right_adjust);
        printf("%f, %f\n",left_PWM, right_PWM);
    }
    
}
