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

#include <QtGui>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QTreeView>
#include <QListView>
#include <QHeaderView>
#include <QStandardItem>


#include "appconfig.h"
#include "fxmainwindow.h"
#include "fxloginwindow.h"
#include "fxshowAccountInfo.h"
#include "fxqunwindow.h"
#include "fxverifyAccount.h"
#include "fxrefuseSMS.h"
#include "fxsendGroupSMS.h"
#include "fxconfigDlg.h"
#include "fxskinmanage.h"
#include "fxscheduleSmsManage.h"
#include "fxscheduleSms.h"
#include "LibFetionEventHandle.cpp"

FxMainWindow::FxMainWindow(QWidget *parent)
	:FxWidget(parent)
	,trayIcon(NULL)
	,traySetStatusMenu(NULL)
	,buddySetStatusMenu(NULL)
	,msgHistroyMenu(NULL)
	,skinMenu(NULL)
	,traySendSmsMenu(NULL)
	,optSendSmsMenu(NULL)
{
	setObjectName("mainWindow");
	setupUi(contentWidget);
	newVersion = 0;
	new_msg_count = 0;
	new_qun_msg_count = 0;
	fx_status = NO_SET;
	isQuit = false;
	isBreakOut = false;
	isHaveminimized = false;
	isNeedRecordWinPos = false;
	tmp_addBuddy = NULL;

	init_UI();
	setMinimizetoHide(true);
	setAutoHide(true);
	initAllActions();
	createMenu();
	initTrayIcon();
	init_slot_signal();

	//set the fetion system msg call back function
	fx_set_system_msg_cb (Sys_EventListener, this);

	//save the account info to db, 
	//will a bug: when the account is changed, but the db info maybe not changed follow
#if DEBUG_GUI
	QTreeWidgetItem * tm = new QTreeWidgetItem(view, 0);
	tm->setText(0, "haha");
#else
	saveAccountInfo();
#endif
	
	minimizedTimer.start(100);
	checkSkinsTimer.start(10000);
	updateAccountInfoTimer.start(2000);
}

FxMainWindow::~FxMainWindow()
{
	checkSkinsTimer.stop();
	if(buddyMge)
		delete buddyMge;
	if(msgwin)
		delete msgwin;
}

void FxMainWindow::UI_enable_impresa()
{
	UI_Impresa->setDisabled (false);
	UI_Impresa->setFocus ();
}

void FxMainWindow::UI_enable_search()
{
	UI_Search->setDisabled (false);
	UI_Search->setFocus ();
}

void FxMainWindow::setUINiceName()
{
	QString impresa; 
	if (fx_data_get_PersonalInfo())
		impresa = QString::fromUtf8(fx_data_get_PersonalInfo()->impresa); 

	if (impresa.isEmpty())
	{
		UI_Impresa->setToolTip (NULL);
		impresa = tr("please input impresa...");
	}
	
	UI_Impresa->setText(impresa);
	UI_Impresa->setToolTip(impresa);

	UI_Impresa->setCursorPosition (0);
	m_impresa = impresa;
	
	QString state;
	switch (fx_get_user_state())
	{
		case FX_STATUS_ONLINE:
			state = tr("online");
			break;
		case FX_STATUS_OFFLINE:
			state = tr("hide");
			break;
		case FX_STATUS_BUSY:
			state = tr("busy");
			break;
		default:
			state = tr("away");
			break;
	}

	QString name = QString::fromUtf8(fx_get_usr_show_name());
	//if (name.size >12)
	QString showstr= name + "(" +state + ")";

	UI_NiceName->setText (showstr);
}

void FxMainWindow::SearcheditingFinished ()
{
	QString text = UI_Search->text();
	if (text.isEmpty())
	{
		UI_Search->setText(tr("search friends..."));
		UI_Search->setCursorPosition (0);
		//show mian tree list
	}
//	UI_Search->setDisabled (true);
}

void FxMainWindow::SearchtextChanged (const QString &text)
{
	Q_UNUSED(text);
	QString content = UI_Search->text();
	if (content.isEmpty() || content == tr("search friends..."))
	{
		search->hide();
		view->show();
		return;
	}
	
	view->hide(); //hide mian tree list 
	
	search->clear();
	QList<QTreeWidgetItem *>  *items = searchAccountInfo(content.toUtf8().data());
	search->insertTopLevelItems(0, *items);
	search->show();
}

void FxMainWindow::changeNiceName()
{
	UI_Edit_NiceName->hide();
	QString text = UI_Edit_NiceName->text();
	if (text != QString::fromUtf8(fx_get_usr_show_name()))
		fx_set_user_nickname(text.toUtf8().data(), NULL, NULL); 
}

void FxMainWindow::changeImpresa()
{
	QString text = UI_Impresa->text();
	if (text == m_impresa)
		return;

	if (text.isEmpty())
	{
		UI_Impresa->setText(tr("please input impresa..."));
		UI_Impresa->setCursorPosition (0);
		text = tr("please input impresa...");
		
		if (m_impresa != tr("please input impresa..."))
			fx_set_user_impresa(NULL, NULL, NULL);
	}
	else
		fx_set_user_impresa(text.toUtf8().data(), NULL, NULL); 

	UI_Impresa->setToolTip (text);
	m_impresa = text;
}

void FxMainWindow::SearchFocusIn ()
{
	if (UI_Search->text() == tr("search friends...") )
		UI_Search->setText("");
}

void FxMainWindow::ImpresaFocusIn ()
{
	if (UI_Impresa->text() == tr("please input impresa...") )
		UI_Impresa->setText("");
}

void FxMainWindow::addNewMsgCount(bool isQunMsg)
{
	if (!isQunMsg)
		new_msg_count++;
	else
		new_qun_msg_count++;

	if (isHaveTray) 
		startFlickerTray();
}

bool FxMainWindow::showNewMsgDlg()
{
	if (new_msg_count > 0)
	{
		subNewMsgCount();
		msgwin->showNormal();
		return true;
	}

	if (new_qun_msg_count > 0)
	{
		subNewMsgCount(true);
		return true;
	}

	return false;
}

void FxMainWindow::subNewMsgCount(bool isQunMsg)
{
	if (!isQunMsg)
		new_msg_count--;
	else
		new_qun_msg_count--;

	if ( !new_msg_count && !new_qun_msg_count && isHaveTray) 
		endFlickerTray();

	if(new_msg_count < 0)
		new_msg_count = 0;

	if (new_qun_msg_count < 0)
		new_qun_msg_count = 0;
}

void FxMainWindow::startFlickerTray()
{
	if (trayFlickTimer.isActive())
		return;
	trayFlickTimer.start(300);
}

void FxMainWindow::endFlickerTray()
{
	if (trayFlickTimer.isActive())
		trayFlickTimer.stop();
	trayIcon->setIcon(getSysTrayIcon (fx_get_user_state()));
}


void  Relogin_EventListener (int message, WPARAM wParam, LPARAM lParam, void* args)
{
	if(!args)
		return;
	FxMainWindow * mainwind = (FxMainWindow *) args;
	mainwind->handleFx_relogin_Event(message, wParam, lParam);
}

