#include "math.h"

void Math_Perspective(Mat4 m, float fovy, float aspect, float near, float far) {
	float nmf = near-far;
	float f = 1.0/tan(fovy/2.0);
	m[0] = f / aspect; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = f; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = (near + far) / nmf; m[11] = -1;
	m[12] = 0; m[13] = 0; m[14] = (2.0 * far * near) / nmf; m[15] = 0;
}

void Math_LookAtV(Mat4 m, Vec3 eye, Vec3 center, Vec3 up) {
	Vec3 f, s, u;
	Mat4 tx;

	Vec3_Sub(f, center, eye);
	Vec3_Normalize(f, f);

	Vec3_Normalize(s, up);
	Vec3_Cross(s, f, s);
	Vec3_Normalize(s, s);

	Vec3_Cross(u, s, f);
	
	m[0] = s[0]; m[1] = u[0]; m[2] = -f[0]; m[3] = 0;
	m[4] = s[1]; m[5] = u[1]; m[6] = -f[1]; m[7] = 0;
	m[8] = s[2]; m[9] = u[2]; m[10] = -f[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;

	Math_Translate3D(tx, -eye[0], -eye[1], -eye[2]);
	Mat4_Mul(m, m, tx);
}

void Math_Translate3D(Mat4 m, float Tx, float Ty, float Tz) {
	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = Tx; m[13] = Ty; m[14] = Tz; m[15] = 1;
}

float Math_DegToRad(float angle) {
	return angle * PI / 180;
}

float Vec3_Len(Vec3 v1) {
	return sqrt(v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2]);
}

void Vec3_Sub(Vec3 v, Vec3 v1, Vec3 v2) {
	v[0] = v1[0]-v2[0]; 
	v[1] = v1[1]-v2[1]; 
	v[2] = v1[2]-v2[2];
}

void Vec3_Normalize(Vec3 v, Vec3 v1) {
	float l = 1.0 / Vec3_Len(v1);
	v[0] = v1[0] * l;
	v[1] = v1[1] * l;
	v[2] = v1[2] * l;
}

void Vec3_Cross(Vec3 v, Vec3 v1, Vec3 v2) {
	v[0] = v1[1]*v2[2] - v1[2]*v2[1];
	v[1] = v1[2]*v2[0] - v1[0]*v2[2];
	v[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

void Mat4_Mul(Mat4 m, Mat4 m1, Mat4 m2) {
	m[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3]; 
	m[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3]; 
	m[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
	m[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7]; 
	m[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7]; 
	m[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7]; 
	m[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
	m[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11]; 
	m[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11]; 
	m[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11]; 
	m[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
	m[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15]; 
	m[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15]; 
	m[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15]; 
	m[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
}
