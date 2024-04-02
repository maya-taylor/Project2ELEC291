SHELL=cmd

EFM8_prog.exe: EFM8_prog.c
	@docl EFM8_prog.c
	
clean:
	del EFM8_prog.obj EFM8_prog.exe

dummy: docl.bat COMPORT.inc
	@echo hello from dummy!

blinky: ..\Blinky\Blinky.hex
	EFM8_prog -r -ft230 ..\Blinky\Blinky.hex

HelloWorld: ..\HelloWorld\HelloWorld.hex
	EFM8_prog -ft230 ..\HelloWorld\HelloWorld.hex

Music: ..\Music\EFM8_Music.hex
	EFM8_prog -ft230 ..\Music\EFM8_Music.hex

ADC: ..\ADC\ADC.hex
	EFM8_prog -ft230 ..\ADC\ADC.hex

Restore_CBUS: ..\ADC\ADC.hex
	EFM8_prog -ft230 -cbus
