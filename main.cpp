#include <QApplication>
#include <qobject.h>

#include "ObjViewer.h"

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	QLocale::setDefault(QLocale::C);//��ϵͳ���Ա��ػ�
#if QT_VERSION >= 0x050100
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //���廯���Ҹ߰汾���иù���
#endif
	QCoreApplication::setApplicationName("ObjViewer");

	ObjViewer window;
	
	window.setWindowTitle("ObjViewer");
	window.showMaximized();//����ʱ���
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));//���ùرմ���

	return app.exec();
}
