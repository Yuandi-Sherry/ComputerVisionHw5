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
#include "Matrix.h"
using namespace std;
using namespace cimg_library;
class Morphing
{
public:
	Morphing(const string & imgPath1, const string & imgPath2 ) {
		initialize(imgPath1, imgPath2);
		generateMesh(img1,pointsSet1, mesh1);
		generateMesh(img2,pointsSet2, mesh2);
		mesh1.sortTriangle();
		midTriangle = getMidTriangle();
		/*for(int i = 0; i <mesh1.triangleVector.size(); i++) {
			if(mesh1.triangleVector[i].vertices[0] != mesh2.triangleVector[i].vertices[0]
				|| mesh1.triangleVector[i].vertices[1] != mesh2.triangleVector[i].vertices[1]
				|| mesh1.triangleVector[i].vertices[2] != mesh2.triangleVector[i].vertices[2])
			cout << " not same " << endl;
		}*/
		running();
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
	std::vector<std::vector<Triangle> > midTriangle;
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

		//修改图片大小
		double XRate = (double)img1.width()/img2.width();
		double YRate = (double)img1.height()/img2.height();
		cout << "XRate: " << XRate << "  YRate: " << YRate << endl;
		for(int i = 0; i < pointsSet2.size(); i++) {
			pointsSet2[i].x*=XRate;
			pointsSet2[i].y*=YRate;
		} 
		img2.resize(img1.width(), img1.height());
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
	Matrix transformMatrix (const int & frame, const int & n) {
		// 计算当前位置的三角形
		std::vector<std::vector<double> > beforeData;
		std::vector<double> v;
		for(int j = 0; j < 3; j++) {
			v.push_back(midTriangle[frame][n].points[j].x);
		}
		beforeData.push_back(v);

		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(midTriangle[frame][n].points[j].y);
		}
		beforeData.push_back(v);
		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(1);
		}
		beforeData.push_back(v);
		Matrix before(3,3,beforeData);
		
		// 计算下一帧的三角形
		std::vector<std::vector<double> > afterData;
		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(midTriangle[frame+1][n].points[j].x);
		}
		afterData.push_back(v);

		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(midTriangle[frame+1][n].points[j].y);
		}
		afterData.push_back(v);
		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(1);
		}
		afterData.push_back(v);
		Matrix after(2, 3, afterData);
		Matrix trans = after*before.getInverse();
		return trans;
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
		const unsigned char color[3] = { 0, 255, 0 };
		//for(int j = 0; j <= FRAMESNUMBER; j++) {
			for (int i = 0; i < mesh1.triangleVector.size(); i++) {
				Triangle temp = mesh1.triangleVector[i];
				Point p1 = midTriangle[6][i].points[0];
				Point p2 = midTriangle[6][i].points[1];
				Point p3 = midTriangle[6][i].points[2];
				img1.draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, 1, ~0U);
			}
			img1.display();
		//}
		

		return midTriangle;
		
	}


	void running() {
		std::vector<std::vector<Matrix> > transformMatrices;
		for(int i = 0; i < FRAMESNUMBER; i++) {
			std::vector<Matrix> tempRow;
			for(int j = 0; j < mesh1.triangleVector.size(); j++) {
				tempRow.push_back(transformMatrix(i,j));
			}
		}

	}
};
#endif
