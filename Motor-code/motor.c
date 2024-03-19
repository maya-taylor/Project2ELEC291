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


//#define LEFT_MOTOR [pinout]
//#define RIGHT_MOTOR [pinout]

//we'll probably have to use turning speed (rpm?) as parameter
//then use conditional statements in the main function to determine speeds n stuff???
void stop (void) {
   
}

void forward (void) {
    
}

void reverse(void) {

}

void turnLeft () {

}
void turnRight () {

}



/* UNTESTED
 * return PWM which is the control signal to a motor
 * 
 * returns: void
 */
/* Control Scheme based on joystic position
* Diagram roughly to scale
* 
*       Left Wheel Power            Right wheel Power
*      0       1      1              1      1      0
*        \     |     /                \     |     /
*          \   |   /                    \   |   /
*            \ | /                        \ | /
*  -1  ------------------1        1 ------------------ -1
*            / | \                        / | \
*          /   |   \                    /   |   \
*        /     |     \                /     |     \     
*     -1      -1      0             0      -1      -1 
* 
* negative values = CCW motion
* scale duty cycle based on power
*/
void spinLeftMotor(float refAngle) {
    // https://components101.com/motors/servo-motor-basics-pinout-datasheet
    // PWM signal probably adjusts motor speed
    // direction of motor spin based on voltages applied to MOSFETs in the Hbridge
    float angle = refAngle * 180 / PI;
    float PWM_adjust = 1;

    if (angle > 0 & angle <= 90) { 
        PWM_adjust = 1;
    }
    else if (angle > 90 & angle <= 180) {
        PWM_adjust = -cos(2*refAngle);
    }
    else if (angle <= 0 & angle > -90) {
        PWM_adjust = cos(2*refAngle);
    }
    else {
        PWM_adjust = -1;
    }
}

void spinRightMotor(float refAngle) {
    float angle = refAngle * 180 / PI;
    float PWM_adjust = 1;

    if (angle > 0 & angle <= 90) { 
        PWM_adjust = -cos(2*refAngle);
    }
    else if (angle > 90 & angle <= 180) {
        PWM_adjust = 1
    }
    else if (angle <= 0 & angle > -90) {
        PWM_adjust = -1;
    }
    else {
        PWM_adjust = cos(2*refAngle);
    }
}

int main (void) {
    // need to add in buffer for the midpoint values

    // hardcoded x and y values for testing
    double x = 1.1199;
    double y = 1.2288;
    
    // the below voltage values are used as zeros
    double midX = 0; // 2.30355 V
    double midY = 0; // 2.37534 V
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

    /* GL */
    
   /* Control Scheme based on joystic position (power in %)
    * Diagram roughly to scale
    * 
    *       Left Wheel Power            Right wheel Power
    *      0      100     100           100    100      0
    *        \     |     /                \     |     /
    *          \   |   /                    \   |   /
    *            \ | /                        \ | /
    *  -100 ----------------100     100 ----------------- -100
    *            / | \                        / | \
    *          /   |   \                    /   |   \
    *        /     |     \                /     |     \     
    *     -100   -100      0            0      -100    -100 
    * 
    * negative values = CCW motion
    * scale duty cycle based on power
    */
    
    // function takes magnitude of velocity and angle
    // assume 
    //  - PWM maps linearly to motor spin speed
    //  - apply scaling factor on the PWM duty cycle based on how far forward the joystick is being pushed
    // angle controls relative speed of each one
    // cases
}
