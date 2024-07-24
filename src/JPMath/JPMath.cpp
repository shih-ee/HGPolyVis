#include "JPMath.h"
#include "JPMathConstants.h"

bool equals(double val1, double val2, double errTol){
	return fabs(val1-val2) < errTol;
}

bool lessOrEquals(double val1, double val2, double errTol){
	return val1 <= val2 + errTol;
}
bool greaterOrEquals(double val1, double val2, double errTol){
	return val1 + errTol >= val2;
}


double normalizeAngle(double angle){
	double na = angle;
	double twoPI = 2*M_PI;
	while(na > twoPI) na -= twoPI;
	while(na < 0.0)	  na += twoPI;
	return na;
}

double subtractAngles(double angle1, double angle2){
	angle1 = normalizeAngle(angle1);
	angle2 = normalizeAngle(angle2);
	double diff = angle1 - angle2;
	if(diff > M_PI)
		diff = diff - 2.*M_PI;
	if(diff < -M_PI)
		diff = diff + 2.*M_PI;
	return diff;
}

double acot(double x){
	if(x >= 0)
		return M_PI/2 - atan(x);
	else
		return -M_PI/2 - atan(x);
}

bool lineSegIntersect2D(Vec2 q00, Vec2 q01, Vec2 q10, Vec2 q11, Vec2 *ip, double *sout, double *tout){
	Vec2 p0 = q00;
	Vec2 p1 = q10;
	Vec2 d0 = q01-q00;
	Vec2 d1 = q11-q10;
	Vec2 delta = p1-p0;
	double den = dotPerp(d0, d1);

	if(den == 0.0){
		*ip = Vec2(0.0, 0.0);
		return false;
	}

	double s = dotPerp(delta, d1)/den;
	double t = dotPerp(delta, d0)/den;

	if(sout != NULL)
		*sout = s;
	if(tout != NULL)
		*tout = t;

	*ip = p0 + s*d0;

	return true;
}

bool lineSegPlaneIntersect3D(Vec3 p0, Vec3 p1, Vec3 p, Vec3 n, double *sout){
	Vec3 u(p1-p0);
	Vec3 w(p0-p);

	*sout = dot(-n, w)/dot(n, u);

	if(*sout >= 0 && *sout <= 1)
		return true;

	return false;
}

bool rayLineSegIntersect2D(Vec2 rO, Vec2 rD, Vec2 q0, Vec2 q1, Vec2 *ip, double *sout, double *tout){
	Vec2 p0 = rO;
	Vec2 p1 = q0;
	Vec2 d0 = rD;
	Vec2 d1 = q1-q0;
	Vec2 delta = p1-p0;
	double den = dotPerp(d0, d1);

	if(den == 0.0){
		*ip = Vec2(0.0, 0.0);
		return false;
	}

	double s = dotPerp(delta, d1)/den;
	double t = dotPerp(delta, d0)/den;

	if(sout != NULL)
		*sout = s;
	if(tout != NULL)
		*tout = t;

	*ip = p0 + s*d0;

	return true;
}

double degrees(double radians){
     return radians*180/M_PI;
}


double angleVectors2(Vec2 v0, Vec2 v1){
	normalize(v0);
	normalize(v1);
	Vec2 v0Perp(v0.rotate90CCW());
	v1.set(dot(v1, v0), dot(v1, v0Perp));
	normalize(v1);
	return normalizeAngle(fabs(atan2(v1.b, v1.a)));
}

double angleVectors3(Vec3 v03, Vec3 v13){
	normalize(v03);
	normalize(v13);

	double dotv03v13 = dot(v03, v13);
	if(abs(dotv03v13 - 1) < EPS)
		return 0.0;
	if(abs(dotv03v13 + 1) < EPS)
		return M_PI;

	Vec3 v0Perp3(cross(v03, cross(v03, v13)));
	normalize(v0Perp3);
	
	Vec2 v1(dot(v13, v03), dot(v13, v0Perp3));
	normalize(v1);
	return normalizeAngle(fabs(atan2(v1.b, v1.a)));
}

