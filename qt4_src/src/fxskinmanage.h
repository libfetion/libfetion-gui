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
#ifndef _FX_SKIN_MANAGE_H
#define _FX_SKIN_MANAGE_H
#include "appconfig.h"

bool validateSkinPath(QString skinPath);
QString getSkinPath();
QString getSkinName();
bool setSkins(QString skinPath, QString skinName);

QList<Skin_Info *> *searchAllSkins();
void get_skin_search_result(QList<Skin_Info *>  *items, QString path);

void setupStyleSheet();

Skin_Info * getCurrentSkinInfo();
#endif
