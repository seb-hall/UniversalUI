use std::sync::Arc;
use std::ptr::*;
use vulkano::VulkanLibrary;
use vulkano::instance::{Instance, InstanceCreateInfo};
use vulkano::device::{QueueFlags, Device, DeviceCreateInfo, QueueCreateInfo};



pub fn load_vulkan_library() -> bool {

    let library: Arc<VulkanLibrary> = VulkanLibrary::new().expect("no local Vulkan library/DLL");

    let instance: Arc<Instance> = Instance::new(library, InstanceCreateInfo::default())
        .expect("failed to create instance");


    let physical_device = instance
        .enumerate_physical_devices()
        .expect("could not enumerate devices")
        .next()
        .expect("no devices available");

    for family in physical_device.queue_family_properties() {
        println!("Found a queue family with {:?} queue(s)", family.queue_count);
    }

    let queue_family_index = physical_device
    .queue_family_properties()
    .iter()
    .enumerate()
    .position(|(_queue_family_index, queue_family_properties)| {
        queue_family_properties.queue_flags.contains(QueueFlags::GRAPHICS)
    })
    .expect("couldn't find a graphical queue family") as u32;

    let (device, mut queues) = Device::new(
        physical_device,
        DeviceCreateInfo {
            // here we pass the desired queue family to use by index
            queue_create_infos: vec![QueueCreateInfo {
                queue_family_index,
                ..Default::default()
            }],
            ..Default::default()
        },
    )
    .expect("failed to create device");

    let queue = queues.next().unwrap();

    return true;

}


pub fn init_graphics() {

    println!("graphics init!");
}