#pragma once

#include <qstring.h>
#include <QList>

#include <string>
#include <gl/glut.h>

using namespace std;

//渲染模式
#define _GL_FLAT (1<<0)//1

//点
struct Vert{
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
};

//点法向量
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
	int index_Face;//平面法向量的索引

	QList<int> vertIndexList;//面的点集合在model中的索引
	QList<int> vertNormIndexList;
};

class Model{
public:
	Model();
	Model(QString filename);
	~Model();
	//Model(const Model&);

	size_t vertNum;//节点个数
	size_t vertNormNum;//节点向量的个数
	size_t faceNum;//面的个数

	QList<Vert*> originVertList;//原始坐标的点集
	QList<Vert*> vertList;//点集合
	QList<VertNorm*> vertNormList;//点法向量集合
	QList<Face*> faceList;//面集合
	QList<FaceNorm*> faceNormList;//面法向量集合

	float center[3];
};

//构造面法向量，假定逆时针
void _glFacetNormals(Model* model);
//根据指定的模式渲染模型
void _glDraw(Model *model, size_t mode);

//初始化完了模型，重新设置面的节点，纹理，法向量的索引
void  _glReconstructFaceIndexes(Model* model);