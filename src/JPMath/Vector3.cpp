#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

Vec3::Vec3(const Vec4 &a){
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
	elements[2] = a.elements[2];
}

Vec3::Vec3(const Vec2 &a, double b){
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
	elements[2] = b;
}


