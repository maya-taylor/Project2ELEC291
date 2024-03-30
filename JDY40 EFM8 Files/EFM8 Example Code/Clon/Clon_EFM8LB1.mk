SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=Clon_EFM8LB1.obj

Clon_EFM8LB1.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
Clon_EFM8LB1.obj: Clon_EFM8LB1.c
	$(CC) -c Clon_EFM8LB1.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r Clon_EFM8LB1.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: Clon_EFM8LB1.hex Clon_EFM8LB1.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		