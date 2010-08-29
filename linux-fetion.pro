TEMPLATE = app
################################################################################
#               Global Configuration
################################################################################
# product release configure
CONFIG -= debug
# development configure
#CONFIG += debug

QT += xml xmlpatterns
    
win32 { 
    message ("WIN32 build start...")
    TARGET = LibFx
    CONFIG += static
    QTPLUGIN += qgif
    QTPLUGIN += qjpeg
    RC_FILE = ./misc/libfetion.rc
    LIBS += qgif.lib qjpeg.lib \
        ./libfetion/lib/libcurl_imp.lib \
        ./libfetion/lib/libeay32.lib \
        ./libfetion/lib/ssleay32.lib
}

unix { 
    message ("UNIX build start...")
    TARGET = linux-fetion
    LIBS += -lcurl \
        -lssl -ldl -lcrypto\
        ./libfetion/lib/libfetion_32.a
}
mac { 
    message ("MAC build start...")
    CONFIG += static
    TARGET = Mac-Fetion
    ICON = ./misc/mac_fetion.icns
    QMAKE_INFO_PLIST = ./misc/mac_fetion.plist
    LIBS =  -lcurl \
            -lcrypto \
            -lssl \
            ./libfetion/lib/libfetion_mac.a
}

TRANSLATIONS = ./resource/fetion_utf8_CN.ts
QMAKE_CXXFLAGS += -I./libfetion/include \
    -I./src \
    -I./.ui
OBJECTS_DIR = ./.tmp
MOC_DIR = ./.moc
UI_HEADERS_DIR = ./.ui

# sub-directory sources
include (src/gui.pri)
include (ui/ui.pri)

# PR build, no log4qt linked in
# contains ($$CONFIG, debug) 
debug {
    message ("DEBUG profile entered...")
    # CONFIG contains 'debug'
    include (log4qt/log4qt.pri)
    DEFINES += HAVE_LOG4QT_DEBUG_ENABLED
}

################################################################################
#               Distribute files (data, config)
################################################################################
DISTFILES += resource/data/Location.xml
DISTFILES += src/LibFetionEventHandle.cpp
DISTFILES += 64_libfetion.sh \
    CREDITS.txt \
    install.sh \
    install_mac.sh \
    resource/fetion_utf8_CN.qm \
    resource/qt_zh_CN.qm \
    COPYING \
    README
DISTFILES += libfetion/lib/*.a \
    libfetion/lib/libcurl_imp.lib \
    resource/sound/msg.wav
DISTFILES += libfetion/include/*.h

################################################################################
#               Installation
################################################################################
# run a stricp to install our project
unix:target.extra = ./install.sh

# target
target.path += /usr/bin
INSTALLS += target
