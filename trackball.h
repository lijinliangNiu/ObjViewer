#pragma once

//������ʼ��������յ����깹����Ԫ����ת����
void trackBall(float q[4], float x1, float y1, float x2, float y2);

//����һ��opengl�ľ���
void trackBall_GL_Matrix(double m[16], float x1, float y1, float x2, float y2);

//����������Ԫ�飬�õ���һ����Ԫ��
void add_Quats(float *q1, float *q2, float *dest);

//������Ԫ��õ�һ����ת����
void build_Rotmatrix(float m[4][4], float q[4]);

//�������һ��������ת�ĽǶȼ����һ����Ԫ�飬�Ƕ���һ�����ȣ���������ڵ����������С�
void axis_To_Quat(float a[3], float phi, float q[4]);