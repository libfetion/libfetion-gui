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
#include <QTextEdit>

#include "fxmsgwindow.h"
#include "fxuitl.h"

//find is have the ac_id
inline AccountTab *findFromMsgWindow(QTabWidget * tabWidget, qlonglong ac_id)
{
	AccountTab *ac_tab = NULL;
	int tabCount = tabWidget->count ();

	for(int i = 0; i< tabCount; i++)
	{
		ac_tab = (AccountTab *) tabWidget->widget(i); 
		if(ac_tab && ac_tab->account_id == ac_id)
			return ac_tab;
	}
	return NULL;
}


FxMsgWindow::FxMsgWindow(QWidget *parent)
    : QMainWindow(parent)
{
	setupUi(this);
	m_mainwindow = NULL;
	m_willQuit = false;
	closeTabButton = NULL;

	init();
}
void FxMsgWindow::init()
{
	init_inputFace();
	init_UI();

	connect(tabWidget, SIGNAL( currentChanged(int) ), this, SLOT( currentChangedName(int) ));
	connect(tabWidget, SIGNAL( mouseDblClick(int) ), this, SLOT( closeTabWid(int) ));
}

void FxMsgWindow::init_inputFace()
{
	inputFace = new FxInputFace(this);
	QPalette pal;
	pal.setBrush(QPalette::Window,QBrush(getInputFaceIcon()));
	inputFace->setPalette(pal);
	inputFace->setMsgWindow(this);
	inputFace->setGeometry (QCursor::pos().x(), QCursor::pos().y(), 320, 261);
	inputFace->setWindowFlags(Qt::Dialog| Qt::FramelessWindowHint);
}

void FxMsgWindow::init_UI()
{
	QPalette pal;
	pal = palette();
	pal.setColor(QPalette::Active, QPalette::Button, pal.color(QPalette::Active, QPalette::Window));
	pal.setColor(QPalette::Disabled, QPalette::Button, pal.color(QPalette::Disabled, QPalette::Window));
	pal.setColor(QPalette::Inactive, QPalette::Button, pal.color(QPalette::Inactive, QPalette::Window));

	closeTabButton = new QToolButton(tabWidget);
	closeTabButton->setAutoRaise(true);
	closeTabButton->setEnabled(true);
	closeTabButton->setPalette(pal);

	tabWidget->setCornerWidget(closeTabButton, Qt::TopRightCorner);
	closeTabButton->setCursor(Qt::ArrowCursor);
	closeTabButton->setIcon(getCloseTabImage());
	connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeTab()));
	closeTabButton->setToolTip(tr("close current Tab"));

	tabWidget->setParent(this);
	tabWidget->clear();

	move(Settings::instance().MsgWinPos());
	resize(Settings::instance().MsgWinSize());
}

void FxMsgWindow::currentChangedName(int index)
{
	/*	if(tabWidget->currentIndex() == index)
		return; */
	AccountTab *accountTab = (AccountTab *) tabWidget->widget (index); 
	setCurrentTabTitle(accountTab); 
}	

void FxMsgWindow::setCurrentTabTitle(AccountTab *accountTab)
{
	if (!accountTab)
		return;

	accountTab->endFlickerTab();
	QString title = tr("with") + accountTab->account_name + tr("chating... "); 
	this->setWindowTitle(title);

	int online_state;
	if (!fx_is_pc_user_by_account(accountTab->m_account))
		online_state = -1;
	else
		online_state = fx_get_online_status_by_account(accountTab->m_account);

	this->setWindowIcon (getOnlineStatusIcon(online_state));
}

void FxMsgWindow::closeTab()
{
	closeTabWid (tabWidget->currentIndex());
}

void FxMsgWindow::closeTabWid(int index)
{
	AccountTab *accountTab = (AccountTab *) tabWidget->widget(index); 

	tabWidget->removeTab (index);
	//if the tabWidget have no tab, hide it..
	if (tabWidget->count() <= 0)
		this->hide();

	if (accountTab)
		delete accountTab;
}

FxQunWindow* FxMsgWindow::findQunWindow(qlonglong qun_id)
{
	int size = qunWindow.size();
	for(int i = 0; i< size; i++ )
		if(qunWindow.at(i)->qun_id == qun_id)
			return qunWindow.at(i);
	return NULL;
}

