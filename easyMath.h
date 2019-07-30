#pragma once

#include <cmath>
#include <assert.h>
#include <gl/glut.h>

using namespace std;

#define X_1 0
#define Y_1 1
#define Z_1 2
#define W_1 3

static void vZero(float *v){
	v[X_1] = 0.0;
	v[Y_1] = 0.0;
	v[Z_1] = 0.0;
}

static void vSet(float *v, float x, float y, float z){
	v[X_1] = x;
	v[Y_1] = y;
	v[Z_1] = z;
}

static void vSub(const float *src1, const float *src2, float *dst){
	dst[X_1] = src1[X_1] - src2[X_1];
	dst[Y_1] = src1[Y_1] - src2[Y_1];
	dst[Z_1] = src1[Z_1] - src2[Z_1];
}

//把数组一复制到数组二
static void vCopy(const float *v1, float *v2){
	register int i;
	for (i = 0; i < 3; i++)
		v2[i] = v1[i];
}

static void vCross(const float *v1, const float *v2, float *cross){
	float temp[3];

	temp[X_1] = (v1[Y_1] * v2[Z_1]) - (v1[Z_1] * v2[Y_1]);
	temp[Y_1] = (v1[Z_1] * v2[X_1]) - (v1[X_1] * v2[Z_1]);
	temp[Z_1] = (v1[X_1] * v2[Y_1]) - (v1[Y_1] * v2[X_1]);
	vCopy(temp, cross);
}

//空间点到原点的长度
static float vLength(const float *v){
	return sqrt(v[X_1] * v[X_1] + v[Y_1] * v[Y_1] + v[Z_1] * v[Z_1]);
}

//将每个坐标乘以一个浮点数
static void vScale(float *v, float div){
	v[X_1] *= div;
	v[Y_1] *= div;
	v[Z_1] *= div;
}
//计算法向量
static void vNormal(float *v){
	vScale(v, 1.0f / vLength(v));
}
//标量积
static float vDot(const float *v1, const float *v2){
	return v1[X_1] * v2[X_1] + v1[Y_1] * v2[Y_1] + v1[Z_1] * v2[Z_1];
}

static void vAdd(const float *src1, const float *src2, float *dst){
	dst[X_1] = src1[X_1] + src2[X_1];
	dst[Y_1] = src1[Y_1] + src2[Y_1];
	dst[Z_1] = src1[Z_1] + src2[Z_1];
}