void FxMainWindow::relogin_fetion()
{	
	if (reloginTimer.isActive())
		return;
	if (isHaveTray) 
		trayIcon->setContextMenu(reloginTrayMenu);

	this->hide();
	fx_relogin(Relogin_EventListener, this);
	reloginTimer.start(1000*35);
}

void FxMainWindow::handleFx_relogin_Event(int message, WPARAM wParam, LPARAM lParam)
{
	Q_UNUSED(wParam);
	Q_UNUSED(lParam);
	switch(message)
	{
		case FX_LOGIN_URI_ERROR:
		case FX_LOGIN_FAIL:
		case FX_LOGIN_NETWORK_ERROR:
		case FX_LOGIN_UNKOWN_ERROR :   
		case FX_LOGIN_UNKOWN_USR:
		case FX_LOGIN_GP_FAIL:

			relogin_fetion();
			if (!isBreakOut && isHaveTray) //here just one warnning
			{
				isBreakOut = true;
				QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(3);
				trayIcon->showMessage (tr("connect break off"), 
						tr("disconnect from fetion server, will relogin fetion"), 
						icon, 1*1000);
			}
			break;
		case FX_LOGIN_OK :
			fx_status = NO_SET;
			isBreakOut = false;
			if (isHaveTray) { 
				trayIcon->setContextMenu(trayIconMenu);
				trayIcon->setIcon(getSysTrayIcon (fx_get_user_state()));
			}
			if (reloginTimer.isActive())
				reloginTimer.stop();

			//set the fetion system msg call back function
			fx_set_system_msg_cb (Sys_EventListener, this);
			break;
	}
}

void FxMainWindow::relogin_timer()
{
	fx_relogin(Relogin_EventListener, this);
	/*
	   if (reloginTimer.isActive())
	   reloginTimer.stop();
	   */
}

void FxMainWindow::updateAccountInfo_timer()
{
    Account_Info* account = buddyMge->fetchNoUpdateAccount();
    if (!account)
    {
        updateAccountInfoTimer.stop();
        return;
    }

    fx_update_account_info_by_id(account->accountID);
    account->haveUpdate = true;
}

void FxMainWindow::flickerTray()
{
	static bool flick_flag = false;
	trayIcon->setIcon(getFlickIcon(flick_flag));
	flick_flag = !flick_flag;
}


void FxMainWindow::checkSkinPath()
{
	if (check_dir_state(SkinPath().toUtf8().data()))
		createSkinMenu(skinMenu);
}

void FxMainWindow::minimizedWind()
{
	if (!isHaveminimized)
	{
		isHaveminimized = true;
		
		#ifdef WIN32
		this->showMinimized();
		#endif

	} else {
		#ifdef WIN32
//		this->setWindowState(Qt::WindowNoState);
		//registed the hot key, when the mainwindow is created, and can get the winId
		if (Settings::instance().isEnableGetMsgHotKey())
			Settings::instance().setGetMsgHotKey(Settings::instance().GetMsgHotKey(), Settings::instance().GetMsgHotKeyMod());
		#endif
		this->showNormal();
		minimizedTimer.stop();
		if (Settings::instance().isStartHide())
			this->hide();
		
		move(Settings::instance().MainWinPos());
		isNeedRecordWinPos = true;
	}
}


void FxMainWindow::haveCurrentVersionMessage(int version)
{
	newVersion = version;
	if (version > CURRENT_VERSION)
	{
		QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(1);
		trayIcon->showMessage(tr("LibFetion"),
				tr("LibFetion Have New Version,"
					"access http://www.libfetion.cn for more infomation"),
				icon, 10*1000);
	}
}


void FxMainWindow::haveAddAccountAppMessage(char* uri, char*desc)
{
	FxVerifyAccount * verifyAccount = new FxVerifyAccount(this, uri, desc, this);
	verifyAccount->show();
}


void FxMainWindow::haveNewSysMessage(qlonglong sys_id)
{
	Q_UNUSED(sys_id);
#if 0 //not using system message
	Fetion_MSG * fxMsg = fx_get_msg(sys_id);
	if(!fxMsg)
		return;

	QString newmsg ;
	char *msg = fx_msg_qt_format(fxMsg->message); 
	newmsg = newmsg.fromUtf8(msg);

	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(1);
	trayIcon->showMessage(tr("sys message"), newmsg, 
			icon, 5*1000);

	fx_destroy_msg (fxMsg);
	if(msg)
		free(msg);
#endif
}

void FxMainWindow::haveNewMessage(qlonglong account_id)
{
	msgwin->haveNewMessage(account_id);
}

void FxMainWindow::haveNewQunMessage(qlonglong qun_id)
{
	msgwin->haveQunMessage(qun_id);
}

void FxMainWindow::slot_SystemNetErr(int err)
{
	Q_UNUSED(err);
	relogin_fetion();

	if (fx_status == SYS_RELOGIN)
		return ;

	fx_status = SYS_RELOGIN;
	if (isHaveTray)  
		trayIcon->setIcon (getSysTrayIcon(0));
}

void FxMainWindow::slot_receive_nudge(qlonglong account_id)
{
	QString nudgemsg = "<b style=\"color:rgb(250,0,255);\">" +tr("send a nudge to you") + "</b><br>";
	msgwin->addMessage(nudgemsg, account_id);

	if (!Settings::instance().isDisableNudge())
		msgwin->nudge_shake();
}

void FxMainWindow::slot_DeRegistered()
{
	if(isHaveTray) {
		trayIcon->setIcon(getSysTrayIcon(0));
		fx_status = SYS_DeRegist;
		QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(3);
		trayIcon->showMessage(tr("relogin"), 
				tr("you have login in other pc, libfetion will quit"), 
				icon, 20*1000);
	} else {
		QMessageBox::critical(this, tr("relogin"), 
				tr("you have login in other pc, libfetion will quit") ); 
		isQuit = true;
		close();
	}
}

#define MSG_OK       1
#define MSG_FAIL     2
#define MSG_TIMEOUT  3
#define MSG_FAIL_LIMIT  4

void FxMainWindow::handle_sendmsg(int msgflag, int fx_msg, qlonglong account_id)
{
	if(!fx_msg)
		return;

	int i = 0;
	Fetion_MSG *fxMsg = (Fetion_MSG *) fx_msg;
	char *msg = fx_msg_qt_format(fxMsg->message); 
	QString newmsg;

	switch(msgflag)
	{
		case MSG_OK:
			for (i = 0; i < timeOutMsgVector.size(); ++i) {
				if (timeOutMsgVector.at(i) == fx_msg)
				{
					newmsg = "<b style=\"color:rgb(170,0,255);\">" +tr("auto resend ok:") + "</b>" + newmsg.fromUtf8(msg);
					timeOutMsgVector.remove(i);
					break;
				}
			}
			break;
		case MSG_FAIL:
			for (i = 0; i < timeOutMsgVector.size(); ++i) {
				if (timeOutMsgVector.at(i) == fx_msg)	{
					timeOutMsgVector.remove(i);
					break;
				}
			}
			newmsg = "<b style=\"color:red;\">"+tr("send fail:") +"</b>"+ newmsg.fromUtf8(msg);
			break;

		case MSG_FAIL_LIMIT:
			for (i = 0; i < timeOutMsgVector.size(); ++i) {
				if (timeOutMsgVector.at(i) == fx_msg)	{
					timeOutMsgVector.remove(i);
					break;
				}
			}
			newmsg = "<b style=\"color:red;\">"+tr("send sms fail by limit:") +"</b>"+ newmsg.fromUtf8(msg);
			break;
		case MSG_TIMEOUT:
			timeOutMsgVector.append(fx_msg); // add the msg to vector
			newmsg = "<b style=\"color:rgb(170,0,255);\">" +tr("send timeout:") +"</b>" + newmsg.fromUtf8(msg)+"<br><b style=\"color:rgb(170,0,255);\">" +tr("will auto resend")+"</b>";
			break;
	}

    if (fxMsg->ext_id != 0)
        msgwin->addQunMessage(newmsg, account_id, 0L, true);
    else
        msgwin->addMessage(newmsg, account_id);

	if(msg)
		free(msg);
}