bool FxMsgWindow::addQunMessage(QString msg, qlonglong qun_id, qlonglong sender, bool iscoming_msg)
{
	FxQunWindow	*qunW = findQunWindow(qun_id);
	if (!qunW)
	{
		qunW = new FxQunWindow(qun_id, this);
		qunWindow.append(qunW);
	}

	qunW->show();
	qunW->activateWindow();
	qunW->setWindowState ( Qt::WindowNoState ) ;
	qunW->MsgEdit->setFocus();

	QString str; 
	QString show_msg; 
	if(iscoming_msg) {
		str = "<b style=\"color:rgb(0,85,0);\">"+ 
			qunW->getSenderName(sender) +"("+
			QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
			QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
			"):</b><br>"+ msg;
		show_msg= show_msg.fromUtf8(str.toUtf8().data()); 
	} else {
		show_msg= msg; 
	}

	QTextEdit * msgBrowser = qunW->MsgBrowser;
	msgBrowser->append(show_msg);
	msgBrowser->moveCursor(QTextCursor::End);

	saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10), qun_id, show_msg.toUtf8().data(), NULL);

	this->setFocus();
	return true;
}

bool FxMsgWindow::addMessage(QString msg, qlonglong account_id,  bool iscoming_msg)
{
	if (fx_is_qun_by_id(account_id))
	{
		addQunMessage(msg, account_id, 0L, true);
		return true;
	}

	AccountTab *accountTab = findFromMsgWindow(tabWidget, account_id);

	if (!accountTab)
	{
		accountTab = new AccountTab(account_id, tabWidget);
		accountTab->setMainWind( this->m_mainwindow );
		tabWidget->addTab( accountTab, accountTab->account_name);
	}

	if (!this->isVisible())
	{
		tabWidget->setCurrentWidget(accountTab);
		setCurrentTabTitle(accountTab); 

		if (Settings::instance().isAutoShowMsg())
		{
			this->show();
			this->setWindowState(Qt::WindowNoState) ;
			this->setFocus();
			accountTab->msgSend->MsgEdit->setFocus();
		}
	}
	
	QString str; 
	QString show_msg; 
	if(iscoming_msg) {
		str ="<b style=\"color:rgb(0,85,0);\">"+ accountTab->account_name + "("+
			QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
			QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
			"):</b><br>" + msg;
		show_msg= show_msg.fromUtf8(str.toUtf8().data()); 
	} else {
		show_msg= show_msg.fromUtf8(msg.toUtf8().data()); 
	}

	if (account_id == SYSTEM_ID)
	{
		bool tmp = selectSystemMsg (strtol (fx_get_usr_uid(), NULL, 10),
				SYSTEM_ID, show_msg.toUtf8().data());
		if (tmp)
			return true;
	}

	QTextEdit *msgBrowser = accountTab->msgSend->MsgBrowser;
	msgBrowser->append(show_msg);
	msgBrowser->moveCursor(QTextCursor::End);

	saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10), account_id, show_msg.toUtf8().data(), NULL);

	if (Settings::instance().isAutoReply() && iscoming_msg)
		exec_autoRelpy(msgBrowser, account_id, Settings::instance().getAutoReply());

	accountTab->startFlickerTab();

	if (!Settings::instance().isMute())
		playSound(MSG_SOUND);

	return true;
}

void FxMsgWindow::exec_autoRelpy(QTextEdit* msgBrowser, qlonglong account_id, QString msg)
{
	bool sendFlag = false;
	msg = QObject::tr("(LibFetion auto reply msg)") + msg;

	if (!fx_is_pc_user_by_id(account_id))
		sendFlag = fx_send_sms (account_id, msg.toUtf8().data(), NULL, NULL); 
	else {
		msg.replace(QString("<"), QString("&lt;"));
		msg.replace(QString(">"), QString("&gt;"));
		sendFlag = fx_dialog_send( account_id, msg.toUtf8().data(), NULL, NULL); 
	}

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
	msgBrowser->append(show_msg);

	saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10), account_id, show_msg.toUtf8().data(), NULL);
}

