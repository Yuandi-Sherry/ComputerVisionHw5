#include "Triangle.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "Edge.h"
using namespace std;
int compare(const void * a, const void * b) {
	int *pa = (int*)a;
    int *pb = (int*)b;
    return (*pa )- (*pb);  //从小到大排序
}


Triangle::Triangle(const int & point1ID, const int & point2ID, const int & point3ID) {
	vertices[0] = point1ID;
	vertices[1] = point2ID;
	vertices[2] = point3ID;
	qsort(vertices, 3, sizeof(vertices[0]),compare);
	Edge temp1(point1ID, point2ID, 1);
	Edge temp2(point1ID, point3ID, 1);
	Edge temp3(point2ID, point3ID, 1);
	edges[0] = temp1;
	edges[1] = temp2;
	edges[2] = temp3;
}

Triangle::Triangle(const Point & point1, const Point & point2, const Point & point3) {
	points[0] = point1;
	points[1] = point2;
	points[2] = point3;
}
Triangle::~Triangle() {

}
/**
 * 计算三角形的外接圆
 */
void Triangle::calculateCircle(const std::vector<Point> & pointVector) {
	double delta = 2*(pointVector[vertices[0]].x- pointVector[vertices[2]].x) * (pointVector[vertices[1]].y - pointVector[vertices[2]].y) - 2*(pointVector[vertices[0]].y - pointVector[vertices[2]].y) * (pointVector[vertices[1]].x - pointVector[vertices[2]].x);
	x = (pointVector[vertices[1]].y - pointVector[vertices[2]].y) * (pow(pointVector[vertices[0]].x, 2) + pow(pointVector[vertices[0]].y, 2) - pow(pointVector[vertices[2]].x, 2) - pow(pointVector[vertices[2]].y, 2))
		- (pointVector[vertices[0]].y - pointVector[vertices[2]].y) * (pow(pointVector[vertices[1]].x, 2) + pow(pointVector[vertices[1]].y, 2) - pow(pointVector[vertices[2]].x, 2) - pow(pointVector[vertices[2]].y, 2));
	y = (pointVector[vertices[0]].x - pointVector[vertices[2]].x) * (pow(pointVector[vertices[1]].x, 2) + pow(pointVector[vertices[1]].y, 2) - pow(pointVector[vertices[2]].x, 2) - pow(pointVector[vertices[2]].y, 2))
		- (pointVector[vertices[1]].x - pointVector[vertices[2]].x) * (pow(pointVector[vertices[0]].x, 2) + pow(pointVector[vertices[0]].y,2) - pow(pointVector[vertices[2]].x, 2) - pow(pointVector[vertices[2]].y, 2));
	x /= delta;
	y /= delta;
	r = sqrt(pow(pointVector[vertices[0]].x - x, 2) + pow(pointVector[vertices[0]].y - y, 2));
}
