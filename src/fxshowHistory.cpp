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
#include "fxshowHistory.h"

FxShowHistory::FxShowHistory(long uid, QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);
	m_uid = uid;

	connect(todayBT, SIGNAL(clicked()), this, SLOT(today()));
	connect(oneweekBT, SIGNAL(clicked()), this, SLOT(oneweek()));
	connect(twoweekBT, SIGNAL(clicked()), this, SLOT(twoweek()));
	connect(onemonthBT, SIGNAL(clicked()), this, SLOT(onemonth()));
	connect(allBT, SIGNAL(clicked()), this, SLOT(all()));

	today();
	
	char * showname = fx_get_account_show_name(fx_get_account_by_id(uid), FALSE);
	QString show_name = QString::fromUtf8(showname);
	if(showname)
		free(showname);
	QString title = tr("look") + show_name + tr("histroy"); 
	this->setWindowTitle(title);
}

FxShowHistory::~FxShowHistory()
{

}

void FxShowHistory::today()
{
	MsgBrowser->clear();
	MsgBrowser->append(selectHistroyMsg((qlonglong)strtol(fx_get_usr_uid(), NULL, 10), m_uid, TODAY_HISTORY));
}

void FxShowHistory::oneweek()
{
	QString history = selectHistroyMsg(
			(qlonglong)strtol(fx_get_usr_uid(), NULL, 10), 
			m_uid,
			ONE_WEEK_HISTORY);

	if (history.isNull())
		history= tr("no history");

	MsgBrowser->clear();
	MsgBrowser->append(history);
}

void FxShowHistory::twoweek()
{
	QString history = selectHistroyMsg(
			(qlonglong)strtol(fx_get_usr_uid(), NULL, 10), 
			m_uid,
			TWO_WEEK_HISTORY);

	if (history.isNull())
		history= tr("no history");

	MsgBrowser->clear();
	MsgBrowser->append(history);
}

void FxShowHistory::onemonth()
{
	QString history = selectHistroyMsg(
			(qlonglong)strtol(fx_get_usr_uid(), NULL, 10), 
			m_uid,
			ONE_MONTH_HISTORY);

	if (history.isNull())
		history= tr("no history");

	MsgBrowser->clear();
	MsgBrowser->append(history);
}

void FxShowHistory::all()
{
	QString history = selectHistroyMsg(
			(qlonglong)strtol(fx_get_usr_uid(), NULL, 10), 
			m_uid,
			ALL_DATE_HISTORY);

	if (history.isNull())
		history= tr("no history");

	MsgBrowser->clear();
	MsgBrowser->append(history);
}

