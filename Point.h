#ifndef _POINT_H_
#define _POINT_H_
class Point
{
public:
	Point() {
		
	}
	Point(const int& x, const int& y) {
		this->x = x;
		this->y = y;
	}
	~Point() {

	}
	int x, y;	
};
#endif