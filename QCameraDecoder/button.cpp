#include "button.h"
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>

Button::Button(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{
    m_downPixmap = 0;
    m_disabled = false;
}

Button::~Button()
{
}

void Button::disableBtn(bool b)
{
    m_disabled = b;
    if (m_disabled) {
        setPixmap(m_downPixmap);
    } else {
        setPixmap(m_upPixmap);
    }
}

void Button::mousePressEvent(QMouseEvent *event)
{
    if (!m_disabled) {
        event->accept();
        setPixmap(m_downPixmap);
        repaint();
        // Lift button back to up after 300ms
        QTimer::singleShot(300, this, SLOT(backToUp()));
    }
}

void Button::backToUp()
{
    setPixmap(m_upPixmap);
    repaint();
    emit pressed();
}

void Button::setPixmap(const QPixmap& p)
{
    // Set up and down picture for the button
    // Set pixmap
    if (!p.isNull())
        QLabel::setPixmap(p);

    // Make down pixmap if it does not exists
    if (m_downPixmap.isNull()) {
        // Store up pixmap
        m_upPixmap = *pixmap();

        // Create down pixmap
        // Make m_downPixmap as a transparent m_upPixmap
        QPixmap transparent(m_upPixmap.size());
        transparent.fill(Qt::transparent);
        QPainter painter(&transparent);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawPixmap(0, 0, m_upPixmap);
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.fillRect(transparent.rect(), QColor(0, 0, 0, 150));
        painter.end();
        m_downPixmap = transparent;
    }
}
