SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=rxtx.obj nrf24.obj radioPinFunctions.obj

tx.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
rxtx.obj: rxtx.c
	$(CC) -c rxtx.c

nrf24.obj: nrf24.c
	$(CC) -c nrf24.c

radioPinFunctions.obj: radioPinFunctions.c
	$(CC) -c radioPinFunctions.c

clean:
	@del *.asm *.lkr *.lst *.map *.hex *.obj 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r rxtx.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Circuit_Wiring_Picture:
	cmd /c start Circuit_Wiring.jpg

Dummy: rxtx.hex rxtx.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		