#!/bin/sh
#
# install the mac-fetion

mkdir -p ./Mac-Fetion.app/Contents/Resources/
cp misc/mac_fetion.icns Mac-Fetion.app/Contents/Resources/
touch Mac-Fetion.app

sudo mkdir -p /usr/share/libfetion/
sudo cp -rf README COPYING ChangeLog CREDITS.txt skins faces_image fetion_utf8_CN.qm qt_zh_CN.qm sound /usr/share/libfetion/
sudo chmod -R 755 /usr/share/libfetion/

#install lib & frame
#for qgif
install_name_tool -change libqgif.dylib @loader_path/../SharedSupport/libqgif.dylib ./Mac-Fetion.app/Contents/MacOS/Mac-Fetion
#for Qt Framework
#...

