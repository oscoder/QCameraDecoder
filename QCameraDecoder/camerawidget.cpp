#include "camerawidget.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPaintEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QStackedWidget>
#include <QtGui/QDesktopServices>

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "button.h"
#include "cameravideosurface.h"

CameraWidget::CameraWidget(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("QCameraDecoder");

    setAutoFillBackground(false);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, Qt::black);
    setPalette(palette);

    QWidget* mainWidget = new QWidget(this);
    mainWidget->setPalette(palette);

    QHBoxLayout* hboxl = new QHBoxLayout;
    hboxl->setSpacing(0);
    hboxl->setMargin(0);

    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->setPalette(palette);

    m_videoWidget = new QWidget();
    m_videoWidget->setPalette(palette);
    m_stackedWidget->addWidget(m_videoWidget);

    QWidget* secondWidget = new QWidget(this);
    secondWidget->setPalette(palette);
    m_stackedWidget->addWidget(secondWidget);
    m_stackedWidget->setCurrentIndex(0);

    hboxl->addWidget(m_stackedWidget);

    QSize iconSize(80, 80);
    QVBoxLayout* vboxl = new QVBoxLayout;
    vboxl->setSpacing(0);
    vboxl->setMargin(0);

    m_exitBtn = new Button(this);
    QObject::connect(m_exitBtn, SIGNAL(pressed()),qApp , SLOT(quit()));
    QPixmap p = QPixmap(":/icons/exit.png");
    m_exitBtn->setPixmap(p.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    vboxl->addWidget(m_exitBtn);
    vboxl->setAlignment(m_exitBtn, Qt::AlignHCenter | Qt::AlignTop);

    m_cameraBtn = new Button(this);
    QObject::connect(m_cameraBtn, SIGNAL(pressed()), this, SLOT(searchAndLock()));
    p = QPixmap(":/icons/camera.png");
    m_cameraBtn->setPixmap(p.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    vboxl->addWidget(m_cameraBtn);
    vboxl->setAlignment(m_cameraBtn, Qt::AlignCenter);

    hboxl->addLayout(vboxl);
    mainWidget->setLayout(hboxl);

    setCentralWidget(mainWidget);


    m_videoSurface = 0;
    m_focusing = false;
    m_pictureCaptured = false;
    m_showViewFinder = false;

    QTimer::singleShot(1000,this,SLOT(enableCamera()));
}

CameraWidget::~CameraWidget()
{
    delete m_videoWidget;
    delete m_stackedWidget;
    delete m_exitBtn;
    delete m_cameraBtn;
}

void CameraWidget::searchAndLock()
{
    m_focusing = false;
    if (m_pictureCaptured) {
        // Starting view finder again
        m_pictureCaptured = false;
        m_stackedWidget->setCurrentIndex(0);
        if (m_videoSurface) {
            m_showViewFinder = true;
        }
    } else {
        // Search and lock picture (=focus)
        if (m_camera->supportedLocks() & QCamera::LockFocus) {
            m_focusing = true;
            m_camera->searchAndLock(QCamera::LockFocus);
        } else {
        // No focus functionality, take picture right away
            captureImage();
        }
    }
}

void CameraWidget::enableCamera()
{
    m_camera = new QCamera();
    m_camera->setCaptureMode(QCamera::CaptureStillImage);
    connect(m_camera, SIGNAL(error(QCamera::Error)),
            this, SLOT(error(QCamera::Error)));
    connect(m_camera, SIGNAL(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)),
            this, SLOT(lockStatusChanged(QCamera::LockStatus,QCamera::LockChangeReason)));
    connect(m_camera, SIGNAL(lockFailed()),
            this, SLOT(lockFailed()));
    connect(m_camera, SIGNAL(locked()), this, SLOT(locked()));
    QMediaService *ms = m_camera->service();
    QVideoRendererControl *vrc = ms->requestControl<QVideoRendererControl*>();
    m_videoSurface = new CameraVideoSurface(this,this,this);
    vrc->setSurface(m_videoSurface);

    m_cameraFocus = m_camera->focus();
    setCameraFocus();
    m_cameraFocus->setFocusMode(m_focusModel);

    m_stillImageCapture = new QCameraImageCapture(m_camera);
    connect(m_stillImageCapture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(imageCaptured(int, QImage)));

    if (m_camera->state() == QCamera::ActiveState) {
        m_camera->stop();
    }
    m_videoWidget->show();
    m_camera->start();
    m_showViewFinder = true;
}

