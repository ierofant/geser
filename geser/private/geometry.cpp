#include <libxml/tree.h>
#include <geser/private/geometry.hpp>

#include <iostream>

geser::Geometry::Geometry(xmlpp::DomParser &_dom, RsvgHandle *&_handle)
    : dom(_dom),
      handle(_handle)
{

}

Glib::ustring geser::Geometry::get_current_grab() const
{
    return current_grab;
}

Glib::ustring geser::Geometry::get_element_at(int _x, int _y) const
{
    Glib::ustring id;
    for(auto &item:items)
    {
	if(item.second.inside(_x, _y))
	{
	    id = item.first;
	    break;
	}
    }
    return id;
}

void geser::Geometry::grab_group(Glib::ustring const &_id)
{
    items.clear();
    current_grab = _id;

    if(handle)
    {
	Glib::ustring xpath = "/descendant-or-self::*[@id = '" + _id + "']";
        xmlpp::NodeSet nodes = dom.get_document()->get_root_node()->find(xpath);
	if(!nodes.empty())
	{
	    xmlpp::Node::NodeList children = nodes.front()->get_children();
	    for(xmlpp::Node::NodeList::iterator itr = children.begin(); itr != children.end(); ++itr)
	    {
		if((*itr)->cobj()->type == XML_ELEMENT_NODE)
	        {
		    xmlpp::Element *element = static_cast<xmlpp::Element*>(*itr);
		    Glib::ustring id = element->get_attribute_value("id"), sharp_id = "#" + id;;
		    RsvgDimensionData dimension;
		    RsvgPositionData position;
		    if(rsvg_handle_get_dimensions_sub(handle, &dimension, sharp_id.c_str()) 
			&& rsvg_handle_get_position_sub(handle, &position, sharp_id.c_str()))
		    {
			items[id] = Bounds(position.x, position.y, position.x + dimension.width, position.y + dimension.height);
std::cout << id << '\t' << position.x << '\t' << position.y << '\t' <<  position.x + dimension.width << '\t' <<  position.y + dimension.height << std::endl;
		    }
		}
	    }
	}
    }
}
