// robot code - using new position mapping to characters

#include "../Common/Include/stm32l051xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../Common/Include/serial.h"
#include "UART2.h"

#define F_CPU 32000000L
#define DEF_F 10000L // a 100us tick
#define SYSCLK 32000000L

#define PI 3.141592654
#define FORWARD_VELOCITY 		15.0  // cm/s -- measured value for x = 0, y = 50
#define CW_VELOCITY      		115.0 // angular velocity in degrees/s CW -- measured for forward (this value is good)
#define CCW_VELOCITY     		121.0 // angular velocity in degrees/s CCW -- measured for forward (not measured yet)
#define DIAG_VELOCITY_LEFT		35.0 // diagonal velocity for forward left
#define DIAG_VELOCITY_RIGHT		34.0 // diagonal velocity for forward right
#define F_CPU 32000000L
#define AVG_NUM 5

#define S_MANUAL 				1 // maybe keep
#define S_SQUARE 				2
#define S_FIGURE8 				3
#define S_PATH   				4

#define metalLevel_1   			225
#define metalLevel_2   			300
#define metalLevel_3   			400
#define metalLevel_4   			500
#define metalLevel_5   			700

// Global variables	
volatile int PWM_Counter = 0;
volatile unsigned char pwm1 = 0, pwm2 = 0;

// Global timer variables
volatile int timerCount_100us = 0;
volatile int timerCount_ms = 0;
// volatile int timerCount_s = 0;

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

void stopMotors (void) {
	pwm1 = 0;
	pwm2 = 0;
	GPIOA->ODR &= ~BIT2; // pin is GND
	GPIOA->ODR &= ~BIT4; // pin is GND
}
// This process is blocking
void stopCar_ms (int milliSeconds) {

	timerCount_ms = 0;
	// then turn off motors
	while (timerCount_ms < (milliSeconds)) {
		// printf("millis passed %d\n", timerCount_ms); // debug for testing purposes
		pwm1 = 0;
		pwm2 = 0;
		GPIOA->ODR &= ~BIT2; // pin is GND
		GPIOA->ODR &= ~BIT4; // pin is GND
	}
}

void forward (int cm) {
	timerCount_ms=0;

	// motors go forward at max speed
	int milliSeconds = cm / FORWARD_VELOCITY*1000.0; 
	while (timerCount_ms < milliSeconds) {
		pwm1 = 4; // right motor (slower is faster)
		pwm2 = 0; // left motor  (slower is faster)
		GPIOA->ODR |= BIT2; // pin is GND
		GPIOA->ODR |= BIT4; // pin is GND
	}

	stopMotors();
}

void CW_turn(int degrees) {
	timerCount_ms=0;

	int milliseconds = degrees/CW_VELOCITY*1000.0;
	while (timerCount_ms < milliseconds) {
		pwm2 = 20; 			 	// left forward (lower is faster)
		GPIOA->ODR |= BIT4;  	// pin is at 5V
		pwm1 = 80; 		 		// right backwards
		GPIOA->ODR &= ~BIT2; 	// pin is GND
	}
}

void CCW_turn (int degrees) {
	timerCount_ms = 0;
	
	int milliseconds = degrees/CCW_VELOCITY*1000.0;
	while (timerCount_ms < milliseconds) {
		pwm2 = 80; 				// left forward (lower is faster)
		GPIOA->ODR &= ~BIT4; 	// pin is at 5V
		pwm1 = 20; 				// right backwards
		GPIOA->ODR |= BIT2; 	// pin is GND
	}
}

void diag_left (int degrees) {
	timerCount_ms = 0;

	int milliseconds = degrees/DIAG_VELOCITY_LEFT*1000.0;
	while (timerCount_ms < milliseconds) {
			pwm2 = 40; 			 	// left forward slow
			GPIOA->ODR |= BIT4;  	// pin is at 5V
			pwm1 = 0; 		 		// right forward fast
			GPIOA->ODR |= BIT2; 	// pin is 5V
	}
}

