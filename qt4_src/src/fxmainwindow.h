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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui_fetionwindow.h"
#include "fxbuddy.h"
#include "fxbuddyManage.h"
#include "fxmsgwindow.h"
#include "fxaddBuddyWindow.h"

#include "fxwidget.h"
using namespace fxgui;

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class FxScheduleSMSManage;
class FxScheduleSMS;

class FxMainWindow : public FxWidget, public Ui::FetionWindow
{
    Q_OBJECT

public:
    FxMainWindow(QWidget *parent = 0);
	~FxMainWindow();
	void handleFx_Sys_Event(int message, WPARAM wParam, LPARAM lParam);
	void handleFx_relogin_Event(int message, WPARAM wParam, LPARAM lParam);

	void addNewMsgCount(bool isQunMsg = false);
	void subNewMsgCount(bool isQunMsg = false);

	void UpdateSkins();
	void UpdateSkinsMenu();
	void SetAllFont(QFont font);

    FxAddBuddy * getTmpAddBuddyWindow() {return tmp_addBuddy;}  //save the tmp addbuddy handle
    FxMsgWindow * getMsgWindow() {return msgwin;}
    FxScheduleSMSManage *getScheduleSmsManagerWindows() { return scheduleSmsManager;}
    FxScheduleSMS *getScheduleSmsWindow() { return scheduleSms; }
protected:
#if WIN32
    virtual bool winEvent(MSG *msg, long *result);
#else
	bool event ( QEvent * event );
#endif
	void closeEvent(QCloseEvent *event);
	void moveEvent(QMoveEvent * event);

	void startFlickerTray();
	void endFlickerTray();

	//this function will beremoved in later version
	void setPersonalInfo(QTextEdit *AcInfo, const Fetion_Personal *personal);


	bool showNewMsgDlg();

	void createSkinMenu(QMenu *skinMenu);		


signals:
	void signal_Current_Version(int);
	void signal_AddAccountApp(char*, char*);
	void signal_MoveGroup(qlonglong, int);
	void signal_NewSysMsg(qlonglong account_id);
	void signal_NewMsg(qlonglong account_id);
	void signal_NewQunMsg(qlonglong qun_id);
	void signal_UpdateAcInfo(qlonglong account_id);
	void signal_SysDialogMsg(int, int, qlonglong);
	void signal_SystemNetErr(int);
	void signal_DeRegistered();
	void signal_receive_nudge(qlonglong);
	void signal_reName_group(int, int, qlonglong);
	void signal_reName_buddy(int, int, qlonglong);
	void signal_add_group(int, int, qlonglong);
	void signal_del_group(int, int, qlonglong);

	void signal_add_buddy(int, int, qlonglong);
	void signal_del_buddy(int, int, qlonglong);
	void signal_set_state(int);
	void signal_set_nickname_ok();
	void signal_UpdateSmsDay(int);
    void signal_update_scheduleList();

private slots:
	void checkSkinPath();
	void minimizedWind();
	void flickerTray();
	void relogin_fetion();
	void relogin_timer();
    void updateAccountInfo_timer();
	void tmp_exit();
	void personlInfo();
	void addBuddy();
	void sendself();
	void sendgroupsms();
	void about();
	void showMenu();
	void searchaccountDoubleClicked (QTreeWidgetItem * item, int column );
	void haveCurrentVersionMessage(int);
	void haveAddAccountAppMessage(char* uri, char*desc);
	void slot_SystemNetErr (int);
	void slot_DeRegistered ();

	void slot_set_state(int);

	void slot_updateSmsDay(int);

	void slot_ShowSkinMenu() { createSkinMenu(skinMenu); }



    void setImpresa();



	void checkNewVersion();
	void reportBugAct();
	void aboutLibFetion();
	void aboutCM();
    void setOnlineStatus();
    void setOfflineStatus();
    void setBusyStatus();
    void setAwayStatus();
	void setacceptSMS();
	void setrefuseSMS();

	void trayMessageClicked();
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void menu_setAutoLogin();
	void menu_setmute();
	void menu_setautoshowmsg();
	void menu_setlongsms();
	void saveMsgHistroy();
	void cleanMsgHistroy();
	void showConfigDlg();
	

	void UI_enable_impresa();
	void UI_enable_search();

	void skinMenutriggered(QAction *);
	void SearcheditingFinished();
	void SearchtextChanged (const QString &text);
	void changeNickName(QString);
	void changeImpresa(QString);
	void ImpresaFocusIn();
	void SearchFocusIn();
	void showPortrait();
	void setUINickName();

	void schedule_SMS();
private:
    void init_UI();
    void initAllActions();
    void createMenu();
	void initTrayIcon();
	void init_slot_signal();

private:
	int newVersion;
	int new_msg_count;
	int new_qun_msg_count;
	int fx_status;
	bool isHaveTray;
	bool isQuit;
	bool isBreakOut;
	QString m_impresa;


	QSystemTrayIcon *trayIcon;
	QMenu *mainMenu;
	QMenu *trayIconMenu;
	QMenu *reloginTrayMenu;
	QMenu *traySetStatusMenu;
	QMenu *buddySetStatusMenu;
	QMenu *msgHistroyMenu;
	QMenu *skinMenu;
	QMenu *traySendSmsMenu; //tray menu of send sms
	QMenu *optSendSmsMenu; //opt menu of send sms

	QAction *Schedule_SMS_Act;
    QAction *personlInfoAct;
    QAction *addBuddyAct;
    QAction *sendselfAct;
    QAction *sendgroupsmsAct;
    QAction *exitAct;

    QAction *OnlineAct;
    QAction *OfflineAct;
    QAction *BusyAct;
    QAction *AwayAct;
    QAction *acceptSMSAct;
	QAction *refuseSMSAct;


	QAction *SetLongSMSAct;
	QAction *SaveMsgHistroyAct;
	QAction *CleanMsgHistroyAct;
	QAction *ConfigAppAct;

	QAction *CheckNewVersionAct;
	QAction *ReportBugAct;
    QAction *AboutLibFetionAct;
    QAction *AboutCMAct;

    bool isNeedRecordWinPos;
    bool isHaveminimized;
    QTimer checkSkinsTimer;
    QTimer minimizedTimer;
    QTimer trayFlickTimer;
    QTimer reloginTimer;
    QTimer updateAccountInfoTimer;

    FxAddBuddy *tmp_addBuddy;  //save the tmp addbuddy handle
    FxMsgWindow *msgwin;
    BuddyMge *buddyMge; 
    FxScheduleSMS *scheduleSms;
    FxScheduleSMSManage *scheduleSmsManager; 
public:
    QAction *AutoLoginAct;
    QAction *MuteAct;
	QAction *IsAutoShowMsgAct;
	
	QMenu *buddyMenu;
	QMenu *menuSetting;
	QMenu *menuAbout;
};

#endif
