SHELL=cmd
CC=c51
COMPORT = $(shell type COMPORT.inc)
OBJS=Robot_Base.obj

Robot_Base.hex: $(OBJS)
	$(CC) $(OBJS)
	@echo Done!
	
Robot_Base.obj: Robot_Base.c
	$(CC) -c Robot_Base.c

clean:
	@del $(OBJS) *.asm *.lkr *.lst *.map *.hex *.map 2> nul

LoadFlash:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	EFM8_prog.exe -ft230 -r Robot_Base.hex
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

putty:
	@Taskkill /IM putty.exe /F 2>NUL | wait 500
	cmd /c start putty -serial $(COMPORT) -sercfg 115200,8,n,1,N

Dummy: Robot_Base.hex Robot_Base.Map
	@echo Nothing to see here!
	
explorer:
	cmd /c start explorer .
		