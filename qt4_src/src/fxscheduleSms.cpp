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

#include "fxscheduleSms.h"
#include "fxbuddy.h"
#include "fxmainwindow.h"
#include "fxscheduleSmsManage.h"

#include "fxutil.h" //fxgui_to_faces

FxScheduleSMS::FxScheduleSMS(FxMainWindow *wind, QWidget *parent):
        QMainWindow(parent)
{
    FX_FUNCTION
    setupUi(this);
    mainwind = wind;

    dateEdit->setDate(QDate::currentDate());
    dateEdit->setMinimumDate(QDate::currentDate());
    dateEdit->setMaximumDate(QDate::currentDate().addYears(1));

    timeEdit->setTime(QTime::currentTime().addSecs(60 *10));

    view->header()->setHidden(1);
    view->setRootIsDecorated(true);
    buddyopt = new BuddyOpt(view, false);

    connect(timeEdit, SIGNAL(editingFinished()),
            this, SLOT(timeEditFinished()));

    connect(MsgEdit, SIGNAL(textChanged()),
            this, SLOT(ChangeInputNM()));
    connect(TB_Send, SIGNAL(clicked()),
            this, SLOT(SendMsg()));
    connect(buddyopt, SIGNAL(m_itemChanged()),
            this, SLOT(ChangechooseNM()));
    connect(Schedule_Sms_Manage, SIGNAL(linkActivated(const QString &)),
            this, SLOT(ShowSCM_Manage(const QString &)));

    ChangechooseNM();
    ChangeInputNM();

    QSize dt_size = QApplication::desktop()->size();
    this->move(dt_size.width() / 3,
               dt_size.height() / 3);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxScheduleSMS::~FxScheduleSMS()
{
    FX_FUNCTION
}


void FxScheduleSMS::timeEditFinished()
{
    FX_FUNCTION
    if (dateEdit->date() > QDate::currentDate())
    {
        return ;
    }
    /* 10 minutes */
    if (timeEdit->time() < QTime::currentTime().addSecs(600))
    {
        timeEdit->setTime(QTime::currentTime().addSecs(600));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxScheduleSMS::moveEvent(QMoveEvent *event)
{
    FX_FUNCTION
    Q_UNUSED(event);
    Settings::instance().setSendMultMsgWinPos(pos());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxScheduleSMS::SendMsg()
{
    FX_FUNCTION
    QString time;
    time = dateEdit->date().toString("yyyy-MM-dd") +
           " " +
           timeEdit->time().toString("hh:mm") +
           ":00";

    QString msg = MsgEdit->toPlainText();
    if (msg.isEmpty())
    {
        MsgBrowser->append("<b style=\"color:red;\">" +
                           tr("please input conntent") +
                           "</b><br>");
        return ;
    }

    if (buddyopt->markedCount <= 0)
    {
        MsgBrowser->append("<b style=\"color:red;\">" +
                           tr("please onereceiver") +
                           " <  / b >  < br > ");
        return ;
    }

    if (buddyopt->markedCount > 32)
    {
        MsgBrowser->append("<b style=\"color:red;\">" +
                           tr("the receier numbers are must less  32") +
                           "</b><br>");
        return ;
    }

    QTreeWidgetItem *RootItem = view->invisibleRootItem();
    if (!RootItem)
    {
        return ;
    }
    int GroupCount = RootItem->childCount();
    QTreeWidgetItem *groupItem = NULL;

    QString sendMsg = msg;

    QString show_msg;
    QString head = "<b style=\"color:rgb(0,0,255);\">" +
                   tr("group send:(") +
                   QDateTime::currentDateTime().toString(tr("hh:mm:ss")) + "--" +
                   QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
                   ")</b><br>";

    msg.replace(QString("<"), QString("&lt;"));
    msg.replace(QString(">"), QString("&gt;"));
    msg.replace(QString("\n"), QString("<br>"));
    msg = fxgui_to_faces(msg);

    show_msg = show_msg.fromUtf8((head + msg).toUtf8().data());
    MsgBrowser->append(show_msg);

    const Fetion_Account *account = NULL;
    DList *receiver = NULL;
    //send qun sms message...
    for (int i = 0; i < GroupCount; i++)
    {
        groupItem = RootItem->child(i);
        if (!groupItem)
        {
            continue;
        }
        int itemCounts = groupItem->childCount();
        QTreeWidgetItem *tmpItem = NULL;

        for (int i = 0; i < itemCounts; i++)
        {
            tmpItem = groupItem->child(i);
            if (!tmpItem || tmpItem->checkState(0) != Qt::Checked)
            {
                continue;
            }
            #if MS_VC6
                Account_Info *ac_info =
                        (Account_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
            #else
                Account_Info *ac_info =
                        tmpItem->data(0, Qt::UserRole).value <Account_Info * >();
            #endif
            account = fx_get_account_by_id(ac_info->accountID);
            receiver = d_list_append(receiver, (void*)account);
        }
    }

    if (!fx_set_schedule_sms(receiver,
                             sendMsg.toUtf8().data(),
                             time.toUtf8().data(),
                             NULL, NULL))
    {
        MsgBrowser->append(tr("send schedule sms failure"));
    }
    else
    {
        MsgBrowser->append(tr("sending schedule sms "));
    }

    //clean the send edit
    MsgEdit->setText("");
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxScheduleSMS::ChangechooseNM()
{
    FX_FUNCTION
    QString txt = tr("have choose [") +
                  QString("%1").arg(buddyopt->markedCount) +
                  tr("] ") +
                  tr("(at most 32)");

    lb_choseUser->setText(txt);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxScheduleSMS::ChangeInputNM()
{
    FX_FUNCTION
    QString msg = MsgEdit->toPlainText();
    if (msg.size() > MAXSMSLENGTH)
    {
        msg = msg.left(MAXSMSLENGTH);
        MsgEdit->setPlainText(msg);
        MsgEdit->moveCursor(QTextCursor::End);
    }

    QString txt = tr("you can input [") +
                  QString("%1").arg(MAXSMSLENGTH - msg.size()) +
                  tr("] character");

    input_nm->setText(txt);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxScheduleSMS::ShowSCM_Manage(const QString &)
{
    FX_FUNCTION
    if (mainwind && mainwind->getScheduleSmsManagerWindows())
    {
        mainwind->getScheduleSmsManagerWindows()->showNormal();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxScheduleSMS::closeEvent(QCloseEvent *event)
{
    FX_FUNCTION
    Q_UNUSED(event);
    if (mainwind)
    {
        mainwind->showNormal();
    }

    this->hide();
}
