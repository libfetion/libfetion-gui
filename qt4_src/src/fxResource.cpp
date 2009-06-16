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
#include <QSound>
#include <QFile>
#include <QDir>
#include "fxResource.h"
#include "fxskinmanage.h"

#ifdef WIN32
#else
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#endif


#include <QDebug>


#ifdef WIN32
#else
QString currentUserPath()
{ 
    static QString currentUserPath;
    static bool init = false;
    if (init)
        return currentUserPath;

    char* CONF_FILE = (char*) malloc(sizeof(char)*(512));
    memset(CONF_FILE, 0, 512);

    struct passwd *pwd;
    if ((pwd = getpwuid (geteuid ())) != NULL) {
        strcpy (CONF_FILE, pwd->pw_dir);
        if (CONF_FILE[ strlen(CONF_FILE) - 1] != '/')
            strcat (CONF_FILE, "/");
        strcat (CONF_FILE, ".");
    }
    currentUserPath = CONF_FILE ;
    free(CONF_FILE);

    init = true;
    return currentUserPath;
}

void moveOldConfigFile()
{
    bool done = false;
    if (done)
        return;

    done = true;

    //create the .libfetion folder
    QDir home(currentUserPath());
    home.mkdir(".libfetion");

    //move old config file to .libfetion folder
    if (QFile::exists(currentUserPath() + DBNAME) && !QFile::exists(chatDBFile()))
    {
        QFile::rename(currentUserPath()+"/."DBNAME, chatDBFile());
    }

    if (QFile::exists(currentUserPath()+"/."CONFFILENAME) && !QFile::exists(configFile()))
    {
        QFile::rename(currentUserPath()+"/."CONFFILENAME, configFile());
    }
}

#endif

QString defaultResPath()
{
#ifdef WIN32
	return ".";
#else //linux
	static QString defaultResPath;
	static bool init = false;
	FILE* fp;

	if(init)
		return defaultResPath;

    //here we set the CREDITS.txt as an identifying item
	  if ((fp = fopen("./CREDITS.txt", "r")))
	  {
		  fclose (fp);
		  defaultResPath = ".";
	  }	
	  else
		  defaultResPath = "/usr/share/libfetion";

	init = true;
	return defaultResPath;
#endif
}

QString dataResPath()
{
#ifdef WIN32
	return ".";
#else //linux
    return currentUserPath() + "/.libfetion" ;
#endif
}

QString configFile()
{
    return dataResPath() + "/"CONFFILENAME;
}

QString chatDBFile()
{
	return dataResPath() + "/"DBNAME;
}

QString SkinPath()
{
    return defaultResPath() + "/skins";
}

QString defaultSkinPath()
{
	return SkinPath() + "/default";
}

QString defaultSoundPath()
{
    return defaultResPath() + "/sound";
}

void execPlaySound(QString music)
{
#ifdef Q_OS_MAC //Mac OS X platfrom
	QSound::play(music);
#else
	#ifdef WIN32 //windows platfrom
	QSound::play(music);
	#else //linux platfrom
	// cjacker(cjacker@gmail.com) implement it on linux of ubuntu
	QString cmd;
	cmd = "aplay "+ music + "&";
	system(cmd.toStdString().c_str());
	#endif
#endif
}

void playSound(SOUND_TYPE type)
{
	switch(type)
	{
		case MSG_SOUND:
			execPlaySound (Settings::instance().MsgRingPath());
			break;
		case ONLINE_SOUND:
			execPlaySound(defaultSoundPath() +"/online.wav");
			break;
		case SYS_SOUND:
			execPlaySound(defaultSoundPath() +"/sys.wav");
			break;
	}
}

