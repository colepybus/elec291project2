@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\leigh\Elec291Project2\"
"C:\Users\leigh\Downloads\CrossIDE\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\leigh\Elec291Project2\joystick_displaying.c"
if not exist hex2mif.exe goto done
if exist joystick_displaying.ihx hex2mif joystick_displaying.ihx
if exist joystick_displaying.hex hex2mif joystick_displaying.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\leigh\Elec291Project2\joystick_displaying.hex
