#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

Vec2::Vec2(const Vec4 &a){
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
}

Vec2::Vec2(const Vec3 &a){
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
}
