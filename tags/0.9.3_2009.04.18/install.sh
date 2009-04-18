#!/bin/sh
#
# install the linux-fetion

sudo mkdir -p /usr/share/pixmaps/
sudo cp -rf ./misc/fetion.png /usr/share/pixmaps/
sudo chmod 644 /usr/share/pixmaps/fetion.png

sudo mkdir -p /usr/share/app-install/icons/ 
sudo cp ./misc/fetion.png /usr/share/app-install/icons/
sudo chmod 644 /usr/share/app-install/icons/fetion.png

sudo mkdir -p /usr/share/libfetion/
sudo cp -rf CREDITS.txt skins/ faces_image/ fetion_utf8_CN.qm sound/ /usr/share/libfetion/
sudo chmod -R 755 /usr/share/libfetion/

sudo mkdir -p /usr/share/applications/ 
sudo cp -rf ./misc/LibFetion.desktop /usr/share/applications/
sudo chmod 644 /usr/share/applications/LibFetion.desktop

sudo cp -rf linux-fetion /usr/bin
sudo strip /usr/bin/linux-fetion
sudo chmod +x /usr/bin/linux-fetion
