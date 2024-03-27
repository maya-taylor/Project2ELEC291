#define PI 3.141592654
#define MAX_VELOCITY 50.00
#define MIN_PWM 65.00
#define RIGHT_MOTOR_ADJUST 1.0000
#define F_CPU 32000000L
#define DEF_F 100000L // 10us tick

//global constants
volatile int PWM_Counter = 0;
volatile unsigned char pwm1, pwm2;

void wait_1ms(void);
void delayms(int len);
void TIM2_Handler(void);
void Hardware_Init(void);
float LeftMotorAdjust_angle(float refAngle);
float RightMotorAdjust_angle(float refAngle);
float get_PWM (float velocity, float PWM_adjust);
void turnLeftMotor (float PWM, float PWM_adjust);
int turnRightMotor (float PWM, float PWM_adjust);
