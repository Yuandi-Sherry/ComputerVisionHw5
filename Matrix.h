#ifndef _MATRIX_H
#define _MATRIX_H
#include <iostream>
#include <cmath>
using namespace std;
class Matrix
{
public:
	int row;
	int column;
	std::vector<std::vector<double> > data;
	Matrix(int row, int column, std::vector<std::vector<double> > data) {
		this->row = row;
		this->column = column;
		for(int i = 0; i < row; i++) {
			std::vector<double> tempRow;
			for(int j = 0; j < column; j++) {
				tempRow.push_back(data[i][j]);
			}
			this->data.push_back(tempRow);
		}
	}

	Matrix() {
		row = -1;
		column = -1;
	}


	Matrix getInverse() {
		Matrix inversedMatrix;
		//cout << *this << endl;
		std::vector<std::vector<double> > adjData; // 伴随矩阵数据
		if(row != column) {
			cout << "In getInverse(), The number of rows is not equal to the number of columns" << endl;
			return Matrix();
		}
 		// 求伴随矩阵
 		for(int i = 0; i < row; i++) {
 			std::vector<double> tempRowInAdj;
 			for(int j = 0; j < row; j++) {
 				std::vector<std::vector<double> > cofactorData; // 余子式矩阵
 				for(int p = 0; p < row; p++) {
 					if(p == i) {
 						continue;
 					}
 					std::vector<double> tempRow;
 					for(int q = 0; q < row; q++) {
 						if(q!=j)
 							tempRow.push_back(data[p][q]);
 					}
 					cofactorData.push_back(tempRow);
 				}
 				
 				Matrix cofactor(row-1, row-1, cofactorData);
 				tempRowInAdj.push_back(cofactor.getDet() * (pow(-1, i + j)));
 			}
 			adjData.push_back(tempRowInAdj);
 		}
 		Matrix adj(row, column, adjData);
 		if(this->getDet()!=0) {
 			inversedMatrix = 1/this->getDet() * (adj.getT());
 		}
 		return inversedMatrix;
	}

	double getDet() { // 求行列式
		double ans = 0;
		if(column != row) {
			cout << "In getDet(), the number of rows is not equal to the number of columns" << endl;
			return 0;
		}
		if(row == 2) {
			ans = data[0][0] * data[1][1] - data[0][1] * data[1][0];
		} else {
			//cout << *this << endl;
			for(int i = 0; i < row; i++) {
				double product = 1;
				for(int j = 0; j < row; j++) {
					product *= data[j][(j + i)%row];
				}
				ans += product;
				// cout << "ans += product; " << product << endl;
				product = 1;
				for(int j = 0; j < row; j++) {
					product *= data[j][(i - j + row)%row];
				}
				ans -= product;
				// cout << "ans -= product; " << product << endl;
			}
		}
		
		return ans;
	}
	int getRow() {
		return row;
	}
	int getColumn() {
		return column;
	}

	~Matrix() {

	}

	friend Matrix operator*(const double & a, const Matrix & other) {
		std::vector<std::vector<double> > newMatrixData;
		for(int i = 0; i < other.row; i++) {
			std::vector<double> tempRow;
			for(int j = 0; j < other.row; j++) {
				tempRow.push_back(a*other.data[i][j]);
			}
			newMatrixData.push_back(tempRow);
		}
		Matrix newMatrix(other.row, other.column, newMatrixData);
		return newMatrix;
	}

	friend Matrix operator*(const Matrix & a, const Matrix & b) {
		if(a.column != b.row) {
			cout << "The number of rows is not equal to the number of columns" << endl;
			return Matrix();
		}
		std::vector<std::vector<double> > newMatrixData;
		for(int i = 0; i < a.row; i++) {
			std::vector<double> tempRow;
			for(int j = 0; j < b.column; j++) {
				double temp = 0;
				for(int k = 0; k < a.column; k++) {
					temp += a.data[i][k] * b.data[k][j];
				}
				tempRow.push_back(temp);
			}
			newMatrixData.push_back(tempRow);
		}
		Matrix newMatrix(a.row, b.column, newMatrixData);
		//cout << newMatrix << endl;
		return newMatrix;
	}


	friend ostream& operator<<(ostream& os,const Matrix& it){
        for(int i = 0; i < it.row; i++) {
        	for(int j = 0; j < it.column; j++) {
        		os << it.data[i][j] << " ";
        	}
        	os << endl;
        }
        return os;
    }

    Matrix getT() { // 求转置
    	std::vector<std::vector<double>> newMatrixData;
    	for(int i = 0; i < row; i++) {
    		std::vector<double> tempRow;
    		for(int j = 0; j < column; j++) {
    			tempRow.push_back(data[j][i]);
    		}
    		newMatrixData.push_back(tempRow);
    	}
    	Matrix newMatrix(this->row, this->column, newMatrixData);
    	return newMatrix;
    }
	
};
#endif