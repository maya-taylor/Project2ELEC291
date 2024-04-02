SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=EFM8_autotest.obj

EFM8_autotest.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
EFM8_autotest.obj: EFM8_autotest.c
	$(CC) -c EFM8_autotest.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog -ft230 -r EFM8_autotest.hex

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: EFM8_autotest.hex EFM8_autotest.Map
	@echo Please don't click me!
	
explorer:
	cmd /c start explorer .

web_page:
	cmd /c start http://www.silabs.com/documents/public/data-sheets/efm8lb1-datasheet.pdf
