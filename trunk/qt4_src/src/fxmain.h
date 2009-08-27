/***************************************************************************
 *   Copyright (C) 2008 by DDD                                             *
 *   dedodong@163.com                                                      *
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
#ifndef FXMAIN_H
#define FXMAIN_H

class FxLoginWindow;
class FxMainWindow;

#include <QObject>

#include "fxdebug.h"

class FxMain : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        FxMain();
        ~FxMain();

    private:
        /* login window, first one you will see */
        FxLoginWindow *loginWin;
        /* the main guy, with your friend list in */
        FxMainWindow *mainWin;

        bool isLoginIn;
        void createMainWindow();
        void doSlotConnection();

    private slots:
        void slotLoginOK();
};
#endif
