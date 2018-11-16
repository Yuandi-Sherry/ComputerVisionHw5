#ifndef _POINT_H_
#define _POINT_H_
#include "Point.h"
#include <cmath>
using namespace std;
class Point
{
public:
	Point();

	Point(const Point & otherPoint);

	Point(double x, double y);

	Point operator-(Point other);

	Point operator+(Point other);

	double operator*(Point other);

	Point operator*(double other);

	Point operator/(double other);

	double static modulus(const Point & point);

	Point static perpendicular(const Point & point);

	~Point();
	
	double x, y;	
};
#endif