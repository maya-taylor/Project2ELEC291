SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=newton.obj startup.obj

newton.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
newton.obj: newton.c
	$(CC) -c newton.c

startup.obj: startup.c global.h
	$(CC) -c startup.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog -ft230 -r newton.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: newton.hex newton.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		