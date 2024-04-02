SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=EFM_UUID.obj

EFM_UUID.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
EFM_UUID.obj: EFM_UUID.c
	$(CC) -c EFM_UUID.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog -ft230 -r EFM_UUID.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N -v

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N -v

Dummy: EFM_UUID.hex EFM_UUID.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		