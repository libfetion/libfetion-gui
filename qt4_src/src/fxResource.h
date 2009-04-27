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
#ifndef IMAGERESOURCE_H
#define IMAGERESOURCE_H

#include "appconfig.h"

typedef enum {
	MSG_SOUND = 0,
	ONLINE_SOUND, 
	SYS_SOUND 
} SOUND_TYPE;

QString defaultResPath();
QString SkinPath();
QString defaultSkinPath();
QString defaultSoundPath();
void playSound(SOUND_TYPE type);
void execPlaySound(QString file);

QPixmap getOnlineStatusIcon(int online_status);
QPixmap getSysTrayIcon(int status);
QPixmap getMenuIcon(int menuID);
QPixmap getQunIcon();
QPixmap getBT_SMSIcon();

QPixmap getInputFaceIcon(); //this function is get the inputFace dialog is background picture
QPixmap getFaceIcon(); //this function is get the face toolbutton is picture
QPixmap getSendIcon(); //this function is get the send toolbutton is picture
QPixmap getNudgeIcon(); //this function is get the nudge toolbutton is picture
QPixmap getHistoryIcon(); //this function is get the history toolbutton is picture
QPixmap getChangeSendModIcon(); //this function is get the change_send_mod toolbutton is picture

QPixmap getFlickIcon(bool flag);

//here will using enmu replace
#define IMBuddyIcon			1
#define SMSBuddyIcon		2
#define GetInfoBuddyIcon    3
#define ReNameBuddyIcon     4
#define DeleteBuddyIcon     5
#define BackInBuddyIcon     6
#define AddBuddyIcon        7
#define SetImpresaIcon      8
#define AddGroupIcon        9 
#define AboutIcon           10 
#define ExitIcon            11
#define ApplyIcon           12
#define CancelIcon          13
#define MoveIcon            14
#define RefreshBuddyIcon    15
#define RemoveBlackIcon     16
#define OptionsIcon         17  
#define HistoryIcon         18
#define SkinIcon         19

#define ReNameGroupIcon ReNameBuddyIcon 
#define DeleteGroupIcon DeleteBuddyIcon     


QPixmap getLoginImage(); //this function is get the login picture
QPixmap getPortraitImage(); //this function is get the portrait picture
QPixmap getImpresaBKImage(); //this function is get the mood background picture
QPixmap getSearchBKImage(); //this function is get the Search background picture
QPixmap getAddImage(); //this function is get the add picture

QPixmap getBTSettingImage(); //this function is get the setting button's picture
QPixmap getBTSendSelfImage(); //this function is get the send self button's picture


QPixmap getLibFetionImage();
QPixmap getLogion_InImage();
QPixmap getLogin_CancelImage();

QPixmap getCloseTabImage(); //this function is get the close tab picture

#endif
