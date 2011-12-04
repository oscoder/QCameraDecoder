#ifndef CAMERWIDGET_H
#define CAMERWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QMainWindow>

#include <QCamera>
#include <QCameraImageCapture>

//QTM_USE_NAMESPACE

#include <QMediaService>
#include <QVideoRendererControl>

#include "videoif.h"

QT_BEGIN_NAMESPACE
class QStackedWidget;
class QImage;
class QString;
class Button;
class CameraVideoSurface;
//class QSystemScreenSaver;
QT_END_NAMESPACE

class CameraWidget : public QMainWindow, public VideoIF
{
    Q_OBJECT
public:
    CameraWidget(QWidget *parent = 0);
    ~CameraWidget();
protected:
    void paintEvent (QPaintEvent *);
protected slots:
    void enableCamera();
    void searchAndLock();
    void error(QCamera::Error);
    void lockFailed();
    void lockStatusChanged(QCamera::LockStatus status,
                           QCamera::LockChangeReason reason);
    void locked();
    void updateVideo();
    void imageCaptured(int, const QImage &);
private:
    void captureImage();
private:

    QWidget *m_videoWidget;
    QStackedWidget *m_stackedWidget;
    Button *m_exitBtn;
    Button *m_cameraBtn;
    QImage m_capturedImage;
    QString m_capturedImageName;
    bool m_focusing;
    bool m_pictureCaptured;
    bool m_showViewFinder;
    QCamera *m_camera;
    QCameraImageCapture *m_stillImageCapture;
    CameraVideoSurface *m_videoSurface;
//    QSystemScreenSaver *m_systemScreenSaver;
};

#endif // CAMERWIDGET_H
