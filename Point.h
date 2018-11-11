#ifndef _POINT_H_
#define _POINT_H_
class Point
{
public:
	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}
	~Point() {
		
	}
	int x, y;	
};
#endif