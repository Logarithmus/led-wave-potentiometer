# Based on WinAVR Sample makefile written by Eric B. Weddington, Jörg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
# Additional material for this makefile was submitted by:
#  Tim Henigan
#  Peter Fleury
#  Reiner Patommel
#  Sander Pool
#  Frederik Rouleau
#  Markus Pfaff

# MCU name
MCU = atmega88pa
CPU_SPEED = 1000000L

# Optimization level, can be [0, 1, 2, 3, s]. 0 turns off optimization.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

SRC_DIR = src/
BUILD_DIR = build/
TARGET = $(BUILD_DIR)libfreertos.a

SRC_FILES = freertos_rs.c \
core/tasks.c \
core/queue.c \
core/list.c \
core/croutine.c \
core/portable/MemMang/heap_1.c \
$(MCU)/port.c \

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

DEBUG_LEVEL=
WARNINGS=-Wall -Werror -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
		-Waggregate-return -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-language-extension-token

CFLAGS = -g -Og -std=gnu99 -Iinclude \
$(DEBUG_LEVEL) -O$(OPT) \
-fsigned-char -fpack-struct -fshort-enums \
-ffunction-sections -fdata-sections \
-mmcu=$(MCU) -DF_CPU=$(CPU_SPEED)\
$(WARNINGS) \

CC = avr-gcc
AR = avr-ar
REMOVE = rm -f

# Define all object files.
OBJ = $(addprefix $(BUILD_DIR), $(SRC_FILES:.c=.o))

# Default target.
all: $(TARGET)

# Compile: create object files from C source files.
$(OBJ): ${BUILD_DIR}%.o : ${SRC_DIR}%.c
	mkdir -p $(dir $<)
	$(CC) -c $(CFLAGS) $< -o $@

# Build: create static lib from object files
$(TARGET): $(OBJ)
	${AR} -crs $@ $^

# Target: clean project.
clean: 
	$(REMOVE) $(OBJ)
	$(REMOVE) $(TARGET)

# Listing of phony targets.
.PHONY: all clean
