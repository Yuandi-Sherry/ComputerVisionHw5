#ifndef _MORPHING_H_
#define _MORPHING_H_
#include "Point.h"
#include "CImg.h"
#include <fstream>
#include <sstream> 
#include <vector>
#include <string>
#include <iostream>
#include "Delaunay.h"
using namespace std;
using namespace cimg_library;
class Morphing
{
public:
	Morphing(const string & imgPath1, const string & imgPath2 ) {
		initialize(imgPath1, imgPath2);
		generateMesh(img1,pointsSet1);
		generateMesh(img2,pointsSet2);

	}
	~Morphing() {

	}

private:
	std::vector<Point> pointsSet1;
	std::vector<Point> pointsSet2;
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

	void generateMesh(CImg<unsigned char>& inputImg, const std::vector<Point>& inputPointSet) {
		Point basicPoints[4];
		Point upperLeft(0,0);
		Point upperRight(inputImg.width()-1, 0);
		Point lowerLeft(0, inputImg.height()-1);
		Point lowerRight(inputImg.width()-1, inputImg.height()-1);
		basicPoints[0] = upperLeft;
		basicPoints[1] = upperRight;
		basicPoints[2] = lowerLeft;
		basicPoints[3] = lowerRight;

		Delaunay mesh(basicPoints);
		for(int i = 0; i < inputPointSet.size(); i++) {
			mesh.addPoint(inputPointSet[i].x, inputPointSet[i].y);
		}
		//测试三角分割
		const unsigned char color[3] = { 0, 255, 0 };
		for(int i = 0; i< mesh.triangleVector.size(); i++) {
			inputImg.draw_triangle(
				mesh.pointVector[mesh.triangleVector[i].vertices[0]].x,
				mesh.pointVector[mesh.triangleVector[i].vertices[0]].y,
				mesh.pointVector[mesh.triangleVector[i].vertices[1]].x,
				mesh.pointVector[mesh.triangleVector[i].vertices[1]].y,
				mesh.pointVector[mesh.triangleVector[i].vertices[2]].x,
				mesh.pointVector[mesh.triangleVector[i].vertices[2]].y,
				color, 1, ~0U);
		}
		inputImg.display();
	}
};
#endif