void diag_right (int degrees) {
	timerCount_ms = 0;

	int milliseconds = degrees/DIAG_VELOCITY_RIGHT*1000.0;
	while (timerCount_ms < milliseconds) {
			pwm2 = 0; 			 	// left forward fast
			GPIOA->ODR |= BIT4;  	// pin is at 5V
			pwm1 = 40; 		 		// right forward slow
			GPIOA->ODR |= BIT2; 	// pin is 5V
	}
}

void emergency_stop () {
	char buff[5];
	waitms(1000);
	while(1){
		pwm1 = 0;
		pwm2 = 0;
		GPIOA->ODR &= ~BIT2; // pin is GND
		GPIOA->ODR &= ~BIT4; // pin is GND
		
		if (ReceivedBytes2()>0)
		{
			egets2(buff, sizeof(buff)-1);
			//printf("%s", buff);
			if (buff[0]=='€')
			{
				waitms(500);
				return;
			}
		}
	
	}
}

void str_line () {
	forward(100);
}

void circle () {
	diag_left(360);
}

// drive in square - blocking!!
void square () { 
	for (int i = 0; i < 4; i++) {
		forward(20);
		stopCar_ms(10); 

		CW_turn(88);
		stopCar_ms(10); 
	}
}

// Drives in a figrue 8 path
void figure8(void) {
	diag_left(280);
	forward(20);
	diag_right(280);
	forward(20);
}


// Follows a path that is created by drawing on a GUI in python
void path(void) {

}

// void testCases (void) {
// 		// Forward max speed for 10s
// 		// int x = 0;
// 		// int y = 50;
// 		// motor_testHandler(x,y,10);

// 		forward(100); // go forward 1m
// 		stop(10);

// 		CW_turn(360); // CW turn 360 degrees
// 		stop(10);

// 		CW_turn(360); // CCW turn 360 degrees
// 		stop();
// }

/* Robot Controller
	NOTE: This FSM is blocking. 
	This means that for non-manual states, the robot will not respond to any inputs, including emergency stop, 
	until the action is finished.
	
	State 0 -- Default:
		- runs motor control loop 
*/
// void robot_control_FSM(int robot_state, int x, int y) {
	
// 	switch (robot_state) {
// 		case S_MANUAL:
// 			motorControlLoop (x,y);
// 			break;
// 		case S_SQUARE: 
// 			square(50); // draws a 50cmx50cm square
// 			break;
// 		case S_FIGURE8: 
// 			figure8();
// 			break;
// 		case S_PATH:
// 			path(); 
// 			break;
// 		default:
// 			motorControlLoop(x,y);

// 		robot_state = S_MANUAL; // set back to default after exiting the FSM. 
// 	}
// }

