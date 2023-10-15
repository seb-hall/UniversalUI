use windows::core::*;
use windows::Win32::Foundation::*;
use windows::Win32::System::LibraryLoader::GetModuleHandleW;
use windows::Win32::UI::WindowsAndMessaging::*;


use super::*;

//  create window and update window handle
pub fn create_window() -> WindowHandle {
    
    fn get_instance() -> Result<HMODULE> {
        unsafe {
            let instance = GetModuleHandleW(None)?;
            return Ok(instance);
        } 
    }

    let instance: Option<HMODULE> = match get_instance() {
        Ok(inst) => Some(inst),
        Err(_) => {
            println!("couldn't get instance handle!"); 
            None
        }
        
    };


    unsafe {
        let win32_window: HWND = CreateWindowExW(
            WINDOW_EX_STYLE::default(),
            w!("window"),
            w!("stuff here"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            600,
            None,
            None,
            instance.unwrap(),
            None,
        );
 
        
        ShowWindow(win32_window, SW_SHOW);

        let window = WindowHandle {
            instance: instance.unwrap(),
            handle: win32_window,
        };

        return window;

    }
}