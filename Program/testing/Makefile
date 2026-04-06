DEVICE = STM32F401xC
STARTUP = startup_stm32f401xc.s
SYSTEM = system_stm32f4xx.c

INC_CMSIS = ../device/CMSIS_5/CMSIS/Core/Include
INC_CMSIS_DEVICE_F4 = ../device/cmsis-device-f4/Include

BUILD_DIR = build
TARGET = $(BUILD_DIR)/tester.elf

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as

STARTUP_SRC = ../device/cmsis-device-f4/Source/Templates/gcc
SYSTEM_SRC = ../device/cmsis-device-f4/Source/Templates

ASFLAGS = -mcpu=cortex-m4 -mthumb
CFLAGS = -mcpu=cortex-m4 -mthumb -D$(DEVICE) -Wall -Wextra -Werror -ffreestanding -I$(INC_CMSIS) -I$(INC_CMSIS_DEVICE_F4)
LDFLAGS = -mcpu=cortex-m4 -mthumb -T../device/linker/STM32F401CC_FLASH.ld -specs=nosys.specs

OBJS = $(BUILD_DIR)/$(STARTUP:.s=.o) $(BUILD_DIR)/$(SYSTEM:.c=.o) $(BUILD_DIR)/main.o

all: $(TARGET)
	arm-none-eabi-objcopy -O binary $(TARGET) $(TARGET:.elf=.bin)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(SYSTEM_SRC)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: $(STARTUP_SRC)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
