#include <qfile.h>
#include <qdebug.h>

#include <assert.h>

#include "Model.h"
#include "transform.h"
#include "easyMath.h"

using namespace std;

enum{ X, Y, Z, W };//代表0，1，2，3

//根据文件的路径获取到文件夹的路径
QString _glGetDir(QString filePath){
	int index = filePath.lastIndexOf('\\');
	if (index == -1)
		index = filePath.lastIndexOf('/');
	filePath.remove(index + 1, filePath.length() - index - 1);
	return filePath;
}

Model::Model(QString filename){
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug(T_Char2Char("无法打开OBJ文件"));
	}
	
	QStringList list; 

	this->faceNum = 0;
	this->vertNormNum = 0;
	this->vertNum = 0;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		QString str(line);
		if (str.length() < 2)//太短
			continue;
		if (str[0] == 'v'){
			if (str[1] == 't'){//纹理
				continue;
			}			
			else if (str[1] == 'v'){//vv
				continue;
			}
			else if (str[1] == 'n'){//法向量
				str = str.remove(0, 2).trimmed();//去掉vt
				list = str.split(' ');
				VertNorm* vn = new VertNorm();
				vn->NX = list[X].toFloat(); vn->NY = list[Y].toFloat(); vn->NZ = list[Z].toFloat();
				this->vertNormNum++;
				this->vertNormList.push_back(vn);
			}
			else{//节点
				str = str.remove(0, 1).trimmed();
				list = str.split(' ');
				Vert* v = new Vert();
				v->X = list[X].toFloat(); v->Y = list[Y].toFloat(); v->Z = list[Z].toFloat();
				this->vertNum++;
				this->originVertList.push_back(v);
				this->vertList.push_back(v);
			}
		}
		else if (str[0] == 'f'){//面
			str = str.trimmed();
			list = str.split(' ');

			Face* f = new Face();

			int currentValue;
			if (list[1].contains('/')){
				for (int i = 1; i < list.length(); i++){
					QStringList sublist = list[i].split('/');
					currentValue = sublist[X].toInt();
					//要么全为正，要么全为负
					f->vertIndexList.push_back(currentValue);
					if (list[1].split('/').length() == 3){//只有v和vt
						f->vertNormIndexList.push_back(sublist[Z].toInt());
					}
				}
			}
			else{//不包括/，那么只有节点
				for (int i = 1; i < list.length(); i++){
					currentValue = list[i].toInt();
					f->vertIndexList.push_back(currentValue);
				}
			}
			this->faceNum++;
			this->faceList.push_back(f);
		}
	}
}

Model::Model(){}

Model::~Model(){
	vertNum = 0;
	vertNormNum = 0;
	faceNum = 0;

	qDeleteAll(originVertList);
	originVertList.clear();

	qDeleteAll(vertNormList);
	vertNormList.clear();

	qDeleteAll(faceList);
	faceList.clear();

	qDeleteAll(faceNormList);
	faceNormList.clear();

	for (int i = 0; i < 3; i++){
		center[i] = 0;
	}
}

void _glReconstructFaceIndexes(Model* model){
	for (size_t i = 0; i < model->faceNum; i++){
		Face* f = model->faceList[i];
		for (int j = 0; j < f->vertIndexList.length(); j++){
			int currentIndex = f->vertIndexList[j];
			if (currentIndex > 0){
				f->vertIndexList[j] -= 1;
				if (f->vertNormIndexList.size() > j)
					f->vertNormIndexList[j] -= 1;
			}
			else{
				f->vertIndexList[j] += model->vertList.size();
				if (f->vertNormIndexList.size() > j)
					f->vertNormIndexList[j] += model->vertNormList.size();
			}
		}
	}
}

//计算面的法向量
void _glFacetNormals(Model* model){
	FaceNorm *fn;
	float u[3];
	float v[3];

	float cross[3];
	for (int i = 0; i < model->faceList.length(); i++){
		fn = new FaceNorm();
		Vert *p0 = model->vertList.at(model->faceList.at(i)->vertIndexList[0]);
		Vert *p1 = model->vertList.at(model->faceList.at(i)->vertIndexList[1]);

		Vert *pn = model->vertList.at(model->faceList.at(i)->vertIndexList.at(model->faceList.at(i)->vertIndexList.length() - 1));//必须使用最后一点才成功

		u[X] = p1->X - p0->X;
		u[Y] = p1->Y - p0->Y;
		u[Z] = p1->Z - p0->Z;

		v[X] = pn->X - p0->X;
		v[Y] = pn->Y - p0->Y;
		v[Z] = pn->Z - p0->Z;

		vCross(u, v, cross);//计算交叉乘积
		vNormal(cross);//单位化

		model->faceList[i]->index_Face = i;
		fn->NX = cross[0];
		fn->NY = cross[1];
		fn->NZ = cross[2];
		model->faceNormList.push_back(fn);
	}
}
//渲染模型
void _glDraw(Model* model, size_t mode){
	if (mode & _GL_FLAT && model->faceNormList.size() == 0){
		qDebug(T_QString2Char("Flat模式不可用！"));
		mode &= ~_GL_FLAT;
	}

	glPushMatrix();
	glTranslatef(model->center[0], model->center[1], model->center[2]);

	for (size_t i = 0; i < model->faceNum; i++){
		Face *f = model->faceList.at(i);

		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_POLYGON);
		if (mode&_GL_FLAT){//自己建立的面向量
			FaceNorm *fn = model->faceNormList.at(f->index_Face);
			glNormal3f(fn->NX, fn->NY, fn->NZ);
		}

		for (int k = 0; k < f->vertIndexList.size(); k++){
			Vert *p = model->vertList.at(f->vertIndexList.at(k));
			glVertex3f(p->X, p->Y, p->Z);
		}
		glEnd();
	}
	glPopMatrix();
}