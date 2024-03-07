#include <gtkmm.h>

class PlotWindow : public Gtk::Window {
public:
    PlotWindow() {
        set_title("Plotter");
        set_default_size(400, 300);

        // Layout
        Gtk::Box* vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 5));
        add(*vbox);

        Gtk::Label* xLabel = Gtk::manage(new Gtk::Label("X: "));
        vbox->pack_start(*xLabel, Gtk::PACK_SHRINK);

        xEntry.set_placeholder_text("Enter X coordinate");
        vbox->pack_start(xEntry, Gtk::PACK_SHRINK);

        Gtk::Label* yLabel = Gtk::manage(new Gtk::Label("Y: "));
        vbox->pack_start(*yLabel, Gtk::PACK_SHRINK);

        yEntry.set_placeholder_text("Enter Y coordinate");
        vbox->pack_start(yEntry, Gtk::PACK_SHRINK);

        plotButton.set_label("Plot");
        plotButton.signal_clicked().connect(sigc::mem_fun(*this, &PlotWindow::on_plot_button_clicked));
        vbox->pack_start(plotButton, Gtk::PACK_SHRINK);

        plotArea.set_size_request(300, 200);
        vbox->pack_start(plotArea);

        show_all_children();
    }

protected:
    void on_plot_button_clicked() {
        double x = std::stod(xEntry.get_text());
        double y = std::stod(yEntry.get_text());

        Cairo::RefPtr<Cairo::Context> cr = plotArea.get_window()->create_cairo_context();
        cr->save();
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->arc(x, y, 3.0, 0.0, 2.0 * M_PI);
        cr->fill();
        cr->restore();
    }

    Gtk::Entry xEntry;
    Gtk::Entry yEntry;
    Gtk::Button plotButton;
    Gtk::DrawingArea plotArea;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    PlotWindow plotWindow;

    return app->run(plotWindow);
}
