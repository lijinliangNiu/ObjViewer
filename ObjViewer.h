#pragma once

#include <qmainwindow.h>
#include <qdockwidget.h>
#include <QtWidgets/QListWidget>
#include <QTimer>
#include <QCheckBox>

#include "Canvas.h"
#include "Model.h"
#include "easyMath.h"

class ObjViewer :public QMainWindow{
	Q_OBJECT
public:
	ObjViewer();
	~ObjViewer();

public://action对应方法
	void OpenOBJFile();//打开obj
	void RestoreView();//视角重置
public:
	Model* getObjModel();//获取当前的模型
		
private:
	void closeEvent(QCloseEvent *event);

private:
	QDockWidget *dockWidget;
	Canvas *glCanvas;
	QListWidget *glObjList;
	void drawObj(QString);

	QAction *importObjAction;
	QAction *restoreMatrixAction;

	QMenu *fileMenu;
	QToolBar *fileToolBar;

	QString Dir;//路径

	QLineEdit *delayTimeText;
	QCheckBox *checkedBox;
	QTimer *mtime; //定时器
	QString delayTime; //延时间隔
	bool autoFlag; //判断是否进入的自动播放格式
	
	public slots:
	void itemClicked(QListWidgetItem*);
	void autoPlayChecked(int);
	void AutoPlay();//自动播放
};