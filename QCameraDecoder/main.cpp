#include <QtGui/QApplication>
#include "camerawidget.h"

int main(int argc, char *argv[])
{
    // NOTE: set this value before creating MyApplication instance
    // http://doc.trolltech.com/qapplication.html#setGraphicsSystem
    QApplication::setGraphicsSystem("opengl"); // NOTE: Seems that raster have to be enabled with Nokia N8

    QApplication a(argc, argv);

    CameraWidget w;
    w.showFullScreen();
    return a.exec();
}
