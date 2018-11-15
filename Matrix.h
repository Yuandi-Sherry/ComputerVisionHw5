#ifndef _MATRIX_H
#define _MATRIX_H
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
class Matrix
{
public:
	int row;
	int column;
	std::vector<std::vector<double> > data;
	Matrix(int row, int column, std::vector<std::vector<double> > data);

	Matrix (const Matrix & otherMatrix);

	Matrix();

	Matrix getInverse();
	double getDet();
	int getRow();
	int getColumn();

	~Matrix();

	friend Matrix operator*(const double & a, const Matrix & other);

	friend Matrix operator*(const Matrix & a, const Matrix & b);


	friend ostream& operator<<(ostream& os,const Matrix& it);
    

    Matrix getT();
	
};
#endif