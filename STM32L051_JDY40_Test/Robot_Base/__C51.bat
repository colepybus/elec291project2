@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\leigh\Elec291Project2\elec291project2\STM32L051_JDY40_Test\Robot_Base\"
"C:\Users\leigh\Downloads\CrossIDE\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\leigh\Elec291Project2\elec291project2\STM32L051_JDY40_Test\Robot_Base\main.c"
if not exist hex2mif.exe goto done
if exist main.ihx hex2mif main.ihx
if exist main.hex hex2mif main.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\leigh\Elec291Project2\elec291project2\STM32L051_JDY40_Test\Robot_Base\main.hex
