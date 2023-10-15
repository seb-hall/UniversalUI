

pub fn init() -> bool {


    println!("universalui init!");

    universalui_system::init();
    universalui_system::native::create_window();
    return true;
}