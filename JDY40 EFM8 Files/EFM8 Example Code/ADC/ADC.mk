SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=ADC.obj

ADC.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
ADC.obj: ADC.c
	$(CC) -c ADC.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r ADC.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: ADC.hex ADC.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		