void FxMsgWindow::haveQunMessage(qlonglong qun_id)
{
	if (!Settings::instance().isMute())
		playSound(MSG_SOUND);

	Fetion_MSG * fxMsg = fx_get_msg(qun_id);
	if (!fxMsg)
		return;

	long sender = fxMsg->ext_id;
	addQunMessage(fxgui_handle_newMsg(fxMsg), qun_id, sender, true);
	fx_destroy_msg(fxMsg);
}

void FxMsgWindow::haveNewMessage(qlonglong account_id)
{
	Fetion_MSG * fxMsg = fx_get_msg(account_id);
	if (!fxMsg)
		return;

	addMessage(fxgui_handle_newMsg(fxMsg), account_id, true);

	fx_destroy_msg (fxMsg);
}

void FxMsgWindow::resizeEvent (QResizeEvent * event) 
{
	Settings::instance().setMsgWinSize(size());
	tabWidget->resize(event->size());
}

void FxMsgWindow::closeEvent(QCloseEvent *event)
{
	hide();
	if(m_willQuit)
		event->accept();
	else
		event->ignore();
}

void FxMsgWindow::moveEvent(QMoveEvent *event)
{
	Settings::instance().setMsgWinPos(pos());
}

void FxMsgWindow::addQunWin(qlonglong qun_id, bool isSendSms)
{
	FxQunWindow	*qun = findQunWindow(qun_id);
	if (!qun)
	{
		qun = new FxQunWindow(qun_id, this, isSendSms);
		qunWindow.append(qun);
	}
	qun->show();
 	qun->activateWindow();
}

bool FxMsgWindow::isVerifiedAccount(qlonglong account_id)
{
	if (fx_is_InBlacklist_by_id(account_id))
	{
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("it have be added in blacklist by you") );
		return true;
	}

	int	authed = fx_is_authed_by_id(account_id);

	if (authed == AUTH_WAIT )
	{
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("wait auth to add friend") );
		return true;
	}

	if (authed == AUTH_REFUS)
	{
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("was refused to add friend") );
		return true;
	}

	return false;
}

void FxMsgWindow::addAccount(qlonglong account_id, bool isSendSms)
{
//	if (!isVerifiedAccount(account_id))
//		return;

	if (fx_is_InBlacklist_by_id(account_id))
	{
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("it have be added in blacklist by you") );
		return;
	}

	int	authed = fx_is_authed_by_id(account_id);

	if (authed == AUTH_WAIT )
	{
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("wait auth to add friend") );
		return;
	}

	if (authed == AUTH_REFUS)
	{
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("was refused to add friend") );
		return;
	}

	//first find is have the instance of the account_id, if have show it, and return.
	//then create a new instance of this account_id, and add to the tabwidget.
	AccountTab *accountTab = findFromMsgWindow(tabWidget, account_id);

	if (!accountTab)
	{
		accountTab = new AccountTab(account_id, tabWidget, isSendSms);
		accountTab->setMainWind( this->m_mainwindow );
		tabWidget->addTab( accountTab, accountTab->account_name);
	}

	tabWidget->setCurrentWidget(accountTab);
	setCurrentTabTitle(accountTab); 

	if (!this->isVisible())
		this->show();

	this->activateWindow();
	this->setWindowState(Qt::WindowNoState);

	accountTab->setSendModle(isSendSms);
	accountTab->msgSend->MsgEdit->setFocus();
}

FxMsgWindow::~FxMsgWindow()
{
	int size = qunWindow.size();
	for(int i = 0; i< size; i++ )
		qunWindow.at(i)->qun_exit();
}

void FxMsgWindow::msg_exit()
{
	m_willQuit = true;
	close();
}

void FxMsgWindow::showFaces()
{
	inputFace->setGeometry (QCursor::pos().x() - 320,
			QCursor::pos().y() - 261, 320, 261);
	inputFace->show();
	inputFace->setFocus();
	//inputFace->setWindowState(Qt::WindowActive);
	//inputFace->grabMouse() ;
}

void FxMsgWindow::UpdateSkins()
{
	if (closeTabButton)
		closeTabButton->setIcon(getCloseTabImage());
}
