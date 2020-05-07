use crate::{adc, usart};
use avrd::atmega88pa as mcu;
use core::ptr::{read_volatile, write_volatile};

fn init() {
    // REFS  - select AVcc (== Vcc, normally 3.3 - 5 V) as reference for ADC
    // ADLAR - left-adjustment of the result, 8-bits precision is enough
    // MUX   - select channel 0 (pin ADCas ADC input
    const ADMUX_VALUE: u8 = 0b0110_0000;

    // ADEN  - ADC enable
    // ADSC  - start first conversion
    // ADATE - auto trigger, enabled
    // ADIF  - conversion complete interrupt flag, unused because ADIE is disabled
    // ADIE  - conversion complete interrupt, disabled
    // ADPS  - ADC prescaler division factor, set to 8 (1 < 0b011 == 8)
    const ADCSRA_VALUE: u8 = 0b0010_0110;

    // ACME  - analog comparator, unused
    // ADTS  - trigger source, 0b000 for free running mode
    const ADCSRB_VALUE: u8 = 0b0000_0000;

    // ADC5..0 - disable register to save power (3..1 are disabled)
    // 5..4 are used for display, 0 is used for potentiometer
    const DIDR0_VALUE: u8 = 0b0000_1110;

    // ADEN  - ADC enable
    // ADSC  - start first conversion
    // see ADCSRA_VALUE
    const ADCSRA_START: u8 = ADCSRA_VALUE | 0b1100_0000;

    unsafe {
        // Write configuration to registers
        // Notice: ADEN and ADSC must be set AFTER configuration,
        // so we aren't setting them yet.
        write_volatile(mcu::ADMUX, ADMUX_VALUE);
        write_volatile(mcu::ADCSRA, ADCSRA_VALUE);
        write_volatile(mcu::ADCSRB, ADCSRB_VALUE);
        write_volatile(mcu::DIDR0, DIDR0_VALUE);
        //write_volatile(mcu::SREG, 1 << 7);

        // Now we can enable ADC and start first conversion
        write_volatile(mcu::ADCSRA, ADCSRA_START);
    }
}

#[inline(always)]
fn read() -> u8 {
    unsafe { read_volatile(mcu::ADCH) }
}

pub(crate) fn task() {
    adc::init();
    usart::init();
    loop {
        usart::send_bytes(b"\x1B[2J\x1B[H");
        usart::send_byte(adc::read());
    }
}
