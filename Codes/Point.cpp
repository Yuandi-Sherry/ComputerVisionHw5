#include "Point.h"
#include <cmath>
using namespace std;

Point::Point() {

}

Point::Point(const Point & otherPoint) {
	this->x = otherPoint.x;
	this->y = otherPoint.y;
}

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}

Point Point::operator-(Point other) {
	Point newPoint;
	newPoint.x = this->x - other.x;
	newPoint.y = this->y - other.y;
	return newPoint;
}

Point Point::operator+(Point other) {
	Point newPoint;
	newPoint.x = this->x + other.x;
	newPoint.y = this->y + other.y;
	return newPoint;
}

double Point::operator*(Point other) {
	double ans = this->x * other.x + this->y * other.y;
	return ans;
}

Point Point::operator*(double other) {
	Point newPoint;
	newPoint.x = this->x * other;
	newPoint.y = this->y * other;
	return newPoint;
}

Point Point::operator/(double other) {
	Point newPoint;
	newPoint.x = this->x / other;
	newPoint.y = this->y / other;
	return newPoint;
}

double Point::modulus(const Point & point) {
	double ans = sqrt( pow(point.x, 2) + pow(point.y, 2));
	return ans;
}

Point Point::perpendicular(const Point & point) {
	Point newPoint;
	//double x;
	newPoint.x = sqrt(pow(modulus(point),2)/(1+pow((double)(point.x)/point.y, 2)));
	newPoint.y = -newPoint.x*point.x/(double)point.y;
	return newPoint;
}
Point::~Point() {

}