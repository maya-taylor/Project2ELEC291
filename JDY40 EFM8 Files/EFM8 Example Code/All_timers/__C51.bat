@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\"
"C:\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.c"
if not exist hex2mif.exe goto done
if exist All_timers.ihx hex2mif All_timers.ihx
if exist All_timers.hex hex2mif All_timers.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\flipa\OneDrive - UBC\University\Course Materials\2nd year - Term 2\ELEC 291 - Electrical Engineering Design Studio\Project2ELEC291\JDY40 EFM8 Files\EFM8 Example Code\All_timers\All_timers.hex
