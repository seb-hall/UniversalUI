

pub fn init() -> bool {


    println!("universalui init!");

    universalui_graphics::load_vulkan_library();

    universalui_system::init();

    
    universalui_system::native::init();

    
    

    loop {
        universalui_system::native::poll_events();
    }
    
    return true;
}