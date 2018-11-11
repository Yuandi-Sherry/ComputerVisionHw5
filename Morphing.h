#ifndef _MORPHING_H_
#define _MORPHING_H_
#include "Point.h"
#include "CImg.h"
#include <fstream>
#include <sstream> 
#include <vector>
#include <string>
#include <iostream>
using namespace std;
using namespace cimg_library;
class Morphing
{
public:
	Morphing(string hhh) {
		initialize();
		cout << "********pointsSet1**********" << endl;
		for(int i = 0; i < pointsSet1.size(); i++){
			cout << pointsSet1[i].x << " " << pointsSet1[i].y << endl;
		}
		cout << "********pointsSet2**********" << endl;
		for(int i = 0; i < pointsSet2.size(); i++){
			cout << pointsSet2[i].x << " " << pointsSet2[i].y << endl;
		}


	}
	~Morphing() {

	}

private:
	std::vector<Point> pointsSet1;
	std::vector<Point> pointsSet2;
	void initialize() {
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
};
#endif
