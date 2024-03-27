SHELL=cmd
CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
CCFLAGS=-mcpu=cortex-m0 -mthumb -g 

# Search for the path of the right libraries.  Works only on Windows.
GCCPATH=$(subst \bin\arm-none-eabi-gcc.exe,\,$(shell where $(CC)))
LIBPATH1=$(subst \libgcc.a,,$(shell dir /s /b "$(GCCPATH)*libgcc.a" | find "v6-m"))
LIBPATH2=$(subst \libc_nano.a,,$(shell dir /s /b "$(GCCPATH)*libc_nano.a" | find "v6-m"))
LIBSPEC=-L"$(LIBPATH1)" -L"$(LIBPATH2)"

OBJS=tb_motor.o startup.o serial.o newlib_stubs.o


# Notice that floating point is enabled with printf (-u _printf_float)
tb_motor.hex: $(OBJS)
	$(LD) $(OBJS) $(LIBSPEC) -Os -u _printf_float -nostdlib -lnosys -lgcc -T ..\STM32L051-Sample-Codes\Common\LDscripts\stm32l051xx.ld --cref -Map tb_motor.map -o tb_motor.elf
	arm-none-eabi-objcopy -O ihex tb_motor.elf tb_motor.hex
	@echo Success!

tb_motor.o: tb_motor.c
	$(CC) -c $(CCFLAGS) tb_motor.c -o tb_motor.o

startup.o: ..\STM32L051-Sample-Codes\Common\Source\startup.c
	$(CC) -c $(CCFLAGS) -DUSE_USART1 ..\STM32L051-Sample-Codes\Common\Source\startup.c -o startup.o

serial.o: ..\STM32L051-Sample-Codes\Common\Source\serial.c
	$(CC) -c $(CCFLAGS) ..\STM32L051-Sample-Codes\Common\Source\serial.c -o serial.o
	
newlib_stubs.o: ..\STM32L051-Sample-Codes\Common\Source\newlib_stubs.c
	$(CC) -c $(CCFLAGS) ..\STM32L051-Sample-Codes\Common\Source\newlib_stubs.c -o newlib_stubs.o

clean: 
	@del $(OBJS) 2>NUL
	@del tb_motor.elf tb_motor.hex tb_motor.map 2>NUL

Load_Flash: tb_motor.hex
	@taskkill /f /im putty.exe /t /fi "status eq running" > NUL
	@echo ..\STM32L051-Sample-Codes\stm32flash\stm32flash -w tb_motor.hex -v -g 0x0 ^^>loadf.bat
	@..\STM32L051-Sample-Codes\stm32flash\BO230\BO230 -b >>loadf.bat
	@loadf
	@echo cmd /c start putty.exe -sercfg 115200,8,n,1,N -serial ^^>sputty.bat
	@..\STM32L051-Sample-Codes\stm32flash\BO230\BO230 -r >>sputty.bat
	@sputty
	
putty:
	@taskkill /f /im putty.exe /t /fi "status eq running" > NUL
	@echo cmd /c start putty.exe -sercfg 115200,8,n,1,N -serial ^^>sputty.bat
	@..\STM32L051-Sample-Codes\stm32flash\BO230\BO230 -r >>sputty.bat
	@sputty
	
explorer:
	cmd /c start explorer .

dummy: tb_motor.map tb_motor.hex
	@echo Hello from 'dummy' target...