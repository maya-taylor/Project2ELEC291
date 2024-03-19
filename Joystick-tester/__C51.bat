@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\keybo\Documents\GitHub\Project2ELEC291\Joystick-tester\"
"C:\Users\keybo\Documents\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\keybo\Documents\GitHub\Project2ELEC291\Joystick-tester\working-joystick.c"
if not exist hex2mif.exe goto done
if exist working-joystick.ihx hex2mif working-joystick.ihx
if exist working-joystick.hex hex2mif working-joystick.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\keybo\Documents\GitHub\Project2ELEC291\Joystick-tester\working-joystick.hex
