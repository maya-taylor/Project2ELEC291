#include <EFM8LB1.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define SYSCLK 72000000
#define BAUDRATE 115200L
#define SARCLK 18000000L
#define XPOS_PIN QFP32_MUX_P1_4
#define YPOS_PIN QFP32_MUX_P1_5


// For normalizing joystick position 
#define external_voltage_reference 1.7854  // in volts

#define VDD 3.3035 // The measured value of VDD in volts
void InitPinADC (unsigned char portno, unsigned char pinno)
{
	unsigned char mask;
	
	mask=1<<pinno;

	SFRPAGE = 0x20;
	switch (portno)
	{
		case 0:
			P0MDIN &= (~mask); // Set pin as analog input
			P0SKIP |= mask; // Skip Crossbar decoding for this pin
		break;
		case 1:
			P1MDIN &= (~mask); // Set pin as analog input
			P1SKIP |= mask; // Skip Crossbar decoding for this pin
		break;
		case 2:
			P2MDIN &= (~mask); // Set pin as analog input
			P2SKIP |= mask; // Skip Crossbar decoding for this pin
		break;
		default:
		break;
	}
	SFRPAGE = 0x00;
}
void Timer3us(unsigned char us)
{
	unsigned char i;               // usec counter
	
	// The input for Timer 3 is selected as SYSCLK by setting T3ML (bit 6) of CKCON0:
	CKCON0|=0b_0100_0000;
	
	TMR3RL = (-(SYSCLK)/1000000L); // Set Timer3 to overflow in 1us.
	TMR3 = TMR3RL;                 // Initialize Timer3 for first overflow
	
	TMR3CN0 = 0x04;                 // Sart Timer3 and clear overflow flag
	for (i = 0; i < us; i++)       // Count <us> overflows
	{
		while (!(TMR3CN0 & 0x80));  // Wait for overflow
		TMR3CN0 &= ~(0x80);         // Clear overflow indicator
	}
	TMR3CN0 = 0 ;                   // Stop Timer3 and clear overflow flag
}
unsigned int ADC_at_Pin(unsigned char pin)
{
	ADC0MX = pin;   // Select input from pin
	ADINT = 0;
	ADBUSY = 1;     // Convert voltage at the pin
	while (!ADINT); // Wait for conversion to complete
	return (ADC0);
}

unsigned int Get_ADC (void)
{
	ADINT = 0;
	ADBUSY = 1;
	while (!ADINT); // Wait for conversion to complete
	return (ADC0);
}

void waitms (unsigned int ms)
{
	unsigned int j;
	unsigned char k;
	for(j=0; j<ms; j++)
		for (k=0; k<4; k++) Timer3us(250);
}

float Volts_at_Pin(unsigned char pin)
{
	 return ((ADC_at_Pin(pin)*VDD)/0b_0011_1111_1111_1111);
}

float map2(float x, float in_min, float in_max, float out_min, float out_max)
{	
	float value =  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return value;
}



void LEDgetposition (float volts[2]) {

    /* pseudocode - hannah
	get the voltage difference between the led volts and the read volts
	^do this for both x and y pins 
	return these by passing by value (utilize array)
	add an additional parameter in getpos2 that receives this array
	set mid_x to arr[0]  and mid_y to arr[1]
	but in this case run this function at the beginning to calibrate on each flash
	if we were to recalibrate throughout were gonna need to modify and utilize pb or smt
	*/

	volts[0] = volts[0] - external_voltage_reference; //convert to midpoint
	volts[1] = volts[1] - external_voltage_reference;

	return;

}

void GetPosition2 (float volts[2], float pos[2], float mid[2]) {
	//float zero_x, zero_y, max_x, max_y;

	// approx values - NEEDS TO BE CHANGED AND NORMALIZED
	float mid_x = mid[0];
	float mid_y = mid[1];


	float vx = volts[0];
	float vy = volts[1];

	float max = 3.3 - external_voltage_reference;
	float min = - external_voltage_reference;
	if (pos[0] < mid_x) {
		pos[0] = map2(vx, min, mid_x, -50.0, 0);
		//printf("neg x\r\n");
	}
	else {
		pos[0] = map2(vx, mid_x, max, 0.0, 50.0);
		//printf("pos x\r\n");
	}

	if (pos[1] < mid_y) {
		pos[1] = map2(vy, min, mid_y, -50.0, 0.0);
		//printf("neg y\r\n");
	}
	else {
		pos[1] = map2(vy, mid_y, max, 0.0, 50.0);
		//printf("pos y\r\n");
	}
}

void main(void)
{
    // Might be good to do some averaging
	float mapped_range = 5.0; //1 to mapped_range+1
	float extract_range = 800.0; //max range of beeping frequency
	int timeout_cnt = 0;
	int sum_count = 0; //count to keep track of first 10 vals
	float sum_freq = 0.0; //where I am adding the frequencies into
	float baseline_freq = 0.0;
	
	float v[2];
    float xy_pos[2]; //positioning array, xy_pos[0] corresponds to the x-coord, y-coord is the latter (HY)
	xdata char buff_x[17];
    xdata char buff_y[17];
	xdata char temp_buff[80];
	float mid[2];

	//float adc[2]
	// TIMER0_Init(); commented out since I'll be using TIMER2 ISR - GL
	LCD_4BIT();
	
	waitms(500);
	printf("\x1b[2J"); // Clear screen using ANSI escape sequence.
	
	printf("ADC test program\n"
	        "File: %s\n"
	        "Compiled: %s, %s\n\n",
	        __FILE__, __DATE__, __TIME__);

	InitPinADC(1, 4); // Configure P1.4 as analog input
	InitPinADC(1, 5); // Configure P1.5 as analog input
    InitADC();
	
	//printf("\r\nPress and hold the BOOT button to transmit.\r\n");
	mid[0] = Volts_at_Pin(XPOS_PIN);
	mid[1] = Volts_at_Pin(YPOS_PIN);

	LEDgetposition(mid);
    printf("LED voltage ref: 1.7854, midXvolts: %f, midYvolts: %f \r\n", mid[0], mid[1]);

	//cnt=0;
	while(1)
	{
		waitms(50);
            
        v[0] = Volts_at_Pin(XPOS_PIN) ;
	    v[1] = Volts_at_Pin(YPOS_PIN) ;
		// ADC 
		// printf("xadc: %7.5f yadc: %7.5f\r\n", ADC_at_Pin(XPOS_PIN), ADC_at_Pin(YPOS_PIN))
        
        GetPosition2(v, xy_pos, mid);

    	printf("normalized values: xvolts: %7.5f, yvolts: %7.5f\r\n", ADC_at_Pin(XPOS_PIN), ADC_at_Pin(YPOS_PIN));

		// sprintf(buff_x, "x=%.4f", xy_pos[0]);
    	// LCDprint(buff_x, 1, 1);
	    // sprintf(buff_y, "y=%.4f", xy_pos[1]);
    	// LCDprint(buff_y, 2, 1);
       
        // Can be commented out for testing
		if (abs(xy_pos[0]) < 5) xy_pos[0]=0;
		if (abs(xy_pos[1]) < 5) xy_pos[1]=0;
		if (xy_pos[0] > 50) xy_pos[0]=50;
		if (xy_pos[1] > 50) xy_pos[1]=50;
		if (xy_pos[0] < -50) xy_pos[0]=-50;
		if (xy_pos[1] < -50) xy_pos[1]=-50;

		sprintf(temp_buff, "%3d,%3d\r\n", (int)xy_pos[0], (int)xy_pos[1]);
	}
}