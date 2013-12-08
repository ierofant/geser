#include <libxml/tree.h>
#include <geser/private/geometry.hpp>

geser::Geometry::Geometry(RsvgHandle *&_handle)
    : handle(_handle)
{

}

geser::Geometry::ElementSet geser::Geometry::get_elements_at(int _x, int _y) const
{
    ElementSet elements;
    for(auto &item:items)
	if(item.second.inside(_x, _y))
	    elements.push_back(item.first);
    return elements;
}

void geser::Geometry::rebuild(xmlpp::NodeSet const &_nodes)
{
    items.clear();
    for(auto itr = _nodes.begin(); itr != _nodes.end(); ++itr)
    {
	if((*itr)->cobj()->type == XML_ELEMENT_NODE)
	{
	    xmlpp::Element *element = static_cast<xmlpp::Element*>(*itr);
	    Glib::ustring id = "#" + element->get_attribute_value("id");
	    RsvgDimensionData dimension;
	    RsvgPositionData position;
	    if(rsvg_handle_get_dimensions_sub(handle, &dimension, id.c_str()) 
	       && rsvg_handle_get_position_sub(handle, &position, id.c_str()))
	    {
		items[element] = Bounds(position.x,
					position.y,
					position.x + dimension.width,
					position.y + dimension.height);
	    }
	}
    }
}
