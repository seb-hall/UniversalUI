use windows::core::*;
use windows::Win32::Foundation::*;
use windows::Win32::System::LibraryLoader::GetModuleHandleW;
use windows::Win32::UI::WindowsAndMessaging::*;

use vulkano::library::*;
use vulkano::instance::InstanceExtensions;

mod events;
mod window;

pub use events::*;


pub use window::*;

pub fn required_vk_extensions(library: &VulkanLibrary) -> InstanceExtensions {
    let ideal = InstanceExtensions {
        khr_surface: true,
        khr_win32_surface: true,
        khr_get_physical_device_properties2: true,
        khr_get_surface_capabilities2: true,
        ..InstanceExtensions::empty()
    };

    library.supported_extensions().intersection(&ideal)
}

pub struct WindowHandle {
    pub instance: HMODULE,
    pub handle: HWND,
}



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

    return true;
}





unsafe extern "system" fn wndproc(window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT { 

    //println!("an event occured! {}", message);
    
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
