#ifndef _BNALGORITHM_H_
#define _BNALGORITHM_H_ value
#include "Triangle.h"
#include "Point.h"
// Beier & Neely
class BNAlgorithm
{
public:
	Point XPrime;
	BNAlgorithm(Triangle source, Triangle target, const std::vector<Point>& pointVector, const Point & targetPixel) {
		Point X = targetPixel;
		Point P = pointVector[target.vertices[0]];
		Point Q = pointVector[target.vertices[1]];
		Point PPrime = pointVector[source.vertices[0]];
		Point QPrime = pointVector[source.vertices[1]];
		double mod = Point::modulus(Q - P);
		double u = (X - P) * (Q - P) / pow(mod, 2);
		double v = (X - P) * Point::perpendicular(Q-P) / mod;
		XPrime = PPrime + (QPrime - PPrime) *u + Point::perpendicular(QPrime-PPrime) * v / Point::modulus(QPrime - PPrime);
	}
	Point getTargetPoint() {
		return XPrime;
	}
	~BNAlgorithm() {

	}
	
};
#endif