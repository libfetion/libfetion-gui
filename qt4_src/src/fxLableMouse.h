#ifndef FXLABLEMOUSE_H
#define FXLABLEMOUSE_H

#include <QLabel>
#include "fxutil.h"

class QLabelMouse : public QLabel // inherit QLabel
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    QLabelMouse(QWidget *parent) : QLabel(parent) { };

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *) { emit clicked(); }
    //void mousePressEvent(QMouseEvent *) { emit clicked(); }
};
#endif


