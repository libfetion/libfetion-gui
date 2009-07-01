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

#ifndef FXQUNWINDOW_H
#define FXQUNWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "ui_qunwindow.h"
#include "appconfig.h"

class FxQunWindow : public QMainWindow, public Ui::FxQunWin
{
    Q_OBJECT

public:
    FxQunWindow(qlonglong qun_id, QWidget *parent = 0,  bool awaySendSms = false);
	~FxQunWindow();
	bool eventFilter(QObject *target, QEvent *event);
	QString getSenderName(qlonglong sender);
	qlonglong qun_id;
	void qun_exit();
	bool isHaveUnReadMsg() {return haveUnreadMsg;}
	void setHaveUnReadMsg(bool flag) { haveUnreadMsg = flag;}
protected:
	void closeEvent(QCloseEvent *event);
	void setQuninfo();
	void setSendModle(bool isSMS);	
	void listQunMem(Fetion_QunInfo *quninfo);
signals:
private slots:
	void changeSendModle();

private:
	const Fetion_Qun * fx_qun;
	bool isQuit;
	bool isAwaySendSMS;
	void SendMsg();
	bool haveUnreadMsg;
};

#endif
