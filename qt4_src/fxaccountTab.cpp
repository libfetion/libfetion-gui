/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QLabel>
#include <QtGui>

#include "fxaccountTab.h"
#include "fxmainwindow.h"
#include "fxshowHistory.h"

AccountTab::AccountTab(qlonglong id, FxMyTabWidget *parent, bool awaySendSms)
    : QWidget(parent)
	, account_id(id)
	, isAwaySendSMS(awaySendSms)
	, tabWidget(parent)
	, flick_flag(FALSE)
	, mainWind(NULL)
{
	setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);

	msgSend = this;
	msgSend->MsgEdit->installEventFilter(this);
	msgSend->MsgBrowser->setText("");

	init();
}

AccountTab::~AccountTab()
{
	fx_end_dialog (account_id);
}

void AccountTab::setSendModle(bool isSMS)
{
	isAwaySendSMS = isSMS;

	if (account_id == SYSTEM_ID)
	{
		QString status = tr("view the system message");
		msgSend->Ac_Status->setText(status); 
		return ;
	}

	if (isSendToSelf)
   	{
		QString status = tr("send the message to your self");
		msgSend->Ac_Status->setText (status); 
		isAwaySendSMS = true;

		changeTableInputNM();
		return;
	}

	if(!m_account)
		return;

	if (!fx_is_pc_user_by_account (m_account)) {
		QString status = account_name + tr("is mobile user, your message just send to his mobile");
		msgSend->Ac_Status->setText (status); 

		isAwaySendSMS = true;
		changeTableInputNM();
		return;
	}

	QString status;
	if(isAwaySendSMS) {
		
		if(fx_get_refuse_sms_day(m_account) > 0)
			status =account_name + tr(" are offline, can't receive you sms immediately, your msg will saved and send later");
		else
			status =tr("your message will send to") + account_name + tr(" 's mobile");

		msgSend->Ac_Status->setText(status); 
	} else {

		if(fx_is_on_line_by_account (m_account)) {
			status = account_name + tr("are online, you message will send to his PC");
			msgSend->Ac_Status->setText(status); 
		} else {
			if(fx_get_refuse_sms_day(m_account) > 0)
				status =account_name + tr(" are offline, can't receive you sms immediately, your msg will saved and send later");
			else
				status = account_name + tr("are offline, your message will send to his mobile"); 
			msgSend->Ac_Status->setText(status); 
		} //end of !fx_is_on_line_by_account (m_account)) 
	} //end of !isAwaySendSMS

	changeTableInputNM();
}

void AccountTab::ShowHistroy()
{
	if (!m_account)
		return;
	FxShowHistory * histroy = new FxShowHistory (m_account->id, mainWind);
	histroy->show();
}


void AccountTab::ShowFaces()
{
	((FxMsgWindow *)(tabWidget->parentWidget()))->showFaces();
}

void AccountTab::changeSendModle()
{
	isAwaySendSMS = !isAwaySendSMS;
	setSendModle(isAwaySendSMS);
}

bool AccountTab::eventFilter(QObject *target, QEvent *event)
{
		if (event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = (QKeyEvent *) event;
			if (keyEvent->key() == Qt::Key_Return)
			{
				if ( (Settings::instance().isEnterSend() && keyEvent->modifiers()!= Qt::ControlModifier)
					|| (!Settings::instance().isEnterSend() && keyEvent->modifiers() == Qt::ControlModifier) 
				   ) 
				{
					SendMsg();
					return true;
				} 
			}

			if (keyEvent->modifiers() == Qt::AltModifier && 
				   	keyEvent->key() >= Qt::Key_1 &&
				   	keyEvent->key() <= Qt::Key_9) {
				handle_alt_num(keyEvent);
				return true;	
			}

			if (keyEvent->key() == Qt::Key_W && 
		(keyEvent->modifiers() == Qt::AltModifier || keyEvent->modifiers() == Qt::ControlModifier )) 
			{
				((FxMsgWindow *)(tabWidget->parentWidget()))->closeTabWid(tabWidget->currentIndex()); 
				return true;
			}

			if (keyEvent->key() == Qt::Key_Escape)
			{
				if (msgSend->MsgEdit->toPlainText().isEmpty())
				{
					((FxMsgWindow *)(tabWidget->parentWidget()))->hide();
					return true;
				}
			}
				
			/*
			if (keyEvent->key() == Qt::Key_H && 
					(keyEvent->modifiers() == Qt::AltModifier || keyEvent->modifiers() == Qt::ControlModifier )) {
				FxShowHistory * histroy = new FxShowHistory (m_account->id, mainWind);
				histroy->show();
				return true;
			}
			*/
        }
    return QObject::eventFilter(target, event);
}

void AccountTab::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->key() == Qt::Key_Return)
	{
		if ((Settings::instance().isEnterSend() && keyEvent->modifiers()!= Qt::ControlModifier)
			|| (!Settings::instance().isEnterSend() && keyEvent->modifiers() == Qt::ControlModifier) ) 
		{
			SendMsg();
			return;
		} 
	}

	QWidget::keyPressEvent(keyEvent);
}

void AccountTab::handle_alt_num( QKeyEvent *keyEvent)
{
	int index = keyEvent->key() - Qt::Key_0;
	if (index <= 0 || index > tabWidget->count())
		return;
	
	tabWidget->setCurrentIndex(index -1); 
}