// Interrupt service routines are the same as normal
// subroutines (or C funtions) in Cortex-M microcontrollers.
// The following should happen at a rate of 100Hz. 
// The following function is associated with the TIM2 interrupt 
// via the interrupt vector table defined in startup.c
void TIM2_Handler(void) 
{
	TIM2->SR &= ~BIT0; // clear update interrupt flag

	PWM_Counter++;
	// -- Debug -- Keep this code commented out so see if this is what is causing the ISR issues
	timerCount_100us++; // Global time elapsed counter (for tb only)

	// update global timer (for tb only)
	if (timerCount_100us >= 10) {
		timerCount_100us = 0;
		timerCount_ms++;
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

// Uses SysTick to delay <us> micro-seconds. 




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

// A giant switch statement that maps compressed letters to left wheel and right wheel pwms
void setPWM(char letter) {
    switch (letter) {
        case 'A': // forward fast (lower is faster)
			pwm1 = 0; // right forward 0;
			pwm2 = 1; // left forward = 0; (lower is faster)
			GPIOA->ODR |= BIT2; // pin is at 5V
			GPIOA->ODR |= BIT4; // pin is at 5V
            break;
        case 'B': //forward mid
			pwm1 = 30; // right forward 0;
			pwm2 = 31; // left forward = 0;
			GPIOA->ODR |= BIT2; // pin is at 5V
			GPIOA->ODR |= BIT4; // pin is at 5V
            break;
        case 'C': //forward slow
			pwm1 = 60; // right forward 0;
			pwm2 = 61; // left forward = 0;
			GPIOA->ODR |= BIT2; // pin is at 5V
			GPIOA->ODR |= BIT4; // pin is at 5V
            break;
        case 'D': // backwards fast
			pwm1 = 90; // right backwards
			pwm2 = 87; // left backwards (higher is faster)
			GPIOA->ODR &= ~BIT2; // pin is GND
			GPIOA->ODR &= ~BIT4; // pin is GND
            break;
        case 'E': //backwards mid
			pwm1 = 70; // right backwards
			pwm2 = 67; // left backwards
			GPIOA->ODR &= ~BIT2; // pin is GND (right motor)
			GPIOA->ODR &= ~BIT4; // pin is GND (left motor)
            break;	
        case 'F': //backwards slow
			pwm1 = 50; 				// right backwards
			pwm2 = 47; 				// left backwards
			GPIOA->ODR &= ~BIT2; 	// pin is GND
			GPIOA->ODR &= ~BIT4; 	// pin is GND
            break;
        case 'G': // CW turn on the spot fast
			pwm2 = 20; 			 	// left forward (lower is faster)
			GPIOA->ODR |= BIT4;  	// pin is at 5V
			pwm1 = 80; 		 	// right backwards
			GPIOA->ODR &= ~BIT2; 	// pin is GND
            break;
        case 'H': //CW turn on spot mid
			pwm2 = 40; 			 	// left forward (lower is faster)
			GPIOA->ODR |= BIT4;  	// pin is at 5V
			pwm1 = 60; 		 		// right backwards
			GPIOA->ODR &= ~BIT2; 	// pin is GND
            break;
        case 'I': //CCW turn on spot  fast
			pwm2 = 80; 				// left forward (lower is faster)
			GPIOA->ODR &= ~BIT4; 	// pin is at 5V
			pwm1 = 20; 				// right backwards
			GPIOA->ODR |= BIT2; 	// pin is GND
            break;
        case 'J': //CCW turn on sport mid
			pwm2 = 60; 		// left forward (lower is faster)
			GPIOA->ODR &= ~BIT4; // pin is at 5V
			pwm1 = 40; 	// right backwards
			GPIOA->ODR |= BIT2; // pin is GND
            break;			
        case 'K': //diagonal NE 
			pwm2 = 0;			// left forward fast (lower is faster)
			GPIOA->ODR |= BIT4; // pin is at 5V
			pwm1 = 30;          // right forward slow (lower is faster)
			GPIOA->ODR |= BIT2; // pin is at 5V
            break;
        case 'L': //diagonal NW 
			pwm2 = 30; 			 	// left forward slow
			GPIOA->ODR |= BIT4;  	// pin is at 5V
			pwm1 = 0; 		 		// right forward fast
			GPIOA->ODR |= BIT2; 	// pin is 5V
            break;
        case 'M': //diagonal SE
			pwm1 = 70; 			 // right backwards slow
			GPIOA->ODR &= ~BIT2; // pin is at GND
			pwm2 = 100; 		 // left backwards fast
			GPIOA->ODR &= ~BIT4; // pin is at GND
            break;
        case 'N': //diagonal SW
			pwm1 = 100; // right backwards fast
			GPIOA->ODR &= ~BIT2; // pin is at GND
			pwm2 = 70; // left backwards slow
			GPIOA->ODR &= ~BIT4; // pin is at GND
            break;
        case 'Z': // stop car
			pwm1 = 0;
			pwm2 = 0;
			GPIOA->ODR &= ~BIT2; // pin is GND
			GPIOA->ODR &= ~BIT4; // pin is GND
        default:
            break;
    }
}
/* Function: Decodes letters as 
   Params:
	- letter -- encoded letter that is sent over
*/
void pathFindingDecoder (int letter) {
	// Hardcoded switch statement range
	// If input character does not match with anything, then this will exit without calling anything
	switch (letter) {
		case '0': CW_turn(0); forward(10); break;
        case '1': CW_turn(15); forward(10); break;
        case '2': CW_turn(30); forward(10); break;
        case '3': CW_turn(45); forward(10); break;
        case '4': CW_turn(60); forward(10); break;
        case '5': CW_turn(75); forward(10); break;
        case '6': CW_turn(90); forward(10); break;
        case '7': CW_turn(105); forward(10); break;
        case '8': CW_turn(120); forward(10); break;
        case '9': CW_turn(135); forward(10); break;
        case 'f': CW_turn(150); forward(10); break;
        case 'g': CW_turn(165); forward(10); break;
        case 'h': CW_turn(180); forward(10); break;
        case 'i': CW_turn(195); forward(10); break;
        case 'j': CW_turn(210); forward(10); break;
        case 'k': CW_turn(225); forward(10); break;
        case 'l': CW_turn(240); forward(10); break;
        case 'n': CW_turn(255); forward(10); break;
        case 'o': CW_turn(270); forward(10); break;
        case 'p': CW_turn(285); forward(10); break;
        case 'q': CW_turn(300); forward(10); break;
        case 'r': CW_turn(315); forward(10); break;
        case 's': CW_turn(330); forward(10); break;
        case 't': CW_turn(345); forward(10); break;
        case 'u': CW_turn(0); forward(25); break;
        case 'v': CW_turn(15); forward(25); break;
        case 'w': CW_turn(30); forward(25); break;
        case 'x': CW_turn(45); forward(25); break;
        case 'y': CW_turn(60); forward(25); break;
        case '`': CW_turn(75); forward(25); break;
        case '~': CW_turn(90); forward(25); break;
        case '!': CW_turn(105); forward(25); break;
        case '@': CW_turn(120); forward(25); break;
        case '#': CW_turn(135); forward(25); break;
        case '$': CW_turn(150); forward(25); break;
        case '%': CW_turn(165); forward(25); break;
        case '^': CW_turn(180); forward(25); break;
        case '&': CW_turn(195); forward(25); break;
        case '*': CW_turn(210); forward(25); break;
        case '(': CW_turn(225); forward(25); break;
        case ')': CW_turn(240); forward(25); break;
        case '-': CW_turn(255); forward(25); break;
        case '_': CW_turn(270); forward(25); break;
        case '+': CW_turn(285); forward(25); break;
        case '=': CW_turn(300); forward(25); break;
        case '{': CW_turn(315); forward(25); break;
        case '}': CW_turn(330); forward(25); break;
        case '[': CW_turn(345); forward(25); break;
        case ']': CW_turn(0); forward(35); break;
        case '|': CW_turn(15); forward(35); break;
        case '\\': CW_turn(30); forward(35); break;
        case ';': CW_turn(45); forward(35); break;
        case ':': CW_turn(60); forward(35); break;
        case '"': CW_turn(75); forward(35); break;
        case '\'': CW_turn(90); forward(35); break;
        case '/': CW_turn(105); forward(35); break;
        case '?': CW_turn(120); forward(35); break;
        case ',': CW_turn(135); forward(35); break;
        case '‚': CW_turn(150); forward(35); break;
        case 'ƒ': CW_turn(165); forward(35); break;
        case '„': CW_turn(180); forward(35); break;
        case '…': CW_turn(195); forward(35); break;
        case '†': CW_turn(210); forward(35); break;
        case '‡': CW_turn(225); forward(35); break;
        case 'ˆ': CW_turn(240); forward(35); break;
        case '‰': CW_turn(255); forward(35); break;
        case 'Š': CW_turn(270); forward(35); break;
        case '‹': CW_turn(285); forward(35); break;
        case 'Œ': CW_turn(300); forward(35); break;
        case 'Ž': CW_turn(315); forward(35); break;
        case '‘': CW_turn(330); forward(35); break;
        case '’': CW_turn(345); forward(35); break;
        case '“': CW_turn(0); forward(50); break;
        case '”': CW_turn(15); forward(50); break;
        case '•': CW_turn(30); forward(50); break;
        case '–': CW_turn(45); forward(50); break;
        case '—': CW_turn(60); forward(50); break;
        case '˜': CW_turn(75); forward(50); break;
        case '™': CW_turn(90); forward(50); break;
        case 'š': CW_turn(105); forward(50); break;
        case '›': CW_turn(120); forward(50); break;
        case 'œ': CW_turn(135); forward(50); break;
        case 'ž': CW_turn(150); forward(50); break;
        case 'Ÿ': CW_turn(165); forward(50); break;
        case '¡': CW_turn(180); forward(50); break;
        case '¢': CW_turn(195); forward(50); break;
        case '£': CW_turn(210); forward(50); break;
        case '¤': CW_turn(225); forward(50); break;
        case '¥': CW_turn(240); forward(50); break;
        case '¦': CW_turn(255); forward(50); break;
        case '§': CW_turn(270); forward(50); break;
        case '¨': CW_turn(285); forward(50); break;
        case '©': CW_turn(300); forward(50); break;
        case 'ª': CW_turn(315); forward(50); break;
        case '«': CW_turn(330); forward(50); break;
        case '¬': CW_turn(345); forward(50); break;
        case '­': CW_turn(0); forward(75); break;
        case '®': CW_turn(15); forward(75); break;
        case '¯': CW_turn(30); forward(75); break;
        case '°': CW_turn(45); forward(75); break;
        case '±': CW_turn(60); forward(75); break;
        case '²': CW_turn(75); forward(75); break;
        case '³': CW_turn(90); forward(75); break;
        case '´': CW_turn(105); forward(75); break;
        case 'µ': CW_turn(120); forward(75); break;
        case '¶': CW_turn(135); forward(75); break;
        case '·': CW_turn(150); forward(75); break;
        case '¸': CW_turn(165); forward(75); break;
        case '¹': CW_turn(180); forward(75); break;
        case 'º': CW_turn(195); forward(75); break;
        case '»': CW_turn(210); forward(75); break;
        case '¼': CW_turn(225); forward(75); break;
        case '½': CW_turn(240); forward(75); break;
        case '¾': CW_turn(255); forward(75); break;
        case '¿': CW_turn(270); forward(75); break;
        case 'À': CW_turn(285); forward(75); break;
        case 'Á': CW_turn(300); forward(75); break;
        case 'Â': CW_turn(315); forward(75); break;
        case 'Ã': CW_turn(330); forward(75); break;
        case 'Ä': CW_turn(345); forward(75); break;
        case 'Å': CW_turn(0); forward(100); break;
        case 'Æ': CW_turn(15); forward(100); break;
        case 'Ç': CW_turn(30); forward(100); break;
        case 'È': CW_turn(45); forward(100); break;
        case 'É': CW_turn(60); forward(100); break;
        case 'Ê': CW_turn(75); forward(100); break;
        case 'Ë': CW_turn(90); forward(100); break;
        case 'Ì': CW_turn(105); forward(100); break;
        case 'Í': CW_turn(120); forward(100); break;
        case 'Î': CW_turn(135); forward(100); break;
        case 'Ï': CW_turn(150); forward(100); break;
        case 'Ð': CW_turn(165); forward(100); break;
        case 'Ñ': CW_turn(180); forward(100); break;
        case 'Ò': CW_turn(195); forward(100); break;
        case 'Ó': CW_turn(210); forward(100); break;
        case 'Ô': CW_turn(225); forward(100); break;
        case 'Õ': CW_turn(240); forward(100); break;
        case 'Ö': CW_turn(255); forward(100); break;
        case '×': CW_turn(270); forward(100); break;
        case 'Ø': CW_turn(285); forward(100); break;
        case 'Ù': CW_turn(300); forward(100); break;
        case 'Ú': CW_turn(315); forward(100); break;
        case 'Û': CW_turn(330); forward(100); break;
        case 'Ü': CW_turn(345); forward(100); break;
        case 'Ý': CW_turn(0); forward(125); break;
        case 'Þ': CW_turn(15); forward(125); break;
        case 'ß': CW_turn(30); forward(125); break;
        case 'à': CW_turn(45); forward(125); break;
        case 'á': CW_turn(60); forward(125); break;
        case 'â': CW_turn(75); forward(125); break;
        case 'ã': CW_turn(90); forward(125); break;
        case 'ä': CW_turn(105); forward(125); break;
        case 'å': CW_turn(120); forward(125); break;
        case 'æ': CW_turn(135); forward(125); break;
        case 'ç': CW_turn(150); forward(125); break;
        case 'è': CW_turn(165); forward(125); break;
        case 'é': CW_turn(180); forward(125); break;
        case 'ê': CW_turn(195); forward(125); break;
        case 'ë': CW_turn(210); forward(125); break;
        case 'ì': CW_turn(225); forward(125); break;
        case 'í': CW_turn(240); forward(125); break;
        case 'î': CW_turn(255); forward(125); break;
        case 'ï': CW_turn(270); forward(125); break;
        case 'ð': CW_turn(285); forward(125); break;
        case 'ñ': CW_turn(300); forward(125); break;
        case 'ò': CW_turn(315); forward(125); break;
        case 'ó': CW_turn(330); forward(125); break;
		case 'ô': CW_turn(345); forward(125); break;
		default:
			// Handle unknown characters or default case
			break; // This will make it so that the robot doesn't do anything. 
    }
}

void ButtonCommand (char letter){

    switch(letter){
        case '€':
			printf("Emergency stop :(\r\n");
            emergency_stop();
			printf("Restart :)\r\n");
            break;
        case 'ý':
            str_line();
            break;
        case 'þ':
            square();
            break;
        case '<':
            circle();
            break;
        case '>':
            figure8();
            break;
        default:
            // Handle the default case if none of the expected characters are matched
            break;
    }   
    return;
}


// LQFP32 pinout
//		           ----------
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

char MapFrequency(int freq) {
    char letter;

	if (freq > metalLevel_5)
		letter = 'e';
	else if (freq > metalLevel_4)
		letter = 'd';
	else if (freq > metalLevel_3)
		letter = 'c';
	else if (freq > metalLevel_2)
		letter = 'b';
	else if (freq > metalLevel_1)
		letter = 'a';
	else 
		letter  = 'z';

    return letter;
}

int main(void)
{
    char buff[4];
	int robot_state = S_MANUAL; // sets robot control mode to `MANUAL` which is the default. Joystick controlled  
	long int count;
	float T, f;
	char dirc, freqc;
	
	int sum_count = 0; //count to keep track of first 10 vals
	int sum_freq = 0; //sum of first 10 frequencies
	int baseline_freq;
	int pastfreqs[AVG_NUM];
	int avg_sum = 0;
	int avg_freq = 0;
	int i = 0;

    RCC->IOPENR |= 0x00000001; // peripheral clock enable for port A Rohan wants to delete this, already declared in hardware init

	GPIOA->MODER &= ~(BIT12 | BIT13); // Make pin PA6 input
	// Activate pull up for pin PA6:
	GPIOA->PUPDR |= BIT12; 
	GPIOA->PUPDR &= ~(BIT13); 

	Hardware_Init();
	initUART2(9600); //from radio
	
	waitms(500); // Give putty a chance to start before we send characters with printf()
	
    printf("Metal Detecting Robot\r\n");

	// We should select an unique device ID.  The device ID can be a hex
	// number from 0x0000 to 0xFFFF.  In this case is set to 0xABBA
	SendATCommand("AT+DVIDA0A0\r\n");  

	// To check configuration
	SendATCommand("AT+VER\r\n");
	SendATCommand("AT+BAUD\r\n");
	SendATCommand("AT+RFID\r\n");
	SendATCommand("AT+DVID\r\n");
	SendATCommand("AT+RFC\r\n");
	SendATCommand("AT+POWE\r\n");
	SendATCommand("AT+CLSS\r\n");


	//timerCount_100us = 0; // increments by 1 every 10us (for tb only)
	//timerCount_ms = 0;   // (for tb only)

	//use motorControlLoop(x,y)

	// = = = = = = = = = Testing pathFindingDecoder with a series of characters = = = = = = = 
	// angular velocity test

	//stopCar_ms(1000); // this process is blocking
	//str_line();
	//stopCar_ms(1000); // this process is blocking
	//circle();
	//stopCar_ms(1000); // this process is blocking
	//square();
	//stopCar_ms(1000); // this process is blocking
	//figure8();
	//stopCar_ms(1000); // this process is blocking
	
	// Path Finding test
//	stopCar_ms(2000);
//	int test_char = 'Å'; // forward
//	pathFindingDecoder(test_char);
//	test_char = '³'; // 75cm and turns 90 degrees
//	pathFindingDecoder(test_char);
//	test_char = '³'; // 75cm and turns 90 degrees
//	pathFindingDecoder(test_char);
//	test_char = 'é';
//	pathFindingDecoder(test_char);
//	stopCar_ms(3000);
	
	// = = = = = = = = = Main Loop = = = = = = = 
	while (1) {
		// insert joystick reading 
		
		if (ReceivedBytes2()>0)
		{
			egets2(buff, sizeof(buff)-1);
			
			//character conversions


			printf("RX: %s", buff);
			//printf("len: %d", strlen(buff));
			if (buff[0]=='m' && (strlen(buff)==3 || strlen(buff)==2 || strlen(buff)==1)) {// remote wants metal detector status

				count=GetPeriod(100);
				T= 1.0*count/(F_CPU*100.0); // Since we have the time of 200 periods, we need to divide by 200
				f=1.0/T;
				
				waitms(1);
				
				//printf("count = %d, f=%.2f\r\n",count, f);
				
				if (count==0) {
					eputs2("z\r\n");
				}
				else {
					// do freq mapping here!
					if(sum_count < 10)
					{
						sum_freq += f;
						sum_count++;
						freqc = 'z';
						if (sum_count > 10-AVG_NUM) {
							avg_sum += f;
							pastfreqs[i] = f;
							i++;
						}
					}
					if(sum_count == 10)
					{
						baseline_freq = sum_freq/10;
						sum_count++;
						freqc = 'z';						
					}
					if(sum_count > 10)
					{
						//this is where we start sending our baseline
						avg_sum-=pastfreqs[i%AVG_NUM];
						pastfreqs[i%AVG_NUM]=f;
						avg_sum+=f;
						avg_freq=avg_sum/AVG_NUM;
						i++;
						//printf("sum = %d, avg = %d\r\n", avg_sum, avg_freq);
						freqc = MapFrequency((int) (avg_freq-baseline_freq));

					}
					sprintf(buff,"%c\r\n",freqc);
					printf(buff);
					eputs2(buff);
				}
			}
			//changing if statement to receive position instead of M being sent
			else if (strlen(buff)==2) // remote sent pos data
			{
				dirc = buff[0];
				setPWM(dirc); 		// use character `dirc` to directly set PWM values
				//ButtonCommand(dirc);
				pathFindingDecoder(dirc);
				//printf("made it!\r\n");
			}
			
		}
	}

}