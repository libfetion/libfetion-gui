
#include "fxmsgwindow.h"
#include "fxmytabwidget.h"
#include "fxshowHistory.h"

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

	if (event->type() == QEvent::KeyPress ) {
		QKeyEvent *keyEvent = (QKeyEvent *) event;

		if (keyEvent->modifiers() == Qt::AltModifier && 
				keyEvent->key() >= Qt::Key_1 &&
				keyEvent->key() <= Qt::Key_9) {

			int index = keyEvent->key() - Qt::Key_0;
			if (index <= 0 || index > this->count())
				return true;	

			this->setCurrentIndex(index -1); 
			return true;	
		}

		if (keyEvent->key() == Qt::Key_W && 
		(keyEvent->modifiers() == Qt::AltModifier || keyEvent->modifiers() == Qt::ControlModifier )
		) {
			((FxMsgWindow *)(this->parentWidget()))->closeTabWid(currentIndex()); 
			return true;
		}

		if (keyEvent->key() == Qt::Key_Escape)
		{
			AccountTab * Actab = ((AccountTab*)this->widget(currentIndex()));
			if (Actab)
			{
				if (Actab->msgSend->MsgEdit->toPlainText().isEmpty())
				{
					((FxMsgWindow *)(this->parentWidget()))->hide();
					return true;
				}
			}
		}
		/*
		if (keyEvent->key() == Qt::Key_H && 
				(keyEvent->modifiers() == Qt::AltModifier || keyEvent->modifiers() == Qt::ControlModifier )) {
			printf("fxmytabwinget will show history...\n");
			Fetion_Account *account = ((AccountTab*)this->widget(currentIndex()))->m_account;
			if (account)
			{
				FxShowHistory * histroy = new FxShowHistory (account->id, this);
				histroy->show();
			}
			return true;
		}
		*/
	}
	return QObject::eventFilter(target, event);
}

QTabBar * FxMyTabWidget::myTabBar()
{
	return tabBar();
}
