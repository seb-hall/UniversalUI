use fltk::{
    prelude::*,
    *,
    app, button::Button, frame::Frame, window::Window,
    image::IcoImage
};
use glow::*;

fn main() {

    let app = app::App::default();
    let mut outerWin = window::Window::default().with_size(800, 600).with_label("OUTER WINDOW");
    outerWin.make_resizable(true);
    let mut but = Button::new(25, 25, 200, 40, "New");

    let mut win = window::GlWindow::new(250, 0, 550, 600, "GLWINDOW");
    win.set_mode(enums::Mode::Opengl3);
    
    win.end();
    win.show();

    outerWin.end();
    outerWin.show();

    //let icon: IcoImage = IcoImage::load(&std::path::Path::new("src/fltk.ico")).unwrap();
    //win.make_resizable(true);
    //win.set_icon(Some(icon));

    //win.end();
   
    
    unsafe {
        let gl = glow::Context::from_loader_function(|s| {
            win.get_proc_address(s) as *const _
        });

        let vertex_array = gl
            .create_vertex_array()
            .expect("Cannot create vertex array");
        gl.bind_vertex_array(Some(vertex_array));

        let program = gl.create_program().expect("Cannot create program");

        let (vertex_shader_source, fragment_shader_source) = (
            r#"const vec2 verts[3] = vec2[3](
                vec2(0.5f, 1.0f),
                vec2(0.0f, 0.0f),
                vec2(1.0f, 0.0f)
            );
            out vec2 vert;
            void main() {
                vert = verts[gl_VertexID];
                gl_Position = vec4(vert - 0.5, 0.0, 1.0);
            }"#,
            r#"precision mediump float;
            in vec2 vert;
            out vec4 color;
            void main() {
                color = vec4(vert, 0.5, 1.0);
            }"#,
        );

        let shader_sources = [
            (glow::VERTEX_SHADER, vertex_shader_source),
            (glow::FRAGMENT_SHADER, fragment_shader_source),
        ];

        let mut shaders = Vec::with_capacity(shader_sources.len());

        for (shader_type, shader_source) in shader_sources.iter() {
            let shader = gl
                .create_shader(*shader_type)
                .expect("Cannot create shader");
            gl.shader_source(shader, &format!("#version 410\n{}", shader_source));
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

        gl.use_program(Some(program));
        gl.clear_color(0.1, 0.2, 0.3, 1.0);

        win.draw(move |w| {
            gl.clear(glow::COLOR_BUFFER_BIT);
            gl.draw_arrays(glow::TRIANGLES, 0, 3);
            w.swap_buffers();
        });
    }

    app.run().unwrap();
}