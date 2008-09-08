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

QString defaultSkinPath()
{
#ifdef WIN32
	return "./image";
#else //linux
	static QString imagepath;
	static bool init = false;
	FILE* fp;

	if(init)
		return imagepath;

	  if ((fp = fopen("./image/online/offline.gif", "r")))
	  {
		  fclose (fp);
		  imagepath = "./image";
	  }	
	  else
		  imagepath = "/usr/share/libfetion/image";

	init = true;
	return imagepath;
#endif
}

QString defaultSoundPath()
{
#ifdef WIN32
	return "./sound";
#else //linux
	static QString soundpath;
	static bool init = false;
	FILE* fp;

	if(init)
		return soundpath;

	  if ((fp = fopen("./sound/msg.wav", "r")))
	  {
		  fclose (fp);
		  soundpath = "./sound";
	  }	
	  else
		  soundpath = "/usr/share/libfetion/sound";

	init = true;
	return soundpath;
#endif
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
			return QPixmap(SkinPath() + "/online/offline.gif");

		case FX_STATUS_BLACK:
			return QPixmap(SkinPath() + "/online/black.gif");

		case FX_STATUS_MOBILE: //mobile user
			return QPixmap(SkinPath() + "/online/mobile.gif");
		case FX_STATUS_WAITING_AUTH:  //the account waiting   
			return QPixmap(SkinPath() + "/online/waiting.gif");
		case FX_STATUS_REFUSE:   //the account is refuse make friends with you
			return QPixmap(SkinPath() + "/online/refuse.gif");
		case FX_STATUS_OFFLINE:
			return QPixmap(SkinPath() + "/online/offline.gif");
		case FX_STATUS_DINNER:
			return QPixmap(SkinPath() + "/online/dinner.gif");
		case FX_STATUS_AWAY:
			return QPixmap(SkinPath() + "/online/away.gif");
		case FX_STATUS_ONLINE:
			return QPixmap(SkinPath() + "/online/online.gif");
		case FX_STATUS_PHONE:
			return QPixmap(SkinPath() + "/online/phone.gif");
		case FX_STATUS_BUSY:
			return QPixmap(SkinPath() + "/online/busy.gif");
		case FX_STATUS_MEETING :
			return QPixmap(SkinPath() + "/online/meeting.gif");
		case FX_STATUS_EXTENDED_AWAY:
			return QPixmap(SkinPath() + "/online/extnded_away.gif");
		case FX_STATUS_NUM_PRIMITIVES:
			return QPixmap(SkinPath() + "/online/num_primitives.gif");
	}

	return QPixmap();
}

QPixmap getSysTrayIcon(int status)
{
	switch(status)
	{
	case 0:  //no login
		return QPixmap(SkinPath() + "/systray/offline.gif");		
	case 1: //login
	case FX_STATUS_ONLINE:
		return QPixmap(SkinPath() + "/systray/online.gif");
	case FX_STATUS_OFFLINE:
		return QPixmap(SkinPath() + "/systray/hide.gif");
	case FX_STATUS_DINNER:
	case FX_STATUS_AWAY:
	case FX_STATUS_MEETING :
	case FX_STATUS_EXTENDED_AWAY:
	case FX_STATUS_NUM_PRIMITIVES:
		return QPixmap(SkinPath() + "/systray/away.gif");
	case FX_STATUS_PHONE:
	case FX_STATUS_BUSY:
		return QPixmap(SkinPath() + "/systray/busy.gif");
	}
	return QPixmap(SkinPath() + "/systray/busy.gif");
}

