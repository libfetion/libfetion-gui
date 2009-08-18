TEMPLATE = app
QT += xml
win32 { 
    TARGET = LibFx
    CONFIG += static
    QTPLUGIN += qgif
    RC_FILE = ./misc/libfetion.rc
    LIBS += qgif.lib \
        ./lib/libcurl_imp.lib
}
unix { 
    TARGET = linux-fetion
    LIBS += -lcurl \
        -lssl \
        ./lib/libfetion_32.a
}
mac { 
    CONFIG += static
    QTPLUGIN += qgif
    TARGET = Mac-Fetion
    ICON = ./misc/mac_fetion.icns
    QMAKE_INFO_PLIST = ./misc/mac_fetion.plist
    LIBS = -lcurl \
        -lssl \
        ./lib/libfetion_mac.a
}
TRANSLATIONS = fetion_utf8_CN.ts
QMAKE_CXXFLAGS += -I./ \
    -I./src \
    -I./.ui
OBJECTS_DIR = ./.tmp
MOC_DIR = ./.moc
UI_HEADERS_DIR = ./.ui
HEADERS = ./src/appconfig.h \
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
    ./src/fxusertree.h \
    ./src/fxeditablelabel.h \
    ./src/sqlite/sqlite3.h \
    ./src/fxlocationxmlparser.h
SOURCES = ./src/main.cpp \
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
    ./src/sqlite/sqlite3.c \
    ./src/fxlocationxmlparser.cpp
FORMS = ./ui/login_window.ui \
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
    ./ui/scheduleSms/scheduleSMS.ui \
    ./ui/scheduleSms/scheduleSMSManage.ui \
    ./ui/refusesms.ui \
    ./ui/configdia.ui
DISTFILES += src/LibFetionEventHandle.cpp
DISTFILES += src/fxprovAndCityCode.cpp
DISTFILES += 64_libfetion.sh \
    CREDITS.txt \
    install.sh \
    fetion_utf8_CN.qm \
    COPYING \
    README
DISTFILES += lib/*.a \
    lib/libcurl_imp.lib \
    sound/msg.wav
DISTFILES += libfetion/*.h

# run a stricp to install our project
unix:target.extra = ./install.sh

# target
target.path += /usr/bin
INSTALLS += target
