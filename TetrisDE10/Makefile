###############################################################################
# Makefile for Nios II Tetris Game
###############################################################################

# ---- Project Settings ----
APP_NAME    := tetris
BSP_DIR     := bsp
SRC_DIR     := src
BUILD_DIR   := build

# ---- Toolchain ----
CC          := nios2-elf-gcc
OBJCOPY     := nios2-elf-objcopy
OBJDUMP     := nios2-elf-objdump
CFLAGS      := -Wall -O2 -std=c99 -g
LDFLAGS     := -msys-crt0=$(BSP_DIR)/obj/HAL/src/crt0.o \
               -L$(BSP_DIR)/obj/HAL/lib \
               -T$(BSP_DIR)/linker.x \
               -Wl,-Map=$(BUILD_DIR)/$(APP_NAME).map

INCLUDES    := -I$(BSP_DIR)/public/include -I$(SRC_DIR)

# ---- Source Files ----
SRCS        := $(wildcard $(SRC_DIR)/*.c)
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# ---- Targets ----
ELF_FILE    := $(BUILD_DIR)/$(APP_NAME).elf
BIN_FILE    := $(BUILD_DIR)/$(APP_NAME).bin

###############################################################################
# Rules
###############################################################################

all: $(ELF_FILE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $< ..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(ELF_FILE): $(OBJS)
	@echo "Linking $(APP_NAME).elf ..."
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BIN_FILE)
	@echo "Build complete: $(ELF_FILE)"

clean:
	@echo "Cleaning project..."
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.map

download: $(ELF_FILE)
	@echo "Downloading to board..."
	nios2-download -r -g $(ELF_FILE)

terminal:
	nios2-terminal

help:
	@echo "Available targets:"
	@echo "  make            - Build ELF and BIN"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make download   - Download ELF to DE10-Lite"
	@echo "  make terminal   - Open Nios II terminal for output"

###############################################################################
