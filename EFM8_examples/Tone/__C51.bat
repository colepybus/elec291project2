@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\leigh\Elec291Project2\EFM8_examples\Tone\"
"C:\Users\leigh\Downloads\CrossIDE\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\leigh\Elec291Project2\EFM8_examples\Tone\Tone_gen.c"
if not exist hex2mif.exe goto done
if exist Tone_gen.ihx hex2mif Tone_gen.ihx
if exist Tone_gen.hex hex2mif Tone_gen.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\leigh\Elec291Project2\EFM8_examples\Tone\Tone_gen.hex
