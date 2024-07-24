#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

Vec4::Vec4(const Vec3 &a, double b){
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
	elements[2] = a.elements[2];
	elements[3] = b;
}

Vec4::Vec4(const Vec2 &a, double b, double c){
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
	elements[2] = b;
	elements[3] = c;
}