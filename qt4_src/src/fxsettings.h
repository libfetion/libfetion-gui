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
#ifndef FXSETTINGS_H
#define FXSETTINGS_H

#include <QSettings>
#include "appconfig.h"
//#include "fxdb.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	class FxMainWindow;
	class Settings : public QSettings { //this class is just store data for setting...
			
			Settings(const QString & fileName, Format format);
			~Settings();
		public:
			static Settings& instance();
			void setUser(long uid);
			void setMainWindow(FxMainWindow *mainwind) { m_mainwind = mainwind; }

			void setSkins(QString m_skinPath, QString m_skinName);
			QString SkinPath() { return m_skinPath; }
			QString SkinName() { return m_skinName; }

			void setDisableNudge(bool isDisableNudge);
			bool isDisableNudge() const { return m_DisableNudge; }

			void setAutoLogin(bool isAutoLogin);
			bool isAutoLogin() const { return m_isAutoLogin; }

			void setMainWindowTopHint(bool isMainWindowTopHint);
			bool isMainWindowTopHint() const { return m_isMainWindowTopHint; }

			void setAutoShowMsg(bool isAutoShow);
			bool isAutoShowMsg() const { return m_isAutoShow; }

			void setMute(bool isMute);
			bool isMute() const { return m_isMute; }

			void setEnableLongSMS(bool isEnableLongSMS);
			bool isEnableLongSMS() const { return m_isEnableLongSMS; }

			void setEnterSend(bool isEnterSend);
			bool isEnterSend() const { return m_isEnterSend; }

			void setStartHide(bool isStartHide);
			bool isStartHide() const { return m_isStartHide; }
                    
			void setAutoReply(bool isAutoReply, QString re_msg = NULL);
			bool isAutoReply() {return m_isAutoReply; }
			QString getAutoReply() {return m_replyMsg; }

			void setMsgRingPath(QString path);
			QString MsgRingPath() {return m_MsgRingPath;}

			void setLoginWinSize(QSize size);
			QSize LoginWinSize(){ return m_LoginWinSize; }
			
			void setLoginWinPos(QPoint pos);
			QPoint LoginWinPos(){ return m_LoginWinPos;}
			
			void setMsgWinPos(QPoint pos);// { m_msgWinPos = pos; }
			QPoint MsgWinPos(){ return m_MsgWinPos; }
			
			void setMsgWinSize(QSize size);
			QSize MsgWinSize(){ return m_MsgWinSize; }
			
			void setMainWinPos(QPoint pos);
			QPoint MainWinPos(){ return m_MainWinPos;}
			
			void setMainWinSize(QSize size);
			QSize MainWinSize(){ return m_MainWinSize;}
			
			void setSendMultMsgWinPos(QPoint pos);
			QPoint SendMultMsgWinPos(){ return m_SendMultMsgWinPos; }
			
			void setSendMultMsgWinSize(QSize size);
			QSize SendMultMsgWinSize(){ return m_SendMultMsgWinSize; }

            void saveFontSetting(const QFont & font);
			QFont getCurrentFont(){ return m_CurrentFont; }

            void setSysDefaultFont(QFont font){ m_DefaultFont = font; }
            QFont setSysDefaultFont(){ return m_DefaultFont; }

			bool setEnableGetMsgHotKey(bool enable);
			bool setGetMsgHotKey(QChar keyvalue, Qt::KeyboardModifiers keyMod, bool isRegister = true);

			bool isEnableGetMsgHotKey(){ return m_isEnableGetMsgHotKey; }
			//get hot key register sate, registed success will return true, or return false;
			bool isRegistedGetMsgHotKey(){ return m_isRegistedGetMsgHotKey; }
			QChar GetMsgHotKey(){ return m_GetMsgHotKey; }
			Qt::KeyboardModifiers GetMsgHotKeyMod(){ return m_GetMsgHotKeyMod; }
			
		private:
			void init_setting();

		private:
			bool m_DisableNudge : 1;
			bool m_isAutoLogin : 1;
			bool m_isMainWindowTopHint : 1;
			bool m_isEnableLongSMS : 1;
			bool m_isMute : 1;
			bool m_isAutoShow : 1;

			bool m_isEnterSend : 1;
			bool m_isStartHide : 1;
			bool m_isAutoReply : 1;
			
			QString m_skinPath;
			QString m_skinName;

			QString m_replyMsg;
			QString m_MsgRingPath;
			
			QPoint m_LoginWinPos;
			QSize m_LoginWinSize;

			QPoint m_MsgWinPos;
			QSize m_MsgWinSize;
			
			QPoint m_MainWinPos;
			QSize m_MainWinSize;

			QPoint m_SendMultMsgWinPos;
			QSize m_SendMultMsgWinSize;

			bool m_isEnableGetMsgHotKey : 1;
			bool m_isRegistedGetMsgHotKey : 1;
			QChar m_GetMsgHotKey;
			Qt::KeyboardModifiers m_GetMsgHotKeyMod;

			QFont m_CurrentFont;
			QFont m_DefaultFont;

			long m_uid;
			FxMainWindow *m_mainwind;
	};
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
