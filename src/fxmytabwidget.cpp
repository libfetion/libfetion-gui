
#include "fxmsgwindow.h"
#include "fxmytabwidget.h"
#include "fxshowHistory.h"
#include <QShortcut>
#include <QKeySequence>

FxMyTabWidget::FxMyTabWidget(QWidget *parent)
		: QTabWidget(parent)
{
	myTabBar()->installEventFilter(this);
	this->installEventFilter(this);
}

bool FxMyTabWidget::eventFilter(QObject *target, QEvent *event)
{
	if (target == myTabBar() ) {
		if (event->type() == QEvent::MouseButtonDblClick ) {
			emit  mouseDblClick( currentIndex() ); 
			return true;
		}
	}
	return QObject::eventFilter(target, event);
}

void FxMyTabWidget::closeCurrentTab(){
	closeTab(currentIndex());
}

void FxMyTabWidget::closeTab(int index){
	//@To FIX
	( (FxMsgWindow *)( this->parentWidget()->parentWidget() ) )->closeTabWid(index);
	//removeTab(index);
}

QTabBar * FxMyTabWidget::myTabBar()
{
	return tabBar();
}
