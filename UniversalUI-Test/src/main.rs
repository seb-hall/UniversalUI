use fltk::{
    prelude::*,
    *,
    app, button::Button, frame::Frame, window::Window,
    image::IcoImage,
    enums::Event,
};
use glow::*;
extern crate glow;

use glow::HasContext;
use std::mem;
use rfd::FileDialog;
use std::path::{Path, PathBuf};
use std::vec::Vec;
use std::{cell::RefCell, rc::Rc};
use std::borrow::BorrowMut;

const VERTEX_SHADER_SOURCE: &str = r#"#version 330
  in vec3 in_position;
  out vec2 position;
  uniform vec2 viewOffset;
  void main() {
    position = in_position.xy;
    gl_Position = vec4(in_position.xy + viewOffset, in_position.z, 1.0);
  }"#;
const FRAGMENT_SHADER_SOURCE: &str = r#"#version 330
  precision mediump float;
  in vec2 position;
  out vec4 color;
  uniform float blue;
  void main() {
    color = vec4(position, blue, 1.0);
  }"#;


// new functions 
fn GetContext(win: &window::GlWindow) -> glow::Context {
    unsafe {
        glow::Context::from_loader_function(|s| {
            win.get_proc_address(s) as *const _
        })    
    }
}

fn CreateNewBuffer(gl: &glow::Context, verts: &Vec<f32>) -> (NativeBuffer, NativeVertexArray, i32) {

    unsafe {
        // This is a flat array of f32s that are to be interpreted as vec2s.
        let triangle_vertices = [0.5f32, 1.0f32, 0.0f32, 0.0f32, 1.0f32, 0.0f32];
        let triangle_vertices_u8: &[u8] = core::slice::from_raw_parts(
            verts.as_ptr() as *const u8,
            verts.len() * core::mem::size_of::<f32>(),
        );

        // We construct a buffer and upload the data
        let vbo = gl.create_buffer().unwrap();
        gl.bind_buffer(glow::ARRAY_BUFFER, Some(vbo));
        gl.buffer_data_u8_slice(glow::ARRAY_BUFFER, triangle_vertices_u8, glow::STATIC_DRAW);

        // We now construct a vertex array to describe the format of the input buffer
        let vao = gl.create_vertex_array().unwrap();
        gl.bind_vertex_array(Some(vao));
        gl.enable_vertex_attrib_array(0);
        gl.vertex_attrib_pointer_f32(0, 3, glow::FLOAT, false, 12, 0);

        (vbo, vao,  (verts.len() / 3).try_into().unwrap())
    }

}

fn DestroyBuffer(gl: &glow::Context, buffer: NativeBuffer, array: NativeVertexArray) {
    unsafe {
        gl.delete_vertex_array(array);
        gl.delete_buffer(buffer);
    }
}


// copy paste functions
unsafe fn create_program(
    gl: &glow::Context,
    vertex_shader_source: &str,
    fragment_shader_source: &str,
) -> NativeProgram {
    let program = gl.create_program().expect("Cannot create program");

    let shader_sources = [
        (glow::VERTEX_SHADER, vertex_shader_source),
        (glow::FRAGMENT_SHADER, fragment_shader_source),
    ];

    let mut shaders = Vec::with_capacity(shader_sources.len());

    for (shader_type, shader_source) in shader_sources.iter() {
        let shader = gl
            .create_shader(*shader_type)
            .expect("Cannot create shader");
        gl.shader_source(shader, shader_source);
        gl.compile_shader(shader);
        if !gl.get_shader_compile_status(shader) {
            panic!("{}", gl.get_shader_info_log(shader));
        }
        gl.attach_shader(program, shader);
        shaders.push(shader);
    }

    gl.link_program(program);
    if !gl.get_program_link_status(program) {
        panic!("{}", gl.get_program_info_log(program));
    }

    for shader in shaders {
        gl.detach_shader(program, shader);
        gl.delete_shader(shader);
    }

    program
}

