#ifndef GESER_BOUNDS_HPP_INCLUDED
#define GESER_BOUNDS_HPP_INCLUDED

namespace geser
{
    struct Bounds
    {
	Bounds(int _x1 = 0, int _y1 = 0, int _x2 = 0, int _y2 = 0);
	bool operator==(Bounds const &_bounds) const;
	bool operator!=(Bounds const &_bounds) const;
	bool operator<(Bounds const &_bounds) const;
	bool operator>(Bounds const &_bounds) const;
	bool inside(int _x, int _y) const;
	int x1, y1, x2, y2;
    };
}

#endif //GESER_BOUNDS_HPP_INCLUDED
