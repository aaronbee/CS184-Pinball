// Transform.cpp: implementation of the Transform class.
//
//////////////////////////////////////////////////////////////////////

#include "Transform.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Takes as input the current eye position, and the current up vector.
//up is always normalized to a lenght of 1.
//eye has a length indicating the distance from the viewer to the origin
void Transform::left(float degrees, vec3& eye, vec3& up) {
	mat3 R;
	R.set_rot(degrees * (nv_pi / 180), up);
	eye = R * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
	vec3 right;
	cross(right, eye, up);
	right.normalize();
	mat3 R;
	R.set_rot(degrees* (nv_pi / 180), right);
	up = R * up;
	eye = R * eye;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
