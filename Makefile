CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g3 -O0
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=makefile_project.map

final : makefile_project.elf

makefile_project.elf : main.o stm32f411_startup.o
	$(CC) $(LDFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -o $@

stm32f411_startup.o : stm32f411_startup.c 
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: flash
flash : 
	openocd -f board/st_nucleo_f4.cfg 

.PHONY: debug 
debug:
	arm-none-eabi-gdb makefile_project.elf \
	-ex "target remote localhost:3333" \
	-ex "monitor reset halt" \
	-ex "load" \
	-ex "monitor reset halt" \
	-ex "break main" 

.PHONY: run
run:
	arm-none-eabi-gdb makefile_project.elf \
	-ex "target remote localhost:3333" \
	-ex "monitor reset halt" \
	-ex "load" \
	-ex "monitor reset halt" \
	-ex "continue" 

#for linux 
clean :
	rm -f *.o *.elf *.map

#for windows
cleanWin :
	del -f *.o *.elf *.map