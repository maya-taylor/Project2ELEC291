SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=HelloWorld.obj

HelloWorld.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
HelloWorld.obj: HelloWorld.c
	$(CC) -c HelloWorld.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog -ft230 -r HelloWorld.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: HelloWorld.hex HelloWorld.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		