QPixmap getMenuIcon(int menuID)
{
	switch(menuID)
	{
		case IMBuddyIcon:  //IM
			return QPixmap(SkinPath() + "/menu/im.gif");
		case SMSBuddyIcon: //SMS
			return QPixmap(SkinPath() + "/menu/sms.gif");
		case GetInfoBuddyIcon: //GetInfo
			return QPixmap(SkinPath() + "/menu/profile.gif");
		case ReNameBuddyIcon: //rename
			return QPixmap(SkinPath() + "/menu/edit.gif");
		case AddBuddyIcon:
			return QPixmap(SkinPath() + "/menu/addfriend.gif");
		case AboutIcon:
			return QPixmap(SkinPath() + "/menu/about.gif");
		case ExitIcon:
			return QPixmap(SkinPath() + "/menu/exit.gif");
		case ApplyIcon:
		case RemoveBlackIcon:
			return QPixmap(SkinPath() + "/menu/apply.gif");
		case CancelIcon:
			return QPixmap(SkinPath() + "/menu/cancel.gif");
		case SetImpresaIcon:
			return QPixmap(SkinPath() + "/menu/edit.gif");
		case AddGroupIcon:
			return QPixmap(SkinPath() + "/menu/addqun.gif");
		case BackInBuddyIcon:
			return QPixmap(SkinPath() + "/menu/backlist.gif");
		case DeleteBuddyIcon:
			return QPixmap(SkinPath() + "/menu/delete.gif");
		case MoveIcon:
			return QPixmap(SkinPath() + "/menu/move.gif");
		case RefreshBuddyIcon:
			return QPixmap(SkinPath() + "/menu/refresh.gif");
		case OptionsIcon:
			return QPixmap(SkinPath() + "/menu/options.gif");
		case HistoryIcon:
			return QPixmap(SkinPath() + "/menu/history.gif");
	}
	return QPixmap();
}

QPixmap getQunIcon()
{
	return QPixmap(SkinPath() + "/misc/qun.gif");
}

QPixmap getBT_SMSIcon()
{
	return QPixmap(SkinPath() + "/misc/bt_sms.gif");
}

QPixmap getFlickIcon(bool flag)
{
	if(flag)
		return QPixmap(SkinPath() + "/systray/online.gif");
	else
		return QPixmap(SkinPath() + "/systray/online_flick.gif");
}

QPixmap getInputFaceIcon()
{
	return QPixmap(SkinPath() + "/misc/input_face.gif");
}

QPixmap getFaceIcon()
{
	return QPixmap(SkinPath() + "/misc/tool_face.gif");
}

QPixmap getSendIcon()
{
	return QPixmap(SkinPath() + "/theme/send_button.gif");
}

QPixmap getHistoryIcon()
{
	return QPixmap(SkinPath() + "/theme/history.gif");
}

QPixmap getChangeSendModIcon()
{
	return QPixmap(SkinPath() + "/theme/change_send_mode.gif");
}

QPixmap getCloseTabImage()
{
	return QPixmap(SkinPath() + "/theme/closetab.gif");
}

QPixmap getLoginImage()
{
#if MAC_OS //Mac OS X platfrom
	return QPixmap(SkinPath() + "/theme/login_image-mac.gif");
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(SkinPath() + "/theme/login_image-win.gif");
	#else //linux platfrom
		return QPixmap(SkinPath() + "/theme/login_image-linux.gif");
	#endif
#endif
}

QPixmap getPortraitImage()
{
#if MAC_OS //Mac OS X platfrom
	return QPixmap(SkinPath() + "/theme/portrait-mac.gif");
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(SkinPath() + "/theme/portrait-win.gif");
	#else //linux platfrom
		return QPixmap(SkinPath() + "/theme/portrait-linux.gif");
	#endif
#endif
}

QPixmap getAddImage()
{
	return QPixmap(SkinPath() + "/theme/addfrined.gif");
}

QPixmap getImpresaBKImage()
{
	return QPixmap(SkinPath() + "/theme/impresabk.gif");
}

QPixmap getSearchBKImage()
{
	return QPixmap(SkinPath() + "/theme/search.gif");
}

QPixmap getLibFetionImage()
{
	return QPixmap(SkinPath() + "/theme/LibFetion.gif");
}

QPixmap getLogion_InImage()
{
	return QPixmap(SkinPath() + "/theme/Logion_In.gif");
}

QPixmap getLogin_CancelImage()
{
	return QPixmap(SkinPath() + "/theme/Login_Cancel.gif");
}
