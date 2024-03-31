@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\"
"C:\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\efm8joystickjdyint.c"
if not exist hex2mif.exe goto done
if exist efm8joystickjdyint.ihx hex2mif efm8joystickjdyint.ihx
if exist efm8joystickjdyint.hex hex2mif efm8joystickjdyint.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\efm8joystickjdyint.hex
