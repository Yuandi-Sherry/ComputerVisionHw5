#ifndef _DELAUNAY_H_
#define _DELAUNAY_H_
#include <vector>
#include "Triangle.h"
#include "Point.h"
#include "Edge.h"

bool cmp(const Triangle & a, const Triangle & b) {
    if(a.vertices[0] > b.vertices[0]) {
    	return 1;
    } else if (a.vertices[0] < b.vertices[0]) {
    	return 0;
    } else {
    	if(a.vertices[1] > b.vertices[1]) {
	    	return 1;
	    } else if (a.vertices[1] < b.vertices[1]) {
	    	return 0;
	    } else {
			if(a.vertices[2] > b.vertices[2]) {
		    	return 1;
		    } else if (a.vertices[2] < b.vertices[2]) {
		    	return 0;
			} else {
				return 0;
			} 
 	    }
    }
}

class Delaunay
{
public:
	std::vector<Triangle> triangleVector; // 三角形集合
	std::vector<Point> pointVector; // 点集合
	std::vector<Edge> edgeVector; 
	
	Delaunay() {

	}
	~Delaunay() {

	}

	void initialize(const Point (&boundary4Points) [4]) {
		// points
		for(int i = 0; i < 4; i++) {
			pointVector.push_back(boundary4Points[i]);
		}
		// edges
		Edge topEdge(0, 1, -1);
		Edge leftEdge(0, 2, -1);
		Edge bottomEdge(2, 3, -1);
		Edge rightEdge(1, 3, -1);
		Edge dia(0, 3, 2);
		edgeVector.push_back(topEdge);
		edgeVector.push_back(leftEdge);
		edgeVector.push_back(bottomEdge);
		edgeVector.push_back(rightEdge);
		edgeVector.push_back(dia);

		// triangles
		Triangle newTriangle1(0, 1, 3);
		Triangle newTriangle2(0, 2, 3);	
		newTriangle1.calculateCircle(pointVector);
		newTriangle2.calculateCircle(pointVector);
		triangleVector.push_back(newTriangle1);
		triangleVector.push_back(newTriangle2);
	}

	/**
	 * 加入新点
	 * @x: 新点x坐标
	 * @y: 新点y坐标
	 */
	void addPoint(int x, int y) {
		std::vector<Edge> boundaryEdges;
		Point newPoint(x,y);
		pointVector.push_back(newPoint);
		std::vector<int> affectingTriangles;// 所有影响三角形的id
		for(int i = 0; i < triangleVector.size(); i++) {
			if(isInCircle(triangleVector[i].x, triangleVector[i].y, triangleVector[i].r, newPoint)) {
				// 影响三角形: 将影响三角形在vector中的id加入
				affectingTriangles.push_back(i);
			}
		}

		// 删除影响三角形的公共边
		for(int i = 0; i < affectingTriangles.size(); i++) {
			deleteCommonEdges(affectingTriangles[i], boundaryEdges);
			// 修正id
			for(int j = i+1; j < affectingTriangles.size(); j++) {
				affectingTriangles[j]--;
			}
		}
		
		// 将插入点同影响三角形的全部顶点连接起来
		for(int i = 0; i < boundaryEdges.size(); i++) {
			Triangle newTriangle(boundaryEdges[i].point1ID, boundaryEdges[i].point2ID, pointVector.size() - 1);
			newTriangle.calculateCircle(pointVector);
			// 插入新生成的边
			for(int j = 0; j < 3; j++) {
				bool flag = 1;
				for(int k = 0; k < edgeVector.size(); k++){
					if(newTriangle.edges[j].point1ID == edgeVector[k].point1ID
						&& newTriangle.edges[j].point2ID == edgeVector[k].point2ID 
						&& edgeVector[k].count == -1) {
						// 边缘已经存
						flag = 0;
					} else if(newTriangle.edges[j].point1ID == edgeVector[k].point1ID
						&& newTriangle.edges[j].point2ID == edgeVector[k].point2ID 
						&& edgeVector[k].count != -1) {
						flag = 0;
						edgeVector[k].count++;
					}
 				}
				if(flag == 1) {
					edgeVector.push_back(newTriangle.edges[j]);
				} 
			}
			triangleVector.push_back(newTriangle);
		}
	}	

	void sortTriangle() {
		sort(triangleVector.begin(), triangleVector.end(), cmp);
		for(int i = 0; i < triangleVector.size(); i++) {
			cout << triangleVector[i].vertices[0] << ' '
				 << triangleVector[i].vertices[1] << ' '
				 << triangleVector[i].vertices[2] << endl;
		}
	}

	bool isInCircle(const double & x, const double & y, const double & r, const Point & newPoint) {
		// 新点到圆心的距离小于圆半径即在圆内
		if(sqrt(pow(newPoint.x - x,2) + pow(newPoint.y - y, 2)) < r) {
			return true;
		}
		return false;
	}

	void deleteCommonEdges(const int & badTriangleID, std::vector<Edge> & boundaryEdges) {
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < edgeVector.size(); j++) {
				// 在边缘数组中找到该影响三角形的边
				if(edgeVector[j].point1ID == triangleVector[badTriangleID].edges[i].point1ID
			    && edgeVector[j].point2ID == triangleVector[badTriangleID].edges[i].point2ID) {
					if(edgeVector[j].count == -1) {
						boundaryEdges.push_back(edgeVector[j]);
					} else if(edgeVector[j].count == 2) {
						// 删掉三角形剩下的边
						edgeVector[j].count = 1;
						boundaryEdges.push_back(edgeVector[j]);
					} else if(edgeVector[j].count == 1) {
						// 将该边彻底删除
						for(int p = 0; p < boundaryEdges.size(); p++) {
							if(boundaryEdges[p].point1ID == edgeVector[j].point1ID 
							&& boundaryEdges[p].point2ID == edgeVector[j].point2ID) {
								// 先将边缘从boundaryEdges中删去
								boundaryEdges.erase(boundaryEdges.begin() + p);
								break;
							}
						}
						// 再将边缘从edgesVector中删去
						edgeVector.erase(edgeVector.begin() + j);
						j--;
					}
					break;
				}
			}
		}
		triangleVector.erase(triangleVector.begin() + badTriangleID);
	}
};
#endif