TEMPLATE      = app 

win32 {
	QT 		 += xml
  	TARGET    = LibFx
  	CONFIG   += static 
  	QTPLUGIN += qgif 
	RC_FILE   = ./misc/libfetion.rc 
	LIBS  	 += qgif.lib ./lib/libcurl_imp.lib
}

!win32 {
	QT 		 += xml
	TARGET    = linux-fetion
	LIBS 	 +=  -lcurl -lssl ./lib/libfetion_32.a
}

mac {
    CONFIG 	 += static 
    QTPLUGIN += qgif 
    TARGET    = Mac-Fetion
    ICON      = ./misc/mac_fetion.icns
    QMAKE_INFO_PLIST = ./misc/mac_fetion.plist
}


TRANSLATIONS    = fetion_utf8_CN.ts

HEADERS       =  \
				 appconfig.h \
				 fxmain.h \
				 fxmainwindow.h \ 
				 fxloginwindow.h \
				 fxbuddy.h \
				 fxmsgwindow.h \
				 fxaccountTab.h \
				 fxmytabwidget.h \
				 fxResource.h \
				 fxdb.h \
				 fxshowAccountInfo.h \
				 fxaddBuddyWindow.h \
				 fxqunwindow.h \
				 fxEncrypt.h \
				 fxshowHistory.h \
				 fxverifyAccount.h \
				 fxInputFace.h \
				 fxuitl.h \
				 fxproxy.h \
				 fxLableMouse.h \
				 fxLineEditFocus.h \
				 fxrefuseSMS.h \
				 fxsendGroupSMS.h \
				 fxsettings.h \
				 fxconfigDlg.h \
				 fxskinmanage.h \
				 ./sqlite/sqlite3.h


SOURCES       =  \
				main.cpp \
				fxmain.cpp \
				fxloginwindow.cpp \
				fxmainwindow.cpp \
				fxbuddy.cpp \
				fxmsgwindow.cpp \
				fxaccountTab.cpp \
				fxmytabwidget.cpp \
				fxResource.cpp \
				fxdb.cpp \
				fxshowAccountInfo.cpp \
				fxaddBuddyWindow.cpp \
				fxqunwindow.cpp \
				fxEncrypt.cpp \
				fxshowHistory.cpp \
				fxverifyAccount.cpp \
				fxInputFace.cpp \
				fxuitl.cpp \
				fxproxy.cpp \
				fxrefuseSMS.cpp \
				fxsendGroupSMS.cpp \
				fxsettings.cpp \
				fxconfigDlg.cpp \
				fxskinmanage.cpp \
				./sqlite/sqlite3.c

FORMS         =  \
				./ui/login_window.ui\
				./ui/fetionwindow.ui \
				./ui/msgwindow.ui \	
				./ui/msgsend.ui \
				./ui/showAccountInfo.ui \
				./ui/qunwindow.ui \
				./ui/historyMsg.ui \
				./ui/verifyAccount.ui \
				./ui/addBuddy.ui \
				./ui/proxy.ui \
				./ui/sendgroupsms.ui \
				./ui/refusesms.ui \
				./ui/configdia.ui
