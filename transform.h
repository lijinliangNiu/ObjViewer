#pragma once

#include<QString>

//QStringת��Ϊchar*
static char* T_QString2Char(QString str){
	QByteArray b = str.toLatin1();
	char* strChar = b.data();
	return strChar;
}

/*�ַ����ȵ�ת������*/
//��ô����ԭ������Ϊ��Ҫ��ʾ����
static char* T_Char2Char(char* str){
	QString err = QString::fromLocal8Bit(str);
	return T_QString2Char(err);
}

static QString T_char2QString(char* str){
	QString str1 = QString::fromLocal8Bit(str);
	return str1;
}

static int RoundUpToTheNextHighestPowerOf2(int v){
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

//��1X16��һά����ת��Ϊ4X4�Ķ�ά����
static void Transform_4X4Matrix(float *matrix,float returnMatrix[4][4]){
	returnMatrix[0][0] = matrix[0];
	returnMatrix[0][1] = matrix[1];
	returnMatrix[0][2] = matrix[2];
	returnMatrix[0][3] = matrix[3];

	returnMatrix[1][0] = matrix[4];
	returnMatrix[1][1] = matrix[5];
	returnMatrix[1][2] = matrix[6];
	returnMatrix[1][3] = matrix[7];

	returnMatrix[2][0] = matrix[8];
	returnMatrix[2][1] = matrix[9];
	returnMatrix[2][2] = matrix[10];
	returnMatrix[2][3] = matrix[11];

	returnMatrix[3][0] = matrix[12];
	returnMatrix[3][1] = matrix[13];
	returnMatrix[3][2] = matrix[14];
	returnMatrix[3][3] = matrix[15];
}

//��4X4����ת��Ϊ1X16��һά����
static void Transform_1X16Matrix(float *matrix, float Matrix[4][4]){
	matrix[0] = Matrix[0][0];
	matrix[1] = Matrix[0][1];
	matrix[2] = Matrix[0][2];
	matrix[3] = Matrix[0][3];

	matrix[4] = Matrix[1][0];
	matrix[5] = Matrix[1][1];
	matrix[6] = Matrix[1][2];
	matrix[7] = Matrix[1][3];

	matrix[8] = Matrix[2][0];
	matrix[9] = Matrix[2][1];
	matrix[10] = Matrix[2][2];
	matrix[11] = Matrix[2][3];

	matrix[12] = Matrix[3][0];
	matrix[13] = Matrix[3][1];
	matrix[14] = Matrix[3][2];
	matrix[15] = Matrix[3][3];
}