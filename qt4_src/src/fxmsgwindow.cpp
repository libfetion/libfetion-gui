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

#include <math.h>
#include "fxmainwindow.h"
#include "fxmsgwindow.h"
#include "fxaddBuddyWindow.h"
#include "fxskinmanage.h"

#include "fxInputFace.h"
#define SHAKE_DISTANCE 10
#define SHAKE_CYCLE 9
//msec
#define SHAKE_DURATION 800

//msec
#define SHAKE_TIMER_UPDATE 57

#define MAXNICELENGTH 8

//find is have the ac_id
inline AccountTab *findFromMsgWindow(QTabWidget *tabWidget, qlonglong ac_id)
{
    AccountTab *ac_tab = NULL;
    int tabCount = tabWidget->count();

    for (int i = 0; i < tabCount; i++)
    {
        ac_tab = (AccountTab*)tabWidget->widget(i);
        if (ac_tab && ac_tab->account_id == ac_id)
        {
            return ac_tab;
        }
    }
    return NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
FxMsgWindow::FxMsgWindow(QWidget *parent): FxWidget(parent)
{
    FX_FUNCTION
    setupUi(contentWidget);
    setObjectName("ChatWindow");
    m_mainwindow = NULL;
    m_willQuit = false;
    closeTabButton = NULL;
    m_isNudgeShake = false;

    init();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::init()
{
    FX_FUNCTION
    init_inputFace();
    init_UI();

    CHECK_SystemTiTle();

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT
            (currentChangedTab(int)));
    connect(tabWidget, SIGNAL(mouseDblClick(int)),
            this, SLOT(closeTabWid(int)));
    connect(&nudge_timer, SIGNAL(timeout()),
            this, SLOT(slot_do_shake()));

    // ESCAPE for hide
    QShortcut *hideShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(hideShortcut, SIGNAL(activated()), this, SLOT(hide()));
    // Ctrl+W to close current tab
    QShortcut *closeShortcut = new QShortcut(QKeySequence(tr("Ctrl+W")),
                                             this,
                                             0,
                                             0,
                                             Qt::WindowShortcut);
    connect(closeShortcut, SIGNAL(activated()), this, SLOT(closeCurrentTab()));
    // map Alt+1~ Alt+9 for switch current tab
    QSignalMapper *_signalMapper = new QSignalMapper(this);

    for (int i = 1; i < 10; ++i)
    {
        QShortcut *_tempShortcut = new QShortcut(QString("Alt+%1").arg(i),
                                                 this,
                                                 0,
                                                 0,
                                                 Qt::WindowShortcut);
        connect(_tempShortcut, SIGNAL(activated()), _signalMapper, SLOT(map()));
        _signalMapper->setMapping(_tempShortcut, i - 1);
    }
    connect(_signalMapper, SIGNAL(mapped(int)),
            tabWidget, SLOT(setCurrentIndex(int)));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::init_inputFace()
{
    FX_FUNCTION
    inputFace = new FxInputFace(contentWidget);
    inputFace->setObjectName("facesPopup");
    inputFace->setMsgWindow(this);
    inputFace->setGeometry(QCursor::pos().x(), QCursor::pos().y(), 320, 261);
    inputFace->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::init_UI()
{
    FX_FUNCTION
    QPalette pal;
    pal = palette();
    pal.setColor(QPalette::Active,
                 QPalette::Button,
                 pal.color(QPalette::Active, QPalette::Window));
    pal.setColor(QPalette::Disabled,
                 QPalette::Button,
                 pal.color(QPalette::Disabled, QPalette::Window));
    pal.setColor(QPalette::Inactive,
                 QPalette::Button,
                 pal.color(QPalette::Inactive, QPalette::Window));

    #if QT_VERSION >= 0x040500
        // comment by iptton .... these code should be placed in fxmytabwidget.cpp
        tabWidget->setTabsClosable(true);
        connect(tabWidget, SIGNAL(tabCloseRequested(int)),
                this, SLOT(closeTabWid(int)));
    #else
        closeTabButton = new QToolButton(tabWidget);
        closeTabButton->setAutoRaise(true);
        closeTabButton->setEnabled(true);
        closeTabButton->setPalette(pal);

        tabWidget->setCornerWidget(closeTabButton, Qt::TopRightCorner);
        closeTabButton->setCursor(Qt::ArrowCursor);
        closeTabButton->setIcon(getCloseTabImage());
        connect(closeTabButton, SIGNAL(clicked()),
                this, SLOT(closeCurrentTab()));
        closeTabButton->setToolTip(tr("close current Tab"));
    #endif // end mac of QT_VERSION


    tabWidget->setParent(contentWidget);
    tabWidget->clear();

    move(Settings::instance().MsgWinPos());
    resize(Settings::instance().MsgWinSize());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::currentChangedTab(int index)
{
    FX_FUNCTION
    AccountTab *accountTab = (AccountTab*)tabWidget->widget(index);
    setCurrentTabTitle(accountTab);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::setCurrentTabTitle(AccountTab *accountTab)
{
    FX_FUNCTION
    if (!accountTab)
    {
        return ;
    }

    accountTab->endFlickerTab();
    QString title = tr("with ") + accountTab->account_name + tr(" chating... ");
    this->setWindowTitle(title);

    int online_state;
    if (!fx_is_pc_user_by_account(accountTab->m_account))
    {
        online_state =  - 1;
    }
    else
    {
        online_state = fx_get_online_status_by_account(accountTab->m_account);
    }

    this->setWindowIcon(getOnlineStatusIcon(online_state));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::closeCurrentTab()
{
    FX_FUNCTION
    closeTabWid(tabWidget->currentIndex());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::closeTabWid(int index)
{
    FX_FUNCTION
    AccountTab *accountTab = (AccountTab*)tabWidget->widget(index);

    tabWidget->removeTab(index);
    //if the tabWidget have no tab, hide it..
    if (tabWidget->count() <= 0)
    {
        this->hide();
    }

    // there's warning:
    // QObject do not delete object, 'MsgEdit', during its event handler!
    //if (accountTab)
    //	delete accountTab;
    if (accountTab)
    {
        accountTab->deleteLater();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::showQunUnreadMsg()
{
    FX_FUNCTION
    int size = qunWindow.size();
    for (int i = 0; i < size; i++)
    {
        if (qunWindow.at(i)->isHaveUnReadMsg())
        {
            qunWindow.at(i)->showNormal();
            qunWindow.at(i)->MsgEdit->setFocus();
            qunWindow.at(i)->setHaveUnReadMsg(false);
            return ;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
FxQunWindow *FxMsgWindow::findQunWindow(qlonglong qun_id)
{
    FX_FUNCTION
    int size = qunWindow.size();
    for (int i = 0; i < size; i++)
        if (qunWindow.at(i)->qun_id == qun_id)
        {
            return qunWindow.at(i);
        }
    return NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool FxMsgWindow::addQunMessage(QString msg,
                                qlonglong qun_id,
                                qlonglong sender,
                                bool iscoming_msg)
{
    FX_FUNCTION
    if (msg.isEmpty())
    {
        return false;
    }

    if (!Settings::instance().isMute())
    {
        playSound(MSG_SOUND);
    }

    FxQunWindow *qunW = findQunWindow(qun_id);
    if (!qunW)
    {
        qunW = new FxQunWindow(qun_id, this);
        qunWindow.append(qunW);
    }

    QString str;
    QString show_msg;
    if (iscoming_msg)
    {
        str = "<b style=\"color:rgb(0,85,0);\">" +
              qunW->getSenderName(sender)+
              msg;
        show_msg = show_msg.fromUtf8(str.toUtf8().data());
    }
    else
    {
        show_msg = msg;
    }

    QTextEdit *msgBrowser = qunW->MsgBrowser;
    msgBrowser->append(show_msg);
    msgBrowser->moveCursor(QTextCursor::End);

    saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10),
                   qun_id,
                   show_msg.toUtf8().data(),
                   NULL);

    if (qunW->isVisible() || Settings::instance().isAutoShowMsg())
    {
        qunW->showNormal();
        qunW->MsgEdit->setFocus();
        qunW->setHaveUnReadMsg(false);
    }
    else
    {
        /** if we run here, we need to add addNewMsgCount,
         * and the qun is pre window one count, so do that:
         */
        if (!qunW->isHaveUnReadMsg())
        {
            qunW->setHaveUnReadMsg(true);
            m_mainwindow->addNewMsgCount(true);
        }
    }

    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
bool FxMsgWindow::addMessage(QString msg,
                             qlonglong account_id,
                             bool iscoming_msg)
{
    FX_FUNCTION
    if (msg.isEmpty())
    {
        return false;
    }

	if (!fx_get_account_by_id(account_id))
		return false;

    AccountTab *accountTab = findFromMsgWindow(tabWidget, account_id);

    if (!accountTab)
    {
        accountTab = new AccountTab(account_id, tabWidget);
        accountTab->setMainWind(this->m_mainwindow);
        tabWidget->addTab(accountTab, cropTabName(accountTab->account_name));
    }

    if (!this->isVisible())
    {
        tabWidget->setCurrentWidget(accountTab);
        setCurrentTabTitle(accountTab);

        if (Settings::instance().isAutoShowMsg())
        {
            this->showNormal();
            this->setFocus();
            accountTab->msgSend->MsgEdit->setFocus();
        }
    }

    QString str;
    QString show_msg;
    if (iscoming_msg)
    {
        str = "<b style=\"color:rgb(0,85,0);\">" +
              accountTab->account_name +
              msg;
        show_msg = show_msg.fromUtf8(str.toUtf8().data());
    }
    else
    {
        show_msg = show_msg.fromUtf8(msg.toUtf8().data());
    }

    if (account_id == SYSTEM_ID)
    {
        bool tmp = selectSystemMsg(strtol(fx_get_usr_uid(), NULL, 10),
                                   SYSTEM_ID,
                                   show_msg.toUtf8().data());
        if (tmp)
        {
            return true;
        }
    }

    QTextEdit *msgBrowser = accountTab->msgSend->MsgBrowser;
    msgBrowser->append(show_msg);
    msgBrowser->moveCursor(QTextCursor::End);

    saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10),
                   account_id,
                   show_msg.toUtf8().data(),
                   NULL);

    if (Settings::instance().isAutoReply() && iscoming_msg)
    {
        exec_autoRelpy(msgBrowser,
                       account_id,
                       Settings::instance().getAutoReply());
    }

    accountTab->startFlickerTab();

    if (!Settings::instance().isMute())
    {
        playSound(MSG_SOUND);
    }

    return true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::exec_autoRelpy(QTextEdit *msgBrowser, qlonglong account_id,
                                 QString msg)
{
    FX_FUNCTION
    bool sendFlag = false;
    msg = QObject::tr("(LibFetion auto reply msg)") + msg;

    if (!fx_is_pc_user_by_id(account_id))
    {
        sendFlag = fx_send_sms(account_id, msg.toUtf8().data(), NULL, NULL);
    }
    else
    {
        msg.replace(QString("<"), QString("&lt;"));
        msg.replace(QString(">"), QString("&gt;"));
        sendFlag = fx_dialog_send(account_id, msg.toUtf8().data(), NULL, NULL);
    }

    QString show_msg;
    QString head;
    if (sendFlag)
    {
        head = "<b style=\"color:rgb(0,0,255);\">" +
               tr("Me(") +
               QDateTime::currentDateTime().toString(tr("hh:mm:ss")) +
               "--" +
               QDateTime::currentDateTime().toString(tr("yyyy-MM-dd")) +
               "):</b><br>";
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
    msg = FxInputFace::parseSmileySymbol(msg);

    QString str = head + msg;
    show_msg = show_msg.fromUtf8(str.toUtf8().data());

    //show the send reslut to the browser...
    msgBrowser->append(show_msg);

    saveHistroyMsg(strtol(fx_get_usr_uid(), NULL, 10),
                   account_id,
                   show_msg.toUtf8().data(),
                   NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::slot_haveNewQunMessage(qlonglong qun_id)
{
    FX_FUNCTION
    Fetion_MSG *fxMsg = fx_get_msg(qun_id);
    if (!fxMsg)
    {
        return ;
    }

    long sender = fxMsg->ext_id;
    addQunMessage(parseReceivedMsg(fxMsg), qun_id, sender, true);
    fx_destroy_msg(fxMsg);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::slot_haveNewMessage(qlonglong account_id)
{
    FX_FUNCTION
    Fetion_MSG *fxMsg = fx_get_msg(account_id);
    if (!fxMsg)
    {
        return ;
    }

    if (fxMsg->ext_id != 0)
    {
        addQunMessage(parseReceivedMsg(fxMsg), account_id, 0L, true);
    }
    else
    {
        addMessage(parseReceivedMsg(fxMsg), account_id, true);
    }

    fx_destroy_msg(fxMsg);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::resizeEvent(QResizeEvent *event)
{
    FX_FUNCTION
    Settings::instance().setMsgWinSize(size());
    tabWidget->resize(event->size());
    FxWidget::resizeEvent(event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::closeEvent(QCloseEvent *event)
{
    FX_FUNCTION
    hide();
    if (m_willQuit)
    {
        int size = qunWindow.size();
        for (int i = 0; i < size; i++)
        {
            qunWindow.at(i)->qun_exit();
        }

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

void FxMsgWindow::moveEvent(QMoveEvent *event)
{
    FX_FUNCTION
    Q_UNUSED(event);
    //when it is shaking, we don't recode the pos of window.
    if (m_isNudgeShake)
    {
        return ;
    }
    Settings::instance().setMsgWinPos(pos());
    FxWidget::moveEvent(event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::addQunWin(qlonglong qun_id, bool isSendSms)
{
    FX_FUNCTION
    FxQunWindow *qun = findQunWindow(qun_id);
    if (!qun)
    {
        qun = new FxQunWindow(qun_id, this, isSendSms);
        qunWindow.append(qun);
    }
    qun->showNormal();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::addBuddy(qlonglong account_id)
{
    FX_FUNCTION
    QString id;
    bool ismobile = false;
    if (fx_is_pc_user_by_id(account_id))
    {
        id = QString("%1").arg(account_id);
        ismobile = false;
    }
    else
    {
        char *mobile_no = fx_get_original_ID(account_id);
        id = QString(mobile_no);
        free(mobile_no);
        mobile_no = NULL;

        ismobile = true;
    }

    m_mainwindow->show();
    FxAddBuddy *tmp_addBuddy = new FxAddBuddy(id, ismobile, m_mainwindow);
    tmp_addBuddy->exec();
    //fix a bug when addbuddy show, and mainwindow is hide, quit the addbuddy,
    //mainwindows will quit too.
    m_mainwindow->show();
    delete tmp_addBuddy;
    tmp_addBuddy = NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMsgWindow::addAccount(qlonglong account_id, bool isSendSms)
{
    FX_FUNCTION
    // VerifiedAccount
    if (fx_is_InBlacklist_by_id(account_id))
    {
        QMessageBox::information(this->parentWidget(),
                                 tr("can't send mseeage to he"),
                                 tr("it have be added in blacklist by you"));
        return ;
    }

    int authed = fx_is_authed_by_id(account_id);

    if (authed == AUTH_WAIT)
    {
        #if 0
            QMessageBox::information(this->parentWidget(),
                                     tr("can't send mseeage to he"),
                                     tr("wait auth to add friend"));
        #else
            addBuddy(account_id);
        #endif
        return ;
    }

    if (authed == AUTH_REFUS)
    {
        #if 0
            QMessageBox::information(this->parentWidget(),
                                     tr("can't send mseeage to he"),
                                     tr("was refused to add friend"));
        #else
            addBuddy(account_id);
        #endif
        return ;
    }

    //first find is have the instance of the account_id, if have show it, and return.
    //then create a new instance of this account_id, and add to the tabwidget.
    AccountTab *accountTab = findFromMsgWindow(tabWidget, account_id);
    if (!accountTab)
    {
        accountTab = new AccountTab(account_id, tabWidget, isSendSms);
        accountTab->setMainWind(this->m_mainwindow);
        tabWidget->addTab(accountTab, cropTabName(accountTab->account_name));
    }

    tabWidget->setCurrentWidget(accountTab);
    setCurrentTabTitle(accountTab);

    this->showNormal();

    accountTab->setSendModle(isSendSms);
    accountTab->msgSend->MsgEdit->setFocus();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxMsgWindow::~FxMsgWindow()
{
    FX_FUNCTION
}

void FxMsgWindow::msg_exit()
{
    FX_FUNCTION
    m_willQuit = true;
    close();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
static int libfetion_shake_stamp = 0;
void FxMsgWindow::slot_do_shake()
{
    FX_FUNCTION
    libfetion_shake_stamp += SHAKE_TIMER_UPDATE;
    if (libfetion_shake_stamp >= SHAKE_DURATION)
    {
        m_isNudgeShake = false;
        libfetion_shake_stamp = 0;
        nudge_timer.stop();
        move(Settings::instance().MsgWinPos());
    }

    double waveOffset =
            (double)(libfetion_shake_stamp % SHAKE_CYCLE) / SHAKE_CYCLE;

    double TWO_PI = 3.14 * 2.0;
    double angleX = waveOffset * TWO_PI;
    double angleY = waveOffset * TWO_PI;

    //fix: should we need sin()? it will spent lots of cpus on it.
    int shakenX = (int)((sin(angleX) *SHAKE_DISTANCE) +
                        Settings::instance().MsgWinPos().x());
    int shakenY = (int)((sin(angleY) *SHAKE_DISTANCE) +
                        Settings::instance().MsgWinPos().y());
    move(shakenX, shakenY);

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::nudge_shake()
{
    FX_FUNCTION
    //if have a new nudge recive, we extend the shake time.
    libfetion_shake_stamp = 0;
    if (nudge_timer.isActive())
    {
        return ;
    }

    this->showNormal();

    this->setFocus();

    nudge_timer.start(SHAKE_TIMER_UPDATE);
    m_isNudgeShake = true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::showFaces()
{
    FX_FUNCTION
    inputFace->setGeometry(QCursor::pos().x() - 320, QCursor::pos().y() - 261,
                           320, 261);
    qDebug() << "y:" << inputFace->geometry().y() << "\n";
    inputFace->show();
    qDebug() << "z:" << inputFace->geometry().y() << "\n";
    inputFace->setFocus();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::UpdateSkins()
{
    FX_FUNCTION
    CHECK_SystemTiTle();
    AccountTab *ac_tab = NULL;
    int tabCount = tabWidget->count();
    for (int i = 0; i < tabCount; i++)
    {
        ac_tab = (AccountTab*)tabWidget->widget(i);
        if (ac_tab)
        {
            ac_tab->UpdateSkins();
        }
    }

    //update the chart windows icon
    ac_tab = (AccountTab*)tabWidget->widget(tabWidget->currentIndex());
    setCurrentTabTitle(ac_tab);

    if (closeTabButton)
    {
        closeTabButton->setIcon(getCloseTabImage());
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::updateAccountInfo(qlonglong account_id)
{
    FX_FUNCTION
    // get the current tab
    AccountTab *accountTab =
            (AccountTab*)tabWidget->widget(tabWidget->currentIndex());

    if (!accountTab)
    {
        return ;
    }

    //we just update the accountTab's ICON
    if (accountTab->account_id != account_id)
    {
        return ;
    }

    //update
    setCurrentTabTitle(accountTab);

    //fix me: should we need to update the info of AccountTab's "Ac_Status"
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
QString
FxMsgWindow::cropTabName(QString orig_name)
{
    FX_FUNCTION
    QString new_name = orig_name;
    //here handle the ac_name is length. if too long ,using ...repleace.
    if (new_name.size() > MAXNICELENGTH)
    {
        new_name = new_name.left(MAXNICELENGTH - 3) + QString("...");
    }
    return new_name;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
#define MSG_OK       1
#define MSG_FAIL     2
#define MSG_TIMEOUT  3
#define MSG_FAIL_LIMIT  4

void FxMsgWindow::handle_sendmsg(int msgflag,
                                unsigned long fx_msg,
                                qlonglong account_id)
{
    FX_FUNCTION
    if (!fx_msg)
    {
        return ;
    }

    int i = 0;
    Fetion_MSG *fxMsg = (Fetion_MSG*)fx_msg;
    char *msg = fx_msg_qt_format(fxMsg->message);
    QString newmsg;

    switch (msgflag)
    {
        case MSG_OK:
            for (i = 0; i < timeOutMsgVector.size(); ++i)
            {
                if (timeOutMsgVector.at(i) == (long)fx_msg)
                {
                    newmsg = "<b style=\"color:rgb(170,0,255);\">" +
                             tr("auto resend ok:") +
                             "</b>" +
                             newmsg.fromUtf8(msg);
                    timeOutMsgVector.remove(i);
                    break;
                }
            }
            break;
        case MSG_FAIL:
            for (i = 0; i < timeOutMsgVector.size(); ++i)
            {
                if (timeOutMsgVector.at(i) == (long)fx_msg)
                {
                    timeOutMsgVector.remove(i);
                    break;
                }
            }
            newmsg = "<b style=\"color:red;\">" +
                     tr("send fail:") +
                     "</b>" +
                     newmsg.fromUtf8(msg);
            break;

        case MSG_FAIL_LIMIT:
            for (i = 0; i < timeOutMsgVector.size(); ++i)
            {
                if (timeOutMsgVector.at(i) == (long)fx_msg)
                {
                    timeOutMsgVector.remove(i);
                    break;
                }
            }
            newmsg = "<b style=\"color:red;\">" +
                     tr("send sms fail by limit:") +
                     "</b>" +
                     newmsg.fromUtf8(msg);
            break;
        case MSG_TIMEOUT:
            timeOutMsgVector.append(fx_msg); // add the msg to vector
            newmsg = "<b style=\"color:rgb(170,0,255);\">" +
                     tr("send timeout:") +
                     "</b>" +
                     newmsg.fromUtf8(msg) +
                     "<br><b style=\"color:rgb(170,0,255);\">" +
                     tr("will auto resend") +
                     "</b>";
            break;
    }

    if (fxMsg->ext_id != 0)
    {
        this->addQunMessage(newmsg, account_id, 0L, true);
    }
    else
    {
        this->addMessage(newmsg, account_id);
    }

    if (msg)
    {
        free(msg);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::slot_SysDialogMsg(int message, unsigned long fx_msg, qlonglong who)
{
    FX_FUNCTION
    int msgflag = 0;

    if (!fx_msg)
    {
        return ;
    }

    switch (message)
    {
        case FX_SMS_OK:
        case FX_DIA_SEND_OK:
        case FX_QUN_SEND_OK:
        case FX_QUN_SMS_OK:
            msgflag = MSG_OK;
            break;

        case FX_SMS_FAIL:
        case FX_DIA_SEND_FAIL:
        case FX_QUN_SEND_FAIL:
        case FX_QUN_SMS_FAIL:
            msgflag = MSG_FAIL;
            break;

        case FX_SMS_TIMEOUT:
        case FX_DIA_SEND_TIMEOUT:
        case FX_QUN_SEND_TIMEOUT:
        case FX_QUN_SMS_TIMEOUT:
            msgflag = MSG_TIMEOUT;
            break;

        case FX_SMS_FAIL_LIMIT:
        case FX_QUN_SMS_FAIL_LIMIT:
            msgflag = MSG_FAIL_LIMIT;
            break;
    }

    handle_sendmsg(msgflag, fx_msg, who);

    //time out should not to destroy msg, beacuse the system will resend by itself..
    if (msgflag != MSG_TIMEOUT)
    {
        fx_destroy_msg((Fetion_MSG*)fx_msg);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::slot_haveNewSysMessage(qlonglong sys_id)
{
    FX_FUNCTION
    Q_UNUSED(sys_id);
    #if 0 //not using system message
        Fetion_MSG *fxMsg = fx_get_msg(sys_id);
        if (!fxMsg)
        {
            return ;
        }

        QString newmsg;
        char *msg = fx_msg_qt_format(fxMsg->message);
        newmsg = newmsg.fromUtf8(msg);

        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(1);
        trayIcon->showMessage(tr("sys message"), newmsg, icon, 5 *1000);

        fx_destroy_msg(fxMsg);
        if (msg)
        {
            free(msg);
        }
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMsgWindow::slot_receive_nudge(qlonglong account_id)
{
    FX_FUNCTION
    QString nudgemsg = "<b style=\"color:rgb(250,0,255);\">" +
                       tr("send a nudge to you") +
                       "</b><br>";
    this->addMessage(nudgemsg, account_id);

    if (!Settings::instance().isDisableNudge())
    {
        this->nudge_shake();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
QString FxMsgWindow::parseReceivedMsg(Fetion_MSG *fxMsg)
{
    //note: there are two formats of the fetion msg: text/plain and text/html-fragment.
    //text/plain  is sended from mobile client.
    //text/html-fragment is sended from pc client, and it is the xml format.
    if (!fxMsg)
    {
        return "";
    }

    QString newmsg;

    if (fxMsg->msgformat && strstr(fxMsg->msgformat, "plain"))
    //message from mobile  text/plain
    {
        newmsg = newmsg.fromUtf8(fxMsg->message);
        newmsg.replace(QString("<"), QString("&lt;"));
        newmsg.replace(QString(">"), QString("&gt;"));
    }
    else
    {
        //message from pc  text/html-fragment
        char *msg = fx_msg_qt_format(fxMsg->message);
        newmsg = newmsg.fromUtf8(msg);
        if (msg)
        {
            free(msg);
        }
    }
    newmsg.replace(QString("\n"), QString("<br>"));
    newmsg = FxInputFace::parseSmileySymbol(newmsg);
    #if FX_USE_SERVER_TIME
        newmsg = "(" + fxgui_format_time(fxMsg->msgtime) + "):</b><br>" +
            newmsg;
    #else
        newmsg = "(" +
                 QDateTime::currentDateTime().toString("hh:mm:ss") +
                 "--" +
                 QDateTime::currentDateTime().toString("yyyy-MM-dd") +
                 "):</b><br>" +
                 newmsg;
    #endif
    return newmsg;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
QString FxMsgWindow::formatTime(QString stamp)
{
    QString dateTime;
    bool formatRight;
    QRegExp regexp("\\s(\\d{2})\\s(\\w{3})\\s(\\d{4})\\s((\\d{2}):(\\d{2}):(\\d{2}))\\s(\\w{3})");

    if (regexp.indexIn(stamp) !=  - 1)
    {
        QStringList list = regexp.capturedTexts();
        formatRight = TRUE;
    }
    else
    {
        formatRight = FALSE;
    }

    QString day, month, year;
    QString time;
    day = regexp.cap(1);
    month = regexp.cap(2);
    if (month == "Jan")
    {
        month = "01";
    }
    else if (month == "Feb")
    {
        month = "02";
    }
    else if (month == "Mar")
    {
        month = "03";
    }
    else if (month == "Apr")
    {
        month = "04";
    }
    else if (month == "May")
    {
        month = "05";
    }
    else if (month == "Jun")
    {
        month = "06";
    }
    else if (month == "Jul")
    {
        month = "07";
    }
    else if (month == "Aug")
    {
        month = "08";
    }
    else if (month == "Sep")
    {
        month = "09";
    }
    else if (month == "Oct")
    {
        month = "10";
    }
    else if (month == "Nov")
    {
        month = "11";
    }
    else if (month == "Dec")
    {
        month = "12";
    }
    else
    {
        month = "";
    }
    year = regexp.cap(3);
    time = regexp.cap(4);

    if (formatRight)
    {
        dateTime = time + "--" + year + "-" + month + "-" + day;
    }
    else
    {
        dateTime = "";
    }

    return dateTime;
}
