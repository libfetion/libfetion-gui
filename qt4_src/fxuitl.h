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

#ifndef FXUITL_H
#define FXUITL_H
#include <stdlib.h>
#include <string.h>
#include <QString>
#include "appconfig.h"

bool RegistHotkey(QWidget *window, QChar keyValue, Qt::KeyboardModifiers keyMod);
bool UnRegistHotkey(QWidget *window, QChar keyValue, Qt::KeyboardModifiers keyMod);
QString fxgui_handle_newMsg(Fetion_MSG *fxMsg);
QString fxgui_to_faces(QString newmsg);

#endif