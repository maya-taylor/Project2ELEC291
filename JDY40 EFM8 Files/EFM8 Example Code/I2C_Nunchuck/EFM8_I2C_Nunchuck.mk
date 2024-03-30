SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=EFM8_I2C_Nunchuck.obj

EFM8_I2C_Nunchuck.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
EFM8_I2C_Nunchuck.obj: EFM8_I2C_Nunchuck.c
	$(CC) -c EFM8_I2C_Nunchuck.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r EFM8_I2C_Nunchuck.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: EFM8_I2C_Nunchuck.hex EFM8_I2C_Nunchuck.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		