double angle(Vec2 v){
	return normalizeAngle(atan2(v.b, v.a));
}

double pointLineDist2D(const Vec2 &a, const Vec2 &b, const Vec2 &p, Vec2 &closest){
	Vec2 c = p - a;	// Vector from a to Point
	Vec2 v = (b - a);	// Unit Vector from a to b
	normalize(v);
	double d = length(b - a);	// Length of the line segment
	double t = dot(v, c);	// Intersection point Distance from a

	//Vector2 closest;
	// Check to see if the point is on the line
	// if not then return the endpoint
	if(t < 0) closest = a;
	else if(t > d) closest = b;
	else{
		// get the distance to move from point a
		v *= t;

		// move from point a to the nearest point on the segment
		closest = a + v;
	}

	return length(p-closest);
}

double pointLineDist3D(const Vec3 &a, const Vec3 &b, const Vec3 &p, Vec3 &closest){
	Vec3 c = p - a;	// Vector from a to Point
	Vec3 v = (b - a);	// Unit Vector from a to b
	normalize(v);
	double d = length(b - a);	// Length of the line segment
	double t = dot(v, c);	// Intersection point Distance from a

	// Check to see if the point is on the line
	// if not then return the endpoint
	if(t < 0) closest = a;
	else if(t > d) closest = b;
	else{
		// get the distance to move from point a
		v *= t;

		// move from point a to the nearest point on the segment
		closest = a + v;
	}

	return length(p-closest);
}

