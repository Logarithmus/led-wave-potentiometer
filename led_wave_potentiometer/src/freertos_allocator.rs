use core::alloc::{GlobalAlloc, Layout};
use core::ffi::c_void;

extern "C" {
    fn pvPortMalloc(size: u32) -> *mut c_void;
    fn vPortFree(p: *mut c_void);
}

#[alloc_error_handler]
fn foo(_: core::alloc::Layout) -> ! {
    panic!("OOM!");
}

pub struct FreeRtosAllocator;

unsafe impl GlobalAlloc for FreeRtosAllocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        pvPortMalloc(layout.size() as u32) as *mut u8
    }

    unsafe fn dealloc(&self, ptr: *mut u8, _: Layout) {
        vPortFree(ptr as *mut c_void)
    }
}
