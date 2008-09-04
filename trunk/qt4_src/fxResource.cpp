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

#ifdef WIN32
#else
#include <fcntl.h>
#endif

QString SoundPath()
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
			execPlaySound(SoundPath() +"/online.wav");
			break;
		case SYS_SOUND:
			execPlaySound(SoundPath() +"/sys.wav");
			break;
	}
}

QString ImagePath()
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

QPixmap getOnlineStatusIcon(int status)
{
	switch(status)
	{
		case 0:  //pc offline
			return QPixmap(ImagePath() + "/online/offline.gif");

		case FX_STATUS_BLACK:
			return QPixmap(ImagePath() + "/online/black.gif");

		case FX_STATUS_MOBILE: //mobile user
			return QPixmap(ImagePath() + "/online/mobile.gif");
		case FX_STATUS_WAITING_AUTH:  //the account waiting   
			return QPixmap(ImagePath() + "/online/waiting.gif");
		case FX_STATUS_REFUSE:   //the account is refuse make friends with you
			return QPixmap(ImagePath() + "/online/refuse.gif");
		case FX_STATUS_OFFLINE:
			return QPixmap(ImagePath() + "/online/offline.gif");
		case FX_STATUS_DINNER:
			return QPixmap(ImagePath() + "/online/dinner.gif");
		case FX_STATUS_AWAY:
			return QPixmap(ImagePath() + "/online/away.gif");
		case FX_STATUS_ONLINE:
			return QPixmap(ImagePath() + "/online/online.gif");
		case FX_STATUS_PHONE:
			return QPixmap(ImagePath() + "/online/phone.gif");
		case FX_STATUS_BUSY:
			return QPixmap(ImagePath() + "/online/busy.gif");
		case FX_STATUS_MEETING :
			return QPixmap(ImagePath() + "/online/meeting.gif");
		case FX_STATUS_EXTENDED_AWAY:
			return QPixmap(ImagePath() + "/online/extnded_away.gif");
		case FX_STATUS_NUM_PRIMITIVES:
			return QPixmap(ImagePath() + "/online/num_primitives.gif");
	}

	return QPixmap();
}

QPixmap getSysTrayIcon(int status)
{
	switch(status)
	{
	case 0:  //no login
		return QPixmap(ImagePath() + "/systray/offline.gif");		
	case 1: //login
	case FX_STATUS_ONLINE:
		return QPixmap(ImagePath() + "/systray/online.gif");
	case FX_STATUS_OFFLINE:
		return QPixmap(ImagePath() + "/systray/hide.gif");
	case FX_STATUS_DINNER:
	case FX_STATUS_AWAY:
	case FX_STATUS_MEETING :
	case FX_STATUS_EXTENDED_AWAY:
	case FX_STATUS_NUM_PRIMITIVES:
		return QPixmap(ImagePath() + "/systray/away.gif");
	case FX_STATUS_PHONE:
	case FX_STATUS_BUSY:
		return QPixmap(ImagePath() + "/systray/busy.gif");
	}
	return QPixmap(ImagePath() + "/systray/busy.gif");
}

QPixmap getMenuIcon(int menuID)
{
	switch(menuID)
	{
		case IMBuddyIcon:  //IM
			return QPixmap(ImagePath() + "/menu/im.gif");
		case SMSBuddyIcon: //SMS
			return QPixmap(ImagePath() + "/menu/sms.gif");
		case GetInfoBuddyIcon: //GetInfo
			return QPixmap(ImagePath() + "/menu/profile.gif");
		case ReNameBuddyIcon: //rename
			return QPixmap(ImagePath() + "/menu/edit.gif");
		case AddBuddyIcon:
			return QPixmap(ImagePath() + "/menu/addfriend.gif");
		case AboutIcon:
			return QPixmap(ImagePath() + "/menu/about.gif");
		case ExitIcon:
			return QPixmap(ImagePath() + "/menu/exit.gif");
		case ApplyIcon:
		case RemoveBlackIcon:
			return QPixmap(ImagePath() + "/menu/apply.gif");
		case CancelIcon:
			return QPixmap(ImagePath() + "/menu/cancel.gif");
		case SetImpresaIcon:
			return QPixmap(ImagePath() + "/menu/edit.gif");
		case AddGroupIcon:
			return QPixmap(ImagePath() + "/menu/addqun.gif");
		case BackInBuddyIcon:
			return QPixmap(ImagePath() + "/menu/backlist.gif");
		case DeleteBuddyIcon:
			return QPixmap(ImagePath() + "/menu/delete.gif");
		case MoveIcon:
			return QPixmap(ImagePath() + "/menu/move.gif");
		case RefreshBuddyIcon:
			return QPixmap(ImagePath() + "/menu/refresh.gif");
		case OptionsIcon:
			return QPixmap(ImagePath() + "/menu/options.gif");
		case HistoryIcon:
			return QPixmap(ImagePath() + "/menu/history.gif");
	}
	return QPixmap();
}

QPixmap getQunIcon()
{
	return QPixmap(ImagePath() + "/misc/qun.gif");
}

QPixmap getBT_SMSIcon()
{
	return QPixmap(ImagePath() + "/misc/bt_sms.gif");
}

QPixmap getFlickIcon(bool flag)
{
	if(flag)
		return QPixmap(ImagePath() + "/systray/online.gif");
	else
		return QPixmap(ImagePath() + "/systray/online_flick.gif");
}

QPixmap getInputFaceIcon()
{
	return QPixmap(ImagePath() + "/misc/input_face.gif");
}

QPixmap getFaceIcon()
{
	return QPixmap(ImagePath() + "/misc/tool_face.gif");
}

QPixmap getSendIcon()
{
	return QPixmap(ImagePath() + "/theme/send_button.gif");
}

QPixmap getHistoryIcon()
{
	return QPixmap(ImagePath() + "/theme/history.gif");
}

QPixmap getChangeSendModIcon()
{
	return QPixmap(ImagePath() + "/theme/change_send_mode.gif");
}

QPixmap getCloseTabImage()
{
	return QPixmap(ImagePath() + "/theme/closetab.gif");
}

QPixmap getLoginImage()
{
#if MAC_OS //Mac OS X platfrom
	return QPixmap(ImagePath() + "/theme/login_image-mac.gif");
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(ImagePath() + "/theme/login_image-win.gif");
	#else //linux platfrom
		return QPixmap(ImagePath() + "/theme/login_image-linux.gif");
	#endif
#endif
}

QPixmap getPortraitImage()
{
#if MAC_OS //Mac OS X platfrom
	return QPixmap(ImagePath() + "/theme/portrait-mac.gif");
#else
	#ifdef WIN32 //windows platfrom
		return QPixmap(ImagePath() + "/theme/portrait-win.gif");
	#else //linux platfrom
		return QPixmap(ImagePath() + "/theme/portrait-linux.gif");
	#endif
#endif
}

QPixmap getAddImage()
{
	return QPixmap(ImagePath() + "/theme/addfrined.gif");
}

QPixmap getImpresaBKImage()
{
	return QPixmap(ImagePath() + "/theme/impresabk.gif");
}

QPixmap getSearchBKImage()
{
	return QPixmap(ImagePath() + "/theme/search.gif");
}

QPixmap getLibFetionImage()
{
	return QPixmap(ImagePath() + "/theme/LibFetion.gif");
}

QPixmap getLogion_InImage()
{
	return QPixmap(ImagePath() + "/theme/Logion_In.gif");
}

QPixmap getLogin_CancelImage()
{
	return QPixmap(ImagePath() + "/theme/Login_Cancel.gif");
}
