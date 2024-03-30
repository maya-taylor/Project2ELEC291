SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=SPI_MCP3008.obj

SPI_MCP3008.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
SPI_MCP3008.obj: SPI_MCP3008.c
	$(CC) -c SPI_MCP3008.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r SPI_MCP3008.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: SPI_MCP3008.hex SPI_MCP3008.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		