unsafe fn create_vertex_buffer(gl: &glow::Context) -> (NativeBuffer, NativeVertexArray) {
    // This is a flat array of f32s that are to be interpreted as vec2s.
    let triangle_vertices = [0.5f32, 1.0f32, 0.0f32, 0.0f32, 1.0f32, 0.0f32];
    let triangle_vertices_u8: &[u8] = core::slice::from_raw_parts(
        triangle_vertices.as_ptr() as *const u8,
        triangle_vertices.len() * core::mem::size_of::<f32>(),
    );

    // We construct a buffer and upload the data
    let vbo = gl.create_buffer().unwrap();
    gl.bind_buffer(glow::ARRAY_BUFFER, Some(vbo));
    gl.buffer_data_u8_slice(glow::ARRAY_BUFFER, triangle_vertices_u8, glow::STATIC_DRAW);

    // We now construct a vertex array to describe the format of the input buffer
    let vao = gl.create_vertex_array().unwrap();
    gl.bind_vertex_array(Some(vao));
    gl.enable_vertex_attrib_array(0);
    gl.vertex_attrib_pointer_f32(0, 3, glow::FLOAT, false, 8, 0);

    (vbo, vao)
}


fn main() {

    let app = app::App::default();
    let mut outerWin = window::Window::default().with_size(800, 600).with_label("OUTER WINDOW");
    outerWin.make_resizable(true);
    




    let mut but = Button::new(25, 25, 200, 40, "Load OBJ");
    

    let mut win = window::GlWindow::new(250, 0, 550, 600, "GLWINDOW");
    win.set_mode(enums::Mode::Opengl3);
    
    win.end();
    win.show();
    
    


    outerWin.end();
    outerWin.show();

    let gl: glow::Context = GetContext(&win);

    unsafe {
        // Create a shader program from source
        let program = create_program(&gl, VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
        gl.use_program(Some(program));
        gl.clear_color(0.2, 0.2, 0.2, 1.0);
        // Upload some uniforms
        

        let mut vertsVec: Vec<f32> = Vec::new();

        let mut filePath: Option<PathBuf> = FileDialog::new()
            .add_filter("object", &["obj"])
            .set_directory("C://")
            .pick_file(); 
        
            match filePath {
                None => println!("Cancelled without selecting a file"),
                Some(file) => {
                    let mut pathString: PathBuf = file;
                    println!("{:?}", pathString.as_mut_os_string());
    
                    let (models, materials) =
                    tobj::load_obj(
                        &pathString,
                        &tobj::LoadOptions::default()
                    )
                    .expect("Failed to OBJ load file");
                    
                    println!("loaded ok! (I think)");
                    
                    println!("Number of models          = {}", models.len());
                    //println!("Number of materials       = {}", materials.expect("failed to load materials").len());
    
                    for (i, m) in models.iter().enumerate() {
                        let mesh = &m.mesh;
                        /*println!("");
                        println!("model[{}].name             = \'{}\'", i, m.name);
                        println!("model[{}].mesh.material_id = {:?}", i, mesh.material_id);
    
                        println!(
                            "model[{}].face_count       = {}",
                            i,
                            mesh.face_arities.len()
                        );
    
                        let mut next_face = 0;
                        for face in 0..mesh.face_arities.len() {
                            let end = next_face + mesh.face_arities[face] as usize;
    
                            let face_indices = &mesh.indices[next_face..end];
                            println!(" face[{}].indices          = {:?}", face, face_indices);
    
                            if !mesh.texcoord_indices.is_empty() {
                                let texcoord_face_indices = &mesh.texcoord_indices[next_face..end];
                                println!(
                                    " face[{}].texcoord_indices = {:?}",
                                    face, texcoord_face_indices
                                );
                            }
                            if !mesh.normal_indices.is_empty() {
                                let normal_face_indices = &mesh.normal_indices[next_face..end];
                                println!(
                                    " face[{}].normal_indices   = {:?}",
                                    face, normal_face_indices
                                );
                            }
    
                            next_face = end;
                        }
    
                        // Normals and texture coordinates are also loaded, but not printed in
                        // this example.
                        println!(
                            "model[{}].positions        = {}",
                            i,
                            mesh.positions.len() / 3
                        );
                        assert!(mesh.positions.len() % 3 == 0);
                        */
                        for vtx in 0..mesh.positions.len() / 3 {
                            /*println!(
                                "              position[{}] = ({}, {}, {})",
                                vtx,
                                mesh.positions[3 * vtx],
                                mesh.positions[3 * vtx + 1],
                                mesh.positions[3 * vtx + 2]
                            );*/
                            vertsVec.push( mesh.positions[3 * vtx]);
                            vertsVec.push( mesh.positions[3 * vtx + 1]);
                            vertsVec.push( mesh.positions[3 * vtx + 2]);
                        }
                        
                       
                        

                    }
    
                },
            }


        let verts = vec![
            // first triangle
            0.5f32,  0.5f32, 0.0f32,  // top right
            0.5f32, -0.5f32, 0.0f32,  // bottom right
            -0.5f32,  0.5f32, 0.0f32,  // top left 
            // second triangle
            0.5f32, -0.5f32, 0.0f32,  // bottom right
            -0.5f32, -0.5f32, 0.0f32,  // bottom left
            -0.5f32,  0.5f32, 0.0f32   // top left
        ];
        // Create a vertex buffer and vertex array object
        let (vbo, vao, vl) = CreateNewBuffer(&gl, &vertsVec);

        let buffers = vec![(vbo, vao, vl)];
        
        let last_x: Rc<RefCell<f32>> = Rc::new(RefCell::new(0.0));
        let last_y: Rc<RefCell<f32>> = Rc::new(RefCell::new(0.0));

            
        win.draw(move |w| {
            //println!("DRAW");
            unsafe {

                        // Get mutable references to last_x and last_y
            let mut last_x = last_x.borrow();
            let mut last_y = last_y.borrow();

                let uniform_location = gl.get_uniform_location(program, "viewOffset");
                // See also `uniform_n_i32`, `uniform_n_u32`, `uniform_matrix_4_f32_slice` etc.
                gl.uniform_2_f32(uniform_location.as_ref(), last_x.clone(), last_y.clone());
                println!("UNIFORM: x={}, y={}", last_x, last_y);

                gl.clear(glow::COLOR_BUFFER_BIT);
                for i in &buffers {
                    gl.bind_vertex_array(Some(i.1));
                    gl.draw_arrays(glow::LINES, 0, i.2);
                }

                
            }

            w.swap_buffers();
        });


        
        win.handle({

            
            
            move |_, event| match event {
                Event::Push => {
                    //last_x = app::event_x();
                    //last_y = app::event_y();
                    true
                }
                Event::Drag => {
                    let current_x = app::event_x();
                    let current_y = app::event_y();
    
                    
    
                    

                                    // Get mutable references to last_x and last_y
                    let mut lastx = last_x.borrow_mut();
                    let mut lasty = last_y.borrow_mut();

                    // Calculate the distance dragged
                    let dx = current_x - (lastx.borrow().clone()) as i32;
                    let dy = current_y - (lasty.borrow().clone()) as i32;

                    // Do something with the drag distance or mouse position
                    println!("Mouse Dragged: dx={}, dy={}", dx, dy);

                    // Now you can update last_x and last_y within this closure
                    *lastx = Rc::new(RefCell::new(0.0)); // Replace new_x_value with the actual value you want
                    *lasty = Rc::new(RefCell::new(0.0)); // Replace new_y_value with the actual value you want


                    
                    true
                }
                _ => false,
            }
        });

        let (s, r) = app::channel();

        while app.wait() {
            if let Some(msg) = r.recv() {
                match msg {
                    true => println!("Clicked"),
                    false => (), // Here we basically do nothing
                }
            }
            
            win.flush();
            println!("flush");
        }
    
        

    }


    
}