#include <geser/bounds.hpp>

geser::Bounds::Bounds(int _x1, int _y1, int _x2, int _y2)
    : x1(_x1),
      y1(_y1),
      x2(_x2),
      y2(_y2)
{

}

int geser::Bounds::width() const
{
    return x2 - x1;
}

int geser::Bounds::height() const
{
    return y2 - y1;
}

int geser::Bounds::area() const
{
    return (x2 - x1) * (y2 - y1);
}

bool geser::Bounds::inside(int _x, int _y) const
{
    return ((_x >= x1) && (_x <= x2) && (_y >= y1) && (_y <= y2));
}
