use windows::{core::*};
use windows::Win32::Foundation::*;
use windows::Win32::System::LibraryLoader::GetModuleHandleA;
use windows::Win32::UI::WindowsAndMessaging::*;



//  create window and update window handle
pub fn create_window() {
    fn get_instance() -> Result<HMODULE> {
        unsafe {
            let instance = GetModuleHandleA(None)?;
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

    fn create_class(instance: HMODULE) -> Result<WNDCLASSA> {
        unsafe {
            let window_class = s!("window");
            
            let wc = WNDCLASSA {
                hCursor: LoadCursorW(None, IDC_ARROW)?,
                hInstance: instance,
                lpszClassName: window_class,
                style: CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
                lpfnWndProc: Some(wndproc),
                ..Default::default()
            };

            return Ok(wc);
        }
    }

    let class = match create_class(Some(instance)) {
        Ok(wc) => wc,
        Err(_) => {
            println!("couldn't create a window class!"); 
            return; 
        }
    };

    
    fn register_class(wc: WNDCLASSA) -> Result<bool> {
        unsafe {
            let atom = RegisterClassA(&wc);
            if atom == 0 {
                return Ok(false);
            }

            return Ok(true);
        }
    }



    let _ = match register_class(class) {
        Ok(result) => if !result { return; }
        Err(_) => {
            println!("couldn't register class!"); 
            return; 
        }
    };


    unsafe {
        let win32_window: HWND = CreateWindowExA(
            WINDOW_EX_STYLE::default(),
            s!("window"),
            s!("stuff here"),
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

        //  IMPORTANT NOTE: multiple events such as window creation are called at this stage, 
        //  before the delegate pointer is assigned. Calling a method on the delegate before
        //  it's pointer has been assigned will naturally lead to a crash.

        //SetWindowLongPtrW(win32_window, GWL_USERDATA, provider);

        //let mut window_handle = Win32WindowHandle::empty();
        //window_handle.hwnd = win32_window.0 as *mut c_void;
        //window_handle.hinstance = instance.unwrap().0 as *mut c_void;

        //let handle = uWindowHandle{ raw_handle: Some(RawWindowHandle::from(window_handle)), raw_display_handle: Some(RawDisplayHandle::from(WindowsDisplayHandle::empty()))};
        
       // graphics_provider.setup_for_window(window, &handle);
  
        ShowWindow(win32_window, SW_SHOW);

        while (true) {

        }

    }
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

  
        return DefWindowProcA(window, message, wparam, lparam);
    }
}
