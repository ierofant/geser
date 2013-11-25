#include <geser/private/geometry.hpp>

geser::Bounds::Bounds(int _x1, int _y1, int _x2, int _y2)
    : x1(_x1),
      y1(_y1),
      x2(_x2),
      y2(_y2)
{

}

bool geser::Bounds::operator==(Bounds const &_bounds) const
{
    return ((x2 - x1) * (y2 - y2)) == ((_bounds.x2 - _bounds.x1) * (_bounds.y2) - (_bounds.y1));
}

bool geser::Bounds::operator!=(Bounds const &_bounds) const
{
    return this->operator==(_bounds);
}

bool geser::Bounds::operator<(Bounds const &_bounds) const
{
    return ((x2 - x1) * (y2 - y2)) < ((_bounds.x2 - _bounds.x1) * (_bounds.y2) - (_bounds.y1));
}

bool geser::Bounds::operator>(Bounds const &_bounds) const
{
    return ((x2 - x1) * (y2 - y2)) == ((_bounds.x2 - _bounds.x1) * (_bounds.y2) - (_bounds.y1));
}

bool geser::Bounds::inside(int _x, int _y) const
{
    return ((_x >= x1) && (_x <= x2) && (_y >= y1) && (_y <= y2));
}

geser::Geometry::Geometry(xmlpp::DomParser &_dom, RsvgHandle *&_handle)
    : dom(_dom),
      handle(_handle)
{

}

void geser::Geometry::rebuild(Glib::ustring const &_id)
{
    items.clear();

    xmlpp::NodeSet nodes = dom.get_document().get_root_node().
}
