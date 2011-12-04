#ifndef CAMERAVIDEOSURFACE_H
#define CAMERAVIDEOSURFACE_H

#include <QtGlobal>
#include <QList>

#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QAbstractVideoBuffer>
#include <QAbstractVideoSurface>

QT_BEGIN_NAMESPACE
class QWidget;
class VideoIF;
class QObject;
class QPainter;
class QImage;
QT_END_NAMESPACE

class CameraVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    CameraVideoSurface(QWidget* widget, VideoIF* target, QObject * parent = 0);
    ~CameraVideoSurface();

    bool start(const QVideoSurfaceFormat &format);

    bool present(const QVideoFrame &frame);

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

    void paint(QPainter*);
private:
    QImage scaleImage(const QImage & srcImage)const;
private:
    QWidget* m_targetWidget;
    VideoIF* m_target;
    QVideoFrame m_frame;
    int frWidth;
    int frHeight;
    QImage m_completeImage;
    QImage::Format m_imageFormat;
    QVideoSurfaceFormat m_videoFormat;
};

#endif // CAMERAVIDEOSURFACE_H
