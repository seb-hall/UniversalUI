use std::mem;
use std::ptr;
use windows::Win32::Foundation::*;
use windows::Win32::UI::WindowsAndMessaging::*;


pub fn poll_events() {
    let mut msg: MSG = unsafe { mem::zeroed() };

    while unsafe { GetMessageW(&mut msg, None, 0, 0) }.as_bool() {
        unsafe {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}