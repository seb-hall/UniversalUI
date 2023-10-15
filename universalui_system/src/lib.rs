
#[cfg_attr(windows, path = "win32/mod.rs")]
pub mod native;

mod window;
pub use window::*;

pub fn init() -> bool {

    println!("system init!");

    return true;
}