QPixmap getOnlineStatusIcon(int status)
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/online/offline.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	
	switch(status)
	{
		case 0:  //pc offline
			return QPixmap(getSkinPath() + "/online/offline" + postfix);

		case FX_STATUS_BLACK:
		case FX_STATUS_BLACK + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/online/black" + postfix);
		case FX_STATUS_MOBILE: //mobile user
		case FX_STATUS_MOBILE + MOBILE_LOGIN: //mobile user
			return QPixmap(getSkinPath() + "/online/mobile" + postfix);
		case FX_STATUS_WAITING_AUTH:  //the account waiting   
		case FX_STATUS_WAITING_AUTH + MOBILE_LOGIN:  //the account waiting   
			return QPixmap(getSkinPath() + "/online/waiting" + postfix);
		case FX_STATUS_REFUSE + MOBILE_LOGIN:   //the account is refuse make friends with you
		case FX_STATUS_REFUSE:   //the account is refuse make friends with you
			return QPixmap(getSkinPath() + "/online/refuse" + postfix);
		case FX_STATUS_OFFLINE:
			return QPixmap(getSkinPath() + "/online/offline" + postfix);
		case FX_STATUS_DINNER:
			return QPixmap(getSkinPath() + "/online/dinner" + postfix);
		case FX_STATUS_AWAY:
			return QPixmap(getSkinPath() + "/online/away" + postfix);
		case FX_STATUS_ONLINE:
			return QPixmap(getSkinPath() + "/online/online" + postfix);
		case FX_STATUS_PHONE:
			return QPixmap(getSkinPath() + "/online/phone" + postfix);
		case FX_STATUS_BUSY:
			return QPixmap(getSkinPath() + "/online/busy" + postfix);
		case FX_STATUS_MEETING :
			return QPixmap(getSkinPath() + "/online/meeting" + postfix);
		case FX_STATUS_EXTENDED_AWAY:
			return QPixmap(getSkinPath() + "/online/extnded_away" + postfix);
		case FX_STATUS_NUM_PRIMITIVES:
			return QPixmap(getSkinPath() + "/online/num_primitives" + postfix);

		
		case FX_STATUS_ONLINE + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/online/m_online" + postfix);
		case FX_STATUS_DINNER + MOBILE_LOGIN:
		case FX_STATUS_AWAY + MOBILE_LOGIN:
		case FX_STATUS_EXTENDED_AWAY + MOBILE_LOGIN:
		case FX_STATUS_NUM_PRIMITIVES + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/online/m_away" + postfix);
		case FX_STATUS_PHONE + MOBILE_LOGIN:
		case FX_STATUS_MEETING + MOBILE_LOGIN:
		case FX_STATUS_BUSY + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/online/m_busy" + postfix);
	}

	return QPixmap();
}

QPixmap getSysTrayIcon(int status)
{
QString postfix;
	if(QFile::exists(getSkinPath() + "/online/offline.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	  
	switch(status)
	{
	case 0:  //no login
		return QPixmap(getSkinPath() + "/systray/offline" + postfix);		
	case 1: //login
	case FX_STATUS_ONLINE:
		return QPixmap(getSkinPath() + "/systray/online" + postfix);
	case FX_STATUS_OFFLINE:
		return QPixmap(getSkinPath() + "/systray/hide" + postfix);
	case FX_STATUS_DINNER:
	case FX_STATUS_AWAY:
	case FX_STATUS_MEETING :
	case FX_STATUS_EXTENDED_AWAY:
	case FX_STATUS_NUM_PRIMITIVES:
		return QPixmap(getSkinPath() + "/systray/away" + postfix);
	case FX_STATUS_PHONE:
	case FX_STATUS_BUSY:
		return QPixmap(getSkinPath() + "/systray/busy" + postfix);
	}
	return QPixmap(getSkinPath() + "/systray/busy" + postfix);
}

QPixmap getMenuIcon(int menuID)
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/menu/im.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	switch(menuID)
	{
		case IMBuddyIcon:  //IM
			return QPixmap(getSkinPath() + "/menu/im" + postfix);
		case SMSBuddyIcon: //SMS
			return QPixmap(getSkinPath() + "/menu/sms" + postfix);
		case GetInfoBuddyIcon: //GetInfo
			return QPixmap(getSkinPath() + "/menu/profile" + postfix);
		case ReNameBuddyIcon: //rename
			return QPixmap(getSkinPath() + "/menu/edit" + postfix);
		case AddBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/addfriend" + postfix);
		case AboutIcon:
			return QPixmap(getSkinPath() + "/menu/about" + postfix);
		case ExitIcon:
			return QPixmap(getSkinPath() + "/menu/exit" + postfix);
		case ApplyIcon:
		case RemoveBlackIcon:
			return QPixmap(getSkinPath() + "/menu/apply" + postfix);
		case CancelIcon:
			return QPixmap(getSkinPath() + "/menu/cancel" + postfix);
		case SetImpresaIcon:
			return QPixmap(getSkinPath() + "/menu/edit" + postfix);
		case AddGroupIcon:
			return QPixmap(getSkinPath() + "/menu/addqun" + postfix);
		case BackInBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/backlist" + postfix);
		case DeleteBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/delete" + postfix);
		case MoveIcon:
			return QPixmap(getSkinPath() + "/menu/move" + postfix);
		case RefreshBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/refresh" + postfix);
		case OptionsIcon:
			return QPixmap(getSkinPath() + "/menu/options" + postfix);
		case HistoryIcon:
			return QPixmap(getSkinPath() + "/menu/history" + postfix);
		case SkinIcon:
			return QPixmap(getSkinPath() + "/menu/skin" + postfix);
	}
	return QPixmap();
}

QPixmap getQunIcon()
{
			QString postfix;
	if(QFile::exists(getSkinPath() + "/misc/qun.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/misc/qun" + postfix);
}

QPixmap getBT_SMSIcon()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/misc/bt_sms.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/misc/bt_sms" + postfix);
}

QPixmap getFlickIcon(bool flag)
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/systray/online.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	  
	if(flag)
		return QPixmap(getSkinPath() + "/systray/online" + postfix);
	else
		return QPixmap(getSkinPath() + "/systray/online_flick" + postfix);
}

