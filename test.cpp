#include <gtkmm.h>

int main(int argc, char* argv[]) {
    Gtk::Main kit(argc, argv);

    Gtk::Window window;

    window.set_title("Missile Flight Path");
    window.set_default_size(640, 480);
    window.set_position(Gtk::WIN_POS_CENTER);
    Gtk::Main::run(window);

    return 0;
}
