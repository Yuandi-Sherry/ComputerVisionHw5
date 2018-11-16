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
#include "Matrix.h"
using namespace std;
using namespace cimg_library;
class Morphing
{
public:
	Morphing(const string & imgPath1, const string & imgPath2 );
	~Morphing();

//private:
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
	void initialize(const string & imgPath1, const string & imgPath2);

	void generateMesh(CImg<unsigned char>& inputImg, const std::vector<Point>& inputPointSet, Delaunay & mesh);

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
	 * @frame - 当前帧编号
	 * @n - 当前三角形下标
	 * @mesh - mesh1/mesh2
	 * @matrixArg - 常量矩阵数组
	 */
	Matrix transformMatrix (const int & frame, const int & n, const Delaunay & mesh, std::vector<Matrix> & matrixArg);
	// 计算中间三角形
	std::vector<std::vector<Point> > getMidTrianglePoints ();

	bool isInTriangle(const int & triangleID, int x, int y, int frame);

	void running();
	double bilinearInterpolate(const Point & point, const CImg<unsigned char> & img, const int & channel);
	void calculateConstantMatrices(std::vector<Matrix> & start, std::vector<Matrix> & end);
	
};
#endif