void FxMainWindow::slot_SysDialogMsg (int message, int fx_msg, qlonglong who)
{
    int msgflag=0; 

    if (!fx_msg)
        return;

	switch(message)
	{
		case FX_SMS_OK: 
		case FX_DIA_SEND_OK: 
		case FX_QUN_SEND_OK: 
        case FX_QUN_SMS_OK: 
            msgflag = MSG_OK;
            break;

		case FX_SMS_FAIL: 
		case FX_DIA_SEND_FAIL: 
		case FX_QUN_SEND_FAIL: 
		case FX_QUN_SMS_FAIL: 
            msgflag = MSG_FAIL;
            break;

		case FX_SMS_TIMEOUT: 
		case FX_DIA_SEND_TIMEOUT: 
		case FX_QUN_SEND_TIMEOUT: 
		case FX_QUN_SMS_TIMEOUT: 
            msgflag = MSG_TIMEOUT;
			break;

        case FX_SMS_FAIL_LIMIT: 
        case FX_QUN_SMS_FAIL_LIMIT: 
            msgflag = MSG_FAIL_LIMIT;
            break;
	}

    handle_sendmsg(msgflag, fx_msg, who);

    //time out should not to destroy msg, beacuse the system will resend by itself..
    if (msgflag != MSG_TIMEOUT)
        fx_destroy_msg((Fetion_MSG *)fx_msg);
}

void FxMainWindow::slot_set_state(int state)
{
	setUINiceName();

	if (traySetStatusMenu)
		traySetStatusMenu->setIcon (getOnlineStatusIcon(state));
	if (buddySetStatusMenu)
		buddySetStatusMenu->setIcon (getOnlineStatusIcon(state));

	if(isHaveTray) 
		trayIcon->setIcon(getSysTrayIcon (state));
}

void FxMainWindow::slot_updateSmsDay(int day)
{
	if (day)
	{
		refuseSMSAct->setIcon(getMenuIcon(ApplyIcon));
		acceptSMSAct->setIcon(QPixmap());
	} else {
		acceptSMSAct->setIcon(getMenuIcon(ApplyIcon));
		refuseSMSAct->setIcon(QPixmap());
	}
}

void FxMainWindow::createSkinMenu(QMenu *sub_skinMenu)
{
	//fixme: here have a big bug.. 
	if (!sub_skinMenu)
		return;

	sub_skinMenu->clear();

	QList<Skin_Info *> * skinlist = searchAllSkins();
	if (skinlist == NULL)
		return;

	for (QList<Skin_Info *>::Iterator it = skinlist->begin(); 
			it != skinlist->end(); ++it)
	{
		Skin_Info *sk_info = (*it);
		QAction *action = new QAction(sk_info->name, this);

		if (sk_info->name == getSkinName() && sk_info->skinpath == getSkinPath())
			action->setIcon(getMenuIcon(ApplyIcon));
		else 
			action->setIcon(getMenuIcon(CancelIcon));

#if MS_VC6
		QVariant Var((uint)sk_info);
#else
		QVariant Var;
		Var.setValue (sk_info); 
#endif
		action->setData(Var);
		sub_skinMenu->addAction(action);
	}

	//fixme: here need to releas the skinlist!!! note: not release the sk_info
	connect(sub_skinMenu, SIGNAL(triggered(QAction *)), this, SLOT(skinMenutriggered(QAction *)));
//	connect(sub_skinMenu, SIGNAL(aboutToShow()), this, SLOT(slot_ShowSkinMenu()));
}

void FxMainWindow::skinMenutriggered(QAction *action)
{
#if MS_VC6
	Skin_Info *sk_info = (Skin_Info *)(action->data().toUInt());
#else
	Skin_Info *sk_info = action->data().value<Skin_Info*>() ;
#endif
	if (!sk_info)
		return;
	setSkins(sk_info->skinpath, sk_info->name);

	this->UpdateSkins();
}

void FxMainWindow::searchaccountDoubleClicked (QTreeWidgetItem * item, int column )
{
	Q_UNUSED(column);
    if (buddyMge)
        buddyMge->handleAccountDoubleClicked(item);
}

#if WIN32
#include <windows.h>
bool FxMainWindow::winEvent(MSG *msg, long *result)
{
	if(WM_HOTKEY == msg->message)
	{
		if (showNewMsgDlg())
			return true;

		//here codes should be ajust after later..
		if (fx_status == SYS_RELOGIN) //relogin state will not handle this message..
			return true;

		trayMessageClicked();
        this->showNormal();	
        		
		return true;
	}

	return false;
}
#else
bool FxMainWindow::event( QEvent * event )
{
/* //for new UI, we shouldn't do anything about the QEvent::WindowStateChange.
	if( event->type() == QEvent::WindowStateChange) 
	{
		QWindowStateChangeEvent *mEvent = (QWindowStateChangeEvent *)event;
		if( mEvent->oldState() == Qt::WindowNoState)
		{
			FxWidget::event(event);
			hide();
			return true;
		}
	}
*/
	return FxWidget::event(event);
}
#endif

void FxMainWindow::closeEvent(QCloseEvent *event)
{
	if(isQuit){
		if(isHaveTray) 
			trayIcon->hide();
		msgwin->msg_exit();
		event->accept();
		return;
	}

	if(isHaveTray) { 
		if (trayIcon->isVisible()) {
			hide();
			event->ignore();
		}
	} else {
		msgwin->hide();
		event->accept();
	}
}

void FxMainWindow::moveEvent(QMoveEvent * event)
{
	Q_UNUSED(event);
	if (isNeedRecordWinPos)
		Settings::instance().setMainWinPos(pos());
}

void FxMainWindow::trayMessageClicked()
{
	switch(fx_status)
	{
#if 0
		case NET_ERR:
			QMessageBox::critical (this, tr("connect break off"), 
					tr("disconnect from fetion server, will relogin fetion"));
			printf("the system network have some error, we will relogin fetion \n");

			isQuit = true;
			close();
			break;
#endif

		case SYS_DeRegist:
			QMessageBox::critical(this, tr("relogin"), 
					tr("you have login in other pc, libfetion will quit") ); 
			isQuit = true;
			close();
			break;
		case NEW_MSG:
			break;
	}
}

void FxMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
		case QSystemTrayIcon::Trigger:

			if (showNewMsgDlg())
				return;

			//here codes should be ajust after later..
			if (fx_status == SYS_RELOGIN) //relogin state will not handle this message..
				return;

			trayMessageClicked();

#if MAC_OS
				this->showNormal();	
			
#endif
			break;

		case QSystemTrayIcon::DoubleClick:

			//here codes should be ajust after later..
			if (fx_status == SYS_RELOGIN) //relogin state will not handle this message..
				return;

			trayMessageClicked();
            this->showNormal();

            //special handle for autohide, we double click the trayIcon.
            this->clearFocus();
            this->endAutoHide();
            // end the special handle
			break;
		case QSystemTrayIcon::MiddleClick:
			break;
		default:
			;
	}
}

void FxMainWindow::init_UI()
{
	Settings::instance().setMainWindow(this);
#if DEBUG_GUI 
	Settings::instance().setUser(1000);
#else
	Settings::instance().setUser((qlonglong)strtol(fx_get_usr_uid(), NULL, 10));
#endif


#if WIN32
	setWindowTitle(QString::fromUtf8(fx_get_usr_show_name()) + "--Win Fetion");
#else
#if MAC_OS
	setWindowTitle(QString::fromUtf8(fx_get_usr_show_name()) + "--Mac Fetion");
#else
	setWindowTitle(QString::fromUtf8(fx_get_usr_show_name()) + "--Linux Fetion");
#endif
#endif
	this->setWindowIcon(getSysTrayIcon(1));
	
	move(Settings::instance().MainWinPos());

	//UI set all images of main windows
	//****************************************************/
	LibFetion_image->setPixmap(getLibFetionImage());
	UI_Portrait->setPixmap (getPortraitImage());
	UI_ImpresaBK->setPixmap (getImpresaBKImage());
	UI_SearchBK->setPixmap (getSearchBKImage());
	UI_AddFriend->setPixmap (getAddImage());
	UI_BT_SETTING->setPixmap(getBTSettingImage());
	UI_BT_SENDSELF->setPixmap(getBTSendSelfImage());

	UI_Search->setText(tr("search friends..."));
	UI_Edit_NiceName->setText(QString::fromUtf8(fx_get_usr_show_name()));
	UI_Edit_NiceName->hide();
	setUINiceName();
	version->setText(VERSION_NO);
	///****************************************************/


	///************* init some contorl ***************************************/
	view->header()->setHidden(1);
	view->setRootIsDecorated(true);

	//add all account to main view
	buddyMge = new BuddyMge(view, this);

	//init search control
	search->header()->setHidden(1);
	search->setRootIsDecorated(true);
	search->hide();

	msgwin = new FxMsgWindow(0);
	msgwin->setMainWind(this);
	//****************************************************//
}
void FxMainWindow::initAllActions()
{
	Schedule_SMS_Act = new QAction(tr("schedulesms"), this);
	//Schedule_SMS_Act->setIcon(getOnlineStatusIcon(FX_STATUS_ONLINE));
	connect(Schedule_SMS_Act, SIGNAL(triggered()), this, SLOT(schedule_SMS()));
	
	OnlineAct = new QAction(tr("online"), this);
	OnlineAct->setIcon(getOnlineStatusIcon(FX_STATUS_ONLINE));
	connect(OnlineAct, SIGNAL(triggered()), this, SLOT(setOnlineStatus()));

	OfflineAct = new QAction(tr("offline"), this);
	OfflineAct->setIcon(getOnlineStatusIcon(FX_STATUS_OFFLINE));
	connect(OfflineAct, SIGNAL(triggered()), this, SLOT(setOfflineStatus()));

	BusyAct = new QAction(tr("busy"), this);
	BusyAct->setIcon(getOnlineStatusIcon(FX_STATUS_BUSY));
	connect(BusyAct, SIGNAL(triggered()), this, SLOT(setBusyStatus()));

	AwayAct = new QAction(tr("away"), this);
	AwayAct->setIcon(getOnlineStatusIcon(FX_STATUS_AWAY));
	connect(AwayAct, SIGNAL(triggered()), this, SLOT(setAwayStatus()));

	acceptSMSAct = new QAction(tr("accept SMS"), this);
	connect(acceptSMSAct, SIGNAL(triggered()), this, SLOT(setacceptSMS()));
	refuseSMSAct = new QAction(tr("refuse SMS"), this);
	connect(refuseSMSAct, SIGNAL(triggered()), this, SLOT(setrefuseSMS()));

	if (fx_get_user_refuse_sms_day()) //is get from libfetion..	
	{
		refuseSMSAct->setIcon(getMenuIcon(ApplyIcon));
		acceptSMSAct->setIcon(QPixmap());
	} else { 
		acceptSMSAct->setIcon(getMenuIcon(ApplyIcon));
		refuseSMSAct->setIcon(QPixmap());
	}
	addBuddyAct = new QAction(tr("add friend"), this);
	addBuddyAct->setStatusTip(tr("add friend"));
	addBuddyAct->setIcon(getMenuIcon(AddBuddyIcon));
	connect(addBuddyAct, SIGNAL(triggered()), this, SLOT(addBuddy()));

	sendselfAct = new QAction(tr("send sms to self"), this);
	sendselfAct->setStatusTip(tr("send sms to self"));
	sendselfAct->setIcon(getMenuIcon(SMSBuddyIcon));
	connect(sendselfAct, SIGNAL(triggered()), this, SLOT(sendself()));

	sendgroupsmsAct = new QAction(tr("send group sms"), this);
	sendgroupsmsAct->setStatusTip(tr("send group sms"));
	sendgroupsmsAct->setIcon(getMenuIcon(SMSBuddyIcon));
	connect(sendgroupsmsAct, SIGNAL(triggered()), this, SLOT(sendgroupsms()));

	exitAct = new QAction(tr("Exit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	exitAct->setIcon(getMenuIcon(ExitIcon));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(tmp_exit()));

	personlInfoAct = new QAction(tr("personlInfo"), this);
	personlInfoAct->setStatusTip(tr("personlInfo"));
	personlInfoAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
	connect(personlInfoAct, SIGNAL(triggered()), this, SLOT(personlInfo()));

	CheckNewVersionAct = new QAction(tr("checkNewVersion"), this);
	CheckNewVersionAct->setStatusTip(tr("checkNewVersion"));
	CheckNewVersionAct->setIcon(getMenuIcon(AboutIcon));
	connect(CheckNewVersionAct, SIGNAL(triggered()), this, SLOT(checkNewVersion()));

	ReportBugAct = new QAction(tr("reportBugAct"), this);
	ReportBugAct->setStatusTip(tr("reportBugAct"));
	ReportBugAct->setIcon(getMenuIcon(AboutIcon));
	connect(ReportBugAct, SIGNAL(triggered()), this, SLOT(reportBugAct()));

	AboutLibFetionAct = new QAction(tr("aboutLibFetion"), this);
	AboutLibFetionAct->setStatusTip(tr("aboutLibFetion"));
	AboutLibFetionAct->setIcon(getMenuIcon(AboutIcon));
	connect(AboutLibFetionAct, SIGNAL(triggered()), this, SLOT(aboutLibFetion()));

	AboutCMAct = new QAction(tr("aboutChinaMobile"), this);
	AboutCMAct->setStatusTip(tr("aboutChinaMobile"));
	AboutCMAct->setIcon(getMenuIcon(AboutIcon));
	connect(AboutCMAct, SIGNAL(triggered()), this, SLOT(aboutCM()));

	AutoLoginAct = new QAction(tr("autologin"), this);
	if (isAutoLogin(NULL, NULL, NULL))
		AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		AutoLoginAct->setIcon(getMenuIcon(CancelIcon));

	AutoLoginAct->setStatusTip(tr("autologin"));
	connect(AutoLoginAct, SIGNAL(triggered()), this, SLOT(menu_setAutoLogin()));

	MuteAct = new QAction(tr("mute"), this);
	if (Settings::instance().isMute())
		MuteAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		MuteAct->setIcon(getMenuIcon(CancelIcon));

	MuteAct->setStatusTip(tr("autoshowmsg"));
	connect(MuteAct, SIGNAL(triggered()), this, SLOT(menu_setmute()));

	IsAutoShowMsgAct = new QAction(tr("autoshowmsg"), this);
#if MAC_OS
	IsAutoShowMsgAct->setVisible(false); 
#endif
	if (Settings::instance().isAutoShowMsg())
		IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));

	IsAutoShowMsgAct->setStatusTip(tr("autoshowmsg"));
	connect(IsAutoShowMsgAct, SIGNAL(triggered()), this, SLOT(menu_setautoshowmsg()));

#if 0
	SetUndgeMsgAct = new QAction(tr("undgemsg"), this);

	if (1)
	//if (!isAutoShowMsg(fx_get_usr_uid()))
		SetUndgeMsgAct->setIcon(getMenuIcon(CancelIcon));
	else 
		SetUndgeMsgAct->setIcon(getMenuIcon(ApplyIcon));
	SetUndgeMsgAct->setStatusTip(tr("undgemsg"));
	connect(SetUndgeMsgAct, SIGNAL(triggered()), this, SLOT(menu_setundgemsg()));
#endif

	SetLongSMSAct = new QAction(tr("catsms"), this);
	if (Settings::instance().isEnableLongSMS())
		SetLongSMSAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		SetLongSMSAct->setIcon(getMenuIcon(CancelIcon));
	SetLongSMSAct->setStatusTip(tr("catsms"));
	connect(SetLongSMSAct, SIGNAL(triggered()), this, SLOT(menu_setlongsms()));

	SaveMsgHistroyAct = new QAction(tr("savemsghistroy"), this);
	SaveMsgHistroyAct->setStatusTip(tr("savemsghistroy"));
	SaveMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
	connect(SaveMsgHistroyAct, SIGNAL(triggered()), this, SLOT(saveMsgHistroy()));

	CleanMsgHistroyAct = new QAction(tr("cleanmsghistroy"), this);
	CleanMsgHistroyAct->setStatusTip(tr("cleanmsghistroy"));
	CleanMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
	connect(CleanMsgHistroyAct, SIGNAL(triggered()), this, SLOT(cleanMsgHistroy()));
	
	
	ConfigAppAct = new QAction(tr("moresetting"), this);
	ConfigAppAct->setStatusTip(tr("moresetting"));
	ConfigAppAct->setIcon(getMenuIcon(OptionsIcon));
	connect(ConfigAppAct, SIGNAL(triggered()), this, SLOT(showConfigDlg()));


}

