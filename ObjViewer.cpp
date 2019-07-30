#include <qaction.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

#include <ObjViewer.h>
#include <transform.h>
#include <iostream>

ObjViewer::ObjViewer(){
	glObjList = new QListWidget(this);
	connect(glObjList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemClicked(QListWidgetItem*)));

	dockWidget = new QDockWidget(QString::fromLocal8Bit("目录"), this);
	dockWidget->setWidget(glObjList);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	//画布
	glCanvas = new Canvas(this);
	setCentralWidget(glCanvas);

	//4个动作
	importObjAction = new QAction(QString::fromLocal8Bit("选择路径"), this);
	importObjAction->setShortcutContext(Qt::ApplicationShortcut);
	importObjAction->setShortcut(Qt::CTRL + Qt::Key_O);//快捷键
	connect(importObjAction, &QAction::triggered, this, &ObjViewer::OpenOBJFile);

	restoreMatrixAction = new QAction(QString::fromLocal8Bit("还原视图"), this);
	connect(restoreMatrixAction, &QAction::triggered, this, &ObjViewer::RestoreView);

	//工具条捆上动作
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(importObjAction);
	fileToolBar->addAction(restoreMatrixAction);

	delayTimeText = new QLineEdit(this);
	delayTimeText->setClearButtonEnabled(true);
	delayTimeText->setMaximumWidth(250);
	delayTimeText->setPlaceholderText(QString::fromLocal8Bit("播放间隔时间(s)"));
	checkedBox = new QCheckBox(QString::fromLocal8Bit("自动播放"));
	connect(checkedBox, SIGNAL(stateChanged(int)), this, SLOT(autoPlayChecked(int)));
	connect(checkedBox, SIGNAL(stateChanged(int)), this, SLOT(AutoPlay()));
	fileToolBar->addWidget(delayTimeText);
	fileToolBar->addWidget(checkedBox);

	//菜单
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(importObjAction);

	mtime = new QTimer();
	delayTime = 1;
	connect(mtime, SIGNAL(timeout()), this, SLOT(AutoPlay()));
}

ObjViewer::~ObjViewer(){
	wglMakeCurrent(NULL, NULL);
}

void ObjViewer::RestoreView(){
	glCanvas->ReviewInit();
}

void ObjViewer::OpenOBJFile(){
	Dir = QFileDialog::getExistingDirectory(this);
	//todo：找个地方显示路径

	QDir dir(Dir);
	QStringList files;
	QFileInfoList fileList = dir.entryInfoList(files, QDir::Files); //获取目录下的文件

	//列出目录下的文件
	for (int i = 0; i < fileList.count(); i++){
		QFileInfo info = fileList.at(i);

		QString filename = info.fileName();

		if (info.suffix() == "obj"){ //只显示obj文件		
			QListWidgetItem *item = new QListWidgetItem(filename);
			glObjList->addItem(item);
		}
	}
	//以上显示出了整个目录下的obj

	//QFileInfo fileInfo(QString::fromLocal8Bit("C:/Users/Niu/Documents/使用mitsuba渲染/OBJ/fallingtosphere_anticlockwise/lr/obj"));//初始目录
	//QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开OBJ"), fileInfo.absolutePath(), "OBJ Files (*.obj)");	//选择单个obj
}
void ObjViewer::drawObj(QString objPath){
	if (glCanvas->pModel){
		delete glCanvas->pModel;
		glCanvas->pModel = NULL;
	}
	glCanvas->pModel = new Model(objPath);

	_glReconstructFaceIndexes(glCanvas->pModel);

	_glFacetNormals(glCanvas->pModel);

	glCanvas->pModel->center[0] = 0.0;
	glCanvas->pModel->center[1] = 0.0;
	glCanvas->pModel->center[2] = 0.0;

	// Init the modelview matrix as an identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(glCanvas->modelMatrix);//乘上上一帧的modelMatrix
	glGetDoublev(GL_MODELVIEW_MATRIX, glCanvas->pModelViewMatrix);

	glCanvas->setFocus();
	glCanvas->update();
}

void ObjViewer::itemClicked(QListWidgetItem* item){
	QString objPath = NULL;
	objPath.append(Dir + "/");
	objPath.append(item->text());

	drawObj(objPath);
}

void ObjViewer::autoPlayChecked(int state){
	if (delayTimeText->text().isEmpty()){
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入播放间隔时间(s)"));
		checkedBox->setCheckState(Qt::Unchecked);
		return;
	}

	if (glObjList->count() == 0){
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("没有可以播放的图形"));
		checkedBox->setCheckState(Qt::Unchecked);
		return;
	}

	if (state==Qt::Checked){ //启动定时器
		delayTime = delayTimeText->text();
		mtime->start(delayTime.toInt() * 1000);//启动定时器并设置播放时间间隔
		autoFlag = true;
	}
	else if (state==Qt::Unchecked){//停止定时器
		mtime->stop();
		delayTime.clear();
		autoFlag = false;
	}
}

void ObjViewer::AutoPlay(){
	if (!glObjList->currentItem()){
		glObjList->setCurrentRow(0);
	}

	QString objPath = NULL;
	objPath.append(Dir + "/");
	objPath.append(glObjList->currentItem()->text());

	drawObj(objPath);

	//判断自动播放的时候是否播放到了最后一张图片，如果是则停止自动播放
	if (glObjList->currentRow() == 	glObjList->count() - 1){
		mtime->stop();
		autoFlag = false;
		checkedBox->setCheckState(Qt::Unchecked);
	}

	if (autoFlag){
		glObjList->setCurrentRow(glObjList->currentRow() + 1);
	}
}

void ObjViewer::closeEvent(QCloseEvent* event)
{
	//以后还要确定贴图后的保存操作
	//伪代码：
	//if(需要保存，显示提示信息)
	/*
		IF(true)
		save()
		ELSE
		quit()
		*/

	event->accept();
}