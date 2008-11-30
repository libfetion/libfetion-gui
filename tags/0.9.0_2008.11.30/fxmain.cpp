/***************************************************************************
 *   Copyright (C) 2008 by DDD                                          *
 *   dedodong@163.com                                                     *
 *                                                                         *
 *
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
#include "fxmain.h"
#include "fxloginwindow.h"
#include "fxmainwindow.h"

//#include "fxqunwindow.h"


FxMain::FxMain() 
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	
	isLoginIn = false;

	/*
	   QMessageBox::about(mainWin, ("test version for v 0.2.0"),
	   ("Thank you for testing. your working will make Libfetion more strong!!"));
	   */
	loginWin = new FxLoginWindow(0);
	doSlotConnection();
	loginWin->show();
}

FxMain::~FxMain() 
{
	if(isLoginIn)
	{
		printf("fx_loginout \n");
		fx_loginout();
	}
}

void FxMain::doSlotConnection()
{
	QObject::connect(loginWin, SIGNAL(signal_LoginOK()), this, SLOT(slotLoginOK()));
}

void FxMain::slotLoginOK()
{
	isLoginIn = true;
	mainWin = new FxMainWindow(0);
	loginWin->hide();
	mainWin->show();
	delete loginWin; 
}

