#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159

typedef float Mat4[16];
typedef float Mat3[9];

typedef float Vec2[2];
typedef float Vec3[3];
typedef float Vec4[4];


void Math_Perspective(Mat4 m, float fovy, float aspect, float near, float far);
void Math_LookAtV(Mat4 m, Vec3 eye, Vec3 center, Vec3 up);
void Math_Translate3D(Mat4 m, float Tx, float Ty, float Tz);
float Math_DegToRad(float angle);

float Vec3_Len(Vec3 v1);
void Vec3_Sub(Vec3 v, Vec3 v1, Vec3 v2);
void Vec3_Normalize(Vec3 v, Vec3 v1);
void Vec3_Cross(Vec3 v, Vec3 v1, Vec3 v2);
void Vec3_Print(Vec3 v);

void Mat4_Mul(Mat4 m, Mat4 m1, Mat4 m2);
void Mat4_Print(Mat4 m);