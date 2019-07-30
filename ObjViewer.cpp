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

	dockWidget = new QDockWidget(QString::fromLocal8Bit("Ŀ¼"), this);
	dockWidget->setWidget(glObjList);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	//����
	glCanvas = new Canvas(this);
	setCentralWidget(glCanvas);

	//4������
	importObjAction = new QAction(QString::fromLocal8Bit("ѡ��·��"), this);
	importObjAction->setShortcutContext(Qt::ApplicationShortcut);
	importObjAction->setShortcut(Qt::CTRL + Qt::Key_O);//��ݼ�
	connect(importObjAction, &QAction::triggered, this, &ObjViewer::OpenOBJFile);

	restoreMatrixAction = new QAction(QString::fromLocal8Bit("��ԭ��ͼ"), this);
	connect(restoreMatrixAction, &QAction::triggered, this, &ObjViewer::RestoreView);

	//���������϶���
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(importObjAction);
	fileToolBar->addAction(restoreMatrixAction);

	delayTimeText = new QLineEdit(this);
	delayTimeText->setClearButtonEnabled(true);
	delayTimeText->setMaximumWidth(250);
	delayTimeText->setPlaceholderText(QString::fromLocal8Bit("���ż��ʱ��(s)"));
	checkedBox = new QCheckBox(QString::fromLocal8Bit("�Զ�����"));
	connect(checkedBox, SIGNAL(stateChanged(int)), this, SLOT(autoPlayChecked(int)));
	connect(checkedBox, SIGNAL(stateChanged(int)), this, SLOT(AutoPlay()));
	fileToolBar->addWidget(delayTimeText);
	fileToolBar->addWidget(checkedBox);

	//�˵�
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
	//todo���Ҹ��ط���ʾ·��

	QDir dir(Dir);
	QStringList files;
	QFileInfoList fileList = dir.entryInfoList(files, QDir::Files); //��ȡĿ¼�µ��ļ�

	//�г�Ŀ¼�µ��ļ�
	for (int i = 0; i < fileList.count(); i++){
		QFileInfo info = fileList.at(i);

		QString filename = info.fileName();

		if (info.suffix() == "obj"){ //ֻ��ʾobj�ļ�		
			QListWidgetItem *item = new QListWidgetItem(filename);
			glObjList->addItem(item);
		}
	}
	//������ʾ��������Ŀ¼�µ�obj

	//QFileInfo fileInfo(QString::fromLocal8Bit("C:/Users/Niu/Documents/ʹ��mitsuba��Ⱦ/OBJ/fallingtosphere_anticlockwise/lr/obj"));//��ʼĿ¼
	//QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��OBJ"), fileInfo.absolutePath(), "OBJ Files (*.obj)");	//ѡ�񵥸�obj
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
	glMultMatrixd(glCanvas->modelMatrix);//������һ֡��modelMatrix
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
		QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�����벥�ż��ʱ��(s)"));
		checkedBox->setCheckState(Qt::Unchecked);
		return;
	}

	if (glObjList->count() == 0){
		QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("û�п��Բ��ŵ�ͼ��"));
		checkedBox->setCheckState(Qt::Unchecked);
		return;
	}

	if (state==Qt::Checked){ //������ʱ��
		delayTime = delayTimeText->text();
		mtime->start(delayTime.toInt() * 1000);//������ʱ�������ò���ʱ����
		autoFlag = true;
	}
	else if (state==Qt::Unchecked){//ֹͣ��ʱ��
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

	//�ж��Զ����ŵ�ʱ���Ƿ񲥷ŵ������һ��ͼƬ���������ֹͣ�Զ�����
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
	//�Ժ�Ҫȷ����ͼ��ı������
	//α���룺
	//if(��Ҫ���棬��ʾ��ʾ��Ϣ)
	/*
		IF(true)
		save()
		ELSE
		quit()
		*/

	event->accept();
}