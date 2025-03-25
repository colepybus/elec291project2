@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\leigh\Elec291Project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\"
"C:\Users\leigh\Downloads\CrossIDE\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\leigh\Elec291Project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c"
if not exist hex2mif.exe goto done
if exist EFM8_JDY40_test.ihx hex2mif EFM8_JDY40_test.ihx
if exist EFM8_JDY40_test.hex hex2mif EFM8_JDY40_test.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\leigh\Elec291Project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.hex
