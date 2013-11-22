#ifndef GESER_PRIVATE_GEOMETRY_HPP_INCLUDED
#define GESER_PRIVATE_GEOMETRY_HPP_INCLUDED

#include <libxml++/parsers/domparser.h>
#include <librsvg/rsvg.h>

namespace geser
{
    struct Bounds
    {
	bool inside(int _x, int _y);
	int x1, y1, x2, y2;
    };

    class Geometry
    {
	public:
	    Geometry();

	public:
	    void rebuild(xmlpp::Document const &_doc, RsvgHandle *_handle);

    };
}

#endif //GESER_PRIVATE_GEOMETRY_HPP_INCLUDED

