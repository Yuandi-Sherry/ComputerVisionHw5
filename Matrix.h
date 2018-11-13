##ifndef _MATRIX_H
#define _MATRIX_H
class Matrix
{
public:
	int data[3][3];
	Matrix(int * column1, int * column2, int *column3) {
		for(int i = 0; i < 3; i++) {
			data[i][0] = column1[i];
			data[i][1] = column2[i];
			data[i][2] = column3[i];
		}
	}
	~Matrix() {

	}
	
};
#endif