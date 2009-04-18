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
#ifndef _FXSENDGROUPSMS_H
#define _FXSENDGROUPSMS_H

#include <QMainWindow>
#include <QDialog>
#include "fxbuddy.h"
#include "fxmainwindow.h"
#include "ui_sendgroupsms.h"



class FxSendGroupSMS : public QMainWindow, public Ui::SendGroupSMS
{
    Q_OBJECT
public:
   FxSendGroupSMS(FxMainWindow * mainwind, QWidget *parent = 0);
    ~FxSendGroupSMS();

protected:
	void closeEvent(QCloseEvent *event);
	void moveEvent(QMoveEvent * event);
private slots:
	void ChangechooseNM();
	void ChangeInputNM();
	void SendMsg();
private:
	BuddyOpt *buddyopt; 
	FxMainWindow * mainwind;
};

#endif
