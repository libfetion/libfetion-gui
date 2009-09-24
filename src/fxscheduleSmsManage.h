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
#ifndef _FXSCHEDULESMSMANAGE_H
#define _FXSCHEDULESMSMANAGE_H

#include <QMainWindow>
#include <QDialog>
#include "fxbuddy.h"
#include "fxmainwindow.h"
#include "ui_scheduleSMSManage.h"

#include "fxdebug.h"
class FxScheduleSMSManage: public QMainWindow, public Ui::ScheduleSmSManage
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        FxScheduleSMSManage(FxMainWindow * mainwind, QWidget *parent = 0);
        ~FxScheduleSMSManage();

    private slots:
        void loadALlSCMList();
        void createItemOfSCMList();
        void deleteItemOfSCMList();
        void refreshSCMList();
        void chooseAllSCMList();

        void slot_update_scheduleList();

    protected:
        void addItemToSCMList(Fetion_Schedule_SMS*);
        void addItemToSCMList(QString &receiver, QString& send_time,
                QString &message, int scm_id);
        void closeEvent(QCloseEvent *event);
    private:
        void init_UI();
    private:
        FxMainWindow * mainwind;
};

#endif
