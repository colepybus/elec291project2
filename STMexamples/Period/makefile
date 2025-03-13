CC= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-gcc
AS= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-as
LD= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/bin/arm-none-eabi-ld
CCFLAGS=-mcpu=cortex-m0 -mthumb -g 
PORTN=/dev/$(shell ls /dev | grep "cu.usbserial")

# Search for the path of libraries.
LIBPATH1= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/lib/gcc/arm-none-eabi/14.2.1/thumb/v6-m/nofp
LIBPATH2= /Applications/ArmGNUToolchain/14.2.rel1/arm-none-eabi/arm-none-eabi/lib/thumb/v6-m/nofp
LIBSPEC=-L"$(LIBPATH1)" -L"$(LIBPATH2)"

OBJS=main.o startup.o serial.o newlib_stubs.o

main.hex: $(OBJS)
	$(LD) $(OBJS) $(LIBSPEC) -Os -u _printf_float -nostdlib -lnosys -lgcc -T ../Common/LDscripts/stm32l051xx.ld --cref -Map main.map -o main.elf
	arm-none-eabi-objcopy -O ihex main.elf main.hex
	@echo Success!

main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o

startup.o: ../Common/Source/startup.c
	$(CC) -c $(CCFLAGS) -DUSE_USART1 ../Common/Source/startup.c -o startup.o

serial.o: ../Common/Source/serial.c
	$(CC) -c $(CCFLAGS) ../Common/Source/serial.c -o serial.o
	
newlib_stubs.o: ../Common/Source/newlib_stubs.c
	$(CC) -c $(CCFLAGS) ../Common/Source/newlib_stubs.c -o newlib_stubs.o

clean: 
	rm -f $(OBJS) main.elf main.hex main.map

Load_Flash: main.hex
	../stm32flash/stm32flash -w main.hex -v -g 0x0 $(PORTN)


