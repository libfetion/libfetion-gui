#ifndef FXLABLEMOUSE_H
#define FXLABLEMOUSE_H

#include <QLabel>
class QLabelMouse : public QLabel // inherit QLabel
{
    Q_OBJECT
public:
    QLabelMouse(QWidget *parent) : QLabel(parent) { };

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *) { emit clicked(); }
    //void mousePressEvent(QMouseEvent *) { emit clicked(); }
};
#endif


