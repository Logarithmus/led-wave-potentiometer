#![warn(clippy::all)]
#![feature(asm, alloc_error_handler, lang_items)]
#![no_std]
#![no_main]

mod adc;
mod display;
mod freertos_allocator;
mod leds;
mod usart;

extern crate alloc;
extern crate panic_abort;

use crate::freertos_allocator::FreeRtosAllocator;
use core::ptr::write_volatile;
use freertos_rs::{Task, TaskPriority};

#[global_allocator]
static GLOBAL: FreeRtosAllocator = FreeRtosAllocator;

extern "C" {
    pub fn freertos_rs_vTaskStartScheduler();
}

pub fn blink() {
    unsafe {
        write_volatile(avrd::atmega88pa::DDRD, 0xFF);
        loop {
            write_volatile(avrd::atmega88pa::PORTD, 0xFF);
        }
    }
}

#[no_mangle]
pub extern "C" fn main() {
    let _task = Task::new().name("test").stack_size(64).start(blink).unwrap();
    unsafe {
        freertos_rs_vTaskStartScheduler();
    }
    // let leds_task = Task::new().name("leds").stack_size(32).start(leds::leds_task);
    // let display_task = Task::new().name("display").stack_size(32).start(display::display_task);
}