void FxMainWindow::createMenu()
{
	QMenu *mainMenu = new QMenu(btnMenu);
	buddyMenu = mainMenu->addMenu(tr("buddy"));
	menuSetting = mainMenu->addMenu(tr("settings"));
	menuAbout = mainMenu->addMenu(tr("about"));
	mainMenu->addSeparator();
	mainMenu->addAction(exitAct);
	btnMenu->setMenu(mainMenu);
	
	reloginTrayMenu = new QMenu(this);
	reloginTrayMenu->addAction(exitAct);

	trayIconMenu = new QMenu(this);
	traySetStatusMenu = trayIconMenu->addMenu(tr("change online status"));
	traySetStatusMenu->setIcon( getOnlineStatusIcon(fx_get_user_state()));
	traySetStatusMenu->addAction(OnlineAct);
	traySetStatusMenu->addAction(OfflineAct);
	traySetStatusMenu->addAction(BusyAct);
	traySetStatusMenu->addAction(AwayAct);

	
	traySendSmsMenu  = trayIconMenu->addMenu(tr("sendsms"));
	traySendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
	traySendSmsMenu->addAction(sendselfAct);
	traySendSmsMenu->addAction(sendgroupsmsAct);
	trayIconMenu->addAction(Schedule_SMS_Act);
	trayIconMenu->addAction(addBuddyAct); 
	trayIconMenu->addAction(personlInfoAct); 

	trayIconMenu->addSeparator();
	trayIconMenu->addAction(exitAct);

	buddySetStatusMenu = buddyMenu->addMenu(tr("change online status"));
	buddySetStatusMenu->setIcon( getOnlineStatusIcon(fx_get_user_state()));
	buddySetStatusMenu->addAction(OnlineAct);
	buddySetStatusMenu->addAction(OfflineAct);
	buddySetStatusMenu->addAction(BusyAct);
	buddySetStatusMenu->addAction(AwayAct);
	buddySetStatusMenu->addSeparator();
	buddySetStatusMenu->addAction(acceptSMSAct);
	buddySetStatusMenu->addAction(refuseSMSAct);

	buddyMenu->addMenu(buddySetStatusMenu );
	optSendSmsMenu = buddyMenu->addMenu(tr("sendsms"));
	optSendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
	optSendSmsMenu->addAction(sendselfAct);
	optSendSmsMenu->addAction(sendgroupsmsAct);
	buddyMenu->addAction(Schedule_SMS_Act);
	buddyMenu->addAction(addBuddyAct); 
	buddyMenu->addAction(personlInfoAct); 

#if 0
	buddyMenu->addSeparator();
	buddyMenu->addAction(exitAct);
#endif

	//menuSetting->addAction (SetImpresaAct); 
	menuSetting->addAction (AutoLoginAct); 
	menuSetting->addAction (MuteAct); 
	//menuSetting->addAction (IsAutoShowMsgAct); 
	//menuSetting->addAction (SetUndgeMsgAct); 
	//menuSetting->addAction (SetLongSMSAct); 
	
	skinMenu = menuSetting->addMenu(tr("skins"));
	skinMenu->setIcon(getMenuIcon(SkinIcon));
	createSkinMenu(skinMenu);

	msgHistroyMenu = menuSetting->addMenu(tr("msghistroy"));
	msgHistroyMenu->setIcon(getMenuIcon(HistoryIcon));
	msgHistroyMenu->addAction (SaveMsgHistroyAct); 
	msgHistroyMenu->addAction (CleanMsgHistroyAct); 
	menuSetting->addAction (ConfigAppAct); 

	menuAbout->addAction (CheckNewVersionAct);
	menuAbout->addAction (ReportBugAct);
	menuAbout->addAction (AboutLibFetionAct);
	menuAbout->addAction (AboutCMAct);

}

