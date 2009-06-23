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
#ifndef FX_ACCOUNTTAB_H
#define FX_ACCOUNTTAB_H

#include <QDialog>
#include <QTabWidget>

#include "appconfig.h"
#include "ui_msgsend.h"
#include "fxmytabwidget.h"
#include "fxshowHistory.h"

#define SYSTEM_ID 10000
class FxMainWindow;

class AccountTab : public QWidget, public Ui::MsgSend
{
    Q_OBJECT

public:
	AccountTab(qlonglong id, FxMyTabWidget *parent = 0, bool awaySendSms = false);
	~AccountTab();
	bool eventFilter(QObject *target, QEvent *event);
	qlonglong account_id;
	QString account_name;

	void UpdateSkins();
	void SetAllFont(const QFont & font);
	void startFlickerTab();
	void endFlickerTab();
	bool isAwaySendSMS;
	const Fetion_Account *m_account;
	//MsgSend *msgSend;
	AccountTab * msgSend;

	void setMainWind( FxMainWindow *wind) {mainWind = wind;}
	void setSendModle(bool isSMS);
	void handle_alt_num( QKeyEvent *keyEvent);


private slots:
	void flickerTab();
	void changeSendModle();
	void SendNudge();
	void SendMsg();
	void ShowHistroy();
	void ShowFaces();
	void changeTableInputNM();

protected:
	void resizeEvent ( QResizeEvent * event ) ;
	void init();
	void init_slot();

private:
	bool isSendToSelf;
	FxMyTabWidget *tabWidget;
	QTimer flickTimer;
	bool flick_flag;
	FxMainWindow *mainWind;
	FxShowHistory *histroy;
};

#endif
