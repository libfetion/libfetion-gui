#ifndef FXMY_TAB_WIDGET_H
#define FXMY_TAB_WIDGET_H

#include <QtGui>
#include <QTabWidget>
#include <QTabBar>


class FxMyTabWidget : public QTabWidget 
{
    Q_OBJECT

	public:
		FxMyTabWidget(QWidget *parent);
		QTabBar* myTabBar();
		bool eventFilter(QObject *target, QEvent *event);

	signals:
		void mouseDblClick(int index); 

};

#endif