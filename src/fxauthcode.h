/***************************************************************************
 *   Copyright (C) 2010 by DDD                                          *
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
#ifndef FXAUTHCODE_H
#define FXAUTHCODE_H

#include <QDialog>
#include "ui_auth_code.h"

#include "fxglobal.h"

#include "fxdebug.h"
class FxAuthCode: public QDialog, public Ui::AUTHCODE
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        FxAuthCode(QWidget *parent = 0);
        ~FxAuthCode();
        void closeEvent(QCloseEvent *event);
		void authcode_destroy();
		void reset(char* guid);
    private slots:
		void SetAuthCode();
    private:
        bool isQuit;
		QPixmap auth_pic;
		Fetion_Verfy *verfy;
};

#endif
