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
#ifndef FXCONFIGDLG_H
#define FXCONFIGDLG_H

#include <QDialog>
#include "ui_configdia.h"
#include "fxmainwindow.h"

class FxConfigDia : public QDialog, public Ui::ConfigApp
{
    Q_OBJECT
public:
    FxConfigDia(FxMainWindow *wind, QWidget *parent = 0);
    ~FxConfigDia();
    bool eventFilter(QObject *target, QEvent *event);
    static bool hasInstance;

private:
    void _updateFonts(const QFont & font);

private slots:
    void slot_DisableNudge();
    void slot_MainTopHit();
    void slot_AutoShowMsg();
    void slot_RemberPwd();
    void slot_MainStartHide();
    void slot_Mute();
    void slot_LongMsg();
    void slot_SendMode();
    void slot_AutoReplyMsg();
    void slot_LimitReplyMsgLenth();
    void slot_TestRing();
    void slot_ChangeRing();
    void slot_DefaultRing();
    void slot_EnableHotKey();
    void slot_SetFont();
    void slot_SetDefaultFont();

protected:
    void closeEvent(QCloseEvent *event);
    void init_state();
    void init_connect();
    bool getHotKeyValue(QKeyEvent *keyEvent);
    void setHotKeyValue();

private:
    FxMainWindow *mainwind;
};
#endif
