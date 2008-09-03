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

class FxMainWindow;

//class FxMsgWindow : public QDialog, public Ui::MsgWindow
class FxMsgWindow : public QMainWindow, public Ui::MsgWindow
{
    Q_OBJECT

public:
    FxMsgWindow(QWidget *parent = 0);
    ~FxMsgWindow();
	void addAccount(qlonglong account_id, bool isSendSms = false);
	void haveNewMessage(qlonglong account_id);
	bool addMessage(QString msg, qlonglong account_id, bool iscomeing_msg = false);

	void msg_exit();
	void setMainWind(FxMainWindow *mainwindow);

	void exec_autoRelpy(QTextEdit* msgBrowser, qlonglong account_id, QString msg);
	void addQunWin(qlonglong qun_id, bool isSendSms = false);
	void haveQunMessage(qlonglong qun_id);
	bool addQunMessage(QString msg, qlonglong qun_id, qlonglong sender, bool iscomeing_msg = false);

	void showFaces();

	FxInputFace  *inputFace;
	
	QList<FxQunWindow *> qunWindow;
	FxQunWindow *findQunWindow(qlonglong qun_id);

protected:
	void closeEvent(QCloseEvent *event);
	void moveEvent(QMoveEvent * event);
	void resizeEvent ( QResizeEvent * event ) ;

public slots:
	void currentChangedName(int index); 
	void closeTabWid(int index);
	void closeTab();

signals:

private:
	bool isQuit;
	QTimer *timer;
	FxMainWindow *mainwindow;
};



#endif
