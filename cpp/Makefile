MCU := atmega88pa
AVRDUDE_MCU := m88p
BUILD_DIR := build/
TARGET := led_wave_potentiometer
ELF := ${BUILD_DIR}${TARGET}.elf
BIN := ${BUILD_DIR}${TARGET}.bin
EEP := ${BUILD_DIR}${TARGET}.eep
PROGRAMMER := usbasp-clone

SRC := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
CPPFLAGS := -DF_CPU=1000000L -flto -std=gnu++17 -Os -s -mmcu=${MCU} -I../freertos-10.3.1/include \
-I/usr/avr/include -L../freertos-10.3.1/build
WARNINGS=-Wall -Werror -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-align -Wsign-compare \
-Waggregate-return -Wno-int-to-pointer-cast -Wno-language-extension-token \
-Wno-unused-parameter -Wno-unused-function

all: ${BIN} ${EEP} size

${ELF}: ${SRC}
	mkdir -p ${BUILD_DIR}
	avr-g++ ${WARNINGS} ${CPPFLAGS} $^ -lfreertos -Wl,--gc-sections -o $@

strip: ${ELF}
	avr-strip --strip-all $<

${BIN}: ${ELF} strip
	avr-objcopy -R .eeprom -O binary $< $@

${EEP}: ${ELF} strip
	avr-objcopy -j .eeprom -O binary $< $@

size: ${BIN}
	echo "Binary size: `ls -l $< | cut -f5 -d " "` bytes"

program: ${BIN} ${EEP}
	avrdude -p ${AVRDUDE_MCU} -c ${PROGRAMMER} -U flash:w:$<
	avrdude -p ${AVRDUDE_MCU} -c ${PROGRAMMER} -U eeprom:w:$(word 2,$^):r

clean:
	rm ${ELF}
	rm ${BIN}

# Targets which run always, even if Make thinks that nothing changes
# Required because Make isn't so smart
.PHONY: clean