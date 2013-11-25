#ifndef GESER_PRIVATE_GEOMETRY_HPP_INCLUDED
#define GESER_PRIVATE_GEOMETRY_HPP_INCLUDED

#include <libxml++/parsers/domparser.h>
#include <librsvg/rsvg.h>

namespace geser
{
    struct Bounds
    {
	Bounds(int _x1, int _y1, int _x2, int _y2);
	bool operator==(Bounds const &_bounds) const;
	bool operator!=(Bounds const &_bounds) const;
	bool operator<(Bounds const &_bounds) const;
	bool operator>(Bounds const &_bounds) const;
	bool inside(int _x, int _y) const;
	int x1, y1, x2, y2;
    };

    class Geometry
    {
	public:
	    Geometry(xmlpp::DomParser &_dom, RsvgHandle *&_handle);

	public:
	    void rebuild(Glib::ustring const &_id);

	private:
	    typedef std::map<Glib::ustring, geser::Bounds> items_type;

	private:
	    xmlpp::DomParser &dom;
	    RsvgHandle *&handle;
	    items_type items;
    };
}

#endif //GESER_PRIVATE_GEOMETRY_HPP_INCLUDED

