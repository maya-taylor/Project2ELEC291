SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=Servo_Tester.obj
export PATH := $(shell cd);$(PATH)

Servo_Tester.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
Servo_Tester.obj: Servo_Tester.c
	$(CC) -c Servo_Tester.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r Servo_Tester.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: Servo_Tester.hex Servo_Tester.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
	
test:
	@echo $(PATH)
		