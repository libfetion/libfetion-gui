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
#include "fxResource.h"
#include "fxskinmanage.h"

#ifdef WIN32
#else
#include <fcntl.h>
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
#if MAC_OS //Mac OS X platfrom
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
			return QPixmap(getSkinPath() + "/online/offline.gif");

		case FX_STATUS_BLACK:
			return QPixmap(getSkinPath() + "/online/black.gif");

		case FX_STATUS_MOBILE: //mobile user
			return QPixmap(getSkinPath() + "/online/mobile.gif");
		case FX_STATUS_WAITING_AUTH:  //the account waiting   
			return QPixmap(getSkinPath() + "/online/waiting.gif");
		case FX_STATUS_REFUSE:   //the account is refuse make friends with you
			return QPixmap(getSkinPath() + "/online/refuse.gif");
		case FX_STATUS_OFFLINE:
			return QPixmap(getSkinPath() + "/online/offline.gif");
		case FX_STATUS_DINNER:
			return QPixmap(getSkinPath() + "/online/dinner.gif");
		case FX_STATUS_AWAY:
			return QPixmap(getSkinPath() + "/online/away.gif");
		case FX_STATUS_ONLINE:
			return QPixmap(getSkinPath() + "/online/online.gif");
		case FX_STATUS_PHONE:
			return QPixmap(getSkinPath() + "/online/phone.gif");
		case FX_STATUS_BUSY:
			return QPixmap(getSkinPath() + "/online/busy.gif");
		case FX_STATUS_MEETING :
			return QPixmap(getSkinPath() + "/online/meeting.gif");
		case FX_STATUS_EXTENDED_AWAY:
			return QPixmap(getSkinPath() + "/online/extnded_away.gif");
		case FX_STATUS_NUM_PRIMITIVES:
			return QPixmap(getSkinPath() + "/online/num_primitives.gif");
	}

	return QPixmap();
}

QPixmap getSysTrayIcon(int status)
{
	switch(status)
	{
	case 0:  //no login
		return QPixmap(getSkinPath() + "/systray/offline.gif");		
	case 1: //login
	case FX_STATUS_ONLINE:
		return QPixmap(getSkinPath() + "/systray/online.gif");
	case FX_STATUS_OFFLINE:
		return QPixmap(getSkinPath() + "/systray/hide.gif");
	case FX_STATUS_DINNER:
	case FX_STATUS_AWAY:
	case FX_STATUS_MEETING :
	case FX_STATUS_EXTENDED_AWAY:
	case FX_STATUS_NUM_PRIMITIVES:
		return QPixmap(getSkinPath() + "/systray/away.gif");
	case FX_STATUS_PHONE:
	case FX_STATUS_BUSY:
		return QPixmap(getSkinPath() + "/systray/busy.gif");
	}
	return QPixmap(getSkinPath() + "/systray/busy.gif");
}

QPixmap getMenuIcon(int menuID)
{
	switch(menuID)
	{
		case IMBuddyIcon:  //IM
			return QPixmap(getSkinPath() + "/menu/im.gif");
		case SMSBuddyIcon: //SMS
			return QPixmap(getSkinPath() + "/menu/sms.gif");
		case GetInfoBuddyIcon: //GetInfo
			return QPixmap(getSkinPath() + "/menu/profile.gif");
		case ReNameBuddyIcon: //rename
			return QPixmap(getSkinPath() + "/menu/edit.gif");
		case AddBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/addfriend.gif");
		case AboutIcon:
			return QPixmap(getSkinPath() + "/menu/about.gif");
		case ExitIcon:
			return QPixmap(getSkinPath() + "/menu/exit.gif");
		case ApplyIcon:
		case RemoveBlackIcon:
			return QPixmap(getSkinPath() + "/menu/apply.gif");
		case CancelIcon:
			return QPixmap(getSkinPath() + "/menu/cancel.gif");
		case SetImpresaIcon:
			return QPixmap(getSkinPath() + "/menu/edit.gif");
		case AddGroupIcon:
			return QPixmap(getSkinPath() + "/menu/addqun.gif");
		case BackInBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/backlist.gif");
		case DeleteBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/delete.gif");
		case MoveIcon:
			return QPixmap(getSkinPath() + "/menu/move.gif");
		case RefreshBuddyIcon:
			return QPixmap(getSkinPath() + "/menu/refresh.gif");
		case OptionsIcon:
			return QPixmap(getSkinPath() + "/menu/options.gif");
		case HistoryIcon:
			return QPixmap(getSkinPath() + "/menu/history.gif");
		case SkinIcon:
			return QPixmap(getSkinPath() + "/menu/skin.gif");
	}
	return QPixmap();
}

