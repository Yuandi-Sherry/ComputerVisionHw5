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
		//mesh1.sortTriangle();
		//mesh2.sortTriangle();
		mesh2.triangleVector = mesh1.triangleVector;
		for(int i = 0; i < mesh1.triangleVector.size(); i++) {
			if(mesh1.pointVector[mesh1.triangleVector[i].vertices[0]].x == pointsSet1[0].x 
			&& mesh1.pointVector[mesh1.triangleVector[i].vertices[0]].y == pointsSet1[0].y) {
				cout << "triangleID : " << i << endl;
			}
		}
		midTrianglesPoints = getMidTrianglePoints();
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
	int count;
	std::vector<std::vector<Point> > midTrianglesPoints;
	/**
	 * 读取文件，将两幅图像的特征点分别加入对应的vector中
	 */
	void initialize(const string & imgPath1, const string & imgPath2) {
		count = 0;
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
		const unsigned char color[3] = { 0, 255, 0 };
		double XRate = (double)img1.width()/img2.width();
		double YRate = (double)img1.height()/img2.height();
		img2.resize(img1.width(), img1.height());
		for(int i = 0; i < pointsSet2.size(); i++) {
			pointsSet2[i].x = round(pointsSet2[i].x * XRate);
			pointsSet2[i].y = round(pointsSet2[i].y * YRate);
			//img2.draw_circle(pointsSet2[i].x, pointsSet2[i].y, 1, color);
		} 
		
		//img2.display();
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

	/**
	 * 计算变换矩阵
	 * frame: 当前帧编号
	 * n: 当前三角形编号
	 * forward: true => 从图一向后变换，
	 *          false => 从图二向前变换
	 */
	Matrix transformMatrix (const int & frame, const int & n, const Delaunay & mesh, std::vector<Matrix> & matrixArg) {
		std::vector<std::vector<double> > targetFrameData;
		std::vector<double> v;
		for(int j = 0; j < 3; j++) {
			v.push_back(midTrianglesPoints[frame][mesh.triangleVector[n].vertices[j]].x);
		}
		targetFrameData.push_back(v);
		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(midTrianglesPoints[frame][mesh.triangleVector[n].vertices[j]].y);
		}
		targetFrameData.push_back(v);
		v.clear();
		for(int j = 0; j < 3; j++) {
			v.push_back(1);
		}
		targetFrameData.push_back(v);
		// 目标帧当前三角形的矩阵
		Matrix targetFrame(3,3,targetFrameData); 
		// 变换矩阵
		Matrix trans = matrixArg[n]*targetFrame.getInverse();
		return trans;
	}

	// 计算中间三角形
	std::vector<std::vector<Point> > getMidTrianglePoints () {
		std::vector<std::vector<Point> > midTrianglesPoints;
		for(int i = 0; i <= FRAMESNUMBER; i++) {
			std::vector<Point> newPointsPerFrame; // 每一帧每一点的坐标
			for(int j = 0; j < mesh1.pointVector.size(); j++) {
				double x, y;
				x = mesh1.pointVector[j].x * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[j].x * (double)i/FRAMESNUMBER;
				y = mesh1.pointVector[j].y * (1-(double)i/FRAMESNUMBER) 
					+ mesh2.pointVector[j].y * (double)i/FRAMESNUMBER;	
				Point point1(x,y);
				newPointsPerFrame.push_back(point1);
			}
			midTrianglesPoints.push_back(newPointsPerFrame);
		}

		// debugging
		// const unsigned char color[3] = { 0, 255, 0 };
		// for(int j = 0; j <= FRAMESNUMBER; j++) {
		// 	for (int i = 0; i < mesh2.triangleVector.size(); i++) {
		// 		Triangle temp = mesh2.triangleVector[i];
		// 		Point p1 = midTrianglesPoints[j][mesh2.triangleVector[i].vertices[0]];
		// 		Point p2 = midTrianglesPoints[j][mesh2.triangleVector[i].vertices[1]];
		// 		Point p3 = midTrianglesPoints[j][mesh2.triangleVector[i].vertices[2]];
		// 		img2.draw_triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, 1, ~0U);
		// 	}
		// 	img2.display();
		// }
		// debugging

		return midTrianglesPoints;
		
	}

	bool isInTriangle(const int & triangleID, int x, int y, int frame, const Delaunay & mesh) {
		Point pointA(mesh.pointVector[mesh.triangleVector[triangleID].vertices[0]]);
		Point pointB(mesh.pointVector[mesh.triangleVector[triangleID].vertices[1]]);
		Point pointC(mesh.pointVector[mesh.triangleVector[triangleID].vertices[2]]);
		Point pointP(x,y);
		// 下面Point表示向量
		Point v0 = pointC - pointA;
		Point v1 = pointB - pointA;
		Point v2 = pointP - pointA;
		double u = ((v1*v1)*(v2*v0)-(v1*v0)*(v2*v1)) / ((v0*v0)*(v1*v1) - (v1*v0)*(v0*v1));
		double v = ((v0*v0)*(v2*v1)-(v0*v1)*(v2*v0))/((v0*v0)*(v1*v1) - (v0*v1)*(v1*v0));
		if(u >= 0 && v >= 0 && u+v <= 1) {
			return true;
		}
		return false;
	}


	void running() {
		std::vector<std::vector<Matrix> > transformMatrices; // 变换矩阵数组
		std::vector<Matrix> start;
		std::vector<Matrix> end;
		calculateConstantMatrices(start, end);
		//cout << "debugging 1" << endl;
		for(int i = 0; i <= FRAMESNUMBER; i++) {
			//cout << "i " << i << endl;
			std::vector<Matrix> tempRow;
			CImg<unsigned char> frameImg(img1.width(), img1.height(), 1, 3);
			
			int triangleID = 0;
			// 当前帧的当前点，当前帧为TARGETFRAME，而origin为我要找的点
			cimg_forXY(img1, x, y) {
				bool sameTri = false;
				bool flag = 0; // 未匹配三角形
				// 判断和上一个像素在同一个三角形中
				if(isInTriangle(triangleID, x, y, i, mesh1)) {
					flag = 1;
					sameTri = true;
				} else {
					//cout << "not the same" << x << " " << y << endl;
					for(int j = 0; j < mesh1.triangleVector.size(); j++) {
						// 如果该像素在三角形内部，则对它使用对应三角形的矩阵变换
						if(isInTriangle(j, x, y, i, mesh1)) {
							triangleID = j;
							flag = 1;
							break;
						}
					}
				}
				
				Matrix T1(transformMatrix(i,triangleID, mesh1, start)); // 获得变换矩阵
				// 获得当前像素点对应的向量

				std::vector<std::vector<double> > vectorData;
				std::vector<double> temp;
				temp.push_back(x);
				vectorData.push_back(temp);
				temp.clear();
				temp.push_back(y);
				vectorData.push_back(temp);
				temp.clear();
				temp.push_back(1);
				vectorData.push_back(temp);
				Matrix pointVec1(3, 1, vectorData);
				Matrix ans1 = T1*pointVec1;
				Point point1(ans1.data[0][0], ans1.data[1][0]); // 图一中点在该帧位置
				if(x == pointsSet1[0].x && y == pointsSet1[0].y) {
					cout << "the zero triangle's first vertex is pointSet1[0] " << endl;
					cout << point1.x << " " << point1.y << endl;
					cout << "------------" << endl;
					cout << midTrianglesPoints[i][mesh1.triangleVector[7].vertices[0]].x << " " << midTrianglesPoints[i][mesh1.triangleVector[7].vertices[0]].y << endl;
				}
				Matrix T2(transformMatrix(i,triangleID, mesh2, end)); // 获得变换矩阵
				// 获得当前像素点对应的向量
				vectorData.clear();
				temp.clear();
				temp.push_back(x);
				vectorData.push_back(temp);
				temp.clear();
				temp.push_back(y);
				vectorData.push_back(temp);
				temp.clear();
				temp.push_back(1);
				vectorData.push_back(temp);
				Matrix pointVec2(3, 1, vectorData);
				Matrix ans2 = T2*pointVec2;
				Point point2(ans2.data[0][0], ans2.data[1][0]);
				if(x == pointsSet2[0].x && y == pointsSet2[0].y) {
					cout << "the zero triangle's first vertex is pointSet1[0] " << endl;
					cout << point2.x << " " << point2.y << endl;
					cout << "------------" << endl;
					cout << midTrianglesPoints[i][mesh1.triangleVector[7].vertices[0]].x << " " << midTrianglesPoints[i][mesh1.triangleVector[7].vertices[0]].y << endl;
				}
				//cout << "point2 " << point2.x << " " << point2.y << endl;
				for(int channel = 0; channel < 3; channel++) {
					//cout << "debugging channel " << channel << endl;
					double rgb1 = bilinearInterpolate(point1, img1, channel);
					double rgb2 = bilinearInterpolate(point2, img2, channel);
					//cout << rgb1 << " " << rgb2 << endl;
					double temp = (1-(double)i/FRAMESNUMBER) * rgb1 + (double)i/FRAMESNUMBER * rgb2;
					//cout << temp << endl;
					if(temp > 255) {
						frameImg(x,y, 0, channel) = 255;
					} else if(temp < 0) {
						frameImg(x,y, 0, channel) = 0;
					} else {
						frameImg(x,y, 0, channel) = temp;
					}
				}
				//cout << "out of break" << endl;
				if(x == 1) {
					cout << "." ;
				}
				if(flag == 0) {
					cout << "no matching triangle" << endl;
				}
			}
			//cout << "debugging 3" << endl;	
			//frameImg.display();	
			const unsigned char color[3] = { 0, 255, 0 };
			string name = "frame";
			name += to_string(i) + ".bmp";
			for(int i = 0; i < pointsSet2.size(); i++) {
				frameImg.draw_circle(pointsSet2[i].x, pointsSet2[i].y, 1, color);
			} 
			frameImg.save(name.c_str());
			
			frameImg.display();
		}
	}

	double bilinearInterpolate(const Point & point, const CImg<unsigned char> & img, const int & channel) {
		//cout << "x " << point.x << "   y " << point.y << endl;
		double a = point.x - floor(point.x);
		double b = point.y - floor(point.y);
		int i = floor(point.x);
		int j = floor(point.y);
		double ans = 255;
		//cout << "i " << i << "   j " << j << "  i+1 " << i+1 << "  j+1 " << j+1 << endl;

		if(i + 1 < img.width() && j + 1 < img.height() ) {
			ans = a*b*img(i,j,0,channel) + a*(1-b)*img(i+1, j, 0, channel)
				+(1-a)*b*img(i, j+1, 0, channel) + (1-a)*(1-b)*img(i+1, j+1, 0, channel);
		} else if( j + 1 >= img.height()){
			ans = a*b*img(i,j,0,channel) + a*(1-b)*img(i+1, j, 0, channel)
				+(1-a)*b*img(i, j, 0, channel) + (1-a)*(1-b)*img(i+1, j, 0, channel);
		} else if( i + 1 >= img.height()){
			ans = a*b*img(i,j,0,channel) + a*(1-b)*img(i, j, 0, channel)
				+(1-a)*b*img(i, j+1, 0, channel) + (1-a)*(1-b)*img(i, j+1, 0, channel);
		}
		/*else if(i+1 < img.width()){
			ans = a*b*img(i,j,0,channel) + a*(1-b)*img(i+1, j, 0, channel)
				+(1-a)*b*img(i, j, 0, channel) + (1-a)*(1-b)*img(i+1, j, 0, channel);
		} else if(j + 1 < img.height()) {
			ans = a*b*img(i,j,0,channel) + a*(1-b)*img(i, j, 0, channel)
				+(1-a)*b*img(i, j+1, 0, channel) + (1-a)*(1-b)*img(i, j+1, 0, channel);
		}*/
		//cout << "ans " << ans << endl;
		return ans;
	}
	void calculateConstantMatrices(std::vector<Matrix> & start, std::vector<Matrix> & end) {
		for(int i =0; i < mesh1.triangleVector.size(); i++) {
			std::vector<std::vector<double> > startData;
			std::vector<double> v;
			for(int j = 0; j < 3; j++) {
				v.push_back(mesh1.pointVector[mesh1.triangleVector[i].vertices[j]].x);
			}
			startData.push_back(v);
			v.clear();
			for(int j = 0; j < 3; j++) {
				v.push_back(mesh1.pointVector[mesh1.triangleVector[i].vertices[j]].y);
			}
			startData.push_back(v);
			// v.clear();
			// for(int j = 0; j < 3; j++) {
			// 	v.push_back(1);
			// }
			// startData.push_back(v);
			start.push_back(Matrix(2,3,startData));	

			std::vector<std::vector<double> > endData;
			v.clear();
			for(int j = 0; j < 3; j++) {
				v.push_back(mesh2.pointVector[mesh2.triangleVector[i].vertices[j]].x);
			}
			endData.push_back(v);

			v.clear();
			for(int j = 0; j < 3; j++) {
				v.push_back(mesh2.pointVector[mesh2.triangleVector[i].vertices[j]].y);
			}
			endData.push_back(v);
			// v.clear();
			// for(int j = 0; j < 3; j++) {
			// 	v.push_back(1);
			// }
			// endData.push_back(v);
			end.push_back(Matrix(2,3,endData));
		}		
	}



	
};
#endif
