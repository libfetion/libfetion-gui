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
#ifndef FXAPPCONFIG_H 
#define FXAPPCONFIG_H 


#include <QMainWindow>
#include <QTreeWidget>
#include <qobject.h>
#include <libfetion/libfetion.h>
#include "fxResource.h"
#include "fxdb.h"
#include "fxsettings.h"
#include "fxutil.h"

#define BUILD_DATE	20090417
#define	CURRENT_VERSION	93
#define VERSION_NO "v0.9.3"

#define MAXSMSLENGTH 180 

#define CONFFILENAME    "libfetion.conf"
#define DBNAME    "libfx.db"
#define SKIN_CONFG_FILE    "/skin.xml"
#define USING_SERVER_TIME 0

#define DEBUG_GUI 0

#ifdef WIN32 
#pragma comment(lib, "./lib/LibFetion.lib")  
#pragma comment(lib, "./lib/libcurl_imp.lib")  
#pragma comment(lib, "Ws2_32.lib") 
#pragma comment(lib, "user32.lib")
#endif

#ifdef WIN32 
#if _MSC_VER < 1300  //vc6 complile 
#define MS_VC6 1 //now vc6 is no support...
#else  //vc7 up 
#define MS_VC6 0
#endif
#else //linux platform
#define MS_VC6 0
#endif //#ifdef WIN32 

#define MOBILE_LOGIN 0x011

#define NO_SET  0
#define NET_ERR 1
#define NEW_MSG 2
#define SYS_DeRegist 3
#define SYS_RELOGIN 4
#define NEW_VERSION 5

typedef struct _Group_Info
{
	QString groupName;
	qlonglong groupID;
	int online_no;
}Group_Info;

typedef struct _Account_Info
{
	QString accountName;
	qlonglong accountID;
	int onlinestate;
    bool haveUpdate;
}Account_Info;

typedef struct _Qun_Info
{
	QString qunName;
	qlonglong qunID;
}Qun_Info;

typedef struct _Skin_Info
{
	QString name;
	QString author;
	QString describe;
	QString skinpath;
}Skin_Info;

#if MS_VC6
#else
Q_DECLARE_METATYPE(Group_Info *);
Q_DECLARE_METATYPE(Account_Info *);
Q_DECLARE_METATYPE(Qun_Info *);
Q_DECLARE_METATYPE(Skin_Info *);
#endif

#endif
