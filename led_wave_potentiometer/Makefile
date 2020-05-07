MCU := atmega88pa
AVRDUDE_MCU := m88p
BUILD_DIR := target/${MCU}/release
ELF := ${BUILD_DIR}/led_wave_potentiometer.elf
BIN := ${BUILD_DIR}/led_wave_potentiometer.bin
XARGO := RUST_TARGET_PATH=`pwd` rustup run avr xargo
XARGO_FLAGS := --target ${MCU} --release
PROGRAMMER := usbasp-clone

# Targets which run always, even if Make thinks that nothing changes
# Required because Make isn't so smart
.PHONY: ${ELF} clean check clippy

all: ${BIN} size

check:
	${XARGO} check ${XARGO_FLAGS}

clippy:
	${XARGO} clippy ${XARGO_FLAGS}

${ELF}:
	${XARGO} build ${XARGO_FLAGS}

strip: ${ELF}
	avr-strip --strip-all $<

${BIN}: ${ELF} strip
	avr-objcopy -O binary $< $@

size: ${BIN}
	echo "Binary size: `ls -l $< | cut -f5 -d " "` bytes"

program: ${BIN}
	avrdude -p ${AVRDUDE_MCU} -c ${PROGRAMMER} -U flash:w:$<

clean:
	cargo clean
