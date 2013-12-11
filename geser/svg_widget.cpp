#include <iostream>
#include <cstring>
#include <geser/svg_widget.hpp>
#include <geser/private/geometry.hpp>

#ifdef RSVG_CAIRO_INCLUDE
#include <librsvg/rsvg-cairo.h>
#endif

geser::SvgWidget::SvgWidget()
    : Glib::ObjectBase(typeid(*this)),
      property_scalable_(*this, "scalable", true),
      property_scale_(*this, "scale", 1),
      handle(nullptr),
      geometry(std::make_shared<geser::Geometry>(handle))
{
    set_has_window(true);
}

geser::SvgWidget::~SvgWidget()
{
    if(handle) g_object_unref(handle);
}

geser::SvgWidget::ElementSet geser::SvgWidget::get_elements_at(int _x, int _y) const
{
    ElementSet elements;
    if(geometry) elements = geometry->get_elements_at(_x, _y);
    return elements;
}

geser::Bounds geser::SvgWidget::get_bounds(xmlpp::Element *_element)
{
    geser::Bounds bounds;
    if(geometry) bounds = geometry->get_bounds(_element);
    return bounds;
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
    refresh();
    grab_items(get_document()->get_root_node()->find("/*"));
}

void geser::SvgWidget::set_source_file(Glib::ustring const &_filename)
{
    dom.parse_file(_filename);
    refresh();
    grab_items(get_document()->get_root_node()->find("/*"));
}

void geser::SvgWidget::refresh()
{
    refresh_renderer();
    queue_resize();
}

void geser::SvgWidget::grab_items(xmlpp::NodeSet const &_nodes)
{
    if(geometry) geometry->rebuild(_nodes);
}

void geser::SvgWidget::grab_items(xmlpp::Node::NodeList const &_node)
{
    xmlpp::NodeSet nodes;
    std::copy(_node.begin(), _node.end(), std::back_inserter(nodes));
    grab_items(nodes);
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
    Glib::ustring str;
    auto *doc = get_document();
    if(doc)
    {
	auto *root = doc->get_root_node();
	if(root) str = root->get_attribute_value("width");
    }

    if(str.empty()) Gtk::Widget::get_preferred_width_vfunc(_minimum_width, _natural_width);
    else _minimum_width = _natural_width = std::ceil(std::atof(str.c_str()));
}

void geser::SvgWidget::get_preferred_height_vfunc(int &_minimum_height, int &_natural_height) const
{
    Glib::ustring str;
    auto *doc = get_document();
    if(doc)
    {
	auto *root = doc->get_root_node();
	if(root) str = root->get_attribute_value("height");
    }

    if(str.empty()) Gtk::Widget::get_preferred_height_vfunc(_minimum_height, _natural_height);
    else _minimum_height = _natural_height = std::ceil(std::atof(str.c_str()));
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
    if(_cr)
    {
	if(property_scalable_ = true) _cr->scale(property_scale_, property_scale_);
	if(handle) rsvg_handle_render_cairo(handle, _cr->cobj());
    }
    return true;
}

bool geser::SvgWidget::on_button_press_event(GdkEventButton *_event)
{
    return false;
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
