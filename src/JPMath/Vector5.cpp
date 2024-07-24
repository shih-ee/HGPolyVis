#include "Vector5.h"
#include "Vector3.h"
#include "Vector2.h"

Vec5::Vec5(const Vec3 &a, double b, double c) {
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
	elements[2] = a.elements[2];
	elements[3] = b;
	elements[4] = c;
}

Vec5::Vec5(const Vec2 &a, double b, double c, double d) {
	elements[0] = a.elements[0];
	elements[1] = a.elements[1];
	elements[2] = b;
	elements[3] = c;
	elements[4] = d;
}

double magnitude(const Vec5 &v) {
	double sum = 0.0;
	sum = sqrt(v.a * v.a + v.b * v.b + v.c * v.c + v.d * v.d + v.e * v.e);
	return sum;
}