void CameraWidget::setCameraFocus ()
{
    if (!m_cameraFocus)
        return;
    if (m_cameraFocus->isFocusModeSupported(QCameraFocus::MacroFocus)) {
        m_focusModel = QCameraFocus::MacroFocus;
        return;
    }
    if (m_cameraFocus->isFocusModeSupported(QCameraFocus::HyperfocalFocus)) {
        m_focusModel = QCameraFocus::HyperfocalFocus;
        return;
    }
    if (m_cameraFocus->isFocusModeSupported(QCameraFocus::InfinityFocus)) {
        m_focusModel = QCameraFocus::InfinityFocus;
        return;
    }
    if (m_cameraFocus->isFocusModeSupported(QCameraFocus::ContinuousFocus)) {
        m_focusModel = QCameraFocus::ContinuousFocus;
        return;
    }
    if (m_cameraFocus->isFocusModeSupported(QCameraFocus::AutoFocus)) {
        m_focusModel = QCameraFocus::AutoFocus;
        return;
    } else {
        m_focusModel = QCameraFocus::ManualFocus;
        return;
    }
}

void CameraWidget::error(QCamera::Error e)
{
    switch (e) {
    case QCamera::NoError:
        break;
    case QCamera::CameraError:
        QMessageBox::warning(this, "QCameraExample", "An error has occurred");
        break;
    case QCamera::InvalidRequestError:
        QMessageBox::warning(this, "QCameraExample", "System resource doesn't support requested functionality.");
        break;
    case QCamera::ServiceMissingError:
        QMessageBox::warning(this, "QCameraExample", "No camera service available.");
        break;
    case QCamera::NotSupportedFeatureError :
        QMessageBox::warning(this, "QCameraExample", "The feature is not supported.");
        break;
    };
}

void CameraWidget::lockFailed()
{
    qDebug() << "[error]: lockFailed!";
}

void CameraWidget::lockStatusChanged(QCamera::LockStatus status,
                                     QCamera::LockChangeReason reason)
{
    if (status == QCamera::Locked) {
        if (reason == QCamera::LockAcquired) {
            m_focusing = false;
            captureImage();
            m_camera->unlock();
        }
    } else if (status == QCamera::Unlocked && m_focusing) {
    }
}

void CameraWidget::locked()
{
    qDebug() << "[information] : locked";
}

void CameraWidget::updateVideo()
{
    if (m_showViewFinder)
        repaint();
}

void CameraWidget::captureImage()
{
    if (m_pictureCaptured) {
        m_pictureCaptured = false;
        m_stackedWidget->setCurrentIndex(0);
        m_showViewFinder = true;
    } else {
        QString path(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));
        QDir dir(path);

        // Get next filename
        QStringList files = dir.entryList(QStringList() << "camera_*.jpg");
        int lastImage = 0;
        Q_FOREACH (const QString &fileName, files) {
                int imgNumber = fileName.mid(7, fileName.size() - 11).toInt();
                lastImage = qMax(lastImage, imgNumber);
        }

        // Capture image
        if (m_stillImageCapture->isReadyForCapture()) {
            m_capturedImageName = QString("camera_%1.jpg").arg(lastImage + 1);
            m_stillImageCapture->capture(m_capturedImageName);
        }
    }
}

void CameraWidget::imageCaptured(int id, const QImage &preview)
{
    m_showViewFinder = false;
    m_focusing = false;

    // Image captured, show it to the user
    m_stackedWidget->setCurrentIndex(1);

    // Get picture location
    QString path(QDesktopServices::storageLocation(QDesktopServices::PicturesLocation));
    m_capturedImageName.prepend(path + "/");

    m_capturedImage = preview;

    // Set suitable size to the image
    QSize s = m_videoWidget->size();
    s = s - QSize(20, 20);
    m_capturedImage = m_capturedImage.scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_pictureCaptured = true;
    update();
}

void CameraWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect r = this->rect();

    QFont font = painter.font();
    font.setPixelSize(20);
    painter.setFont(font);
    painter.setPen(Qt::white);

    if (m_showViewFinder && m_videoSurface && m_videoSurface->isActive()) {
        m_videoSurface->paint(&painter);

    } else {
        // Draw black
        painter.fillRect(event->rect(), palette().background());
        if (m_pictureCaptured) {
            // Paint captured image
            QPoint centerPic((qAbs(r.size().width() - m_capturedImage.size().width())) / 2,
                             (qAbs(r.size().height() - m_capturedImage.size().height())) / 2);

            painter.drawImage(centerPic, m_capturedImage);

            // Paint filename
            painter.drawText(r, Qt::AlignBottom | Qt::AlignCenter, m_capturedImageName);
        }
    }
}
