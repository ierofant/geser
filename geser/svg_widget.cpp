#include <iostream>
#include <cstring>
#include <geser/svg_widget.hpp>
#include <geser/private/geometry.hpp>

#if RSVG_CAIRO_INCLUDE == ON
#include <librsvg/rsvg-cairo.h>
#endif

geser::SvgWidget::SvgWidget()
    : Glib::ObjectBase(typeid(*this)),
      handle(nullptr),
      geometry(std::make_shared<geser::Geometry>(dom, handle))
{
    set_has_window(true);
}

geser::SvgWidget::~SvgWidget()
{
    if(handle) g_object_unref(handle);
}

Glib::ustring geser::SvgWidget::get_element_at(int _x, int _y) const
{
    Glib::ustring id;
    if(geometry) id = geometry->get_element_at(_x, _y);
    return id;
}

const xmlpp::Document* geser::SvgWidget::get_document() const
{
    return dom.get_document();
}

xmlpp::Document* geser::SvgWidget::get_document()
{
    return dom.get_document();
}

void geser::SvgWidget::set_source(Glib::ustring const &_str)
{
    dom.parse_memory(_str);
    Glib::ustring id = get_document()->get_root_node()->get_attribute_value("id");
    refresh();
    grab_group(id);
}

void geser::SvgWidget::set_source_file(Glib::ustring const &_filename)
{
    dom.parse_file(_filename);
    Glib::ustring id = get_document()->get_root_node()->get_attribute_value("id");
    refresh();
    grab_group(id);
}

void geser::SvgWidget::refresh()
{
    refresh_renderer();
    queue_draw();
}

void geser::SvgWidget::grab_group(Glib::ustring const &_id)
{
    if(geometry) geometry->grab_group(_id);
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

void geser::SvgWidget::get_preferred_width_vfunc(int &_minimum_width, int &_natural_width) const
{
    xmlpp::Element *element = get_document()->get_root_node();
    Glib::ustring str = element->get_attribute_value("width");
    if(!str.empty()) _minimum_width = _natural_width = std::ceil(std::atof(str.c_str()));
}

void geser::SvgWidget::get_preferred_height_vfunc(int &_minimum_height, int &_natural_height) const
{
    xmlpp::Element *element = get_document()->get_root_node();
    Glib::ustring str = element->get_attribute_value("height");
    if(!str.empty()) _minimum_height = _natural_height = std::ceil(std::atof(str.c_str()));
}

void geser::SvgWidget::on_size_allocate(Gtk::Allocation &_allocation)
{
    set_allocation(_allocation);
    if(window) window->move_resize(_allocation.get_x(),
				   _allocation.get_y(),
				   _allocation.get_width(),
				   _allocation.get_height());
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
	std::cout << "x: " << _event->x << "\ty: " << _event->y << "\tid: " << get_element_at(_event->x, _event->y) << std::endl;
    }
    return true;
}

void geser::SvgWidget::refresh_renderer()
{
    if(handle) g_object_unref(handle);

    Glib::ustring str = dom.get_document()->write_to_string();
    GError *error = nullptr;
    handle = rsvg_handle_new_from_data(reinterpret_cast<const guchar*>(str.c_str()),
				       str.bytes(),
				       &error);
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
