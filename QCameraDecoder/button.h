#ifndef BUTTON_H
#define BUTTON_H

#include <QtGui/QLabel>
#include <QtGui/QPixmap>

class Button: public QLabel
{
Q_OBJECT

public:
    Button(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~Button();

    void mousePressEvent(QMouseEvent *);
    void disableBtn(bool);

public Q_SLOTS:
    void setPixmap(const QPixmap &);
    void backToUp();

    signals:
    void pressed();

private:
    QPixmap m_upPixmap;
    QPixmap m_downPixmap;
    bool    m_disabled;

};

#endif // BUTTON_H
