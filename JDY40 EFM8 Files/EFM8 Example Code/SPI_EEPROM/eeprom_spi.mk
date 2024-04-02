# Since we are compiling in windows, select 'cmd' as the default shell.  This
# is important because make will search the path for a linux/unix like shell
# and if it finds it will use it instead.  This is the case when cygwin is
# installed.  That results in commands like 'del' and echo that don't work.
SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=eeprom_spi.obj FT93C66.obj

eeprom_spi.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
FT93C66.obj: FT93C66.c
	$(CC) -c FT93C66.c

eeprom_spi.obj: eeprom_spi.c
	$(CC) -c eeprom_spi.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r eeprom_spi.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: eeprom_spi.hex eeprom_spi.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start .

cmd:
	cmd /c start cmd

manual:
	cmd /c start Datasheet\93C66.pdf
	