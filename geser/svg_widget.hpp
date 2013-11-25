#ifndef SVG_WIDGET_HPP_INCLUDED
#define SVG_WIDGET_HPP_INCLUDED

#include <memory>
#include <gtkmm/widget.h>
#include <libxml++/parsers/domparser.h>
#include <librsvg/rsvg.h>

namespace geser
{
    class Geometry;
    class SvgWidget : public Gtk::Widget
    {
	public:
	    SvgWidget();
	    virtual ~SvgWidget();

	public:
	    Glib::ustring get_element_at(int _x, int _y) const;
	    const xmlpp::Document* get_document() const;
	    xmlpp::Document* get_document();
	    void set_source(Glib::ustring const &_str);
	    void set_source_file(Glib::ustring const &_filename);
	    void refresh();
	    void grab_group(Glib::ustring const &_id);

	protected:
	    virtual void on_realize() override;
	    virtual void on_unrealize() override;
	    virtual void get_preferred_width_vfunc(int &_minimum_width, int &_natural_width) const override;
	    virtual void get_preferred_height_vfunc(int &_minimum_height, int &_natural_height) const override;
	    virtual void on_size_allocate(Gtk::Allocation &_allocation) override;
	    virtual bool on_draw(Cairo::RefPtr<Cairo::Context> const &_cr) override;
	    virtual bool on_button_press_event(GdkEventButton *_event) override;

	private:
	    void refresh_renderer();

	private:
	    Glib::RefPtr<Gdk::Window> window;
	    RsvgHandle *handle;
	    xmlpp::DomParser dom;
	    std::shared_ptr<geser::Geometry> geometry;
    };
}

#endif //SVG_WIDGET_HPP_INCLUDED
