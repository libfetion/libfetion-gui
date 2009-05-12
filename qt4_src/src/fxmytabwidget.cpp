
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
/* use QShortcut
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
*/
/*	use QShortcut
		if (keyEvent->key() == Qt::Key_W && 
		(keyEvent->modifiers() == Qt::AltModifier || keyEvent->modifiers() == Qt::ControlModifier )
		) {
				qDebug()<<"begin";
			// @TO FIX
			FxMsgWindow *_tmp=0;
			QWidget *_pre = this;
			while(_tmp == 0 && _pre!=0){
				qDebug()<<"*]";
				_pre = dynamic_cast<QWidget *> (_pre->parent());
				_tmp = dynamic_cast<FxMsgWindow *>(_pre);
			}
			qDebug()<<"\n";
			_tmp->closeTabWid(currentIndex());
			//((FxMsgWindow *)(this->parentWidget()->parentWidget() ))->closeTabWid(currentIndex()); 
			return true;
		}

		if (keyEvent->key() == Qt::Key_Escape)
		{
			AccountTab * Actab = ((AccountTab*)this->widget(currentIndex()));
			if (Actab)
			{
				if (Actab->msgSend->MsgEdit->toPlainText().isEmpty())
				{
					
					// @TO FIX
					( (FxMsgWindow *)( this->parentWidget()->parentWidget() ) )->hide();
					return true;
				}
			}
		}
*/
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
		
	}*/
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
