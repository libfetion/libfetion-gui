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

QDomDocument *ResXML = NULL;

void setXMLRes(QDomDocument *xml)
{
//	if (ResXML)
//		ResXML;

	ResXML = xml;
}

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
	switch(status)
	{
		case 0:  //pc offline
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_offline", ""));

		case FX_STATUS_BLACK:
		case FX_STATUS_BLACK + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_black", ""));

		case FX_STATUS_MOBILE: //mobile user
		case FX_STATUS_MOBILE + MOBILE_LOGIN: //mobile user
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_mobile", ""));

		case FX_STATUS_WAITING_AUTH:  //the account waiting   
		case FX_STATUS_WAITING_AUTH + MOBILE_LOGIN:  //the account waiting   
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_waiting", ""));

		case FX_STATUS_REFUSE + MOBILE_LOGIN:   //the account is refuse make friends with you
		case FX_STATUS_REFUSE:   //the account is refuse make friends with you
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_refuse", ""));

		case FX_STATUS_OFFLINE:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_offline", ""));

		case FX_STATUS_DINNER:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_dinner", ""));

		case FX_STATUS_AWAY:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_away", ""));

		case FX_STATUS_ONLINE:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_online", ""));

		case FX_STATUS_PHONE:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_phone", ""));

		case FX_STATUS_BUSY:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_busy", ""));

		case FX_STATUS_MEETING :
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_meeting", ""));

		case FX_STATUS_EXTENDED_AWAY:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_extnded_away", ""));

		case FX_STATUS_NUM_PRIMITIVES:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_num_primitives", ""));

		case FX_STATUS_ONLINE + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_m_online", ""));

		case FX_STATUS_DINNER + MOBILE_LOGIN:
		case FX_STATUS_AWAY + MOBILE_LOGIN:
		case FX_STATUS_EXTENDED_AWAY + MOBILE_LOGIN:
		case FX_STATUS_NUM_PRIMITIVES + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_m_away", ""));

		case FX_STATUS_PHONE + MOBILE_LOGIN:
		case FX_STATUS_MEETING + MOBILE_LOGIN:
		case FX_STATUS_BUSY + MOBILE_LOGIN:
			return QPixmap(getSkinPath() + "/" +  
					ResXML->documentElement().attribute("online_m_busy", ""));

	}

	return QPixmap();
}

QPixmap getSysTrayIcon(int status)
{
	switch(status)
	{
	case 0:  //no login
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("systray_offline", ""));
	case 1: //login
	case FX_STATUS_ONLINE:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("systray_online", ""));
		
	case FX_STATUS_OFFLINE:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("systray_hide", ""));
		
	case FX_STATUS_DINNER:
	case FX_STATUS_AWAY:
	case FX_STATUS_MEETING :
	case FX_STATUS_EXTENDED_AWAY:
	case FX_STATUS_NUM_PRIMITIVES:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("systray_away", ""));
/*	case FX_STATUS_PHONE:
	case FX_STATUS_BUSY:
	deafult:
*/
	}
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("systray_busy", ""));
}

QPixmap getMenuIcon(int menuID)
{
	switch(menuID)
	{
		case IMBuddyIcon:  //IM
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_im", ""));
		case SMSBuddyIcon: //SMS
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_sms", ""));
		case GetInfoBuddyIcon: //GetInfo
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_profile", ""));
		case ReNameBuddyIcon: //rename
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_edit", ""));
		case AddBuddyIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_addfriend", ""));
		case AboutIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_about", ""));
		case ExitIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_exit", ""));
		case ApplyIcon:
		case RemoveBlackIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_apply", ""));
		case CancelIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_cancel", ""));
		case SetImpresaIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_edit", ""));
		case AddGroupIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_addqun", ""));
		case BackInBuddyIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_backlist", ""));
		case DeleteBuddyIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_delete", ""));
		case MoveIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_move", ""));
		case RefreshBuddyIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_refresh", ""));
		case OptionsIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_options", ""));
		case HistoryIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_history", ""));
		case SkinIcon:
		return QPixmap(getSkinPath() + "/" + 
					ResXML->documentElement().attribute("menu_skin", ""));
	}
	return QPixmap();
}

QPixmap getQunIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("qun_icon", ""));
}

QPixmap getBT_SMSIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("bt_smsicon", ""));
}

QPixmap getFlickIcon(bool flag)
{
	if(flag)
		return QPixmap(getSkinPath() + "/" + 
				ResXML->documentElement().attribute("systray_online", ""));
	else
		return QPixmap(getSkinPath() + "/" + 
				ResXML->documentElement().attribute("systray_online_flick", ""));
}

QPixmap getInputFaceIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("input_face", ""));
}

QPixmap getFaceIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("tool_face", ""));
}

QPixmap getNudgeIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("nudge_button", ""));
}

QPixmap getSendIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("send_button", ""));
}

QPixmap getHistoryIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("history", ""));
}

QPixmap getChangeSendModIcon()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("change_send_mode", ""));
}

QPixmap getCloseTabImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("closetab", ""));
}

QPixmap getLoginImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("login_image", ""));
}

QPixmap getPortraitImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("portrait", ""));
}

QPixmap getBTSettingImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("bt_setting", ""));
}

QPixmap getBTSendSelfImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("bt_sendself", ""));
}

QPixmap getAddImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("addfriend", ""));
}

QPixmap getImpresaBKImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("impresabk", ""));
}

QPixmap getSearchBKImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("search", ""));
}

QPixmap getLibFetionImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("LibFetion", ""));
}

QPixmap getLogion_InImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("login_in", ""));
}

QPixmap getLogin_CancelImage()
{
	return QPixmap(getSkinPath() + "/" + 
			ResXML->documentElement().attribute("login_cancel", ""));
}
