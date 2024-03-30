SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=FreqEFM8.obj

FreqEFM8.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
FreqEFM8.obj: FreqEFM8.c
	$(CC) -c FreqEFM8.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r FreqEFM8.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: FreqEFM8.hex FreqEFM8.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		