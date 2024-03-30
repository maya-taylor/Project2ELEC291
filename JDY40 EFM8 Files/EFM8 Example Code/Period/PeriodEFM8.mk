SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=PeriodEFM8.obj

PeriodEFM8.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
PeriodEFM8.obj: PeriodEFM8.c
	$(CC) -c PeriodEFM8.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r PeriodEFM8.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: PeriodEFM8.hex PeriodEFM8.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		