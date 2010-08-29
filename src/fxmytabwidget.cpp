#include <QShortcut>
#include <QKeySequence>

#include "fxmsgwindow.h"
#include "fxmytabwidget.h"
#include "fxshowHistory.h"

FxMyTabWidget::FxMyTabWidget(QWidget *parent): QTabWidget(parent)
{
    FX_FUNCTION
    myTabBar()->installEventFilter(this);
    this->installEventFilter(this);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool FxMyTabWidget::eventFilter(QObject *target, QEvent *event)
{
    FX_FUNCTION
    if (target == myTabBar())
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            emit mouseDblClick(currentIndex());
            return true;
        }
    }
    return QObject::eventFilter(target, event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMyTabWidget::closeCurrentTab()
{
    FX_FUNCTION
    closeTab(currentIndex());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMyTabWidget::closeTab(int index)
{
    FX_FUNCTION
    ((FxMsgWindow*)(this->parentWidget()->parentWidget()))->closeTabWid(index);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
QTabBar *FxMyTabWidget::myTabBar()
{
    FX_FUNCTION
    return tabBar();
}
