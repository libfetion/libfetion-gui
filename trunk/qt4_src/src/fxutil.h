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

/* stmt quick check macro */
#define FX_RETURN_IF_FAILED(x) do{ if (!x) return; }while(0);
#define FX_RETURN_WITH_VALUE_IF_FAILED(x, v) do{ if (!x) return v; }while(0);

/* DEBUG Logger */
#include "log4qt/logger.h"

#define FX_LOGGER   logger()
#define FX_DEBUG(x) do{logger()->debug(x);}while(0);
#define FX_INFO(x) do{logger()->info(x);}while(0);
#define FX_WARN(x) do{logger()->warn(x);}while(0);
#define FX_ERROR(x) do{logger()->error(x);}while(0);
#define FX_FATAL(x) do{logger()->fatal(x);}while(0);

#ifdef WIN32 
#define FX_FUNCTION FX_DEBUG(__FUNCTION__)
#else
#define FX_FUNCTION FX_DEBUG(__PRETTY_FUNCTION__)
#endif

QString getProvince(QString Province);

QString getCity(int cityID);

bool RegistHotkey(QWidget *window,
                  QChar keyValue,
                  Qt::KeyboardModifiers keyMod);

bool UnRegistHotkey(QWidget *window,
                    QChar keyValue,
                    Qt::KeyboardModifiers keyMod);

QString fxgui_handle_newMsg(Fetion_MSG *fxMsg);

QString fxgui_to_faces(QString newmsg);

QString fxgui_format_time(QString stamp);

void displayAboutLibFetion();


#define PERM 	(S_IRUSR | S_IWUSR)
//#define KEY 	36264
#define KEYPATH	"/tmp"
#define STRKEY	"LIBFETIONKETFORWINDOWS"

int initshared(const char* path, const char* keyString);
int addCount();
int reduceCount();
int detachandremove( void *shmaddr);

int check_dir_state(const char *path);

class m_Instance
{
    private:
        m_Instance();
        ~m_Instance();
        int GetInstancesNum();
	public:
        static int isHaveInstance();
};
#endif
