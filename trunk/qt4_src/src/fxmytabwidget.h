#ifndef FXMY_TAB_WIDGET_H
#define FXMY_TAB_WIDGET_H

#include <QtGui>
#include <QTabWidget>
#include <QTabBar>

#include "fxutil.h"
class FxMyTabWidget: public QTabWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        FxMyTabWidget(QWidget *parent);
        QTabBar* myTabBar();
        bool eventFilter(QObject *target, QEvent *event);
    public slots:
        void closeCurrentTab();
        void closeTab(int index);
    signals:
        void mouseDblClick(int index);

};

#endif
