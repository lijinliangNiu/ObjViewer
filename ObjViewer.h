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

public://action��Ӧ����
	void OpenOBJFile();//��obj
	void RestoreView();//�ӽ�����
public:
	Model* getObjModel();//��ȡ��ǰ��ģ��
		
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

	QString Dir;//·��

	QLineEdit *delayTimeText;
	QCheckBox *checkedBox;
	QTimer *mtime; //��ʱ��
	QString delayTime; //��ʱ���
	bool autoFlag; //�ж��Ƿ������Զ����Ÿ�ʽ
	
	public slots:
	void itemClicked(QListWidgetItem*);
	void autoPlayChecked(int);
	void AutoPlay();//�Զ�����
};