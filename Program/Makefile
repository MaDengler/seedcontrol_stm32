DEVICE = STM32F401xC
STARTUP = startup_stm32f401xc.s
SYSTEM = system_stm32f4xx.c

INC_CMSIS = device/CMSIS_5/CMSIS/Core/Include
INC_CMSIS_DEVICE_F4 = device/cmsis-device-f4/Include
INC = inc

SRC_DIR = src
BUILD_DIR = build

STARTUP_SRC = device/cmsis-device-f4/Source/Templates/gcc
SYSTEM_SRC = device/cmsis-device-f4/Source/Templates/
SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(BUILD_DIR)/$(STARTUP:.s=.o)
OBJS += $(BUILD_DIR)/$(SYSTEM:.c=.o)
OBJS += $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))
TARGET = $(BUILD_DIR)/seedcontrol_stm32.elf

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as

ASFLAGS = -mcpu=cortex-m4 -mthumb
CFLAGS = -mcpu=cortex-m4 -mthumb -D$(DEVICE) -Wall -Wextra -Werror -ffreestanding -lc -lm -I$(INC_CMSIS) -I$(INC_CMSIS_DEVICE_F4) -I$(INC) #-g -Og
LDFLAGS = -mcpu=cortex-m4 -mthumb -Tdevice/linker/STM32F401CC_FLASH.ld -specs=nosys.specs #-g -Og

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SYSTEM_SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(STARTUP_SRC)/%.s
	$(AS) $(ASFLAGS) $< -o $@

debug: 
	echo $(OBJS)
flash:
	arm-none-eabi-objcopy -O binary $(TARGET) $(TARGET:.elf=.bin)
	st-flash write $(TARGET:.elf=.bin) 0x08000000

clean:
	rm -f $(OBJS) $(TARGET)

