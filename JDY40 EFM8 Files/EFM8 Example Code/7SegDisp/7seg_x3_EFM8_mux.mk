SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=7seg_x3_EFM8_mux.obj

7seg_x3_EFM8_mux.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
7seg_x3_EFM8_mux.obj: 7seg_x3_EFM8_mux.c
	$(CC) -c 7seg_x3_EFM8_mux.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r 7seg_x3_EFM8_mux.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: 7seg_x3_EFM8_mux.hex 7seg_x3_EFM8_mux.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		