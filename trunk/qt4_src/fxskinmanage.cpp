/***************************************************************************
 *   Copyright (C) 2008 by DDD                                             *
 *   dedodong@163.com                                                      *
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

#include "fxskinmanage.h"

//the skin path is stored in the settings
QString getSkinPath()
{
	return	Settings::instance().SkinPath();
}

QString getSkinName()
{
	return	Settings::instance().SkinName();
}

bool setSkins(QString skinPath, QString skinName)
{
	//here checking the skins is usable
	//
	Settings::instance().setSkins(skinPath, skinName);
	return	true;
}

static void get_skin_search_result(QList<Skin_Info *>  *items)
{
#if 0
	Skin_Info *sk_info = new Skin_Info;
	Skin_Info->name;
	Skin_Info->author;
	Skin_Info->describe;
	Skin_Info->skinpath;
	items->append(accountItem);
#endif
}

QList<Skin_Info *> *searchAllSkins()
{
	QList<Skin_Info *> *items = new QList<Skin_Info *>;
	get_skin_search_result(items);
	return items;
}
