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

#include "fxsettings.h"
#include "fxmainwindow.h"
#include "fxskinmanage.h"

/*
 * Copyright (C) 2008
 *
 * An instance check class used in libfetion for Mac/Linux/Windows.
 *
 * Author: YongLi(liyong03@gmail.com)
 */


#ifndef WIN32
    #include <errno.h>
    #include <signal.h>
    #include <stdio.h>
    #include <sys/sem.h>
    #include <sys/shm.h>
    #include <sys/stat.h>
#else
    #include <iostream>
    using namespace std;
    #include <windows.h>
    #define hotkey_id 1
#endif

#define PERM 	(S_IRUSR | S_IWUSR)
//#define KEY 	36264
#define KEYPATH	"/tmp"
#define STRKEY	"LIBFETIONKETFORWINDOWS"
static int *shmPointer;

#ifndef WIN32
    static int shid;
#else
    static HANDLE shHandle = NULL;
#endif

int addCount();
int reducecount();
int detachandremove(void *shmaddr);

int initshared(const char *path, const char *keyString)
{
    #ifndef WIN32
        Q_UNUSED(keyString);

        key_t key = ftok(path, 'S');

        /* get attached memory, creating it if necessary */
        shid = shmget(key, sizeof(int), PERM | IPC_CREAT | IPC_EXCL);
        if ((shid == -1) && (errno != EEXIST))  /* real error */
            return -1;

        if (shid == -1)
        {
             /* already created, access and attach it */
			if ( ((shid = shmget(key, sizeof(int), PERM)) ==  -1) || 
					((shmPointer = (int*)shmat(shid, NULL, 0)) == (void*) -1)
			   )
                return -1;
            //printf("already have a shared memory and the shmPointer = %d !\n", *shmPointer);
            addCount();
        } else {
             /* successfully created, must attach and initialize variables */
            //printf("create a shared memory!\n");
            shmPointer = (int*)shmat(shid, NULL, 0);
            if (shmPointer == (void*)-1)
                return -1;
            *shmPointer = 1;
        }
        //printf("There are %d instances!\n", *shmPointer);
        return 0;
    #else
        Q_UNUSED(path);
        /* Check the handler */
        if ((shHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, LPCWSTR
            (keyString))) == NULL)
        {
            /* There is no file mapping, so create one */
            shHandle = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,
                PAGE_READWRITE, 0, 1, LPCWSTR(keyString));

            if (shHandle == NULL)
                return -1;

            //printf("create a shared memory!\n");

            /* Get the access pointer */
            shmPointer = (int*)MapViewOfFile(shHandle, FILE_MAP_ALL_ACCESS, 0,
                          0, 0);

            if (shmPointer == NULL)
                return -1;
            (*shmPointer) = 1;
        } else {
            /* Get the access pointer */
            shmPointer = (int*)MapViewOfFile(shHandle, FILE_MAP_ALL_ACCESS, 0,
                          0, 0);

            if (shmPointer == NULL)
                return -1;
            //printf("already have a shared memory and the shmPointer = %d !\n", *shmPointer);
            addCount();
        }

        //printf("There are %d instances!\n", *shmPointer);
        return 0;

    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

