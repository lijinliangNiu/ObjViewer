#pragma once

#include <qstring.h>
#include <QList>

#include <string>
#include <gl/glut.h>

using namespace std;

//��Ⱦģʽ
#define _GL_FLAT (1<<0)//1

//��
struct Vert{
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
};

//�㷨����
struct VertNorm{
	GLfloat NX;
	GLfloat NY;
	GLfloat NZ;
};

struct FaceNorm{
	GLfloat NX;
	GLfloat NY;
	GLfloat NZ;
};

struct Face{
	int index_Face;//ƽ�淨����������

	QList<int> vertIndexList;//��ĵ㼯����model�е�����
	QList<int> vertNormIndexList;
};

class Model{
public:
	Model();
	Model(QString filename);
	~Model();
	//Model(const Model&);

	size_t vertNum;//�ڵ����
	size_t vertNormNum;//�ڵ������ĸ���
	size_t faceNum;//��ĸ���

	QList<Vert*> originVertList;//ԭʼ����ĵ㼯
	QList<Vert*> vertList;//�㼯��
	QList<VertNorm*> vertNormList;//�㷨��������
	QList<Face*> faceList;//�漯��
	QList<FaceNorm*> faceNormList;//�淨��������

	float center[3];
};

//�����淨�������ٶ���ʱ��
void _glFacetNormals(Model* model);
//����ָ����ģʽ��Ⱦģ��
void _glDraw(Model *model, size_t mode);

//��ʼ������ģ�ͣ�����������Ľڵ㣬����������������
void  _glReconstructFaceIndexes(Model* model);