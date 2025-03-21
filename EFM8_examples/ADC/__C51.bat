@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\leigh\Elec291Project2\EFM8_examples\ADC\"
"C:\Users\leigh\Downloads\CrossIDE\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\leigh\Elec291Project2\EFM8_examples\ADC\ADC.c"
if not exist hex2mif.exe goto done
if exist ADC.ihx hex2mif ADC.ihx
if exist ADC.hex hex2mif ADC.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\leigh\Elec291Project2\EFM8_examples\ADC\ADC.hex
