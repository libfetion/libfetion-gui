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


#include <QApplication>
#include <QTranslator>
#include <QtPlugin>

//@TO FIX remove it affter fixed styleshee's import
#include <QFile>

#include "appconfig.h"
#include "fxmain.h"
#include "fxdb.h"
#include "fxskinmanage.h"

#ifdef WIN32
#else
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#endif


#ifdef WIN32
Q_IMPORT_PLUGIN(qgif)
#else
	#ifdef Q_OS_MAC
		Q_IMPORT_PLUGIN(qgif)
	#else //linux
		//Q_IMPORT_PLUGIN(qgif)
	#endif
#endif

int main(int argc, char *argv[])
{
	if(!fx_init()) {
		fprintf(stderr, "init the libfetion fail \n");
		exit(0);
	}
	init_db();

#ifdef WIN32
#else
    //compatible old app's config file
    moveOldConfigFile();
#endif

	QApplication app(argc, argv);
	QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath()); 

	QTranslator translator;
	translator.load( "fetion_utf8_CN", defaultResPath());
	app.installTranslator( &translator );

	Settings::instance().setSyetemDefualFont( QApplication::font() );
	app.setFont(Settings::instance().getCurrentFont());
	setSkins(Settings::instance().SkinPath(), Settings::instance().SkinName());
	FxMain *mainWin = new FxMain;

	//QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	app.setQuitOnLastWindowClosed(false);
	int reslut = app.exec();

	if (mainWin)
		delete mainWin;

	destoy_db();
	//destroy the libfetion
	fx_terminate();
	return reslut; 
}
