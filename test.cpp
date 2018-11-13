#include <iostream>
#include <vector>
#include "Matrix.h"
using namespace std;
int main() {
	std::vector<std::vector<double> > v;
	//for(int i = 0; i < 3; i++) {
	std::vector<double> tempV;
	tempV.push_back(1);
	tempV.push_back(2);
	tempV.push_back(3);
	v.push_back(tempV);
	tempV.clear();
	tempV.push_back(4);
	tempV.push_back(5);
	tempV.push_back(6);
	v.push_back(tempV);
	Matrix test(2,3,v);

	v.clear();
	tempV.clear();
	tempV.push_back(1);
	tempV.push_back(4);
	v.push_back(tempV);
	tempV.clear();
	tempV.push_back(2);
	tempV.push_back(5);
	v.push_back(tempV);
	tempV.clear();
	tempV.push_back(3);
	tempV.push_back(6);
	v.push_back(tempV);
	Matrix test2(3,2,v);


	//cout << det << endl;
	cout << test * test2 << endl;
}