QPixmap getQunIcon()
{
	return QPixmap(getSkinPath() + "/misc/qun.gif");
}

QPixmap getBT_SMSIcon()
{
	return QPixmap(getSkinPath() + "/misc/bt_sms.gif");
}

QPixmap getFlickIcon(bool flag)
{
	if(flag)
		return QPixmap(getSkinPath() + "/systray/online.gif");
	else
		return QPixmap(getSkinPath() + "/systray/online_flick.gif");
}

QPixmap getInputFaceIcon()
{
	return QPixmap(getSkinPath() + "/misc/input_face.gif");
}

QPixmap getFaceIcon()
{
	return QPixmap(getSkinPath() + "/misc/tool_face.gif");
}

QPixmap getNudgeIcon()
{
	return QPixmap(getSkinPath() + "/theme/nudge_button.gif");
}

QPixmap getSendIcon()
{
	return QPixmap(getSkinPath() + "/theme/send_button.gif");
}

QPixmap getHistoryIcon()
{
	return QPixmap(getSkinPath() + "/theme/history.gif");
}

QPixmap getChangeSendModIcon()
{
	return QPixmap(getSkinPath() + "/theme/change_send_mode.gif");
}

QPixmap getCloseTabImage()
{
	return QPixmap(getSkinPath() + "/theme/closetab.gif");
}

QPixmap getLoginImage()
{
#if MAC_OS //Mac OS X platfrom
	return QPixmap(getSkinPath() + "/theme/login_image-mac.gif");
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(getSkinPath() + "/theme/login_image-win.gif");
	#else //linux platfrom
		return QPixmap(getSkinPath() + "/theme/login_image-linux.gif");
	#endif
#endif
}

QPixmap getPortraitImage()
{
#if MAC_OS //Mac OS X platfrom
	return QPixmap(getSkinPath() + "/theme/portrait-mac.gif");
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(getSkinPath() + "/theme/portrait-win.gif");
	#else //linux platfrom
		return QPixmap(getSkinPath() + "/theme/portrait-linux.gif");
	#endif
#endif
}

QPixmap getBTSettingImage()
{
	return QPixmap(getSkinPath() + "/theme/bt_setting.gif");
}

QPixmap getBTSendSelfImage()
{
	return QPixmap(getSkinPath() + "/theme/bt_sendself.gif");
}

QPixmap getAddImage()
{
	return QPixmap(getSkinPath() + "/theme/addfriend.gif");
}

QPixmap getImpresaBKImage()
{
	return QPixmap(getSkinPath() + "/theme/impresabk.gif");
}

QPixmap getSearchBKImage()
{
	return QPixmap(getSkinPath() + "/theme/search.gif");
}

QPixmap getLibFetionImage()
{
	return QPixmap(getSkinPath() + "/theme/LibFetion.gif");
}

QPixmap getLogion_InImage()
{
	return QPixmap(getSkinPath() + "/theme/Logion_In.gif");
}

QPixmap getLogin_CancelImage()
{
	return QPixmap(getSkinPath() + "/theme/Login_Cancel.gif");
}
