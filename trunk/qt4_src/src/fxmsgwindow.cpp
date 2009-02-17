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
#include <math.h> 

#include "fxmainwindow.h"
#include "fxmsgwindow.h"
#include "fxutil.h"
#include "fxaddBuddyWindow.h"


#define SHAKE_DISTANCE 10
#define SHAKE_CYCLE 9
//msec
#define SHAKE_DURATION 800

//msec
#define SHAKE_TIMER_UPDATE 57


#define MAXNICELENGTH 8
static QString CropTabName(QString orig_name); 
//find is have the ac_id
inline AccountTab *findFromMsgWindow(QTabWidget * tabWidget, qlonglong ac_id)
{
	AccountTab *ac_tab = NULL;
	int tabCount = tabWidget->count();

	for(int i = 0; i < tabCount; i++)
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
    m_isNudgeShake = false;

	init();
}
void FxMsgWindow::init()
{
	init_inputFace();
	init_UI();

	connect(tabWidget, SIGNAL( currentChanged(int) ), this, SLOT( currentChangedName(int) ));
	connect(tabWidget, SIGNAL( mouseDblClick(int) ), this, SLOT( closeTabWid(int) ));
	connect(&nudge_timer, SIGNAL(timeout()), this, SLOT(slot_do_shake()));
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
	if (!Settings::instance().isMute())
		playSound(MSG_SOUND);

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
		/*
		str = "<b style=\"color:rgb(0,85,0);\">"+ 
			qunW->getSenderName(sender) +"("+
			QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
			QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
			"):</b><br>"+ msg;
			*/
		str = "<b style=\"color:rgb(0,85,0);\">"+ 
			qunW->getSenderName(sender) + msg;
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
		tabWidget->addTab(accountTab, CropTabName(accountTab->account_name));
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
	/*	
	str ="<b style=\"color:rgb(0,85,0);\">"+ accountTab->account_name + "("+
			QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
			QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
			"):</b><br>" + msg;
			*/
		str ="<b style=\"color:rgb(0,85,0);\">"+ accountTab->account_name + msg;
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
	{
		int size = qunWindow.size();
		for(int i = 0; i< size; i++ )
			qunWindow.at(i)->qun_exit();

		event->accept();
	}
	else
		event->ignore();
}

void FxMsgWindow::moveEvent(QMoveEvent *event)
{
	//when it is shaking, we don't recode the pos of window.
	if (m_isNudgeShake)
		return;
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

void FxMsgWindow::addBuddy(qlonglong account_id)
{
	QString id;
	bool ismobile = false;
	if (fx_is_pc_user_by_id (account_id))
	{
		id = QString("%1").arg(account_id);
		ismobile = false;
	} else {
		char* mobile_no = fx_get_original_ID(account_id);
		id = QString(mobile_no); 
		free(mobile_no);
		mobile_no = NULL;

		ismobile = true;
	}

	m_mainwindow->show();
	FxAddBuddy *tmp_addBuddy = new FxAddBuddy(id, ismobile, m_mainwindow);
	tmp_addBuddy->exec();
	//fix a bug when addbuddy show, and mainwindow is hide, quit the addbuddy, 
	//mainwindows will quit too.
	m_mainwindow->show(); 
	delete tmp_addBuddy;
	tmp_addBuddy = NULL;
}

void FxMsgWindow::addAccount(qlonglong account_id, bool isSendSms)
{

/******************************************************/
//**********************VerifiedAccount****************/
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
		#if 0
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("wait auth to add friend") );
		#else
		addBuddy(account_id);
		#endif
		return;
	}

	if (authed == AUTH_REFUS)
	{
		#if 0
		QMessageBox::information(this->parentWidget(),
				tr("can't send mseeage to he"), 
				tr("was refused to add friend") );
		#else
		addBuddy(account_id);
		#endif
		return;
	}
/******************************************************/

	//first find is have the instance of the account_id, if have show it, and return.
	//then create a new instance of this account_id, and add to the tabwidget.
	AccountTab *accountTab = findFromMsgWindow(tabWidget, account_id);
	if (!accountTab)
	{
		accountTab = new AccountTab(account_id, tabWidget, isSendSms);
		accountTab->setMainWind( this->m_mainwindow );
		tabWidget->addTab(accountTab, CropTabName(accountTab->account_name));
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
}

void FxMsgWindow::msg_exit()
{
	m_willQuit = true;
	close();
}


static int libfetion_shake_stamp = 0;
void FxMsgWindow::slot_do_shake()
{
	libfetion_shake_stamp += SHAKE_TIMER_UPDATE;
	if (libfetion_shake_stamp >= SHAKE_DURATION)
	{
		m_isNudgeShake = false;
		libfetion_shake_stamp = 0;
		nudge_timer.stop();
		move(Settings::instance().MsgWinPos());
	}

	double waveOffset = (double)(libfetion_shake_stamp % SHAKE_CYCLE)/SHAKE_CYCLE;

	double TWO_PI = 3.14 * 2.0;
	double angleX = waveOffset * TWO_PI;
	double angleY = waveOffset * TWO_PI;

	//fix: should we need sin()? it will spent lots of cpus on it.
	int shakenX = (int) ((sin(angleX) * SHAKE_DISTANCE) + Settings::instance().MsgWinPos().x());
	int shakenY = (int) ((sin(angleY) * SHAKE_DISTANCE) + Settings::instance().MsgWinPos().y());
	move(shakenX ,shakenY);

}
void FxMsgWindow::nudge_shake()
{
	//if have a new nudge recive, we extend the shake time.
	libfetion_shake_stamp = 0;
	if (nudge_timer.isActive())
		return;

	this->show();
	this->activateWindow();
	this->setWindowState(Qt::WindowNoState) ;
	this->setFocus();

	nudge_timer.start(SHAKE_TIMER_UPDATE);
	m_isNudgeShake = true;
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
    AccountTab *ac_tab = NULL;
	int tabCount = tabWidget->count();
    for(int i = 0; i < tabCount; i++)
    {
        ac_tab = (AccountTab *) tabWidget->widget(i); 
        if(ac_tab)
            ac_tab->UpdateSkins();
    }

	//update the chart windows icon
	ac_tab = (AccountTab *) tabWidget->widget (tabWidget->currentIndex());
	setCurrentTabTitle(ac_tab); 

	if (closeTabButton)
		closeTabButton->setIcon(getCloseTabImage());
}

void FxMsgWindow::SetAllFont(QFont font)
{
	this->setFont(font);
	tabWidget->setFont(font);

    AccountTab *ac_tab = NULL;
	int tabCount = tabWidget->count();
    for(int i = 0; i < tabCount; i++)
    {
        ac_tab = (AccountTab *) tabWidget->widget(i); 
        if(ac_tab)
            ac_tab->SetAllFont(font);
    }

   this->repaint();
}

static QString CropTabName(QString orig_name) 
{
    QString new_name = orig_name;
    //here handle the ac_name is length. if too long ,using ...repleace.
    if (new_name.size() > MAXNICELENGTH)
        new_name = new_name.left(MAXNICELENGTH -3) + QString("...");
    return new_name;
}
