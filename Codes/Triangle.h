#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include <iostream>
#include <cstdlib>
#include <vector>
#include "Point.h"
#include "Edge.h"
using namespace std;

class Triangle
{
public:
	Point points[3];
	int vertices[3]; // 三个顶点对应的特征点在Morphing的PointSet中的下标
	Edge edges[3]; // 三角形的三条边
	double x; // 外接圆圆心横坐标
	double y; // 外接圆圆心纵坐标
	double r; // 外接圆圆心半径

	// 三个顶点ID组成的三角形
	Triangle(const int & point1ID, const int & point2ID, const int & point3ID);

	// 三个顶点组成的三角形...？
	Triangle(const Point & point1, const Point & point2, const Point & point3);

	~Triangle();
	
	/**
	 * 计算三角形的外接圆
	 */
	void calculateCircle(const std::vector<Point> & pointVector);
	
};
#endif
