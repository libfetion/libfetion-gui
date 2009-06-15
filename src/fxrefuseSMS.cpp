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
#include "fxrefuseSMS.h"

FxRefuseSMS::FxRefuseSMS(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);

	smsday = 1;
	connect(RB_24, SIGNAL(clicked()), this, SLOT(RB_changed()));
	connect(RB_3D, SIGNAL(clicked()), this, SLOT(RB_changed()));
	connect(RB_1W, SIGNAL(clicked()), this, SLOT(RB_changed()));
	connect(RB_FE, SIGNAL(clicked()), this, SLOT(RB_changed()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(setRefuseSMS()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void FxRefuseSMS::setRefuseSMS()
{
	fx_set_user_refuse_sms_day (smsday, NULL, NULL);
	this->accept();
}

FxRefuseSMS::~FxRefuseSMS()
{
}


void FxRefuseSMS::RB_changed()
{
	setChanged();
}

void FxRefuseSMS::setChanged()
{
	if (RB_24->isChecked())
	{
		smsday = 1;
		RB_3D->setChecked (false);
		RB_1W->setChecked (false);
		RB_FE->setChecked (false);
		return ;
	}

	if (RB_3D->isChecked())
	{
		smsday = 3;
		RB_24->setChecked (false);
		RB_1W->setChecked (false);
		RB_FE->setChecked (false);
		return ;
	}
	if (RB_1W->isChecked ())
	{
		smsday = 7;
		RB_3D->setChecked (false);
		RB_24->setChecked (false);
		RB_FE->setChecked (false);
		return ;
	}

	if (RB_FE->isChecked ())
	{
		smsday = 36000;
		RB_3D->setChecked (false);
		RB_1W->setChecked (false);
		RB_24->setChecked (false);
		return ;
	}
}
