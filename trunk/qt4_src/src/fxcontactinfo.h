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
#ifndef FXCONTACTINFO_H
#define FXCONTACTINFO_H

#include <QDialog>
#include "ui_contactinfo.h"

#include "fxutil.h"

class FxContactInfo : public QDialog, public Ui::ContactInfo
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        FxContactInfo(QWidget *parent = 0,
                const Fetion_Account *account = 0);
        ~FxContactInfo();
        QString getContactInfo();

    private slots:
        void chang_localname();

    private:
        QString htmlStyledValueString(QString data = NULL);
        QString htmlStyledNewline();

    private:
        const Fetion_Account *m_account;
};

#endif
