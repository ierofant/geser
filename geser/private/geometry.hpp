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
	    typedef std::vector<xmlpp::Element*> ElementSet;

	public:
	    Geometry(RsvgHandle *&_handle);

	public:
	    ElementSet get_elements_at(int _x, int _y) const;
	    void rebuild(xmlpp::NodeSet const &_nodes);

	private:
	    typedef std::map<xmlpp::Element*, geser::Bounds> items_type;

	private:
	    RsvgHandle *&handle;
	    items_type items;
    };
}

#endif //GESER_PRIVATE_GEOMETRY_HPP_INCLUDED
