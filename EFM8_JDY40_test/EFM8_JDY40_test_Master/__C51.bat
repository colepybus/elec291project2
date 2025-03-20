@echo off
::This file was created automatically by CrossIDE to compile with C51.
D:
cd "\Zinnia\school\ubc_second_year\ELEC291\PROJECT02\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\"
"D:\Zinnia\school\ubc_second_year\ELEC291\LAB06\call51\call51\Bin\c51.exe" --use-stdout  "D:\Zinnia\school\ubc_second_year\ELEC291\PROJECT02\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.c"
if not exist hex2mif.exe goto done
if exist EFM8_JDY40_test.ihx hex2mif EFM8_JDY40_test.ihx
if exist EFM8_JDY40_test.hex hex2mif EFM8_JDY40_test.hex
:done
echo done
echo Crosside_Action Set_Hex_File D:\Zinnia\school\ubc_second_year\ELEC291\PROJECT02\elec291project2\EFM8_JDY40_test\EFM8_JDY40_test_Master\EFM8_JDY40_test.hex