// probably not the most efficient...
double pointTriangleDistSquared3D(const Vec3 &p, const Vec3 &triV0, const Vec3 &triV1, const Vec3 &triV2, Vec3 &closest){
    Vec3 diff = triV0 - p;
    Vec3 edge0 = triV1 - triV0;
    Vec3 edge1 = triV2 - triV0;
    double a00 = lengthSq(edge0);
    double a01 = dot(edge0,edge1);
    double a11 = lengthSq(edge1);
    double b0 = dot(diff,edge0);
    double b1 = dot(diff,edge1);
    double c = lengthSq(diff);
    double det = fabs(a00*a11 - a01*a01);
    double s = a01*b1 - a11*b0;
    double t = a01*b0 - a00*b1;
    double sqrDistance;

    if (s + t <= det)
    {
        if (s < (double)0)
        {
            if (t < (double)0)  // region 4
            {
                if (b0 < (double)0)
                {
                    t = (double)0;
                    if (-b0 >= a00)
                    {
                        s = (double)1;
                        sqrDistance = a00 + ((double)2)*b0 + c;
                    }
                    else
                    {
                        s = -b0/a00;
                        sqrDistance = b0*s + c;
                    }
                }
                else
                {
                    s = (double)0;
                    if (b1 >= (double)0)
                    {
                        t = (double)0;
                        sqrDistance = c;
                    }
                    else if (-b1 >= a11)
                    {
                        t = (double)1;
                        sqrDistance = a11 + ((double)2)*b1 + c;
                    }
                    else
                    {
                        t = -b1/a11;
                        sqrDistance = b1*t + c;
                    }
                }
            }
            else  // region 3
            {
                s = (double)0;
                if (b1 >= (double)0)
                {
                    t = (double)0;
                    sqrDistance = c;
                }
                else if (-b1 >= a11)
                {
                    t = (double)1;
                    sqrDistance = a11 + ((double)2)*b1 + c;
                }
                else
                {
                    t = -b1/a11;
                    sqrDistance = b1*t + c;
                }
            }
        }
        else if (t < (double)0)  // region 5
        {
            t = (double)0;
            if (b0 >= (double)0)
            {
                s = (double)0;
                sqrDistance = c;
            }
            else if (-b0 >= a00)
            {
                s = (double)1;
                sqrDistance = a00 + ((double)2)*b0 + c;
            }
            else
            {
                s = -b0/a00;
                sqrDistance = b0*s + c;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            double invDet = ((double)1)/det;
            s *= invDet;
            t *= invDet;
            sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                t*(a01*s + a11*t + ((double)2)*b1) + c;
        }
    }
    else
    {
        double tmp0, tmp1, numer, denom;

        if (s < (double)0)  // region 2
        {
            tmp0 = a01 + b0;
            tmp1 = a11 + b1;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)
                {
                    s = (double)1;
                    t = (double)0;
                    sqrDistance = a00 + ((double)2)*b0 + c;
                }
                else
                {
                    s = numer/denom;
                    t = (double)1 - s;
                    sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                        t*(a01*s + a11*t + ((double)2)*b1) + c;
                }
            }
            else
            {
                s = (double)0;
                if (tmp1 <= (double)0)
                {
                    t = (double)1;
                    sqrDistance = a11 + ((double)2)*b1 + c;
                }
                else if (b1 >= (double)0)
                {
                    t = (double)0;
                    sqrDistance = c;
                }
                else
                {
                    t = -b1/a11;
                    sqrDistance = b1*t + c;
                }
            }
        }
        else if (t < (double)0)  // region 6
        {
            tmp0 = a01 + b1;
            tmp1 = a00 + b0;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)
                {
                    t = (double)1;
                    s = (double)0;
                    sqrDistance = a11 + ((double)2)*b1 + c;
                }
                else
                {
                    t = numer/denom;
                    s = (double)1 - t;
                    sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                        t*(a01*s + a11*t + ((double)2)*b1) + c;
                }
            }
            else
            {
                t = (double)0;
                if (tmp1 <= (double)0)
                {
                    s = (double)1;
                    sqrDistance = a00 + ((double)2)*b0 + c;
                }
                else if (b0 >= (double)0)
                {
                    s = (double)0;
                    sqrDistance = c;
                }
                else
                {
                    s = -b0/a00;
                    sqrDistance = b0*s + c;
                }
            }
        }
        else  // region 1
        {
            numer = a11 + b1 - a01 - b0;
            if (numer <= (double)0)
            {
                s = (double)0;
                t = (double)1;
                sqrDistance = a11 + ((double)2)*b1 + c;
            }
            else
            {
                denom = a00 - ((double)2)*a01 + a11;
                if (numer >= denom)
                {
                    s = (double)1;
                    t = (double)0;
                    sqrDistance = a00 + ((double)2)*b0 + c;
                }
                else
                {
                    s = numer/denom;
                    t = (double)1 - s;
                    sqrDistance = s*(a00*s + a01*t + ((double)2)*b0) +
                        t*(a01*s + a11*t + ((double)2)*b1) + c;
                }
            }
        }
    }

    // Account for numerical round-off error.
    if (sqrDistance < (double)0)
    {
        sqrDistance = (double)0;
    }

    closest = triV0 + s*edge0 + t*edge1;

    return sqrDistance;
}

double pointTriangleDist3D(const Vec3 &p, const Vec3 &triV0, const Vec3 &triV1, const Vec3 &triV2, Vec3 &closest){
	return sqrt(pointTriangleDistSquared3D(p, triV0, triV1, triV2, closest));
}


bool circleCircleInteresect(Vec2 c0, Vec2 c1, double r0, double r1, Vec2 *piC, Vec2 *pi0, Vec2 *pi1){
	double d, a, h;

	d = length(c1 - c0);

	if(d > r0 + r1)
		return false;

	if(d < fabs(r1 - r0))
		return false;

	a = (r0*r0 - r1*r1 + d*d) / (2*d);

	h = sqrt(r0*r0 - a*a);

	piC->set(c0 + a*(c1 - c0)/d);

	pi0->x = piC->x + h*(c1.y - c0.y)/d;
	pi0->y = piC->y - h*(c1.x - c0.x)/d;

	pi1->x = piC->x - h*(c1.y - c0.y)/d;
	pi1->y = piC->y + h*(c1.x - c0.x)/d;

	return true;
}