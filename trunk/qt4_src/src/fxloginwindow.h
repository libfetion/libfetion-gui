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

#ifndef FXLOGINDIALOG_H
#define FXLOGINDIALOG_H

#include <QDialog>
#include <QTimer>

#include "ui_login_window.h"
#include "fxproxy.h"
#include "fxwidget.h"
using namespace fxgui;

class FxLoginWindow : public FxWidget, public Ui::Fetion_Login
{
    Q_OBJECT

public:
    FxLoginWindow(QWidget *parent = 0);
    ~FxLoginWindow();

	void init();
	void checkAutoLogin();

	void LoginOK();

	void enableLoginBT();
	void UpdateSkins();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void BT_Login_clicked();
	void slots_enableLonginBT();
	void Setting(const QString &);
	void login_timer();

	void login();

signals:
	void signal_LoginOK();
	void signal_enableLoginBT();

private:
	void set_login_button_state(bool state);
	void Cancel_logwin();
	void setLogingState(char *ch);

private:
	char *user_id;
	char *user_pwd;
	FxProxy *proxy; 
	QTimer loginTimer;
	bool willLogin;
};

#endif
