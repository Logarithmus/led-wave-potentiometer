use crate::usart;
use avrd::atmega88pa as mcu;
use core::ptr::{read_volatile, write_volatile};

pub(crate) fn init() {
    // Now configure UART

    // Compute UBRR0 value (represents baud rate)
    // See page 155 of the datasheet for the formula
    const CLOCK_SPEED: u32 = 1_000_000;
    const BAUD_RATE: u32 = 4800;
    const DIVISOR: u32 = 16;
    const UBRR0_VALUE: u16 = (CLOCK_SPEED / (BAUD_RATE * DIVISOR) - 1) as u16;

    // Enable transmitter
    const UCSR0B_VALUE: u8 = 0b0100_1000;

    // Set frame format: 8-N-1
    const UCSR0C_VALUE: u8 = 0b0000_0110;

    unsafe {
        // Write config to registers
        write_volatile(mcu::UBRR0, UBRR0_VALUE);
        write_volatile(mcu::UCSR0B, UCSR0B_VALUE);
        write_volatile(mcu::UCSR0C, UCSR0C_VALUE);
    }
}

#[inline(always)]
pub(crate) fn send_byte(byte: u8) {
    unsafe {
        while read_volatile(mcu::UCSR0A) & 0b0010_0000 == 0 {}
        write_volatile(mcu::UDR0, byte);
    }
}

#[inline(always)]
pub(crate) fn send_bytes(bytes: &[u8]) {
    bytes.iter().cloned().for_each(send_byte);
}

pub(crate) fn test() {
    usart::init();
    loop {
        usart::send_bytes(b"hello!\r\n");
    }
}
