use fltk::{prelude::*, *};
use fltk::widget::Widget;
use fltk::button::Button;
use fltk_theme::{ColorTheme, color_themes, WidgetTheme, ThemeType};

static mut isDark: bool = true;
fn changeTheme(btn: &mut Button) {

    unsafe {
        if (!isDark) {
            let theme = ColorTheme::new(color_themes::BLACK_THEME);
            theme.apply();
            let widget_theme = WidgetTheme::new(ThemeType::Dark);
            widget_theme.apply();
            isDark = true;
        } else {
            let theme = ColorTheme::new(color_themes::TAN_THEME);
            theme.apply();
            let widget_theme = WidgetTheme::new(ThemeType::Greybird);
            widget_theme.apply();
            isDark = false;
        }
    }

  btn.clear();


}

fn main() {
    let a = app::App::default().with_scheme(app::Scheme::Gtk);
    let theme = ColorTheme::new(color_themes::BLACK_THEME);
    theme.apply();
    let widget_theme = WidgetTheme::new(ThemeType::Dark);
    widget_theme.apply();
    let mut win = window::Window::default().with_size(400, 300);
    win.make_resizable(true);
    let mut btn = button::Button::new(160, 200, 80, 40, "Hello");
    btn.set_callback(changeTheme);

    win.end();

    win.show();
    a.run().unwrap();
}