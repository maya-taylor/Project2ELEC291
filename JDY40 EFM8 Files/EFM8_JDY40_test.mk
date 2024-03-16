SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=EFM8_JDY40_test.obj

EFM8_JDY40_test.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
EFM8_JDY40_test.obj: EFM8_JDY40_test.c
	$(CC) -c EFM8_JDY40_test.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r EFM8_JDY40_test.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: EFM8_JDY40_test.hex EFM8_JDY40_test.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		