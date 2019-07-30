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
	void ReviewInit();//恢复初始状态
	void updateModelView();//保留更改后的modelmatrix
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);

	void initializeGL();
	void resizeGL(int w, int h); //会自动刷新屏幕
	void paintGL();
	//变量
public:
	Model* pModel;

	double modelMatrix[16];//储存上一帧的视图
	double pModelViewMatrix[16];//模型视图
	double pProjectMatrix[16];//投影视图
private:
	ObjViewer *mainWindow;
	bool leftButtonPress;//当前左键按下
	bool middleButtonPress;
	int oldX;
	int oldY;

	int oldMiddleX;
	int oldMiddleY;
};