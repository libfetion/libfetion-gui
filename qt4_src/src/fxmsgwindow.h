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

#ifndef __FX_MSGWINDOW_H
#define __FX_MSGWINDOW_H

#include <QDialog>
#include <QTabWidget>

#include "appconfig.h"
#include "fxaccountTab.h"
#include "ui_msgwindow.h"
#include "ui_msgsend.h"

#include "fxmytabwidget.h"
#include "fxqunwindow.h"
#include "fxInputFace.h"

#include "fxwidget.h"
using namespace fxgui;

class FxMainWindow;

//class FxMsgWindow : public QDialog, public Ui::MsgWindow
class FxMsgWindow : public FxWidget, public Ui::MsgWindow
{
    Q_OBJECT

public:
    FxMsgWindow(QWidget *parent = 0);
    ~FxMsgWindow();
	void setMainWind(FxMainWindow *wind) { m_mainwindow = wind;}
	void UpdateSkins();
	void SetAllFont(QFont font);

public:
	void showFaces();
	void msg_exit();

	void nudge_shake();
	void addAccount(qlonglong account_id, bool isSendSms = false);
	bool addMessage(QString msg, qlonglong account_id, bool iscomeing_msg = false);

	void addQunWin(qlonglong qun_id, bool isSendSms = false);
	bool addQunMessage(QString msg, qlonglong qun_id, qlonglong sender, bool iscomeing_msg = false);

	FxQunWindow *findQunWindow(qlonglong qun_id);

    void updateAccountInfo(qlonglong account_id);
protected:
	void closeEvent(QCloseEvent *event);
	void moveEvent(QMoveEvent * event);
	void resizeEvent ( QResizeEvent * event);
	void addBuddy(qlonglong account_id);

	void handle_sendmsg(int msgflag, int fx_msg, qlonglong who);

public slots:
	void currentChangedTab(int index); 
	void setCurrentTabTitle(AccountTab *accountTab); 
	void closeTabWid(int index);
	void closeCurrentTab();
	void slot_do_shake();

	void slot_SysDialogMsg (int, int, qlonglong);

	void slot_haveNewSysMessage (qlonglong sys_id);
	void slot_haveNewMessage (qlonglong account_id);
	void slot_haveNewQunMessage(qlonglong qun_id);

signals:

private:
	void init();
	void init_inputFace();
	void init_UI();
	void exec_autoRelpy(QTextEdit* msgBrowser, qlonglong account_id, QString msg);

private:
	bool m_willQuit;
	QTimer *timer;
	QTimer nudge_timer;
	bool m_isNudgeShake;
	FxMainWindow *m_mainwindow;
	QToolButton *closeTabButton;
	FxInputFace  *inputFace;
	QList<FxQunWindow *> qunWindow;
	QVector<int> timeOutMsgVector; //stroe the timeout msg.
};



#endif
