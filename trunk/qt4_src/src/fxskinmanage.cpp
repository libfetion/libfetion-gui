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
#include <QApplication>

//the skin path is stored in the settings
QString getSkinPath()
{
    return Settings::instance().SkinPath();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString getSkinName()
{
    return Settings::instance().SkinName();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool setSkins(QString skinPath, QString skinName)
{
    //here checking the skins is usable
    Settings::instance().setSkins(skinPath, skinName);
    setupStyleSheet();

    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool validateSkinPath(QString skinPath)
{
    bool ret = false;
    QFile file(skinPath + SKIN_CONFG_FILE);
    ret = file.open(QIODevice::ReadOnly);
    file.close();

    return ret;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

Skin_Info *get_skininfo(QString skinPath)
{
    QDomDocument doc("xml");

    QFile file(skinPath + SKIN_CONFG_FILE);
    if (!file.open(QIODevice::ReadOnly))
    {
        return NULL;
    }

    if (!doc.setContent(&file))
    {
        file.close();
        return NULL;
    }
    file.close();

    Skin_Info *sk_info = new Skin_Info;
    sk_info->name =
            doc.documentElement().attribute("name", "unset");
    sk_info->author =
            doc.documentElement().attribute("author", "unset");
    sk_info->describe =
            doc.documentElement().attribute("describe", "unset");
    sk_info->usingSystemTitle =
            doc.documentElement().attribute("usingSystemTitle", "yes");
    sk_info->skinpath = skinPath;

    return sk_info;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


Skin_Info *getCurrentSkinInfo()
{
    static Skin_Info *currentSkinInfo = NULL;
    static QString currentSkinPath;

    if (currentSkinPath == getSkinPath())
    {
        if (!currentSkinInfo)
        {
            currentSkinInfo = get_skininfo(getSkinPath());
        }
        return currentSkinInfo;
    }

    currentSkinPath = getSkinPath();
    if (currentSkinInfo)
    {
        delete currentSkinInfo;
    }
    currentSkinInfo = get_skininfo(getSkinPath());

    return currentSkinInfo;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void get_skin_search_result(QList < Skin_Info * >  * items,
                            QString path)
{
    QDir d(path);
    if (!d.exists())
    {
        return ;
    }

    d.setFilter(QDir::Dirs);
    QStringList dirlist = d.entryList();

    for (QStringList::Iterator it = dirlist.begin(); it != dirlist.end(); ++it)
    {
        QString tmpPath = d.path() + "/" + (*it);
        qDebug(tmpPath.toUtf8().data());

        Skin_Info *sk_info = get_skininfo(tmpPath);
        if (sk_info && items)
        {
            items->append(sk_info);
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

static QList < Skin_Info * >  * g_skinlist = NULL;

QList < Skin_Info * >  * searchAllSkins()
{
    if (g_skinlist == NULL)
    {
        g_skinlist = new QList < Skin_Info * > ;
    }
    else
    {
        g_skinlist->clear();
    }

    get_skin_search_result(g_skinlist, SkinPath());
    return g_skinlist;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void setupStyleSheet()
{
    /*
        add some code for style sheet
        shoul be move to class FxSkinManager
        by iptton
     */

    QString path = getSkinPath();
    if (!QFile::exists(path + "/style.css"))
    {
        // has not style.css use the default/style.css
        path = SkinPath() + "/default";
    }

    // FIXME: if it's not and validated directory, just do nothing.
    // not a good user experiment

    QFile file(path + "/style.css");
    file.open(QFile::ReadOnly);
    QString content = file.readAll();

    // do some replace work, it's case insensitive
    content.replace("%skinpath%", SkinPath(), Qt::CaseInsensitive);
    content.replace("%currentPath%", getSkinPath(), Qt::CaseInsensitive);

    // apply the stylesheet
    qApp->setStyleSheet(content);
    file.close();

    //get the res.xml, maybe this file can be included by style.css
    QDomDocument *doc = new QDomDocument("xml");

    path = getSkinPath();
    if (!QFile::exists(path + "/res.xml"))
    {
        path = SkinPath() + "/default";
    }
    // has not res.xml use the default/res.xml

    QFile file1(path + "/res.xml");
    file1.open(QIODevice::ReadOnly);

    if (!doc->setContent(&file1))
    {
        qDebug("error for geting res.xml file, process will crash...");
        file1.close();
        return ;
    }
    file1.close();

    setXMLRes(doc);
}
