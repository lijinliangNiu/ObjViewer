#include <QApplication>
#include <qobject.h>

#include "ObjViewer.h"

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	QLocale::setDefault(QLocale::C);//按系统语言本地化
#if QT_VERSION >= 0x050100
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); //高清化，且高版本才有该功能
#endif
	QCoreApplication::setApplicationName("ObjViewer");

	ObjViewer window;
	
	window.setWindowTitle("ObjViewer");
	window.showMaximized();//开启时最大化
	QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));//设置关闭窗口

	return app.exec();
}
