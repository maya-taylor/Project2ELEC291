SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=EFM8_Servo.obj

EFM8_Servo.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
EFM8_Servo.obj: EFM8_Servo.c
	$(CC) -c EFM8_Servo.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r EFM8_Servo.hex
	cmd /c start c:\PUTTY\putty -serial $(COMPORT) -sercfg 115200,8,n,1,N -v

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start c:\PUTTY\putty -serial $(COMPORT) -sercfg 115200,8,n,1,N -v

Dummy: EFM8_Servo.hex EFM8_Servo.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		