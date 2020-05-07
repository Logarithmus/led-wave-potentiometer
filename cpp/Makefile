MCU := atmega88pa
AVRDUDE_MCU := m88p
BUILD_DIR := build/
TARGET := led_wave_potentiometer
ELF := ${BUILD_DIR}${TARGET}.elf
BIN := ${BUILD_DIR}${TARGET}.bin
PROGRAMMER := usbasp-clone

SRC := $(wildcard *.cpp) $(wildcard */*.cpp)
CPPFLAGS := -std=gnu++17 -Os -s -mmcu=${MCU} -I../freertos-10.3.1/include \
-I/usr/avr/include -L../freertos-10.3.1/build
WARNINGS=-Wall -Werror -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align -Wsign-compare \
-Waggregate-return -Wno-int-to-pointer-cast -Wno-language-extension-token \
-Wno-unused-parameter -Wno-unused-function

all: ${BIN} size

${ELF}: ${SRC}
	mkdir -p ${BUILD_DIR}
	avr-g++ ${WARNINGS} ${CPPFLAGS} $^ -lfreertos -Wl,--gc-sections -o $@

strip: ${ELF}
	avr-strip --strip-all $<

${BIN}: ${ELF} strip
	avr-objcopy -O binary $< $@

size: ${BIN}
	echo "Binary size: `ls -l $< | cut -f5 -d " "` bytes"

program: ${BIN}
	avrdude -p ${AVRDUDE_MCU} -c ${PROGRAMMER} -U flash:w:$<

clean:
	rm ${ELF}
	rm ${BIN}

# Targets which run always, even if Make thinks that nothing changes
# Required because Make isn't so smart
.PHONY: clean