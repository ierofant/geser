#ifndef GESER_PRIVATE_GEOMETRY_HPP_INCLUDED
#define GESER_PRIVATE_GEOMETRY_HPP_INCLUDED

#include <libxml++/parsers/domparser.h>
#include <librsvg/rsvg.h>
#include <geser/bounds.hpp>

namespace geser
{
    class Geometry
    {
	public:
	    Geometry(xmlpp::DomParser &_dom, RsvgHandle *&_handle);

	public:
	    Glib::ustring get_current_grab() const;
	    Glib::ustring get_element_at(int _x, int _y) const;
	    void grab_group(Glib::ustring const &_id);

	private:
	    typedef std::map<Glib::ustring, geser::Bounds> items_type;

	private:
	    xmlpp::DomParser &dom;
	    RsvgHandle *&handle;
	    Glib::ustring current_grab;
	    items_type items;
    };
}

#endif //GESER_PRIVATE_GEOMETRY_HPP_INCLUDED
