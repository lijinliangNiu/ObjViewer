#pragma once

#include <qopenglwidget.h>
#include <QtOpenGL/qgl.h>
#include <QKeyEvent>

#include <process.h>

#include "trackball.h"
#include "Model.h"

class ObjViewer;
class Canvas :public QGLWidget{//public QOpenGLWidget
	Q_OBJECT
public:
	Canvas(QWidget *parent = 0);
	void ReviewInit();//�ָ���ʼ״̬
	void updateModelView();//�������ĺ��modelmatrix
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);

	void initializeGL();
	void resizeGL(int w, int h); //���Զ�ˢ����Ļ
	void paintGL();
	//����
public:
	Model* pModel;

	double modelMatrix[16];//������һ֡����ͼ
	double pModelViewMatrix[16];//ģ����ͼ
	double pProjectMatrix[16];//ͶӰ��ͼ
private:
	ObjViewer *mainWindow;
	bool leftButtonPress;//��ǰ�������
	bool middleButtonPress;
	int oldX;
	int oldY;

	int oldMiddleX;
	int oldMiddleY;
};