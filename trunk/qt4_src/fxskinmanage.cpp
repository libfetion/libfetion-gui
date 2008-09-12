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
#include <QDir>
#include <QtXml/QDomDocument>

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

Skin_Info *get_skininfo(QString skinpath)
{
	QDomDocument doc("xml");
	QFile file(skinpath + SKIN_CONFG_FILE); 

	if (!file.open(QIODevice::ReadOnly))
		return NULL;

	if (!doc.setContent(&file)) 
	{
		file.close();
		return NULL;
	}
	file.close();

	Skin_Info *sk_info = new Skin_Info;
	sk_info->name = doc.documentElement().attribute("name", "unset");
	sk_info->author = doc.documentElement().attribute("author", "unset");
	sk_info->describe = doc.documentElement().attribute("describe", "unset");
	sk_info->skinpath = skinpath;
#if 0
	qDebug(sk_info->name.toUtf8().data());
	qDebug(sk_info->author.toUtf8().data());
	qDebug(sk_info->describe.toUtf8().data());
	qDebug(sk_info->skinpath.toUtf8().data());
#endif
	return sk_info;
}

void get_skin_search_result(QList<Skin_Info *>  *items, QString path)
{
	QDir d(path);
	if (!d.exists())
		return;

	d.setFilter(QDir::Dirs);
	QStringList dirlist = d.entryList();

	for (QStringList::Iterator it = dirlist.begin(); it != dirlist.end(); ++it)
	{
		QString tmpPath = d.path() +"/"+ (*it);
		//qDebug(tmpPath.toUtf8().data());

		Skin_Info *sk_info = get_skininfo(tmpPath);
		if (sk_info && items)
			items->append(sk_info);
	}
}

QList<Skin_Info *> *searchAllSkins()
{
	QList<Skin_Info *> *items = new QList<Skin_Info *>;
	get_skin_search_result(items, SkinPath());
	return items;
}
