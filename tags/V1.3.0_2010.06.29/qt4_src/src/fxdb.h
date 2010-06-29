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
#ifndef FXDB_H
#define FXDB_H

#include <stdio.h>

#if WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "sqlite/sqlite3.h"

#include <QString>
#include <QList>
#include <QTreeWidgetItem>

#include "fxglobal.h"

typedef enum
{
    TODAY_HISTORY = 0,
    ONE_WEEK_HISTORY,
    TWO_WEEK_HISTORY,
    ONE_MONTH_HISTORY,
    ALL_DATE_HISTORY,
} HISTORY_DATE;

typedef enum
{
    NO_INCOMING = 0,
    INCOMING,
} INCOMING_TYPR;

bool init_db();
void destoy_db();

void disAutoLogin();
bool isAutoLogin(char** id, char** pwd, int* loginState);
bool setAutoLogin(const char* id, const char* pwd, int loginState);

void disProxy();
bool isSetProxy(PROXY_ITEM *proxy_item);
bool setProxy(PROXY_ITEM *proxy_item);

char *getCacheServerAdd(const char* id);
bool setCacheServerAdd(const char* id, const char* prxy);
void cleanCacheServerAdd();

void saveHistroyMsg(long usr, long uid, const char* msg, const char* date);
QString selectHistroyMsg(long usr, long uid, HISTORY_DATE type);
int export_history(long uid, const char* filename);
bool clean_history(long uid);

bool selectSystemMsg(long usr, long uid, const char* msg);

bool GetAccountInfoFromDB(long uid, QString& nicename, QString& impresa);
void UpdateAccountToDB(const Fetion_Account *account);

QList<QTreeWidgetItem *> *searchAccountInfo(char * keyword);

#endif
