@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\rohan\Documents\Elec 291\Project 2\Project2ELEC291\integrated_files\"
"C:\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\rohan\Documents\Elec 291\Project 2\Project2ELEC291\integrated_files\EFM8integration.c"
if not exist hex2mif.exe goto done
if exist EFM8integration.ihx hex2mif EFM8integration.ihx
if exist EFM8integration.hex hex2mif EFM8integration.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\rohan\Documents\Elec 291\Project 2\Project2ELEC291\integrated_files\EFM8integration.hex
