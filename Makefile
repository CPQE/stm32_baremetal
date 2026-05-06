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
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g3 -O0  -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(C_INCLUDES) $(C_DEFS)
#-nostdlib
LDFLAGS = -T stm32_ls.ld -Wl,-Map=makefile_project.map -lgcc -lm -nostartfiles  
SRC = main.c stm32f411_startup.c gpio.c systick.c tim.c uart.c adxl345.c spi.c
OBJ = $(SRC:.c=.o)
	
final : makefile_project.elf

makefile_project.elf : $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

#after running 'make', in the first terminal run 'make flash' or 'make run'

.PHONY: flash flashTest
flash :
	openocd -f board/st_nucleo_f4.cfg 
flashTest : 
	openocd -f board/st_nucleo_f4.cfg -c "init" -c "program makefile_project.elf verify reset exit"

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

# Forces SWD to attach while the chip is held in reset, so even if 
# firmware disables SWD or crashes instantly,  debugger can start
# erase_chip wipes flash and restores the MCU to a clean state.
.PHONY: reset_brick erase_chip
reset_brick:
	openocd -f board/st_nucleo_f4.cfg \
        -c "reset_config srst_only srst_nogate connect_assert_srst" \
        -c "init" \
        -c "reset halt" \
        -c "exit"
erase_chip:
	openocd -f board/st_nucleo_f4.cfg \
		-c "init" \
		-c "reset halt" \
		-c "halt" \
		-c "stm32f4x mass_erase 0" \
		-c "exit"

#for linux 
clean :
	rm -f *.o *.elf *.map

#for windows
cleanWin :
	del -f *.o *.elf *.map

