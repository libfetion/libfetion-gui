TEMPLATE      = app 

QT 		 += xml
win32 {
  	TARGET    = LibFx
  	CONFIG   += static 
  	QTPLUGIN += qgif 
	RC_FILE   = ./misc/libfetion.rc 
	LIBS  	 += qgif.lib ./lib/libcurl_imp.lib
}

unix {
	TARGET    = linux-fetion
	LIBS 	 +=  -lcurl -lssl ./lib/libfetion_32.a
}

mac {
    CONFIG 	 += static 
    QTPLUGIN += qgif 
    TARGET    = Mac-Fetion
    ICON      = ./misc/mac_fetion.icns
    QMAKE_INFO_PLIST = ./misc/mac_fetion.plist
    LIBS 	 =  -lcurl -lssl ./lib/libfetion_mac.a
    QMAKE_CXXFLAGS += -DMAC_OS=1
}

TRANSLATIONS    = fetion_utf8_CN.ts

QMAKE_CXXFLAGS += -I./ -I./src -I./.ui

OBJECTS_DIR = ./.tmp
MOC_DIR = ./.moc
UI_HEADERS_DIR = ./.ui


HEADERS       =  \
				 ./src/appconfig.h \
				 ./src/fxmain.h \
				 ./src/fxmainwindow.h \ 
				 ./src/fxloginwindow.h \
				 ./src/fxbuddy.h \
                 ./src/fxbuddyManage.h \
				 ./src/fxmsgwindow.h \
				 ./src/fxaccountTab.h \
				 ./src/fxmytabwidget.h \
				 ./src/fxResource.h \
				 ./src/fxdb.h \
				 ./src/fxshowAccountInfo.h \
				 ./src/fxaddBuddyWindow.h \
				 ./src/fxqunwindow.h \
				 ./src/fxEncrypt.h \
				 ./src/fxshowHistory.h \
				 ./src/fxverifyAccount.h \
				 ./src/fxInputFace.h \
				 ./src/fxutil.h \
				 ./src/fxproxy.h \
				 ./src/fxLableMouse.h \
				 ./src/fxLineEditFocus.h \
				 ./src/fxrefuseSMS.h \
				 ./src/fxsendGroupSMS.h \
				 ./src/fxscheduleSms.h \
				 ./src/fxscheduleSmsManage.h \
				 ./src/fxsettings.h \
				 ./src/fxconfigDlg.h \
				 ./src/fxskinmanage.h \
           	     ./src/fxwidget.h \
           	     ./src/fxwidgettitlebar.h \
                 ./src/qlayoutengine_p.h \
                 ./src/qwidgetresizehandler_p.h \
				 ./src/sqlite/sqlite3.h


SOURCES       =  \
				./src/main.cpp \
				./src/fxmain.cpp \
				./src/fxloginwindow.cpp \
				./src/fxmainwindow.cpp \
                ./src/fxbuddy.cpp \
                ./src/fxbuddyManage.cpp \
				./src/fxmsgwindow.cpp \
				./src/fxaccountTab.cpp \
				./src/fxmytabwidget.cpp \
				./src/fxResource.cpp \
				./src/fxdb.cpp \
				./src/fxshowAccountInfo.cpp \
				./src/fxaddBuddyWindow.cpp \
				./src/fxqunwindow.cpp \
				./src/fxEncrypt.cpp \
				./src/fxshowHistory.cpp \
				./src/fxverifyAccount.cpp \
				./src/fxInputFace.cpp \
				./src/fxutil.cpp \
				./src/fxproxy.cpp \
				./src/fxrefuseSMS.cpp \
				./src/fxsendGroupSMS.cpp \
				./src/fxscheduleSms.cpp \
				./src/fxscheduleSmsManage.cpp \
				./src/fxsettings.cpp \
				./src/fxconfigDlg.cpp \
				./src/fxskinmanage.cpp \
                ./src/fxwidget.cpp \
                ./src/fxwidgettitlebar.cpp \
                ./src/qlayoutengine.cpp \
                ./src/qwidgetresizehandler.cpp \
				./src/sqlite/sqlite3.c

FORMS         =  \
				./src/ui/login_window.ui\
				./src/ui/fetionwindow.ui \
				./src/ui/msgwindow.ui \	
				./src/ui/msgsend.ui \
				./src/ui/showAccountInfo.ui \
				./src/ui/qunwindow.ui \
				./src/ui/historyMsg.ui \
				./src/ui/verifyAccount.ui \
				./src/ui/addBuddy.ui \
				./src/ui/proxy.ui \
				./src/ui/sendgroupsms.ui \
				./src/ui/scheduleSms/scheduleSMS.ui \
				./src/ui/scheduleSms/scheduleSMSManage.ui \
				./src/ui/refusesms.ui \
				./src/ui/configdia.ui
