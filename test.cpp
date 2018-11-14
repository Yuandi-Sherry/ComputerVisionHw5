#include <iostream>
#include <vector>
#include "Matrix.h"
#include "CImg.h"
using namespace std;
using namespace cimg_library;
int main() {
	CImg<unsigned char> temp1("test2.bmp");
	temp1.display();
}