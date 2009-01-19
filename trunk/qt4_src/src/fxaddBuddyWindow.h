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
#ifndef _FXADDBUDDWINDOWS_H
#define _FXADDBUDDWINDOWS_H

#include <QDialog>
#include "ui_addBuddy.h"

#include "appconfig.h"

class FxAddBuddy : public QDialog, public Ui::AddBuddyDia
{
    Q_OBJECT
public:
   FxAddBuddy(QWidget *parent = 0);
   FxAddBuddy(QString id, bool ismobile = false, QWidget *parent = 0);
    ~FxAddBuddy();
	void setChanged();
	void init_groupItem();

private slots:
	void init();
	void getPersInfo();
	void newgroup();
	void mobileRB();
	void fetionRB();
	void addfriend();
private:
	Fetion_Account *m_account;
};

#endif
