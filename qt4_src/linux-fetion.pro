TEMPLATE = app
################################################################################
#               Global Configuration
################################################################################
# product release configure
# CONFIG -= debug
# development configure
CONFIG += debug

QT += xml \
    xmlpatterns
    
win32 { 
    message ("WIN32 build start...")
    TARGET = LibFx
    CONFIG += static
    QTPLUGIN += qgif
    RC_FILE = ./misc/libfetion.rc
    LIBS += qgif.lib \
        ./lib/libcurl_imp.lib
}

unix { 
    message ("UNIX build start...")
    TARGET = linux-fetion
    LIBS += -lcurl \
        -lssl \
        ./lib/libfetion_32.a
}
mac { 
    message ("MAC build start...")
    CONFIG += static
    QTPLUGIN += qgif
    TARGET = Mac-Fetion
    ICON = ./misc/mac_fetion.icns
    QMAKE_INFO_PLIST = ./misc/mac_fetion.plist
    LIBS =  -lcurl \
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
DISTFILES += data/Location.xml
DISTFILES += src/LibFetionEventHandle.cpp
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

################################################################################
#               Installation
################################################################################
# run a stricp to install our project
unix:target.extra = ./install.sh

# target
target.path += /usr/bin
INSTALLS += target
