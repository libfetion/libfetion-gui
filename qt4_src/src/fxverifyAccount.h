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
#ifndef FXVERIFYACCOUNT_H
#define FXVERIFYACCOUNT_H

#include <QDialog>
#include "ui_verifyAccount.h"

#include "fxmainwindow.h"

#include "fxdebug.h"

class FxVerifyAccount: public QDialog, public Ui::VerifyAccount
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        FxVerifyAccount(FxMainWindow *wind, char*uri, char*desc,
                QWidget *parent = 0);
        ~FxVerifyAccount();
        void init_groupItem();

    protected:
        void closeEvent(QCloseEvent *event);

    private slots:
        void verifyAccount();
        void verifyRB();

    private:
        char * m_uri;
        QString m_desc;
        FxMainWindow *mainwind;
};

#endif
