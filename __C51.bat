@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\keybo\Documents\GitHub\Project2ELEC291\"
"C:\Users\keybo\Documents\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\keybo\Documents\GitHub\Project2ELEC291\JoystickTestCode.c"
if not exist hex2mif.exe goto done
if exist JoystickTestCode.ihx hex2mif JoystickTestCode.ihx
if exist JoystickTestCode.hex hex2mif JoystickTestCode.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\keybo\Documents\GitHub\Project2ELEC291\JoystickTestCode.hex
