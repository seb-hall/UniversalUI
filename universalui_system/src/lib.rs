
#[cfg_attr(windows, path = "win32/mod.rs")]
pub mod native;
pub use native::*;

mod windowManager;
pub use windowManager::*;

pub fn init() -> bool {

    println!("system init!");

    return true;
}