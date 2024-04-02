SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=dump.obj

dump.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
dump.obj: dump.c
	$(CC) -c dump.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r dump.hex
	cmd /c start c:\PUTTY\putty -serial $(COMPORT) -sercfg 115200,8,n,1,N -v

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start c:\PUTTY\putty -serial $(COMPORT) -sercfg 115200,8,n,1,N -v

Dummy: dump.hex dump.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		