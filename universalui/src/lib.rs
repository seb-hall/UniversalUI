

pub fn init() -> bool {


    println!("universalui init!");

    universalui_system::init();
    universalui_system::native::init();
    universalui_system::native::create_window();

    loop {
        universalui_system::native::poll_events();
    }
    
    return true;
}