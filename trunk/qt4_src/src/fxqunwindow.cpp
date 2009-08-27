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
#include <QtGui>

#include "fxqunwindow.h"
#include "fxshowHistory.h"

#include "fxutil.h" //fxgui_to_faces
FxQunWindow::FxQunWindow(qlonglong id, QWidget *parent, bool awaySendSms):
                         QMainWindow(parent)
{
    FX_FUNCTION
    setupUi(this);
    isQuit = false;
    haveUnreadMsg = false;

    view->header()->setHidden(1);
    view->setRootIsDecorated(true);

    qun_id = id;
    isAwaySendSMS = awaySendSms;

    fx_qun = fx_get_qun_by_id(id);

    if (!fx_qun)
    {
        this->hide();
        return ;
    }

    BT_SMS->setIcon(getBT_SMSIcon());
    connect(BT_SMS, SIGNAL(clicked()), this, SLOT(changeSendModle()));
    MsgEdit->installEventFilter(this);
    setQuninfo();
    setSendModle(isAwaySendSMS);

    QSize dt_size = QApplication::desktop()->size();
    this->move(dt_size.width() / 3, dt_size.height() / 3);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
FxQunWindow::~FxQunWindow()
{
    FX_FUNCTION
}

static QString getName(Fetion_QunMember *qunmem)
{
    QString name;
    if (qunmem->iicnickname && strcmp(qunmem->iicnickname, "") != 0)
    {
        name = QString::fromUtf8(qunmem->iicnickname);
    }
    else
        if (qunmem->nickname && strcmp(qunmem->nickname, "") != 0)
        {
            name = QString::fromUtf8(qunmem->nickname);
        }
        else
        {
            name = QString("%1").arg(qunmem->id);
        }

    return name;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
QString FxQunWindow::getSenderName(qlonglong sender)
{
    FX_FUNCTION
    if (!fx_qun)
    {
        return QString("%1").arg(sender);
    }

    QString name;
    Fetion_QunInfo *quninfo = fx_qun->quninfo;

    Fetion_QunMember *qunmem;
    DList *tmp_list = quninfo->QunMember;
    while (tmp_list)
    {
        qunmem = (Fetion_QunMember*)tmp_list->data;
        if (qunmem)
        {
            if (qunmem->id == sender)
            {
                return getName(qunmem);
            }
        }
        tmp_list = d_list_next(tmp_list);
    }

    return QString("%1").arg(sender);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::setQuninfo()
{
    FX_FUNCTION
    if (!fx_qun)
    {
        return ;
    }
    Fetion_QunInfo *quninfo = fx_qun->quninfo;

    if (!quninfo)
    {
        return ;
    }

    QString title = tr("fetion qun ") + QString::fromUtf8(quninfo->name);
    this->setWindowTitle(title);
    this->setWindowIcon(getOnlineStatusIcon(1));

    listQunMem(quninfo);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::listQunMem(Fetion_QunInfo *quninfo)
{
    FX_FUNCTION
    if (!quninfo)
    {
        return ;
    }

    Fetion_QunMember *qunmem;
    DList *tmp_list = quninfo->QunMember;
    while (tmp_list)
    {
        qunmem = (Fetion_QunMember*)tmp_list->data;
        if (qunmem)
        {
            QTreeWidgetItem *item;
            item = new QTreeWidgetItem(view);
            item->setText(0, getName(qunmem));
        }
        tmp_list = d_list_next(tmp_list);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::SendMsg()
{
    FX_FUNCTION
    QString msg = MsgEdit->toPlainText();
    if (msg.isEmpty())
    {
        return ;
    }

    bool sendFlag = false;

    if (isAwaySendSMS)
    {
        sendFlag = fx_send_sms(qun_id, msg.toUtf8().data(), NULL, NULL);
    }
    else
    {
        //note: just dialog_send need replace to html mark.
        msg.replace(QString("<"), QString("&lt;"));
        msg.replace(QString(">"), QString("&gt;"));
        msg.replace(QString("\n"), QString("<br>"));
        sendFlag = fx_dialog_send(qun_id, msg.toUtf8().data(), NULL, NULL);
    }
    QString show_msg;

    QString head;
    if (sendFlag)
    {
        head = "<b style=\"color:rgb(0,0,255);\">" +
               tr("Me:(") +
               QDateTime::currentDateTime().toString(tr("hh:mm:ss")) +
               "--" +
               QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
               ")</b><br>" +
               tr("\r\n");
    }
    else
    {
        head = "<b style=\"color:red;\">" +
               tr("send fail:(") +
               QDateTime::currentDateTime().toString(tr("hh:mm:ss")) +
               "--" +
               QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
               ")</b><br>";
    }

    msg.replace(QString("<"), QString("&lt;"));
    msg.replace(QString(">"), QString("&gt;"));
    msg.replace(QString("\n"), QString("<br>"));

    msg = fxgui_to_faces(msg);

    QString str = head + msg;

    show_msg = show_msg.fromUtf8(str.toUtf8().data());

    saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10),
                   qun_id,
                   show_msg.toUtf8().data(),
                   NULL);
    MsgBrowser->append(show_msg);

    //clean the send edit
    MsgEdit->setText("");
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::changeSendModle()
{
    FX_FUNCTION
    isAwaySendSMS = !isAwaySendSMS;
    setSendModle(isAwaySendSMS);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::setSendModle(bool isSMS)
{
    FX_FUNCTION
    isAwaySendSMS = isSMS;

    if (!fx_qun)
    {
        return ;
    }

    Fetion_QunInfo *quninfo = fx_qun->quninfo;
    if (!quninfo)
    {
        return ;
    }

    QString status;
    if (isAwaySendSMS)
    {
        status = QString::fromUtf8(quninfo->name) +
                 tr("will send qun sms");
    }
    else
    {
        status = QString::fromUtf8(quninfo->name) +
                 tr("will send qun message");
    }

    Ac_Status->setText(status);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
bool FxQunWindow::eventFilter(QObject *target, QEvent *event)
{
    FX_FUNCTION
    if (target == MsgEdit)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = (QKeyEvent*)event;

            if (keyEvent->key() == Qt::Key_Return)
            {
                if ((Settings::instance().isEnterSend() && keyEvent->modifiers()
                    != Qt::ControlModifier) || (!Settings::instance()
                    .isEnterSend() && keyEvent->modifiers() == Qt
                    ::ControlModifier))
                {
                    SendMsg();
                    return true;
                }
            }

            if (keyEvent->key() == Qt::Key_H && (keyEvent->modifiers() == Qt
                ::AltModifier || keyEvent->modifiers() == Qt::ControlModifier))
            {
                FxShowHistory *histroy = new FxShowHistory(qun_id, this);
                histroy->show();
                return true;
            }

            if (keyEvent->key() == Qt::Key_Escape)
            {
                if (MsgEdit->toPlainText().isEmpty())
                {
                    this->hide();
                    return true;
                }
            }
        }
    }
    return QObject::eventFilter(target, event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::closeEvent(QCloseEvent *event)
{
    FX_FUNCTION
    hide();
    if (isQuit)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxQunWindow::qun_exit()
{
    FX_FUNCTION
    isQuit = true;
    close();
}
