HY


file to track pins, timers, etc being used (debugging)

PA2, PA3, PA4, PA5 -- outputs for motors


PA2 -- left motor
PA3 -- right motor

PA13 -- some output
PA8 -- input
PA6 -- some input 

PA6 is used for systick reading
TIMER2: 
SysTick: used in GetPeriod, Delay_us, 
    Delay_us: used in waitms
    waitms: used in SendATCommand, main, (while loop in main)
    SendATCommand: main 