QPixmap getInputFaceIcon()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/misc/input_face.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/misc/input_face" + postfix);
}

QPixmap getFaceIcon()
{
			QString postfix;
	if(QFile::exists(getSkinPath() + "/misc/tool_face.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/misc/tool_face" + postfix);
}

QPixmap getNudgeIcon()
{
			QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/nudge_button.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/nudge_button" + postfix);
}

QPixmap getSendIcon()
{
				QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/send_button.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/send_button" + postfix);
}

QPixmap getHistoryIcon()
{
				QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/history.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/history" + postfix);
}

QPixmap getChangeSendModIcon()
{
			QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/change_send_mode.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/change_send_mode" + postfix);
}

QPixmap getCloseTabImage()
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/closetab.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/closetab" + postfix);
}

QPixmap getLoginImage()
{
    QString postfix;
    if(QFile::exists(getSkinPath() + "/theme/login_image.png"))	
        postfix = ".png";
    else
        postfix = ".gif";
    return QPixmap(getSkinPath() + "/theme/login_image" + postfix);
}

QPixmap getPortraitImage()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/portrait-mac.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
#ifdef Q_OS_MAC //Mac OS X platfrom
	return QPixmap(getSkinPath() + "/theme/portrait-mac" + postfix);
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(getSkinPath() + "/theme/portrait-win" + postfix);
	#else //linux platfrom
		return QPixmap(getSkinPath() + "/theme/portrait-linux" + postfix);
	#endif
#endif
}

QPixmap getBTSettingImage()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/bt_setting.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/bt_setting" + postfix);
}

QPixmap getBTSendSelfImage()
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/bt_sendself.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/bt_sendself" + postfix);
}

QPixmap getAddImage()
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/addfriend.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/addfriend" + postfix);
}

QPixmap getImpresaBKImage()
{
	QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/impresabk.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/impresabk" + postfix);
}

QPixmap getSearchBKImage()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/search.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/search" + postfix);
}

QPixmap getLibFetionImage()
{
			QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/LibFetion.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/LibFetion" + postfix);
}

QPixmap getLogion_InImage()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/Logion_In.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/Logion_In" + postfix);
}

QPixmap getLogin_CancelImage()
{
		QString postfix;
	if(QFile::exists(getSkinPath() + "/theme/Login_Cancel.png"))	
		postfix = ".png";
	else
	  postfix = ".gif";
	return QPixmap(getSkinPath() + "/theme/Login_Cancel" + postfix);
}