void AccountTab::SendMsg()
{
	QString msg = msgSend->MsgEdit->toPlainText();
	if(msg.isEmpty())
		return;

	/*
	char *msgcontent = msg.toUtf8().data();
	int msg_len = strlen(msgcontent);
*/
	//send message to the account...
	bool sendFlag = false;
#if 0	
	if(isAwaySendSMS || !fx_is_pc_user_by_id(account_id)) 
		sendFlag = fs_send_sms( account_id, msg.toUtf8().data());   
	else 
		sendFlag = fs_dialog_send( account_id, msg.toUtf8().data()); 
#else

	if(isSendToSelf) {
		sendFlag = fx_send_sms_to_self( msg.toUtf8().data(), NULL, NULL); 
	}
	else {
	if (isAwaySendSMS || !fx_is_pc_user_by_id(account_id)) // fixme: offline usr can send offline dialog
		sendFlag = fx_send_sms( account_id, msg.toUtf8().data(), NULL, NULL);   
	else 
	{
		//note: just dialog_send need replace to html mark.  
		msg.replace(QString("<"), QString("&lt;"));
		msg.replace(QString(">"), QString("&gt;"));
		sendFlag = fx_dialog_send( account_id, msg.toUtf8().data(), NULL, NULL); 
	}
	}
#endif
	QString show_msg;

	QString head;
	if(sendFlag) 
		head = "<b style=\"color:rgb(0,0,255);\">"+tr("Me:(")+ 
			QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
			QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
			")</b><br>";
	else 
		head = "<b style=\"color:red;\">"+tr("send fail:(")+ 
			QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
			QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
		   	")</b><br>";

	msg.replace(QString("<"), QString("&lt;"));
	msg.replace(QString(">"), QString("&gt;"));
	msg.replace(QString("\n"), QString("<br>"));
	msg = fxgui_to_faces(msg);

	QString str = head+ msg;
	show_msg = show_msg.fromUtf8(str.toUtf8().data());


	//show the send reslut to the browser...
	//	msgSend->MsgBrowser->setText(msgSend->MsgBrowser->toPlainText() + show_msg);
	msgSend->MsgBrowser->append(show_msg);

	saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10), account_id, show_msg.toUtf8().data(), NULL);
	//clean the send edit
	msgSend->MsgEdit->setText("");
}

void AccountTab::startFlickerTab()
{
	if (flickTimer.isActive())
		return;

	QApplication::alert(tabWidget->parentWidget());
	flickTimer.start(400);
	if(mainWind)
		mainWind->addNewMsgCount();
}

void AccountTab::endFlickerTab()
{
	if (flickTimer.isActive())
	{
		if(mainWind)
			mainWind->subNewMsgCount();
		flickTimer.stop();
	}
	tabWidget->setTabIcon(tabWidget->indexOf(this), QIcon()); 
}

void AccountTab::flickerTab()
{
	if (tabWidget->currentWidget() == this &&
			tabWidget->parentWidget()->isVisible() )
	{
		/*
		   if (!tabWidget->parentWidget()->isActiveWindow())
		   QApplication::alert(tabWidget->parentWidget());
		   */
		endFlickerTab();
		return;
	}

	tabWidget->setTabIcon(tabWidget->indexOf(this), getFlickIcon(flick_flag)); 
	flick_flag = !flick_flag;
}

void AccountTab::resizeEvent (QResizeEvent * event) 
{
	QWidget::resizeEvent (event);
//	tabWidget->resize(event->size());
}

void AccountTab::changeTableInputNM()
{
	if (isAwaySendSMS || !fx_is_pc_user_by_account (m_account) 
			|| !fx_is_on_line_by_account (m_account) )
	{
		QString msg = MsgEdit->toPlainText();
		if (msg.size() > MAXSMSLENGTH)
		{
			msg = msg.left(MAXSMSLENGTH);
			MsgEdit->setPlainText(msg);
			MsgEdit->moveCursor(QTextCursor::End);
		}

		QString txt = tr("you can input") 
					+ QString("%1").arg(MAXSMSLENGTH - msg.size()) 
					+ tr("character");

		input_nm->setText(txt); 
	}
	else
		input_nm->setText(""); 
}

void AccountTab::UpdateSkins()
{
	TB_FACE->setPixmap (getFaceIcon()); 
	TB_CHANGEMOD->setPixmap (getChangeSendModIcon()); 
	TB_Histroy->setPixmap (getHistoryIcon()); 
	TB_Send->setPixmap (getSendIcon()); 
}

void AccountTab::init()
{
	UpdateSkins();
	init_slot();

	if (account_id == SYSTEM_ID)
	{
		account_name = tr("sys message");
		m_account = NULL;
	} else { 
		if(account_id == (qlonglong)strtol(fx_get_usr_uid(), NULL, 10) )
		{
			isSendToSelf = true;
			account_name = tr("send to self");
			m_account = NULL;
		} else {
			isSendToSelf = false;

			m_account = fx_get_account_by_id(account_id);
			//begin a dialog init, if the account is mobile, this function will do nothing...
			if (!isAwaySendSMS)
				fx_begin_dialog (account_id, NULL, NULL); 
			char * showname = fx_get_account_show_name(m_account, FALSE);
			account_name = QString::fromUtf8(showname);
			if(showname)
				free(showname);
		}
	} //(account_id == SYSTEM_ID)

	setSendModle(isAwaySendSMS);

	if (account_id == SYSTEM_ID)
		msgSend->MsgEdit->setEnabled(false);
}
void AccountTab::init_slot()
{
	connect(TB_CHANGEMOD, SIGNAL(clicked()), this, SLOT(changeSendModle()));
	connect(TB_Send, SIGNAL(clicked()), this, SLOT(SendMsg()));
	connect(TB_Histroy, SIGNAL(clicked()), this, SLOT(ShowHistroy()));
	connect(TB_FACE, SIGNAL(clicked()), this, SLOT(ShowFaces()));
	connect(MsgEdit, SIGNAL(textChanged()), this, SLOT(changeTableInputNM()));
	connect(&flickTimer, SIGNAL(timeout()), this, SLOT(flickerTab()));

}