void FxMainWindow::init_slot_signal()
{

#if 0	
	connect(UI_ImpresaBK, SIGNAL(clicked ()),
			this, SLOT(UI_enable_impresa()));
	connect(UI_SearchBK, SIGNAL(clicked ()), 
			this, SLOT(UI_enable_search()));
#endif
	connect(UI_Impresa, SIGNAL(clicked ()),
			this, SLOT(UI_enable_impresa()));
	connect(UI_Search, SIGNAL(clicked ()), 
			this, SLOT(UI_enable_search()));


	connect(UI_BT_SETTING, SIGNAL(clicked ()), this, SLOT(showConfigDlg()));
	connect(UI_BT_SENDSELF, SIGNAL(clicked ()), this, SLOT(sendself()));
	connect(UI_AddFriend, SIGNAL(clicked ()), this, SLOT(addBuddy()));
	connect(UI_Portrait, SIGNAL(clicked ()), this, SLOT(showPortrait()));
	connect(UI_NiceName, SIGNAL(clicked ()), this, SLOT(showNiceNameEdit()));

	connect(UI_Edit_NiceName, SIGNAL(editingFinished ()), this, SLOT(changeNiceName ()));
	connect(UI_Edit_NiceName, SIGNAL(outfocus ()), UI_Edit_NiceName, SLOT(hide ()));

	connect(UI_Impresa, SIGNAL(editingFinished ()), this, SLOT(changeImpresa ()));
	connect(UI_Search, SIGNAL(editingFinished ()), this, SLOT(SearcheditingFinished ()));

	connect(UI_Impresa, SIGNAL(infocus ()), this, SLOT(ImpresaFocusIn ()));
	connect(UI_Search, SIGNAL(infocus ()), this, SLOT(SearchFocusIn ()));

	connect(UI_Search, SIGNAL(textChanged (const QString &) ), this, SLOT(SearchtextChanged (const QString &)));


    if(isHaveTray)
    { 
        connect(trayIcon, SIGNAL(messageClicked()),
                this, SLOT(trayMessageClicked()));
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    }


    connect(&checkSkinsTimer, SIGNAL(timeout()),
            this, SLOT(checkSkinPath()));
    connect(&minimizedTimer, SIGNAL(timeout()), 
            this, SLOT(minimizedWind()));
    connect(&trayFlickTimer, SIGNAL(timeout()), 
            this, SLOT(flickerTray()));
    connect(&reloginTimer, SIGNAL(timeout()),
            this, SLOT(relogin_timer()));
    connect(&updateAccountInfoTimer, SIGNAL(timeout()), 
            this, SLOT(updateAccountInfo_timer()));



	connect(this, SIGNAL(signal_UpdateSmsDay(int) ), 
			this, SLOT( slot_updateSmsDay(int) ) );
	connect(search, SIGNAL(itemDoubleClicked (QTreeWidgetItem* , int) ), 
			this, SLOT(searchaccountDoubleClicked ( QTreeWidgetItem *, int)));
	connect(this, SIGNAL(signal_Current_Version(int)), 
			this, SLOT( haveCurrentVersionMessage(int) ) );
	connect(this, SIGNAL(signal_AddAccountApp(char*, char*)), 
			this, SLOT( haveAddAccountAppMessage(char*, char*) ) );
	connect(this, SIGNAL(signal_NewSysMsg(qlonglong) ), 
			this, SLOT( haveNewSysMessage(qlonglong) ) );
	connect(this, SIGNAL(signal_NewMsg(qlonglong) ), 
			this, SLOT( haveNewMessage(qlonglong) ) );
	connect(this, SIGNAL(signal_NewQunMsg(qlonglong) ), 
			this, SLOT( haveNewQunMessage(qlonglong) ) );
	connect(this, SIGNAL(signal_SysDialogMsg(int, int, qlonglong)), 
			this, SLOT( slot_SysDialogMsg(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_SystemNetErr(int)), 
			this, SLOT( slot_SystemNetErr(int)) );
	connect(this, SIGNAL(signal_DeRegistered()), 
			this, SLOT( slot_DeRegistered()) );
	connect(this, SIGNAL(signal_set_nickname_ok()), this, SLOT(setUINiceName()));
	connect(this, SIGNAL(signal_receive_nudge(qlonglong)), 
			this, SLOT( slot_receive_nudge(qlonglong)));
	connect(this, SIGNAL(signal_set_state(int)), 
			this, SLOT( slot_set_state(int)) );

	connect(this, SIGNAL(signal_add_group(int, int, qlonglong)), 
			buddyMge, SLOT( slot_add_group(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_del_group(int, int, qlonglong)), 
			buddyMge, SLOT( slot_del_group(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_reName_group(int, int, qlonglong)), 
			buddyMge, SLOT( slot_reName_group(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_reName_buddy(int, int, qlonglong)), 
			buddyMge, SLOT( slot_reName_buddy(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_add_buddy(int, int, qlonglong)), 
			buddyMge, SLOT( slot_add_buddy(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_del_buddy(int, int, qlonglong)), 
			buddyMge, SLOT( slot_del_buddy(int, int, qlonglong)) );
	connect(this, SIGNAL(signal_UpdateAcInfo(qlonglong) ), 
			buddyMge, SLOT( updateAccountInfo(qlonglong) ) );
	connect(this, SIGNAL(signal_MoveGroup(qlonglong, int)), 
			buddyMge, SLOT( slot_MoveGroup(qlonglong, int) ) );
}

void FxMainWindow::schedule_SMS()
{
	FxScheduleSMS * test = new FxScheduleSMS(this, this);
	test->show();

	FxScheduleSMSManage *schedule_sms = new FxScheduleSMSManage(this, this);
	schedule_sms->show();
}

void FxMainWindow::showPortrait()
{
	buddySetStatusMenu->exec(QCursor::pos());
}

void FxMainWindow::showNiceNameEdit()
{
	UI_Edit_NiceName->setText(QString::fromUtf8(fx_get_usr_show_name()));
	UI_Edit_NiceName->show();
	UI_Edit_NiceName->setFocus();
}

void FxMainWindow::addBuddy()
{
	this->show();
	tmp_addBuddy = new FxAddBuddy(this);
	tmp_addBuddy->exec();
	//fix a bug when addbuddy show, and mainwindow is hide, quit the addbuddy, 
	//mainwindows will quit too.
	this->show(); 
	delete tmp_addBuddy;
	tmp_addBuddy = NULL;
}

void FxMainWindow::sendgroupsms()
{
	FxSendGroupSMS * groupSms = new FxSendGroupSMS(this, this);
	groupSms->show();
}

void FxMainWindow::sendself()
{
	msgwin->addAccount((qlonglong)strtol(fx_get_usr_uid(), NULL, 10), true);
	/*
	   FxSmsWindow *smswin = new FxSmsWindow(this);
	   smswin->show();
	   */
}

void FxMainWindow::about()
{
	QMessageBox::about(this, tr("About Menu"),
			tr("The <b>Menu</b> example shows how to create "
				"menu-bar menus and context menus."));
}


void FxMainWindow::setImpresa()
{
	bool ok;
	QString text = QInputDialog::getText(this, tr("setImpresa"),
			tr("please input newImpresa"), QLineEdit::Normal,
			"", &ok);
	if (ok)
	{
		if (text.isEmpty())
			fx_set_user_impresa(NULL, NULL, NULL);
		else
			fx_set_user_impresa(text.toUtf8().data(), NULL, NULL); 
	}

	setUINiceName();
}

void FxMainWindow::tmp_exit()
{
	isQuit = true;
	close();
}

void FxMainWindow::setOnlineStatus()
{
	fx_set_user_state(FX_STATUS_ONLINE, NULL, NULL, NULL);
}

void FxMainWindow::setOfflineStatus()
{
	fx_set_user_state(FX_STATUS_OFFLINE, NULL, NULL, NULL);
}
void FxMainWindow::setBusyStatus()
{
	fx_set_user_state(FX_STATUS_BUSY, NULL, NULL, NULL);
}

void FxMainWindow::setAwayStatus()
{
	fx_set_user_state(FX_STATUS_AWAY, NULL, NULL, NULL);
}

void FxMainWindow::setacceptSMS()
{
	fx_set_user_refuse_sms_day(0, NULL, NULL);
}

void FxMainWindow::setrefuseSMS()
{
	this->show();
	FxRefuseSMS * xx = new FxRefuseSMS(this);
	xx->exec();
	delete xx;
}


void FxMainWindow::setPersonalInfo(QTextEdit *AcInfo, const Fetion_Personal *personal)
{
	bool hP = false;
	if(personal)
		hP = true;

	QString info;

	info += tr("mobile_no:");
	if(hP)
		info += "<b style=\"color:red; \">" + 
			QString::fromUtf8(fx_get_usr_mobilenum()) 
			+"</b>";
	else 
		info += "<b style=\"color:red; \"> </b>"; 

	AcInfo->append(info);

	info = tr("fetion_no:");
	info += "<b style=\"color:red; \">"+QString::fromUtf8(fx_get_usr_uid()) +"</b>";
	AcInfo->append(info);

	//AcInfo->append(<b style=\"color:red; \">" + tr("personal infomation")+"</b>");

	info = tr("nickname:");
	if(hP)
		info += "<b style=\"color:red; \">" + 
			QString::fromUtf8(personal->nickname) 
			+"</b>";
	else 
		info += "<b style=\"color:red; \"> </b>"; 
	AcInfo->append(info);

	info = tr("name:");
	if(hP)
		info += "<b style=\"color:red; \">" + 
			QString::fromUtf8(personal->name) 
			+"</b>";
	else 
		info += "<b style=\"color:red; \"> </b>"; 
	AcInfo->append(info);

	info = tr("gender:");
	if(hP)
		switch(personal->gender)
		{
			case 2:
				info += "<b style=\"color:red; \">" + tr("girl") +"</b>";
				break;
			case 1:
				info += "<b style=\"color:red; \">" + tr("boy") +"</b>";
				break;
			case 0:
				info += "<b style=\"color:red; \">" + tr("unknow") +"</b>";
				break;
		}
	else
		info += "<b style=\"color:red; \">" + tr("unknow") +"</b>";
	AcInfo->append(info);

	info = tr("score:");
	info += "<b style=\"color:red; \">" + QString("%1").arg(fx_get_usr_score()) +"</b>";
	AcInfo->append(info);

	info = tr("impresa:");
	if(hP)
		info += "<b style=\"color:red; \">" + 
			QString::fromUtf8(personal->impresa) 
			+"</b>";
	else 
		info += "<b style=\"color:red; \"> </b>"; 
	AcInfo->append(info);
}


void FxMainWindow::personlInfo()
{
	this->showNormal();
	QDialog *window = new QDialog(this);
	window->setWindowTitle(tr("see personal info"));

	QTextEdit *AcInfo = new QTextEdit(window);
	QDialogButtonBox *buttonBox = new QDialogButtonBox(window);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(AcInfo);
	layout->addWidget(buttonBox);
	window->setLayout(layout);
	setPersonalInfo(AcInfo, fx_data_get_PersonalInfo());
	connect(buttonBox, SIGNAL(accepted()), window, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), window, SLOT(reject()));
	window->exec();
	delete window;
}



void FxMainWindow::checkNewVersion()
{
	this->show();

	QString info;
	if (newVersion > CURRENT_VERSION)
		info = 	tr("LibFetion Have New Version,"
		"access http://www.libfetion.cn for more infomation");
	else
		info = 	tr("the current Version is the new Version");

	QMessageBox::about(this,tr("LibFetion"), info);
}

void FxMainWindow::reportBugAct()
{
	QDesktopServices::openUrl(QUrl("http://www.libfetion.cn/bbs"));
}

void FxMainWindow::aboutLibFetion()
{
	this->show();
	QMessageBox::about(this, tr("About LibFetion"),
			tr(	"Application Current Version"  ) + VERSION_NO +
			tr(	"<br>This application is based on LibFetion library to writing, for more infomation access<a href=\"http://www.libfetion.cn\"> www.libfetion.cn </a> <br>"
				"Copyright @ 2008 <b> <a href=\"mailto:dedodong@163.com\">DDD</a> (dedodong@163.com)</b>. All Rights reserved."));
}

void FxMainWindow::aboutCM()
{
	this->show();
	QMessageBox::about(this, tr("About CHINA Mobile"),
			tr("<a href=\"http://www.fetion.com.cn\"> China Mobile Fetion</a>"
				"China Mobile Fetion introduce"));
}

void FxMainWindow::menu_setmute()
{
	bool currentIsMute = Settings::instance().isMute();
	Settings::instance().setMute (!currentIsMute);

	if (currentIsMute)
		MuteAct->setIcon(getMenuIcon(CancelIcon));
	else 
		MuteAct->setIcon(getMenuIcon(ApplyIcon));
}

void FxMainWindow::saveMsgHistroy()
{
	this->show();
	bool ok;
	QString text = QInputDialog::getText(this, tr("saveMsgHistroy"),
			tr("please input password"), QLineEdit::Password,
			"", &ok);
	if (ok && !text.isEmpty())
	{
		if (text == QString(fx_get_usr_passwd()))
		{
			QString fileName = QFileDialog::getSaveFileName(this,
					tr("saveMsgHistroy"),
					QDir::homePath(),
					tr("Text Files (*.htm)"));
			if (!fileName.isEmpty())
			{
				int ret = export_history ((qlonglong)strtol(fx_get_usr_uid(), NULL, 10), 
						fileName.toUtf8().data());
				if (ret == 2 )
					QMessageBox::critical(this, tr("save file error"), tr("the path of file can't write") ); 
			}

		} else 
			QMessageBox::critical(this, tr("error"), tr("the password error") ); 
	}

}

void FxMainWindow::cleanMsgHistroy()
{
	this->show();
	bool ok;
	QString text = QInputDialog::getText(this, tr("cleanMsgHistroy"),
			tr("please input password"), QLineEdit::Password,
			"", &ok);
	if (ok && !text.isEmpty())
	{
		if (text == QString(fx_get_usr_passwd()))
			clean_history ((qlonglong)strtol(fx_get_usr_uid(), NULL, 10));
		else 
			QMessageBox::critical(this, tr("error"), tr("the password error") ); 
	}
}

void FxMainWindow::showConfigDlg()
{
	show();
	FxConfigDia * configdia= new FxConfigDia(this, this);
	configdia->show();
}

void FxMainWindow::menu_setautoshowmsg()
{
	bool currentIsAutoShowMsg = Settings::instance().isAutoShowMsg();
	Settings::instance().setAutoShowMsg (!currentIsAutoShowMsg);

	if (currentIsAutoShowMsg)
		IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));
}

void FxMainWindow::menu_setundgemsg()
{
	QMessageBox::critical(this, ("setundgemsg"), ("setundgemsg") ); 
	fx_send_nudge(630352708L);
}

void FxMainWindow::menu_setlongsms()
{
	bool currentIsEnableLongSMS = Settings::instance().isEnableLongSMS();
	Settings::instance().setEnableLongSMS (!currentIsEnableLongSMS);

	if (currentIsEnableLongSMS)
		SetLongSMSAct->setIcon(getMenuIcon(CancelIcon));
	else 
		SetLongSMSAct->setIcon(getMenuIcon(ApplyIcon));
}

void FxMainWindow::menu_setAutoLogin()
{
	bool currentIsAutoLogin = Settings::instance().isAutoLogin();
	Settings::instance().setAutoLogin(!currentIsAutoLogin);

	if (currentIsAutoLogin)
		AutoLoginAct->setIcon(getMenuIcon(CancelIcon));
	else 
		AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
}

void FxMainWindow::initTrayIcon()
{
	isHaveTray = QSystemTrayIcon::isSystemTrayAvailable();

	if(isHaveTray)
	{
		trayIcon = new QSystemTrayIcon(this);
		trayIcon->setContextMenu(trayIconMenu);
		trayIcon->setIcon(getSysTrayIcon (fx_get_user_state()));
		trayIcon->show();

#if WIN32
		trayIcon->setToolTip (QString::fromUtf8(fx_get_usr_show_name()) + " Win Fetion");
#else
#if MAC_OS
		trayIcon->setToolTip (QString::fromUtf8(fx_get_usr_show_name()) + " Mac Fetion");
#else
		trayIcon->setToolTip (QString::fromUtf8(fx_get_usr_show_name()) + " Linux Fetion");
#endif
#endif
	}
}

void FxMainWindow::UpdateSkinsMenu()
{
	if (!skinMenu) 
		return;
	skinMenu->setIcon(getMenuIcon(SkinIcon));

	QList<QAction *> QAlist = skinMenu->actions(); 

	for (QList<QAction *>::Iterator it = QAlist.begin(); 
			it != QAlist.end(); ++it)
	{
		QAction * action = (*it);
		if (!action)
			continue;
#if MS_VC6
		Skin_Info *sk_info = (Skin_Info *)(action->data().toUInt());
#else
		Skin_Info *sk_info = action->data().value<Skin_Info*>() ;
#endif
		if (!sk_info)
			continue;

		if (sk_info->name == getSkinName() && sk_info->skinpath == getSkinPath())
			action->setIcon(getMenuIcon(ApplyIcon));
		else 
			action->setIcon(getMenuIcon(CancelIcon));

	}

}

void FxMainWindow::UpdateSkins()
{
	msgwin->UpdateSkins();
	buddyMge->UpdateSkins();

	setWindowIcon(getSysTrayIcon(1));
	LibFetion_image->setPixmap(getLibFetionImage());
	UI_Portrait->setPixmap(getPortraitImage());
	UI_ImpresaBK->setPixmap(getImpresaBKImage());
	UI_SearchBK->setPixmap(getSearchBKImage());
	UI_AddFriend->setPixmap(getAddImage());

	UI_BT_SETTING->setPixmap(getBTSettingImage());
	UI_BT_SENDSELF->setPixmap(getBTSendSelfImage());

	if (trayIcon)
		trayIcon->setIcon(getSysTrayIcon (fx_get_user_state()));
	if (traySetStatusMenu)
		traySetStatusMenu->setIcon (getOnlineStatusIcon(fx_get_user_state()));
	if (buddySetStatusMenu)
		buddySetStatusMenu->setIcon (getOnlineStatusIcon(fx_get_user_state()));
	if (msgHistroyMenu) 
		msgHistroyMenu->setIcon(getMenuIcon(HistoryIcon));
	if (traySendSmsMenu)
		traySendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
	if (optSendSmsMenu)
		optSendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));

	UpdateSkinsMenu();

	personlInfoAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
	addBuddyAct->setIcon(getMenuIcon(AddBuddyIcon));
	sendselfAct->setIcon(getMenuIcon(SMSBuddyIcon));
	sendgroupsmsAct->setIcon(getMenuIcon(SMSBuddyIcon));
	exitAct->setIcon(getMenuIcon(ExitIcon));



	OnlineAct->setIcon(getOnlineStatusIcon(FX_STATUS_ONLINE));
	OfflineAct->setIcon(getOnlineStatusIcon(FX_STATUS_OFFLINE));
	BusyAct->setIcon(getOnlineStatusIcon(FX_STATUS_BUSY));
	AwayAct->setIcon(getOnlineStatusIcon(FX_STATUS_AWAY));

	if (fx_get_user_refuse_sms_day()) //was got from libfetion..	
	{
		refuseSMSAct->setIcon(getMenuIcon(ApplyIcon));
		acceptSMSAct->setIcon(QPixmap());
	} else { 
		acceptSMSAct->setIcon(getMenuIcon(ApplyIcon));
		refuseSMSAct->setIcon(QPixmap());
	}

	if (Settings::instance().isEnableLongSMS())
		SetLongSMSAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		SetLongSMSAct->setIcon(getMenuIcon(CancelIcon));

	//SetUndgeMsgAct;
	SaveMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
	CleanMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
	ConfigAppAct->setIcon(getMenuIcon(OptionsIcon));

	CheckNewVersionAct->setIcon(getMenuIcon(AboutIcon));
	ReportBugAct->setIcon(getMenuIcon(AboutIcon));
	AboutLibFetionAct->setIcon(getMenuIcon(AboutIcon));
	AboutCMAct->setIcon(getMenuIcon(AboutIcon));

	if (isAutoLogin(NULL, NULL, NULL))
		AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		AutoLoginAct->setIcon(getMenuIcon(CancelIcon));

	if (Settings::instance().isMute())
		MuteAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		MuteAct->setIcon(getMenuIcon(CancelIcon));

	if (Settings::instance().isAutoShowMsg())
		IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
	else 
		IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));
}

void FxMainWindow::SetAllFont(QFont font)
{
	this->setFont(font);
	msgwin->SetAllFont(font);

	addBuddyAct->setIcon(getMenuIcon(AddBuddyIcon));

	UI_Search->setFont(font);
	UI_Edit_NiceName->setFont(font);
	UI_Impresa->setFont(font);
	UI_NiceName->setFont(font);
	version->setFont(font);

    view->setFont(font);
    search->setFont(font);

    buddyMenu->setFont(font);
    menuSetting->setFont(font);
    menuAbout->setFont(font);

	this->repaint();
}
