#ifndef _MORPHING_H_
#define _MORPHING_H_
#define FRAMESNUMBER 11
#include "Point.h"
#include "CImg.h"
#include <fstream>
#include <sstream> 
#include <vector>
#include <string>
#include <iostream>
#include "Delaunay.h"
#include "BNAlgorithm.h"
using namespace std;
using namespace cimg_library;
class Morphing
{
public:
	Morphing(const string & imgPath1, const string & imgPath2 ) {
		initialize(imgPath1, imgPath2);
		generateMesh(img1,pointsSet1, mesh1);
		generateMesh(img2,pointsSet2, mesh2);
		getMidTriangle();
	}
	~Morphing() {

	}

private:
	std::vector<Point> pointsSet1;
	std::vector<Point> pointsSet2;
	Delaunay mesh1;
	Delaunay mesh2;
	CImg<unsigned char> img1;
	CImg<unsigned char> img2;
	/**
	 * 读取文件，将两幅图像的特征点分别加入对应的vector中
	 */
	void initialize(const string & imgPath1, const string & imgPath2) {
		// 读入两张图片
		CImg<unsigned char> temp1(imgPath1.c_str());
		img1 = temp1;
		CImg<unsigned char> temp2(imgPath2.c_str());
		img2 = temp2;

		// 从文件中读入两张图片各自的特征点
		ifstream inFile("point.csv", ios::in);
		if(!inFile) {
			cout << "Fail to open the file" << endl;
		}
		string str;
		int temp[4];
		int i;
		while(getline(inFile, str)) {
			//cout << str << endl;
			stringstream ss(str);
			string tempStr;
			i = 0;
			while(getline(ss, tempStr, ',')) {
				temp[i] = atoi(tempStr.c_str());
				i++;
			}
			Point newPoint1(temp[0], temp[1]);
			Point newPoint2(temp[2], temp[3]);
			pointsSet1.push_back(newPoint1);
			pointsSet2.push_back(newPoint2);
		}
		inFile.close();
	}

	void generateMesh(CImg<unsigned char>& inputImg, const std::vector<Point>& inputPointSet, Delaunay & mesh) {
		Point basicPoints[4];
		Point upperLeft(0,0);
		Point upperRight(inputImg.width()-1, 0);
		Point lowerLeft(0, inputImg.height()-1);
		Point lowerRight(inputImg.width()-1, inputImg.height()-1);
		basicPoints[0] = upperLeft;
		basicPoints[1] = upperRight;
		basicPoints[2] = lowerLeft;
		basicPoints[3] = lowerRight;

		//Delaunay mesh;
		mesh.initialize(basicPoints);
		for(int i = 0; i < inputPointSet.size(); i++) {
			mesh.addPoint(inputPointSet[i].x, inputPointSet[i].y);
		}
	}

	/** 
	 * 为目标图像上的每个点坐标在原图像中找到对应的点。
	 */
	/*void findMapping(Delaunay &mesh) {
		cimg_forXY(img2, x, y) {
			// 找到该像素点所在目标图像中的三角形
			for(int i = 0; i < mesh.triangleVector.size(); i++) {
				if(inTriangle(x,y,mesh.triangleVector[i])) {
					// 根据Beier Neely算法找到目标图像的点和三角形三边的关系，从而根据原图像对应的三角形找到原图像中的位置
					BNAlgorithm mappingRelationship(mesh1.triangleVector[i], mesh2.triangleVector[i], mesh.pointVector, Point(x,y));
					// 在原图像中的像素位置
					Point pointInSource = mappingRelationship.getTargetPoint();
					break;
				}
			}
		}
	}*/

	// 计算变换矩阵
	std::vector<std::vector<int> > transformMatrix () {

	}

	// 计算中间三角形
	std::vector<std::vector<Triangle> > getMidTriangle () {
		std::vector<std::vector<Triangle> > midTriangle;
		for(int i = 0; i <= FRAMESNUMBER; i++) {
			std::vector<Triangle> triangleVectorPerFrame;
			for(int j = 0; j < mesh1.triangleVector.size(); j++) {
				double x, y;
				x = mesh1.pointVector[mesh1.triangleVector[j].vertices[0]].x * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[mesh2.triangleVector[j].vertices[0]].x * (double)i/FRAMESNUMBER;
				y = mesh1.pointVector[mesh1.triangleVector[j].vertices[0]].y * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[mesh2.triangleVector[j].vertices[0]].y * (double)i/FRAMESNUMBER;	
				Point point1(x,y);
				x = mesh1.pointVector[mesh1.triangleVector[j].vertices[1]].x * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[mesh2.triangleVector[j].vertices[1]].x * (double)i/FRAMESNUMBER;
				y = mesh1.pointVector[mesh1.triangleVector[j].vertices[1]].y * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[mesh2.triangleVector[j].vertices[1]].y * (double)i/FRAMESNUMBER;
				Point point2(x,y);
				x = mesh1.pointVector[mesh1.triangleVector[j].vertices[2]].x * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[mesh2.triangleVector[j].vertices[2]].x * (double)i/FRAMESNUMBER;
				y = mesh1.pointVector[mesh1.triangleVector[j].vertices[2]].y * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[mesh2.triangleVector[j].vertices[2]].y * (double)i/FRAMESNUMBER;
				Point point3(x,y);	
				Triangle tempTriangle(point1, point2, point3);
				triangleVectorPerFrame.push_back(tempTriangle);
			}
			midTriangle.push_back(triangleVectorPerFrame);
		}

		/*const unsigned char color[3] = { 0, 255, 0 };
		for (int i = 0; i < mesh1.triangleVector.size(); i++) {
			Triangle temp = mesh1.triangleVector[i];
			Point p1 = midTriangle[11][i].points[0];
			Point p2 = midTriangle[11][i].points[1];
			Point p3 = midTriangle[11][i].points[2];
			img1.draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, 1, ~0U);
		}
		img1.display();*/
	}
};
#endif
