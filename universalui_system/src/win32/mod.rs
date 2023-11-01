use windows::core::*;
use windows::Win32::Foundation::*;
use windows::Win32::System::LibraryLoader::GetModuleHandleW;
use windows::Win32::UI::WindowsAndMessaging::*;


use ash::{vk, Entry};

mod events;
mod window;

pub use events::*;


pub use window::*;



pub fn init() -> bool {

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
            return false;
        }
        
    };

    fn create_class(instance: HMODULE) -> Result<WNDCLASSW> {
        unsafe {
            let window_class = w!("window");
            
            let wc = WNDCLASSW {
                hCursor: LoadCursorW(None, IDC_ARROW)?,
                hInstance: HINSTANCE::from(instance),
                lpszClassName: window_class,
                style: CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
                lpfnWndProc: Some(wndproc),
                ..Default::default()
            };

            return Ok(wc);
        }
    }

    let class = match create_class(instance.unwrap()) {
        Ok(wc) => wc,
        Err(_) => {
            println!("couldn't create a window class!"); 
            return false; 
        }
    };

    
    fn register_class(wc: WNDCLASSW) -> Result<bool> {
        unsafe {
            let atom = RegisterClassW(&wc);
            if atom == 0 {
                return Ok(false);
            }

            return Ok(true);
        }
    }



    let _ = match register_class(class) {
        Ok(result) => if !result { return false; }
        Err(_) => {
            println!("couldn't register class!"); 
            return false; 
        }
    };

    let entry = Entry::linked();
    let app_info = vk::ApplicationInfo {
        api_version: vk::make_api_version(0, 1, 0, 0),
        e
        ..Default::default()
    };
    let create_info = vk::InstanceCreateInfo {
        p_application_info: &app_info,
        ..Default::default()
    };

    let instance = unsafe { entry.create_instance(&create_info, None) };



    return true;
}



unsafe extern "system" fn wndproc(window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT { 

    //crate::windowManager::awindowdidsomething(self);
    unsafe {
        
        unsafe fn loword(x: u32) -> u16 {
            (x & 0xFFFF) as u16
        }

        unsafe fn hiword(x: u32) -> u16 {
            ((x >> 16) & 0xFFFF) as u16
        }

  
        return DefWindowProcW(window, message, wparam, lparam);
    }
}
