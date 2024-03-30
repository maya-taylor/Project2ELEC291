SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=main.obj startup.obj lcd.obj

main.hex: $(OBJS)
	$(CC) $(OBJS)
	@del *.asm *.lst *.lkr 2> nul
	@echo Done!
	
main.obj: main.c lcd.h
	$(CC) -c main.c

startup.obj: startup.c global.h
	$(CC) -c startup.c

lcd.obj: lcd.c lcd.h global.h
	$(CC) -c lcd.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog -ft230 -r main.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: main.hex main.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		