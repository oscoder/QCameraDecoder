#include "cameravideosurface.h"

#include "videoif.h"

#include <QtCore/QObject>

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QWidget>

CameraVideoSurface::CameraVideoSurface(QWidget *widget, VideoIF *target, QObject *parent)
    : QAbstractVideoSurface(parent)
{
    m_targetWidget = widget;
    m_target = target;
    m_imageFormat = QImage::Format_Invalid;
}

CameraVideoSurface::~CameraVideoSurface()
{

}

bool CameraVideoSurface::start (const QVideoSurfaceFormat &format)
{
    m_videoFormat = format;
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
    const QSize size = format.frameSize();

    if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
        m_imageFormat = imageFormat;
        QAbstractVideoSurface::start(format);
        return true;
    } else {
        return false;
    }
}

bool CameraVideoSurface::present (const QVideoFrame &frame)
{
    m_frame = frame;
    if (surfaceFormat().pixelFormat() != m_frame.pixelFormat() ||
            surfaceFormat().frameSize() != m_frame.size()) {
        stop();
        return false;
    } else {
        m_frame.map(QAbstractVideoBuffer::ReadOnly);
        frWidth = m_frame.width();
        frHeight = m_frame.height();
        int line = m_frame.bytesPerLine();
        // build QImage from frame
        m_completeImage = QImage(
                    m_frame.bits(),
                    frWidth, frHeight,
                    line,
                    m_frame.imageFormatFromPixelFormat(m_frame.pixelFormat()));
        m_frame.unmap();

        QImage dstImage = scaleImage(m_completeImage);

        m_frame = QVideoFrame(dstImage);

        // enlarge faces
        m_target->updateVideo();
        return true;
    }
}

QImage CameraVideoSurface::scaleImage(const QImage & srcImage)const
{
    QImage dstImage = srcImage.copy((frWidth - (frWidth/3))/2, (frHeight - (frHeight/3))/2,
                                    frWidth/3, frHeight/3);
    if(320 < frWidth || 240 < frHeight){
        if(frWidth > frHeight)
            dstImage = dstImage.scaledToWidth(320, Qt::SmoothTransformation);
        else
            dstImage = srcImage.scaledToHeight(240, Qt::SmoothTransformation);
    }
    return dstImage;
}

void CameraVideoSurface::paint (QPainter *painter)
{
    if (m_frame.map(QAbstractVideoBuffer::ReadOnly)) {
        QImage image(
                m_frame.bits(),
                m_frame.width(),
                m_frame.height(),
                m_frame.bytesPerLine(),
                m_imageFormat);

        QRect r = m_targetWidget->rect();
        QPoint centerPic((qAbs(r.size().width() - image.size().width())) / 2,
                         (qAbs(r.size().height() - image.size().height())) / 2);

        if (!image.isNull()) {
           painter->drawImage(centerPic,image);
        }

        m_frame.unmap();
    }
}

QList<QVideoFrame::PixelFormat> CameraVideoSurface::supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType) const
{
    if (handleType == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>()
                << QVideoFrame::Format_RGB32
                << QVideoFrame::Format_ARGB32
                << QVideoFrame::Format_ARGB32_Premultiplied
                << QVideoFrame::Format_RGB565
                << QVideoFrame::Format_RGB555;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
