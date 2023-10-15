
use vulkano::swapchain::*;
use std::sync::Arc;
use vulkano::VulkanLibrary;
use vulkano::instance::{Instance, InstanceCreateInfo};


use crate::native::*;

pub struct SystemWindow {

    pub surface: Arc<Surface>,
    pub rawHandle: WindowHandle,

   
}

impl SystemWindow {
    pub fn init(title: &str, width: u32, height: u32) -> Self {

        println!("set up new window");
        
        let handle: WindowHandle = create_window();

        let library: Arc<VulkanLibrary> = VulkanLibrary::new().expect("no local Vulkan library/DLL");
        let required_extensions = required_vk_extensions(&library);

        let vulkan_instance = Instance::new(
            library,
            InstanceCreateInfo {
                enabled_extensions: required_extensions,
                ..Default::default()
            },
        )
        .expect("failed to create instance");
        unsafe {
            let vulkan_surface =  Surface::from_win32(vulkan_instance, &handle.instance, &handle.handle, None).expect("failed to create a vulkan surface!");

            return SystemWindow {
                surface: vulkan_surface,
                rawHandle: handle
            };
        }
        

        
    }
}