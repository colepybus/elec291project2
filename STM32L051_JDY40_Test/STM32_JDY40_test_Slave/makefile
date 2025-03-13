#these paths are edited specifically for coles computer

SHELL=/bin/bash
CC= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-gcc
AS= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-as
LD= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-ld
CCFLAGS=-mcpu=cortex-m0 -mthumb -g 

# Search for the path of the right libraries.  Works only on Windows.
GCCPATH=$(shell dirname $(CC))
LIBPATH1= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/lib/gcc/arm-none-eabi/14.2.1/thumb/v6-m/nofp
LIBPATH2= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/arm-none-eabi/lib/thumb/v6-m/nofp
LIBSPEC=-L"$(LIBPATH1)" -L"$(LIBPATH2)"

OBJS=main.o serial.o UART2.o startup.o newlib_stubs.o

PORTN=$(shell cat COMPORT.inc)

# For smaller hex file remove '-u _printf_float' below
main.elf : $(OBJS)
#	$(LD) $(OBJS) $(LIBSPEC) -Os -u _printf_float -nostdlib -lnosys -lgcc -T ../Common/LDscripts/stm32l051xx.ld --cref -Map main.map -o main.elf
	$(LD) $(OBJS) $(LIBSPEC) -Os -nostdlib -lnosys -lgcc -T ../Common/LDscripts/stm32l051xx.ld --cref -Map main.map -o main.elf
	arm-none-eabi-objcopy -O ihex main.elf main.hex
	@echo Success!

main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o

startup.o: ../Common/Source/startup.c
	$(CC) -c $(CCFLAGS) -DUSE_USART1 ../Common/Source/startup.c -o startup.o

serial.o: ../Common/Source/serial.c
	$(CC) -c $(CCFLAGS) ../Common/Source/serial.c -o serial.o

UART2.o: UART2.c
	$(CC) -c $(CCFLAGS) UART2.c -o UART2.o

newlib_stubs.o: ../Common/Source/newlib_stubs.c
	$(CC) -c $(CCFLAGS) ../Common/Source/newlib_stubs.c -o newlib_stubs.o

clean: 
	@del $(OBJS) 2>NUL
	@del main.elf main.hex main.map 2>NUL
	@del *.lst 2>NUL
	
#Flash_Load:
#	@pkill CoolTerm || true
#	@echo ..\stm32flash\stm32flash -w main.hex -v -g 0x0 ^^>sflash.bat
#	@..\stm32flash\BO230\BO230 -b >>sflash.bat
#	@sflash
#	@echo cmd /c start putty.exe -sercfg 115200,8,n,1,N -serial ^^>sputty.bat
#	@..\stm32flash\BO230\BO230 -r >>sputty.bat
#	@sputty

Flash_Load:
	@pkill CoolTerm || true
	@echo "../stm32flash/stm32flash -w main.hex -v -g 0x0 /dev/cu.usbserial-D30HJN8C" > sflash.sh
	@echo "../stm32flash/BO230/BO230 -b" >> sflash.sh
	@chmod +x sflash.sh
	@bash sflash.sh
	@osascript -e 'tell application "CoolTerm" to activate'
	@echo "Flash loading complete! CoolTerm is now open."


putty:
	@echo cmd /c start putty.exe -sercfg 115200,8,n,1,N -serial ^^>sputty.bat
	@..\stm32flash\BO230\BO230 -r >>sputty.bat
	@sputty
	
explorer:
	@explorer .

