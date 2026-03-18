CC = arm-none-eabi-gcc
C_INCLUDES = -Ichip_headers/CMSIS/Device/ST/STM32F4xx/Include \
			 -Ichip_headers/CMSIS/Include
C_DEFS = \
-DSTM32F411xE \
-DSTM32F411RETx \
-DSTM32F4 \
-DSTM32 \
-DNUCLEO_F411RE \
-DDEBUG
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g3 -O0 $(C_INCLUDES) $(C_DEFS)
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=makefile_project.map
SRC = main.c stm32f411_startup.c gpio.c
OBJ = $(SRC:.c=.o)
final : makefile_project.elf

makefile_project.elf : $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@
#after running 'make', in the first terminal run 'make flash' or 'make run'

.PHONY: flash
flash : 
	openocd -f board/st_nucleo_f4.cfg 

#then in 2nd terminal run 'make debug' or 'make run'

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