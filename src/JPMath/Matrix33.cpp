#include "Matrix33.h"
#include "Matrix22.h"
#include "Matrix44.h"


Matrix33::Matrix33(const Matrix22 &that){
	elements[0][0] = that.elements[0][0];  elements[0][1] = that.elements[0][1];  elements[0][2] = 0;
	elements[1][0] = that.elements[1][0];  elements[1][1] = that.elements[1][1];  elements[1][2] = 0;
	elements[2][0] = 0                  ;  elements[2][1] = 0                  ;  elements[2][2] = 1;
}


Matrix33::Matrix33(const Matrix44 &that){
	elements[0][0] = that.elements[0][0];  elements[0][1] = that.elements[0][1];  elements[0][2] = that.elements[0][2];
	elements[1][0] = that.elements[1][0];  elements[1][1] = that.elements[1][1];  elements[1][2] = that.elements[1][2];
	elements[2][0] = that.elements[2][0];  elements[2][1] = that.elements[2][1];  elements[2][2] = that.elements[2][2];
}

