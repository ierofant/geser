#include <geser/bounds.hpp>

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