int addCount()
{
    (*shmPointer) = (*shmPointer) + 1;
    return 0;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

int reducecount()
{
    //printf("reduce shmPointer!\n");
    *shmPointer =  *shmPointer - 1;
    if (*shmPointer == 0)
        detachandremove(shmPointer);
        //printf("There is no instanceso, so release the shared memory!\n");
    return 0;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

int detachandremove(void *shmaddr)
{
#ifndef WIN32
	int error = 0;
	if (shmdt(shmaddr) == -1)
		error = errno;
	if ((shmctl(shid, IPC_RMID, NULL) == -1) && !error)
		error = errno;
	if (!error)
		return 0;

	errno = error;
	return -1;
#else
	Q_UNUSED(shmaddr);
	if (UnmapViewOfFile(shmPointer) != 0)
		return -1;
	if (CloseHandle(shHandle) != 0)
		return -1;
	return 0;
#endif
}


Settings &Settings::instance()
{
    static Settings instance(configFile(), QSettings::IniFormat);
    return instance;
}


/**************************************************************************/
/*                                                                        */
/**************************************************************************/
Settings::Settings(const QString &fileName, Format format) :
    QSettings(fileName, format)
{
//    FX_FUNCTION
    m_uid = 0L;
    m_mainwind = NULL;
    m_isAutoLogin = ::isAutoLogin(NULL, NULL, NULL);


    initshared(KEYPATH, STRKEY);

    QSize dt_size = QApplication::desktop()->size();
    m_LoginWinPos = value("LoginWinPos", QPoint(dt_size.width() / 3,
            dt_size.height() / 3)).toPoint();
    m_LoginWinSize = value("LoginWinSize", QSize(281, 220)).toSize();

    m_skinPath = value("SkinPath", defaultSkinPath()).toString();
    m_skinName = value("SkinName", "default").toString();

    //check the skinPath is correct. if not correct, set default skin to app.
    if (!validateSkinPath(m_skinPath))
    {
        m_skinPath = defaultSkinPath();
        m_skinName = "default";
    }

    QFont font = QApplication::font();
    font.setPointSize(font.pointSize()+2);
    setSysDefaultFont(font);

    m_CurrentFont = value("AppFont", getSysDefaultFont()).value<QFont> ();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
Settings::~Settings()
{
	reducecount();
//    FX_FUNCTION
    if (m_uid)
    {
        endGroup();
    }
}

int Settings::GetInstancesNum()
{
    return  *shmPointer;
}

int Settings::isSingleInstance()
{
	if (GetInstancesNum() > 1)
		return 0;
	else
		return 1;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setUser(long uid)
{
//    FX_FUNCTION
    if (m_uid)
    {
        endGroup();
    }
    m_uid = uid;
    beginGroup(QString("%1").arg(m_uid));
    init_setting();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::saveFontSetting(const QFont &font)
{
//    FX_FUNCTION
    if (m_uid)
    {
        endGroup();
    }

    m_CurrentFont = font;
    setValue("AppFont", m_CurrentFont);

    if (m_uid)
    {
        beginGroup(QString("%1").arg(m_uid));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setSkins(QString skinPath, QString skinName)
{
//    FX_FUNCTION
    if (m_uid)
    {
        endGroup();
    }

    m_skinPath = skinPath;
    m_skinName = skinName;
    setValue("SkinPath", m_skinPath);
    setValue("SkinName", m_skinName);

    if (m_uid)
    {
        beginGroup(QString("%1").arg(m_uid));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::init_setting()
{
//    FX_FUNCTION
    m_DisableNudge = value("DisableNudge", false).toBool();
    m_isMute = value("Mute", false).toBool();
    m_isEnterSend = value("EnterSend", true).toBool();
    m_isStartHide = value("StartHide", false).toBool();

#ifdef Q_OS_MAC
    m_isAutoShow = true;
#else
    m_isAutoShow = value("AutoShowMsg", false).toBool();
#endif
    m_isMainWindowTopHint = value("MainWindowTopHint", true).toBool();
    setMainWindowTopHint(m_isMainWindowTopHint);

    m_isEnableLongSMS = value("EnableLongSMS", true).toBool();
    setEnableLongSMS(m_isEnableLongSMS);

    m_isAutoReply = value("AutoReply", false).toBool();
    m_replyMsg = value("ReplyMsg", QObject::tr("I'll be back soon")).toString();

    m_MsgRingPath
            = value("MsgRingPath", defaultSoundPath() + "/msg.wav").toString();

    QSize dt_size = QApplication::desktop()->size();

    m_MsgWinPos = value("MsgWinPos", QPoint(dt_size.width() / 3,
            dt_size.height() / 3)).toPoint();
    m_MsgWinSize = value("MsgWinSize", QSize(399, 274)).toSize();

    m_MainWinPos = value("MainWinPos", QPoint(dt_size.width() / 10 * 7,
            dt_size.height() / 10)).toPoint();
    m_MainWinSize = value("MainWinSize", QSize(254, 551)).toSize();

    m_SendMultMsgWinPos = value("SendMultMsgWinPos", QPoint(
            dt_size.width() / 3, dt_size.height() / 3)).toPoint();
    m_SendMultMsgWinSize
            = value("SendMultMsgWinSize", QSize(400, 400)).toSize();

    m_GetMsgHotKey = value("GetMsgHotKey", 'x').toChar();
    m_GetMsgHotKeyMod = (Qt::KeyboardModifiers) value("GetMsgHotKeyMod",
            (qlonglong) (Qt::ControlModifier | Qt::AltModifier)).toLongLong();

    m_isRegisteredGetMsgHotKey = false;
    m_isEnableGetMsgHotKey = value("EnableGetMsgHotKey", true).toBool();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setDisableNudge(bool isDisableNudge)
{
//    FX_FUNCTION
    m_DisableNudge = isDisableNudge;
    setValue("DisableNudge", m_DisableNudge);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setAutoLogin(bool isAutoLogin)
{
//    FX_FUNCTION
    m_isAutoLogin = isAutoLogin;

    if (!m_isAutoLogin)
    {
        ::disAutoLogin();
    }
    else
    {
        int tmp; //bad coding
        switch (fx_get_user_state())
        {
            case FX_STATUS_ONLINE:
                tmp = 0;
                break;
            case FX_STATUS_OFFLINE:
                tmp = 1;
                break;
            case FX_STATUS_AWAY:
                tmp = 2;
                break;
            default:
                tmp = 3;
        }
        ::setAutoLogin(fx_get_usr_uid(), fx_get_usr_passwd(), tmp);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMainWindowTopHint(bool isMainWindowTopHint)
{
FX_FUNCTION
#ifdef Q_OS_MAC
	/* fixme: on the mac machine, there is a bad effect about topHint*/
	Q_UNUSED(isMainWindowTopHint);
	m_isMainWindowTopHint = false;
#else
	m_isMainWindowTopHint = isMainWindowTopHint;
#endif

    setValue("MainWindowTopHint", m_isMainWindowTopHint);

    if (m_mainwind)
    {
        if (m_isMainWindowTopHint)
        {
            m_mainwind->setWindowFlags(m_mainwind->windowFlags()
                    | Qt::WindowStaysOnTopHint);
        }
        else
        {
            m_mainwind->setWindowFlags(m_mainwind->windowFlags()
                    ^ Qt::WindowStaysOnTopHint);
        }
        m_mainwind->show();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setAutoShowMsg(bool isAutoShow)
{
//    FX_FUNCTION
    m_isAutoShow = isAutoShow;
    setValue("AutoShowMsg", m_isAutoShow);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMute(bool isMute)
{
//    FX_FUNCTION
    m_isMute = isMute;
    setValue("Mute", m_isMute);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setEnableLongSMS(bool isEnableLongSMS)
{
//    FX_FUNCTION
    m_isEnableLongSMS = isEnableLongSMS;
    setValue("EnableLongSMS", m_isEnableLongSMS);
    if (m_isEnableLongSMS)
    {
        fx_set_longsms(true);
    }
    else
    {
        fx_set_longsms(false);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setEnterSend(bool isEnterSend)
{
//    FX_FUNCTION
    m_isEnterSend = isEnterSend;
    setValue("EnterSend", m_isEnterSend);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setStartHide(bool isStartHide)
{
//    FX_FUNCTION
    m_isStartHide = isStartHide;
    setValue("StartHide", m_isStartHide);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setAutoReply(bool isAutoReply, QString re_msg)
{
//    FX_FUNCTION
    m_isAutoReply = isAutoReply;
    setValue("AutoReply", m_isAutoReply);

    m_replyMsg = re_msg;
    setValue("ReplyMsg", m_replyMsg);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMsgRingPath(QString path)
{
//    FX_FUNCTION
    m_MsgRingPath = path;
    setValue("MsgRingPath", m_MsgRingPath);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setLoginWinPos(QPoint pos)
{
//    FX_FUNCTION
    if (m_uid)
    {
        endGroup();
    }

    m_LoginWinPos = pos;
    setValue("LoginWinPos", pos);

    if (m_uid)
    {
        beginGroup(QString("%1").arg(m_uid));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMsgWinPos(QPoint pos)
{
//    FX_FUNCTION
    m_MsgWinPos = pos;
    setValue("MsgWinPos", pos);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMainWinPos(QPoint pos)
{
//    FX_FUNCTION
    m_MainWinPos = pos;
    setValue("MainWinPos", pos);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setLoginWinSize(QSize size)
{
//    FX_FUNCTION
    if (m_uid)
    {
        endGroup();
    }

    m_LoginWinSize = size;
    setValue("LoginWinSize", size);

    if (m_uid)
    {
        beginGroup(QString("%1").arg(m_uid));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMainWinSize(QSize size)
{
//    FX_FUNCTION
    m_MainWinSize = size;
    setValue("MainWinSize", size);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setMsgWinSize(QSize size)
{
//    FX_FUNCTION
    m_MsgWinSize = size;
    setValue("MsgWinSize", size);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setSendMultMsgWinPos(QPoint pos)
{
//    FX_FUNCTION
    m_SendMultMsgWinPos = pos;
    setValue("SendMultMsgWinPos", pos);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void Settings::setSendMultMsgWinSize(QSize size)
{
//    FX_FUNCTION
    m_SendMultMsgWinSize = size;
    setValue("SendMultMsgWinSize", size);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
bool Settings::setEnableGetMsgHotKey(bool enable)
{
//    FX_FUNCTION
    if (enable == m_isEnableGetMsgHotKey)
    {
        return true;
    }

    m_isEnableGetMsgHotKey = enable;
    if (m_isEnableGetMsgHotKey)
    {
        setGetMsgHotKey(m_GetMsgHotKey, m_GetMsgHotKeyMod);
    }
    else
    {
        setGetMsgHotKey(m_GetMsgHotKey, m_GetMsgHotKeyMod, false);
    }

    setValue("EnableGetMsgHotKey", m_isEnableGetMsgHotKey);
    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
bool Settings::setGetMsgHotKey(QChar keyValue,
                               Qt::KeyboardModifiers keyMod,
                               bool isRegister)
{
//    FX_FUNCTION
    if (!isRegister)
    {
        //unRegister hot key
        if (m_isRegisteredGetMsgHotKey)
        {
            UnRegistHotkey(m_mainwind, keyValue, keyMod);
        }
        m_isRegisteredGetMsgHotKey = false;
        return true;
    }

    m_GetMsgHotKey = keyValue;
    setValue("GetMsgHotKey", m_GetMsgHotKey);

    m_GetMsgHotKeyMod = keyMod;
    setValue("GetMsgHotKeyMod", (qlonglong) (m_GetMsgHotKeyMod));

    m_isRegisteredGetMsgHotKey = RegistHotkey(m_mainwind, keyValue, keyMod);
    return true;
}

int Settings::QtModToWinMod(Qt::KeyboardModifiers keyMod)
{
#if WIN32
	int Modifiy = 0;
	bool isHaveModifie = false;

	if (keyMod &Qt::ControlModifier) {
		Modifiy = Modifiy | MOD_CONTROL;
		isHaveModifie = true;
	}

	if (keyMod &Qt::ShiftModifier) {
		Modifiy = Modifiy | MOD_SHIFT;
		isHaveModifie = true;
	}

	if (keyMod &Qt::AltModifier) {
		Modifiy = Modifiy | MOD_ALT;
		isHaveModifie = true;
	}

	if (!isHaveModifie)
		Modifiy = MOD_CONTROL | MOD_ALT;

	return Modifiy;
#else
	Q_UNUSED(keyMod);
	return 0;
#endif
}

bool Settings::RegistHotkey(QWidget *window,
		QChar keyValue,
		Qt::KeyboardModifiers keyMod)
{
#if WIN32
	WId w_handle = window ? window->winId(): 0;
		int modifiy = QtModToWinMod(keyMod);
		return RegisterHotKey(w_handle, hotkey_id, modifiy, VkKeyScan
				(keyValue.toAscii()));
#else //liunx or mac os are not implement...
		Q_UNUSED(window);
		Q_UNUSED(keyValue);
		Q_UNUSED(keyMod);
		return false;
#endif
		
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool Settings::UnRegistHotkey(QWidget *window,
		QChar keyValue,
		Qt::KeyboardModifiers keyMod)
{
	Q_UNUSED(keyValue);
	Q_UNUSED(keyMod);
#if WIN32
	WId w_handle = window ? window->winId(): 0;
		return UnregisterHotKey(w_handle, hotkey_id);
#else
		Q_UNUSED(window);
		return false;
#endif
}

