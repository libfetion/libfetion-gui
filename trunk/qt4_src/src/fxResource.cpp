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
#include <QDebug>

#include "fxResource.h"
#include "fxskinmanage.h"

#include "fxglobal.h"

#ifdef WIN32
#else
    #include <fcntl.h>
    #include <sys/types.h>
    #include <pwd.h>
#endif

QDomDocument *ResXML = NULL;

void setXMLRes(QDomDocument *xml)
{
    if (ResXML)
    {
        ResXML->clear();
    }
    ResXML = xml;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString getXMLRes(QString item, QString defValue)
{
    if (!ResXML)
    {
        return "";
    }
    return ResXML->documentElement().attribute(item, defValue);

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

#ifdef WIN32
#else
    QString currentUserPath()
    {
        static QString currentUserPath;
        static bool init = false;
        if (init)
        {
            return currentUserPath;
        }

        char *CONF_FILE = (char*)malloc(sizeof(char)*(512));
        memset(CONF_FILE, 0, 512);

        struct passwd *pwd;
        if ((pwd = getpwuid(geteuid())) != NULL)
        {
            strcpy(CONF_FILE, pwd->pw_dir);
            if (CONF_FILE[strlen(CONF_FILE) - 1] != '/')
            {
                strcat(CONF_FILE, "/");
            }
            strcat(CONF_FILE, ".");
        } currentUserPath = CONF_FILE;
        free(CONF_FILE);

        init = true;
        return currentUserPath;
    }

    void moveOldConfigFile()
    {
        bool done = false;
        if (done)
        {
            return ;
        }

        done = true;

        //create the .libfetion folder
        QDir home(currentUserPath());
        home.mkdir(".libfetion");

        //move old config file to .libfetion folder
        if (QFile::exists(currentUserPath() + FX_DATABASE_NAME) && !QFile::exists
            (chatDBFile()))
        {
            QFile::rename(currentUserPath() + "/." FX_DATABASE_NAME, chatDBFile());
        }

        if (QFile::exists(currentUserPath() + "/." FX_CONFIG_FILE) && !QFile
            ::exists(configFile()))
        {
            QFile::rename(currentUserPath() + "/." FX_CONFIG_FILE, configFile());
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

        if (init)
        {
            return defaultResPath;
        }

        //here we set the CREDITS.txt as an identifying item

        if (QFile::exists("./CREDITS.txt"))
        {
            defaultResPath = ".";
        }
        else
        {
            defaultResPath = "/usr/share/libfetion";
        }

        init = true;
        return defaultResPath;
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString dataResPath()
{
    #ifdef WIN32
        return ".";
    #else //linux
        return currentUserPath() + "/.libfetion";
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString configFile()
{
    return dataResPath() + "/" FX_CONFIG_FILE;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString chatDBFile()
{
    return dataResPath() + "/" FX_DATABASE_NAME;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString SkinPath()
{
    return defaultResPath() + "/skins";
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString defaultSkinPath()
{
    return SkinPath() + "/default";
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString defaultSoundPath()
{
    return defaultResPath() + "/sound";
}

QString defaultDataPath()
{
    return defaultResPath() + "/data";
}
/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString FxFacePath()
{
    #ifdef WIN32
        return "./faces_image";
    #else //linux
        static QString path;
        static bool init = false;

        if (init)
        {
            return path;
        }

        if (QFile::exists("./faces_image/1.gif"))
        {
            path = "./faces_image";
        }
        else
        {
            path = "/usr/share/libfetion/faces_image";
        }

        init = true;
        return path;
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

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
            cmd = "aplay " + music + "&";
            system(cmd.toStdString().c_str());
        #endif
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void playSound(SOUND_TYPE type)
{
    switch (type)
    {
        case MSG_SOUND:
            execPlaySound(Settings::instance().MsgRingPath());
            break;
        case ONLINE_SOUND:
            execPlaySound(defaultSoundPath() + "/online.wav");
            break;
        case SYS_SOUND:
            execPlaySound(defaultSoundPath() + "/sys.wav");
            break;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getOnlineStatusIcon(int status)
{
    switch (status)
    {
        /* pc offline */
        case 0:
        case 0+MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_offline", ""));

        case FX_STATUS_BLACK:
        case FX_STATUS_BLACK + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_black", ""));

        /* mobile user */
        case FX_STATUS_MOBILE:
        case FX_STATUS_MOBILE + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_mobile", ""));

        /* the account waiting */
        case FX_STATUS_WAITING_AUTH:
        case FX_STATUS_WAITING_AUTH + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_waiting", ""));

        /* the account is refuse make friends with you */
        case FX_STATUS_REFUSE:
        case FX_STATUS_REFUSE + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_refuse", ""));

        case FX_STATUS_OFFLINE:
        case FX_STATUS_OFFLINE + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_offline", ""));

        case FX_STATUS_DINNER:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_dinner", ""));

        case FX_STATUS_AWAY:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_away", ""));

        case FX_STATUS_ONLINE:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_online", ""));

        case FX_STATUS_PHONE:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_phone", ""));

        case FX_STATUS_BUSY:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_busy", ""));

        case FX_STATUS_MEETING:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_meeting", ""));

        case FX_STATUS_EXTENDED_AWAY:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_extnded_away", ""));

        case FX_STATUS_NUM_PRIMITIVES:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_num_primitives", ""));

        case FX_STATUS_ONLINE + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_m_online", ""));

        case FX_STATUS_DINNER + MOBILE_LOGIN:
        case FX_STATUS_AWAY + MOBILE_LOGIN:
        case FX_STATUS_EXTENDED_AWAY + MOBILE_LOGIN:
        case FX_STATUS_NUM_PRIMITIVES + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_m_away", ""));

        case FX_STATUS_PHONE + MOBILE_LOGIN:
        case FX_STATUS_MEETING + MOBILE_LOGIN:
        case FX_STATUS_BUSY + MOBILE_LOGIN:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("online_m_busy", ""));

        /* should never reach*/
        default:
            return QPixmap("");
    }

    return QPixmap(getSkinPath() + "/" + getXMLRes("online_offline", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getSysTrayIcon(int status)
{
    switch (status)
    {
        /* no login */
        case 0:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("systray_offline",""));
        /* login */
        case 1:
        case FX_STATUS_ONLINE:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("systray_online", ""));

        case FX_STATUS_OFFLINE:
            return QPixmap(getSkinPath() + "/" +
                           getXMLRes("systray_hide", ""));

        case FX_STATUS_DINNER:
        case FX_STATUS_AWAY:
        case FX_STATUS_MEETING:
        case FX_STATUS_EXTENDED_AWAY:
        case FX_STATUS_NUM_PRIMITIVES:
            return QPixmap(getSkinPath() + "/" + getXMLRes("systray_away", ""));
    }
    return QPixmap(getSkinPath() + "/" + getXMLRes("systray_busy", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getMenuIcon(int menuID)
{
    switch (menuID)
    {
        case IMBuddyIcon:
            //IM
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_im", ""));
        case SMSBuddyIcon:
            //SMS
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_sms", ""));
        case GetInfoBuddyIcon:
            //GetInfo
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_profile", ""));
        case ReNameBuddyIcon:
            //rename
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_edit", ""));
        case AddBuddyIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_addfriend", "")
                           );
        case AboutIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_about", ""));
        case ExitIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_exit", ""));
        case ApplyIcon:
        case RemoveBlackIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_apply", ""));
        case CancelIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_cancel", ""));
        case SetImpresaIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_edit", ""));
        case AddGroupIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_addqun", ""));
        case BackInBuddyIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_backlist", ""))
                           ;
        case DeleteBuddyIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_delete", ""));
        case MoveIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_move", ""));
        case RefreshBuddyIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_refresh", ""));
        case OptionsIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_options", ""));
        case HistoryIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_history", ""));
        case SkinIcon:
            return QPixmap(getSkinPath() + "/" + getXMLRes("menu_skin", ""));
    }
    return QPixmap();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getQunIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("qun_icon", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getBT_SMSIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("bt_smsicon", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getFlickIcon(bool flag)
{
    if (flag)
    {
        return QPixmap(getSkinPath() + "/" +
                       getXMLRes("systray_online", ""));
    }
    else
    {
        return QPixmap(getSkinPath() + "/" +
                       getXMLRes("systray_online_flick",""));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getInputFaceIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("input_face", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getFaceIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("tool_face", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getNudgeIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("nudge_button", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getSendIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("send_button", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getHistoryIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("history", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getChangeSendModIcon()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("change_send_mode", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getCloseTabImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("closetab", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getLoginImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("login_image", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getPortraitImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("portrait", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getBTSettingImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("bt_setting", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getBTSendSelfImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("bt_sendself", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getAddImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("addfriend", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getImpresaBKImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("impresabk", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getSearchBKImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("search", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getLibFetionImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("LibFetion", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getLogion_InImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("login_in", ""));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QPixmap getLogin_CancelImage()
{
    return QPixmap(getSkinPath() + "/" + getXMLRes("login_cancel", ""));
}
