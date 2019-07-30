#include <QFileInfo>
#include <qimage.h>

#include <gl/glut.h>

#include "Canvas.h"
#include "ObjViewer.h"
#include "transform.h"

Canvas::Canvas(QWidget *parent) 
	:QGLWidget(parent),
	pModel(NULL),leftButtonPress(false),middleButtonPress(false),
	oldX(0),oldY(0),
	mainWindow(qobject_cast<ObjViewer *>(parent)){
	for (int i = 0; i < 16; i++){
		if (i % 5 == 0){
			modelMatrix[i] = 1;
		}
		else{
			modelMatrix[i] = 0;
		}
	}
	showFullScreen();
}

void Canvas::initializeGL(){
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(128 / (GLclampd)255, 128 / (GLclampd)255, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)this->width() / (float)this->height(), 0.01f, 100.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_COLOR_MATERIAL);//设置了才显示面颜色
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);//不剔除内部显示
	// Setup other misc features. 混合要素
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);//启用发走样之后可以设置小数线宽

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.0f);
	//
	glShadeModel(GL_FLAT);//颜色模式，flat使用计算法向量，smooth使用自带法向量

	// Setup lighting model.//光照模型
	GLfloat light_model_ambient[] = { 0.125f, 0.125f, 0.125f, 1.0f };
	GLfloat light0_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light0_direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
}

void Canvas::updateModelView(void){
	for (int i = 0; i < 16; i++){
		modelMatrix[i] = pModelViewMatrix[i];
	}
}

void Canvas::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -5.0);//实际调用了glMultMatrixd
	glMultMatrixd(pModelViewMatrix);
	if (pModel){
		_glDraw(pModel, _GL_FLAT);
	}
}

void Canvas::resizeGL(int w, int h){
	if (0 == h)
		h = 1;
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Canvas::mousePressEvent(QMouseEvent *e){
	if (pModel){
		if (e->button() == Qt::LeftButton){
			leftButtonPress = true;
			oldX = e->x();
			oldY = e->y();
		}
		//中键移动整个图形
		else if (e->button() == Qt::MiddleButton){
			middleButtonPress = true;
			oldMiddleX = e->x();
			oldMiddleY = e->y();
		}
		update();
	}
}

void Canvas::mouseReleaseEvent(QMouseEvent *e){
	if (pModel){
		if (e->button() == Qt::LeftButton){
			leftButtonPress = false;
		}
		else if (e->button() == Qt::MiddleButton){
			middleButtonPress = false;
		}
		update();
	}
}

void Canvas::mouseMoveEvent(QMouseEvent *e){
	if (pModel){
		if (leftButtonPress){
			float buffer0 = 3.0f;
			float buffer1 = 1.0f;

			//加buffer的原因？
			float fOldX = buffer0*oldX / (float)this->width() - buffer1;
			float fOldY = -buffer0*oldY / (float)this->height() + buffer1;
			float fNewX = buffer0*e->x() / (float)this->width() - buffer1;
			float fNewY = -buffer0*e->y() / (float)this->height() + buffer1;


			//fOldX = this->devicePixelRatio()*oldX;
			//fOldY = this->devicePixelRatio()*oldY;
			//fNewX = this->devicePixelRatio()*e->x();
			//fNewY = this->devicePixelRatio()*e->y();


			double pMatrix[16];
			trackBall_GL_Matrix(pMatrix, fOldX, fOldY, fNewX, fNewY);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();//加载单位矩阵
			glLoadMatrixd(pMatrix);//将栈顶矩阵变成该旋转矩阵

			glMultMatrixd(pModelViewMatrix);//栈顶矩阵左乘该矩阵，即旋转矩阵左乘当前矩阵
			glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);//获得处理过后的当前矩阵，然后在刷新中调用单位矩阵再次左乘该矩阵
			updateModelView();


			oldX = e->x();
			oldY = e->y();
		}
		else if (middleButtonPress){
			float buffer = 0.005;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();//加载单位矩阵

			float subX = (float)(e->x() - oldMiddleX)*buffer;
			float subY = (float)(e->y() - oldMiddleY)*buffer;
			glTranslatef(subX, -subY, 0.0);

			glMultMatrixd(pModelViewMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
			updateModelView();
			oldMiddleX = e->x();
			oldMiddleY = e->y();
		}
		update();
	}
}

//需要为widget设置setfocus
void Canvas::keyPressEvent(QKeyEvent *e){
	switch (e->key()){
	case Qt::Key_F1://放大
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixd(pModelViewMatrix);
		glScaled(1.05, 1.05, 1.05);
		glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
		updateModelView();
		break;
	case Qt::Key_F2:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixd(pModelViewMatrix);
		glScaled(0.95, 0.95, 0.95);
		glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
		updateModelView();
		break;
	default:
		break;
	}
	update();
}

void Canvas::keyReleaseEvent(QKeyEvent *e){
	e->ignore();
	setFocus();
}

void Canvas::wheelEvent(QWheelEvent *e){
	const float WHEEL_STEP = 1200.0;
	int delta = e->delta();
	float notch = delta / WHEEL_STEP;
	GLdouble scale = (GLdouble)(1 - notch);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixd(pModelViewMatrix);
	glScaled(scale, scale, scale);
	glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
	updateModelView();
	update();
}

//恢复初始视图
void Canvas::ReviewInit(){
	//恢复初始视图
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glLoadMatrixd(pModelViewMatrix);
	glScaled(1.05, 1.05, 1.05);//为何有还原视图的效果
	glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
	updateModelView();
	update();
}
