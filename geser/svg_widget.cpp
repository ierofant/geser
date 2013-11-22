#include <iostream>
#include <cstring>
#include <geser/svg_widget.hpp>
#include <geser/private/geometry.hpp>
#include <librsvg/rsvg-cairo.h>

geser::SvgWidget::SvgWidget()
    : Glib::ObjectBase(typeid(*this)),
      handle(nullptr),
      geometry(std::make_shared<geser::Geometry>())
{
    set_has_window(true);
}

geser::SvgWidget::~SvgWidget()
{
    if(handle) g_object_unref(handle);
}

xmlpp::Document* geser::SvgWidget::get_document()
{
    return dom.get_document();
}

void geser::SvgWidget::set_source(Glib::ustring const &_str)
{
    dom.parse_memory(_str);
}

void geser::SvgWidget::set_source_file(Glib::ustring const &_filename)
{
    dom.parse_file(_filename);
}

void geser::SvgWidget::refresh()
{
    refresh_renderer();
    queue_draw();
}

void geser::SvgWidget::on_realize()
{
    set_realized();
    if(!window)
    {
	auto allocation = get_allocation();

	GdkWindowAttr attrs;
	std::memset(&attrs, 0, sizeof(attrs));
	attrs.x = allocation.get_x();
	attrs.y = allocation.get_y();
	attrs.width = allocation.get_width();
	attrs.height = allocation.get_height();
	attrs.event_mask = get_events() | Gdk::EXPOSURE_MASK;
	attrs.window_type = GDK_WINDOW_CHILD;
	attrs.wclass = GDK_INPUT_OUTPUT;

	window = Gdk::Window::create(get_parent_window(), &attrs, GDK_WA_X | GDK_WA_Y);
	set_window(window);
	window->set_user_data(gobj());
    }
}

void geser::SvgWidget::on_unrealize()
{
    window.reset();
    Gtk::Widget::on_unrealize();
}

void geser::SvgWidget::on_size_allocate(Gtk::Allocation &_allocation)
{
    set_allocation(_allocation);
    if(window) window->move_resize(_allocation.get_x(), _allocation.get_y(), _allocation.get_width(), _allocation.get_height());
}

bool geser::SvgWidget::on_draw(Cairo::RefPtr<Cairo::Context> const &_cr)
{
    if(_cr && handle) rsvg_handle_render_cairo(handle, _cr->cobj());
    return true;
}

bool geser::SvgWidget::on_button_press_event(GdkEventButton *_event)
{
    if(_event)
    {
	std::cout << "x: " << _event->x << "\t" << "y: " << y << std::endl;
    }
    return true;
}

void geser::SvgWidget::refresh_renderer()
{
    if(handle) g_object_unref(handle);

    Glib::ustring str = dom.get_document()->write_to_string();
    GError *error = nullptr;
    handle = rsvg_handle_new_from_data(reinterpret_cast<const guchar *>(str.c_str()), str.bytes(), &error);
    if(error)
    {
	std::cerr << error->message << std::endl;
	g_error_free(error);
    }
    rsvg_handle_close(handle, &error);
    if(error)
    {
	std::cerr << error->message << std::endl;
	g_error_free(error);
    }
}
