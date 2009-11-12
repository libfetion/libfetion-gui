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

#include <QApplication> // to fix
#include <QDir>
#include <QFile>

#include "appconfig.h"
#include "fxmainwindow.h"
#include "fxloginwindow.h"
#include "fxcontactinfo.h"
#include "fxqunwindow.h"
#include "fxverifyAccount.h"
#include "fxrefuseSMS.h"
#include "fxsendGroupSMS.h"
#include "fxconfigDlg.h"
#include "fxskinmanage.h"
#include "fxscheduleSmsManage.h"
#include "fxscheduleSms.h"
#include "LibFetionEventHandle.cpp"

FxMainWindow::FxMainWindow(QWidget *parent): FxWidget(parent), trayIcon(NULL),
                           traySetStatusMenu(NULL), buddySetStatusMenu(NULL),
                           msgHistroyMenu(NULL), skinMenu(NULL),
                           traySendSmsMenu(NULL), optSendSmsMenu(NULL)
{
    FX_FUNCTION
    setObjectName("mainWindow");
    setupUi(contentWidget);
    newVersion = 0;
    new_msg_count = 0;
    new_qun_msg_count = 0;
    fx_status = NO_SET;
    isQuit = false;
    isBreakOut = false;
    isHaveminimized = false;
    isNeedRecordWinPos = false;
    isNeedRecordWinSize = false;
    tmp_addBuddy = NULL;
    scheduleSms = new FxScheduleSMS(this, this);
    scheduleSmsManager = new FxScheduleSMSManage(this, this);

    init_UI();
    setMinimizetoHide(true);
    initAllActions();
    createMenu();
    initTrayIcon();
    init_slot_signal();

    //set the fetion system msg call back function
    fx_set_system_msg_cb(Sys_EventListener, this);

    #ifdef HAVE_GUI_DEBUG_ENABLED
        QTreeWidgetItem *tm = new QTreeWidgetItem(view, 0);
        tm->setText(0, "haha");
    #endif


    minimizedTimer.start(100);
    checkSkinsTimer.start(10000);
    updateAccountInfoTimer.start(1000);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxMainWindow::~FxMainWindow()
{
    FX_FUNCTION
    checkSkinsTimer.stop();
    if (buddyMge)
    {
        delete buddyMge;
    }
    if (msgwin)
    {
        delete msgwin;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::checkSplashScreenFlag()
{
    FX_FUNCTION
    #ifdef WIN32
        bool isSplashScreen;
        Qt::WindowFlags type = (windowFlags() &Qt::WindowType_Mask);
        if (type == Qt::SplashScreen)
        {
            isSplashScreen = true;
        }
        else
        {
            isSplashScreen = false;
        }

        Skin_Info *__sk_info = getCurrentSkinInfo();

        if (!__sk_info)
        {
            return ;
        }
        if (__sk_info->usingSystemTitle != "yes")
        {
            /*we didn't need SystemTitle, so If current is not SplashScreen,
            we need to set the windowsflags to SplashScreen*/
            if (!isSplashScreen)
            {
                bool isvisible = isVisible();
                setWindowFlags(windowFlags() | Qt::SplashScreen);
                // after window's flag's change,the window will hide. @See QtDoc by iptton
                if (isvisible)
                {
                    showNormal();
                }
            }
        }
        else
        {
            /*we need SystemTitle, so If current is SplashScreen,
            we need to remove SplashScreen flag*/
            if (isSplashScreen)
            {
                bool isvisible = isVisible();
                setWindowFlags(this->windowFlags() ^ Qt::SplashScreen);
                // after window's flag's change,the window will hide. @See QtDoc by iptton
                if (isvisible)
                {
                    showNormal();
                }
            }
        }

        /** I swear: this is the worst code of mine!!!!*/
        /*fix the bug of issue 87*/
        Sleep(10);
        if (Settings::instance().isEnableGetMsgHotKey())
        {
            Settings::instance().setEnableGetMsgHotKey(false);
            Settings::instance().setEnableGetMsgHotKey(true);
        }
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::UI_enable_impresa()
{
    FX_FUNCTION
}

void FxMainWindow::UI_enable_search()
{
    FX_FUNCTION
    //mainUserListStack->setCurrentWidget(search);
    mainUserListStack->setCurrentIndex(1);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::setUINickName()
{
    FX_FUNCTION
    QString impresa;
    if (fx_data_get_PersonalInfo())
    {
        impresa = QString::fromUtf8(fx_data_get_PersonalInfo()->impresa);
    }

    if (impresa.isEmpty())
    {
        impresa = tr("please input impresa...");
    }

    impression->setText(impresa);
    m_impresa = impresa;

    QString state;
    switch (fx_get_user_state())
    {
        case FX_STATUS_ONLINE:
            state = tr("online");
            break;
        case FX_STATUS_OFFLINE:
            state = tr("hide");
            break;
        case FX_STATUS_BUSY:
            state = tr("busy");
            break;
        default:
            state = tr("away");
            break;
    }

    QString name = QString::fromUtf8(fx_get_usr_show_name());
    //if (name.size >12)
    QString showstr = name;
    QString suffix = "(" + state + ")";
    nickname->setSuffix(suffix);
    nickname->setText(showstr);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::SearcheditingFinished()
{
    FX_FUNCTION
    QString text = lineSearch->text();
    if (text.isEmpty())
    {
        lineSearch->setText(tr("search friends..."));
        lineSearch->setCursorPosition(0);
        //show mian tree list
    }
    //	UI_Search->setDisabled (true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::SearchtextChanged(const QString &text)
{
    FX_FUNCTION
    Q_UNUSED(text);
    QString content = lineSearch->text();
    if (content.isEmpty() || content == tr("search friends..."))
    {
        //mainUserListStack->setCurrentWidget(view);
        mainUserListStack->setCurrentIndex(0);
        return ;
    }

    //mainUserListStack->setCurrentWidget(search);
    mainUserListStack->setCurrentIndex(1);
    search->clear();
    QList < QTreeWidgetItem * >*items =
            searchAccountInfo(text.toUtf8().data());
    search->insertTopLevelItems(0,  *items);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::changeNickName(QString text)
{
    FX_FUNCTION
    // the check below isn't need . It's check on the FxEditableLabel
    if (text != QString::fromUtf8(fx_get_usr_show_name()))
    {
        fx_set_user_nickname(text.toUtf8().data(), NULL, NULL);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::changeImpresa(QString text)
{
    FX_FUNCTION
    // the check below isn't need
    if (text == m_impresa)
    {
        return ;
    }

    if (text.isEmpty())
    {
        impression->setText(tr("please input impresa..."));
        text = tr("please input impresa...");

        if (m_impresa != tr("please input impresa..."))
        {
            fx_set_user_impresa(NULL, NULL, NULL);
        }
    }
    else
    {
        fx_set_user_impresa(text.toUtf8().data(), NULL, NULL);
    }

    m_impresa = text;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::SearchFocusIn()
{
    FX_FUNCTION
    if (lineSearch->text() == tr("search friends..."))
    {
        lineSearch->setText("");
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::ImpresaFocusIn()
{
    FX_FUNCTION
    if (impression->text() == tr("please input impresa..."))
    {
        impression->setText("");
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::addNewMsgCount(bool isQunMsg)
{
    FX_FUNCTION
    if (!isQunMsg)
    {
        new_msg_count++;
    }
    else
    {
        new_qun_msg_count++;
    }

    if (isHaveTray)
    {
        startFlickerTray();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

bool FxMainWindow::showNewMsgDlg()
{
    FX_FUNCTION
    if (new_msg_count > 0)
    {
        subNewMsgCount();
        msgwin->showNormal();
        return true;
    }

    if (new_qun_msg_count > 0)
    {
        subNewMsgCount(true);
        msgwin->showQunUnreadMsg();
        return true;
    }

    return false;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::subNewMsgCount(bool isQunMsg)
{
    FX_FUNCTION
    if (!isQunMsg)
    {
        new_msg_count--;
    }
    else
    {
        new_qun_msg_count--;
    }

    if (!new_msg_count && !new_qun_msg_count && isHaveTray)
    {
        endFlickerTray();
    }

    if (new_msg_count < 0)
    {
        new_msg_count = 0;
    }

    if (new_qun_msg_count < 0)
    {
        new_qun_msg_count = 0;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::startFlickerTray()
{
    FX_FUNCTION
    if (trayFlickTimer.isActive())
    {
        return ;
    }
    trayFlickTimer.start(300);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::endFlickerTray()
{
    FX_FUNCTION
    if (trayFlickTimer.isActive())
    {
        trayFlickTimer.stop();
    }
    trayIcon->setIcon(getSysTrayIcon(fx_get_user_state()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void Relogin_EventListener(int message, WPARAM wParam, LPARAM lParam, void
                           *args)
{
    if (!args)
    {
        return ;
    }
    FxMainWindow *mainwind = (FxMainWindow*)args;
    mainwind->handleFx_relogin_Event(message, wParam, lParam);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::relogin_fetion()
{
    FX_FUNCTION
    if (reloginTimer.isActive())
    {
        return ;
    }
    if (isHaveTray)
    {
        trayIcon->setContextMenu(reloginTrayMenu);
    }

    this->hide();
    fx_relogin(Relogin_EventListener, this, NULL, NULL);
    reloginTimer.start(1000 *35);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::handleFx_relogin_Event(int message,
                                        WPARAM wParam,
                                        LPARAM lParam)
{
    FX_FUNCTION
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);
    emit signal_Relogin_Event((int)message);
}
void FxMainWindow::slots_Relogin_Event(int message)
{
    switch (message)
    {
        case FX_LOGIN_URI_ERROR:
        case FX_LOGIN_FAIL:
        case FX_LOGIN_NETWORK_ERROR:
        case FX_LOGIN_UNKOWN_ERROR:
        case FX_LOGIN_UNKOWN_USR:
        case FX_LOGIN_GP_FAIL:

            relogin_fetion();
            if (!isBreakOut && isHaveTray)
            //here just one warnning
            {
                isBreakOut = true;
                QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon
                    ::MessageIcon(3);
                trayIcon->showMessage(tr("connect break off"), tr(
                                      "disconnect from fetion server, will relogin fetion"), icon, 1 *1000);
            }
            break;
        case FX_LOGIN_OK:
            fx_status = NO_SET;
            isBreakOut = false;
            if (isHaveTray)
            {
                trayIcon->setContextMenu(trayIconMenu);
                trayIcon->setIcon(getSysTrayIcon(fx_get_user_state()));
            }
            if (reloginTimer.isActive())
            {
                reloginTimer.stop();
            }

            //set the fetion system msg call back function
            fx_set_system_msg_cb(Sys_EventListener, this);
            break;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::relogin_timer()
{
    FX_FUNCTION
    fx_relogin(Relogin_EventListener, this, NULL, NULL);
}

void FxMainWindow::updateAccountInfo_timer()
{
    FX_FUNCTION
    Account_Info *account = buddyMge->fetchNoUpdateAccount();
    if (!account)
    {
        updateAccountInfoTimer.stop();
        return ;
    }

    fx_update_account_info_by_id(account->accountID);
    account->haveUpdate = true;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::flickerTray()
{
    FX_FUNCTION
    static bool flick_flag = false;
    trayIcon->setIcon(getFlickIcon(flick_flag));
    flick_flag = !flick_flag;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void FxMainWindow::checkSkinPath()
{
    FX_FUNCTION
    if (check_dir_state(SkinPath().toUtf8().data()))
    {
        createSkinMenu(skinMenu);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::minimizedWind()
{
    FX_FUNCTION
    if (!isHaveminimized)
    {
        isHaveminimized = true;

        #ifdef WIN32
            this->showMinimized();
        #endif

    }
    else
    {
        #ifdef WIN32
            //		this->setWindowState(Qt::WindowNoState);
            //registed the hot key, when the mainwindow is created, and can get the winId
            if (Settings::instance().isEnableGetMsgHotKey())
            {
                Settings::instance().setGetMsgHotKey(Settings::instance()
                                   .GetMsgHotKey(), Settings::instance()
                                   .GetMsgHotKeyMod());
            }
        #endif
        this->showNormal();
        minimizedTimer.stop();

        move(Settings::instance().MainWinPos());
        resize(Settings::instance().MainWinSize());

        isNeedRecordWinPos = true;
        isNeedRecordWinSize = true;
        enableAutoHide(true);
        setAutoHide(true);
        CHECK_SystemTiTle();
        checkSplashScreenFlag();

        if (Settings::instance().isStartHide())
        {
            this->hide();
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void FxMainWindow::haveCurrentVersionMessage(int version)
{
    FX_FUNCTION
    newVersion = version;
    if (version <= CURRENT_VERSION)
    {
        return ;
    }

    if (isHaveTray && trayIcon)
    {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(1);
        trayIcon->showMessage(tr("LibFetion"), tr("LibFetion Have New Version,"
                              "click to download the new version..."), icon, 10
                              *1000);
        fx_status = NEW_VERSION;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void FxMainWindow::haveAddAccountAppMessage(char *uri, char *desc)
{
    FX_FUNCTION
    FxVerifyAccount *verifyAccount = new FxVerifyAccount(this, uri, desc, this);
    verifyAccount->show();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void FxMainWindow::slot_SystemNetErr(int err)
{
    FX_FUNCTION
    Q_UNUSED(err);
    relogin_fetion();

    if (fx_status == SYS_RELOGIN)
    {
        return ;
    }

    fx_status = SYS_RELOGIN;
    if (isHaveTray)
    {
        trayIcon->setIcon(getSysTrayIcon(0));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void FxMainWindow::slot_DeRegistered()
{
    FX_FUNCTION
    if (isHaveTray)
    {
        trayIcon->setIcon(getSysTrayIcon(0));
        fx_status = SYS_UNREGISTER;
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(3);
        trayIcon->showMessage(tr("relogin"), tr(
                              "you have login in other pc, libfetion will quit")
                              , icon, 20 *1000);
    }
    else
    {
        QMessageBox::critical(this, tr("relogin"), tr(
                              "you have login in other pc, libfetion will quit")
                              );
        isQuit = true;
        close();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::slot_set_state(int state)
{
    FX_FUNCTION
    setUINickName();

    if (traySetStatusMenu)
    {
        traySetStatusMenu->setIcon(getOnlineStatusIcon(state));
    }
    if (buddySetStatusMenu)
    {
        buddySetStatusMenu->setIcon(getOnlineStatusIcon(state));
    }

    if (isHaveTray)
    {
        trayIcon->setIcon(getSysTrayIcon(state));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::slot_updateSmsDay(int day)
{
    FX_FUNCTION
    if (day)
    {
        refuseSMSAct->setIcon(getMenuIcon(ApplyIcon));
        acceptSMSAct->setIcon(QPixmap());
    }
    else
    {
        acceptSMSAct->setIcon(getMenuIcon(ApplyIcon));
        refuseSMSAct->setIcon(QPixmap());
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::createSkinMenu(QMenu *sub_skinMenu)
{
    FX_FUNCTION
    //fixme: here have a big bug..
    if (!sub_skinMenu)
    {
        return ;
    }

    sub_skinMenu->clear();

    QList < Skin_Info * >  * skinlist = searchAllSkins();
    if (skinlist == NULL)
    {
        return ;
    }

    for (QList < Skin_Info * > ::Iterator it = skinlist->begin(); it !=
         skinlist->end(); ++it)
    {
        Skin_Info *sk_info = (*it);
        QAction *action = new QAction(sk_info->name, this);

        if (sk_info->name == getSkinName() && sk_info->skinpath == getSkinPath()
            )
        {
            action->setIcon(getMenuIcon(ApplyIcon));
        }
        else
        {
            action->setIcon(getMenuIcon(CancelIcon));
        }

        #if MS_VC6
            QVariant Var((uint)sk_info);
        #else
            QVariant Var;
            Var.setValue(sk_info);
        #endif
        action->setData(Var);
        sub_skinMenu->addAction(action);
    }

    //fixme: here need to releas the skinlist!!! note: not release the sk_info
    connect(sub_skinMenu, SIGNAL(triggered(QAction*)), this, SLOT
            (skinMenutriggered(QAction*)));
    //	connect(sub_skinMenu, SIGNAL(aboutToShow()), this, SLOT(slot_ShowSkinMenu()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::skinMenutriggered(QAction *action)
{
    FX_FUNCTION
    #if MS_VC6
        Skin_Info *sk_info = (Skin_Info*)(action->data().toUInt());
    #else
        Skin_Info *sk_info = action->data().value < Skin_Info * > ();
    #endif
    if (!sk_info)
    {
        return ;
    }
    if (setSkins(sk_info->skinpath, sk_info->name))
	    this->UpdateSkins();
#ifdef Q_OS_MAC
    QMessageBox::about(this,
                       tr("Lib Changing SKins"),
                       tr("Restart APP to apply the new skins"));
#endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::searchaccountDoubleClicked(QTreeWidgetItem *item, int column)
{
    FX_FUNCTION
    Q_UNUSED(column);
    if (buddyMge)
    {
        buddyMge->handleAccountDoubleClicked(item);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

#if WIN32
    #include <windows.h>
    bool FxMainWindow::winEvent(MSG *msg, long *result)
    {
        if (WM_HOTKEY == msg->message)
        {
            if (showNewMsgDlg())
            {
                return true;
            }

            //here codes should be ajust after later..
            if (fx_status == SYS_RELOGIN)
            //relogin state will not handle this message..
            {
                return true;
            }

            trayMessageClicked();
            this->showNormal();

            return true;
        }

        return false;
    }
#else
    bool FxMainWindow::event(QEvent *event)
    {
        /* //for new UI, we shouldn't do anything about the QEvent::WindowStateChange.
        if( event->type() == QEvent::WindowStateChange)
        {
        QWindowStateChangeEvent *mEvent = (QWindowStateChangeEvent *)event;
        if( mEvent->oldState() == Qt::WindowNoState)
        {
        FxWidget::event(event);
        hide();
        return true;
        }
        }
         */
        return FxWidget::event(event);
    }
#endif

void FxMainWindow::closeEvent(QCloseEvent *event)
{
    FX_FUNCTION
    if (isQuit)
    {
        if (isHaveTray)
        {
            trayIcon->hide();
        }
        msgwin->msg_exit();

        QApplication::quit();

        event->accept();
        return ;
    }

    if (isHaveTray)
    {
        if (trayIcon->isVisible())
        {
            hide();
            event->ignore();
        }
    }
    else
    {
        msgwin->hide();
        event->accept();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::moveEvent(QMoveEvent *event)
{
    FX_FUNCTION
    if (isNeedRecordWinPos)
    {
        Settings::instance().setMainWinPos(pos());
    }
    FxWidget::moveEvent(event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::resizeEvent(QResizeEvent *event)
{
    FX_FUNCTION
    if (isNeedRecordWinSize)
    {
        Settings::instance().setMainWinSize(size());
    }
    FxWidget::resizeEvent(event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::trayMessageClicked()
{
    FX_FUNCTION
    switch (fx_status)
    {
        case NEW_VERSION:
            QDesktopServices::openUrl(QUrl(
                                      "http://www.libfetion.cn/demoapp_download.html"));
            fx_status = NO_SET;
            break;
        case SYS_UNREGISTER:
            QMessageBox::critical(this, tr("relogin"), tr(
                                  "you have login in other pc, libfetion will quit"));
            isQuit = true;
            close();
            break;
        case NEW_MSG:
            break;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    FX_FUNCTION
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:

        if (showNewMsgDlg())
        {
            return ;
        }

        //here codes should be ajust after later..
        if (fx_status == SYS_RELOGIN)
        //relogin state will not handle this message..
        {
            return ;
        }

        trayMessageClicked();

        #ifdef Q_OS_MAC
            this->showNormal();

        #endif
        break;

        case QSystemTrayIcon::DoubleClick:

        //here codes should be ajust after later..
        if (fx_status == SYS_RELOGIN)
        //relogin state will not handle this message..
        {
            return ;
        }

        trayMessageClicked();
        this->showNormal();

        //special handle for autohide, we double click the trayIcon.
        this->clearFocus();
        this->endAutoHide();
        // end the special handle
        break;
        case QSystemTrayIcon::MiddleClick: break;
        default:
            ;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::init_UI()
{
    FX_FUNCTION
    Settings::instance().setMainWindow(this);
    #ifdef HAVE_GUI_DEBUG_ENABLED
        Settings::instance().setUser(1000);
        setWindowTitle("HAVE_GUI_DEBUG_ENABLED Test User --Linux Fetion");
    #else
        Settings::instance().setUser((qlonglong)strtol(fx_get_usr_uid(), NULL,
                           10));
        #if WIN32
            setWindowTitle(QString::fromUtf8(fx_get_usr_show_name()) +
                           "--Win Fetion");
        #else
            #ifdef Q_OS_MAC
                setWindowTitle(QString::fromUtf8(fx_get_usr_show_name()) +
                               "--Mac Fetion");
		setWindowFlags(this->windowFlags() ^ Qt::WindowStaysOnTopHint);
            #else
                setWindowTitle(QString::fromUtf8(fx_get_usr_show_name()) +
                               "--Linux Fetion");
            #endif
        #endif
    #endif
    move(Settings::instance().MainWinPos());

    //UI set all images of main windows
    portrait->setIcon(getPortraitImage());

    lineSearch->setText(tr("search friends..."));
    nickname->setText(QString::fromUtf8(fx_get_usr_show_name()));
    setUINickName();
    version->setText(VERSION_NO);

    /* init some contorl */
    view->header()->setHidden(1);
    view->setRootIsDecorated(true);

    //add all account to main view
    buddyMge = new BuddyMge(view, this);

    //init search control
    search->header()->setHidden(1);
    search->setRootIsDecorated(true);

    //mainUserListStack->setCurrentWidget(view);
    mainUserListStack->setCurrentIndex(0);

    msgwin = new FxMsgWindow(0);
    msgwin->setMainWind(this);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::initAllActions()
{
    FX_FUNCTION
    Schedule_SMS_Act = new QAction(tr("schedule sms"), this);
    //Schedule_SMS_Act->setIcon(getOnlineStatusIcon(FX_STATUS_ONLINE));
    connect(Schedule_SMS_Act, SIGNAL(triggered()),
            this, SLOT(schedule_SMS()));

    OnlineAct = new QAction(tr("online"), this);
    OnlineAct->setIcon(getOnlineStatusIcon(FX_STATUS_ONLINE));
    connect(OnlineAct, SIGNAL(triggered()),
            this, SLOT(setOnlineStatus()));

    OfflineAct = new QAction(tr("offline"), this);
    OfflineAct->setIcon(getOnlineStatusIcon(FX_STATUS_OFFLINE));
    connect(OfflineAct, SIGNAL(triggered()),
            this, SLOT(setOfflineStatus()));

    BusyAct = new QAction(tr("busy"), this);
    BusyAct->setIcon(getOnlineStatusIcon(FX_STATUS_BUSY));
    connect(BusyAct, SIGNAL(triggered()),
            this, SLOT(setBusyStatus()));

    AwayAct = new QAction(tr("away"), this);
    AwayAct->setIcon(getOnlineStatusIcon(FX_STATUS_AWAY));
    connect(AwayAct, SIGNAL(triggered()),
            this, SLOT(setAwayStatus()));

    acceptSMSAct = new QAction(tr("accept SMS"), this);
    connect(acceptSMSAct, SIGNAL(triggered()),
            this, SLOT(setacceptSMS()));
    refuseSMSAct = new QAction(tr("refuse SMS"), this);
    connect(refuseSMSAct, SIGNAL(triggered()),
            this, SLOT(setrefuseSMS()));

    if (fx_get_user_refuse_sms_day())
    //is get from libfetion..
    {
        refuseSMSAct->setIcon(getMenuIcon(ApplyIcon));
        acceptSMSAct->setIcon(QPixmap());
    }
    else
    {
        acceptSMSAct->setIcon(getMenuIcon(ApplyIcon));
        refuseSMSAct->setIcon(QPixmap());
    }
    addBuddyAct = new QAction(tr("add friend"), this);
    addBuddyAct->setStatusTip(tr("add friend"));
    addBuddyAct->setIcon(getMenuIcon(AddBuddyIcon));
    connect(addBuddyAct, SIGNAL(triggered()),
            this, SLOT(addBuddy()));

    sendselfAct = new QAction(tr("send sms to self"), this);
    sendselfAct->setStatusTip(tr("send sms to self"));
    sendselfAct->setIcon(getMenuIcon(SMSBuddyIcon));
    connect(sendselfAct, SIGNAL(triggered()),
            this, SLOT(sendself()));

    sendgroupsmsAct = new QAction(tr("send group sms"), this);
    sendgroupsmsAct->setStatusTip(tr("send group sms"));
    sendgroupsmsAct->setIcon(getMenuIcon(SMSBuddyIcon));
    connect(sendgroupsmsAct, SIGNAL(triggered()),
            this, SLOT(sendgroupsms()));

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    exitAct->setIcon(getMenuIcon(ExitIcon));
    connect(exitAct, SIGNAL(triggered()),
            this, SLOT(tmp_exit()));

    personalInfoAct = new QAction(tr("personalInfo"), this);
    personalInfoAct->setStatusTip(tr("personalInfo"));
    personalInfoAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
    connect(personalInfoAct, SIGNAL(triggered()),
            this, SLOT(personalInfo()));

    CheckNewVersionAct = new QAction(tr("checkNewVersion"), this);
    CheckNewVersionAct->setStatusTip(tr("checkNewVersion"));
    CheckNewVersionAct->setIcon(getMenuIcon(AboutIcon));
    connect(CheckNewVersionAct, SIGNAL(triggered()),
            this, SLOT(checkNewVersion()));

    ReportBugAct = new QAction(tr("reportBugAct"), this);
    ReportBugAct->setStatusTip(tr("reportBugAct"));
    ReportBugAct->setIcon(getMenuIcon(AboutIcon));
    connect(ReportBugAct, SIGNAL(triggered()),
            this, SLOT(reportBugAct()));

    AboutLibFetionAct = new QAction(tr("aboutLibFetion"), this);
    AboutLibFetionAct->setStatusTip(tr("aboutLibFetion"));
    AboutLibFetionAct->setIcon(getMenuIcon(AboutIcon));
    connect(AboutLibFetionAct, SIGNAL(triggered()),
            this, SLOT(aboutLibFetion()));

    AboutCMAct = new QAction(tr("aboutChinaMobile"), this);
    AboutCMAct->setStatusTip(tr("aboutChinaMobile"));
    AboutCMAct->setIcon(getMenuIcon(AboutIcon));
    connect(AboutCMAct, SIGNAL(triggered()),
            this, SLOT(aboutCM()));

    AutoLoginAct = new QAction(tr("autologin"), this);
    if (isAutoLogin(NULL, NULL, NULL))
    {
        AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        AutoLoginAct->setIcon(getMenuIcon(CancelIcon));
    }

    AutoLoginAct->setStatusTip(tr("autologin"));
    connect(AutoLoginAct, SIGNAL(triggered()),
            this, SLOT(menu_setAutoLogin()));

    MuteAct = new QAction(tr("mute"), this);
    if (Settings::instance().isMute())
    {
        MuteAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        MuteAct->setIcon(getMenuIcon(CancelIcon));
    }

    MuteAct->setStatusTip(tr("autoshowmsg"));
    connect(MuteAct, SIGNAL(triggered()),
            this, SLOT(menu_setmute()));

    IsAutoShowMsgAct = new QAction(tr("autoshowmsg"), this);
    #ifdef Q_OS_MAC
        IsAutoShowMsgAct->setVisible(false);
    #endif
    if (Settings::instance().isAutoShowMsg())
    {
        IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));
    }

    IsAutoShowMsgAct->setStatusTip(tr("autoshowmsg"));
    connect(IsAutoShowMsgAct, SIGNAL(triggered()),
            this, SLOT(menu_setautoshowmsg()));

    SetLongSMSAct = new QAction(tr("catsms"), this);
    if (Settings::instance().isEnableLongSMS())
    {
        SetLongSMSAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        SetLongSMSAct->setIcon(getMenuIcon(CancelIcon));
    }
    SetLongSMSAct->setStatusTip(tr("catsms"));
    connect(SetLongSMSAct, SIGNAL(triggered()),
            this, SLOT(menu_setlongsms()));

    SaveMsgHistroyAct = new QAction(tr("savemsghistroy"), this);
    SaveMsgHistroyAct->setStatusTip(tr("savemsghistroy"));
    SaveMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
    connect(SaveMsgHistroyAct, SIGNAL(triggered()),
            this, SLOT(saveMsgHistroy()));

    CleanMsgHistroyAct = new QAction(tr("cleanmsghistroy"), this);
    CleanMsgHistroyAct->setStatusTip(tr("cleanmsghistroy"));
    CleanMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
    connect(CleanMsgHistroyAct, SIGNAL(triggered()),
            this, SLOT(cleanMsgHistroy()));

    ConfigAppAct = new QAction(tr("moresettings"), this);
    ConfigAppAct->setStatusTip(tr("moresettings"));
    ConfigAppAct->setIcon(getMenuIcon(OptionsIcon));
    connect(ConfigAppAct, SIGNAL(triggered()),
            this, SLOT(showConfigDlg()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::createMenu()
{
    FX_FUNCTION
    mainMenu = new QMenu(btnMenu);
    buddyMenu = mainMenu->addMenu(tr("buddy"));
    menuSetting = mainMenu->addMenu(tr("settings"));
    menuAbout = mainMenu->addMenu(tr("about"));
    mainMenu->addSeparator();
    mainMenu->addAction(exitAct);

    reloginTrayMenu = new QMenu(this);
    reloginTrayMenu->addAction(exitAct);

    trayIconMenu = new QMenu(this);
    traySetStatusMenu = trayIconMenu->addMenu(tr("change online status"));
    traySetStatusMenu->setIcon(getOnlineStatusIcon(fx_get_user_state()));
    traySetStatusMenu->addAction(OnlineAct);
    traySetStatusMenu->addAction(OfflineAct);
    traySetStatusMenu->addAction(BusyAct);
    traySetStatusMenu->addAction(AwayAct);


    traySendSmsMenu = trayIconMenu->addMenu(tr("send sms"));
    traySendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
    traySendSmsMenu->addAction(sendselfAct);
    traySendSmsMenu->addAction(sendgroupsmsAct);
    trayIconMenu->addAction(Schedule_SMS_Act);
    trayIconMenu->addAction(addBuddyAct);
    trayIconMenu->addAction(personalInfoAct);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAct);

    buddySetStatusMenu = buddyMenu->addMenu(tr("change online status"));
    buddySetStatusMenu->setIcon(getOnlineStatusIcon(fx_get_user_state()));
    buddySetStatusMenu->addAction(OnlineAct);
    buddySetStatusMenu->addAction(OfflineAct);
    buddySetStatusMenu->addAction(BusyAct);
    buddySetStatusMenu->addAction(AwayAct);
    buddySetStatusMenu->addSeparator();
    buddySetStatusMenu->addAction(acceptSMSAct);
    buddySetStatusMenu->addAction(refuseSMSAct);

    buddyMenu->addMenu(buddySetStatusMenu);
    optSendSmsMenu = buddyMenu->addMenu(tr("send sms"));
    optSendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
    optSendSmsMenu->addAction(sendselfAct);
    optSendSmsMenu->addAction(sendgroupsmsAct);
    buddyMenu->addAction(Schedule_SMS_Act);
    buddyMenu->addAction(addBuddyAct);
    buddyMenu->addAction(personalInfoAct);

    menuSetting->addAction(AutoLoginAct);
    menuSetting->addAction(MuteAct);

    skinMenu = menuSetting->addMenu(tr("skins"));
    skinMenu->setIcon(getMenuIcon(SkinIcon));
    createSkinMenu(skinMenu);

    msgHistroyMenu = menuSetting->addMenu(tr("msghistroy"));
    msgHistroyMenu->setIcon(getMenuIcon(HistoryIcon));
    msgHistroyMenu->addAction(SaveMsgHistroyAct);
    msgHistroyMenu->addAction(CleanMsgHistroyAct);
    menuSetting->addAction(ConfigAppAct);

    menuAbout->addAction(CheckNewVersionAct);
    menuAbout->addAction(ReportBugAct);
    menuAbout->addAction(AboutLibFetionAct);
    menuAbout->addAction(AboutCMAct);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::init_slot_signal()
{
    FX_FUNCTION
    connect(btnMenu, SIGNAL(clicked()), this, SLOT(showMenu()));

    connect(btnSettings, SIGNAL(clicked()), this, SLOT(showConfigDlg()));
    connect(btnSendSelf, SIGNAL(clicked()), this, SLOT(sendself()));
    connect(btnAddFriend, SIGNAL(clicked()), this, SLOT(addBuddy()));
    connect(portrait, SIGNAL(clicked()), this, SLOT(showPortrait()));
    connect(btnSkin, SIGNAL(clicked()), this, SLOT(showSkinMenu()));


    connect(nickname, SIGNAL(textChanged(QString)),
            this, SLOT(changeNickName(QString)));

    connect(impression, SIGNAL(textChanged(QString)),
            this, SLOT(changeImpresa(QString)));
    connect(lineSearch, SIGNAL(editingFinished()),
            this, SLOT(SearcheditingFinished()));

    connect(lineSearch, SIGNAL(infocus()),
            this, SLOT(SearchFocusIn()));

    connect(lineSearch, SIGNAL(textChanged(const QString &)),
            this, SLOT(SearchtextChanged(const QString &)));


    if (isHaveTray)
    {
        connect(trayIcon, SIGNAL(messageClicked()), this, SLOT
                (trayMessageClicked()));
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    }


    connect(&checkSkinsTimer, SIGNAL(timeout()), this, SLOT(checkSkinPath()));
    connect(&minimizedTimer, SIGNAL(timeout()), this, SLOT(minimizedWind()));
    connect(&trayFlickTimer, SIGNAL(timeout()), this, SLOT(flickerTray()));
    connect(&reloginTimer, SIGNAL(timeout()), this, SLOT(relogin_timer()));
    connect(&updateAccountInfoTimer, SIGNAL(timeout()),
            this, SLOT(updateAccountInfo_timer()));

    connect(this, SIGNAL(signal_UpdateSmsDay(int)),
            this, SLOT(slot_updateSmsDay(int)));
    connect(search, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this,
            SLOT(searchaccountDoubleClicked(QTreeWidgetItem *, int)));
    connect(this, SIGNAL(signal_Current_Version(int)),
            this, SLOT(haveCurrentVersionMessage(int)));
    connect(this, SIGNAL(signal_AddAccountApp(char *, char*)),
            this, SLOT(haveAddAccountAppMessage(char *, char*)));
    connect(this, SIGNAL(signal_SystemNetErr(int)),
            this, SLOT(slot_SystemNetErr(int)));
    connect(this, SIGNAL(signal_DeRegistered()),
            this, SLOT(slot_DeRegistered()));
    connect(this, SIGNAL(signal_set_nickname_ok()),
            this, SLOT(setUINickName()));
    connect(this, SIGNAL(signal_set_state(int)),
            this, SLOT(slot_set_state(int)));

    connect(this, SIGNAL(signal_Relogin_Event(int)),
            this, SLOT(slots_Relogin_Event(int)));

    connect(this, SIGNAL(signal_NewSysMsg(qlonglong)),
            msgwin, SLOT(slot_haveNewSysMessage(qlonglong)));
    connect(this, SIGNAL(signal_NewMsg(qlonglong)),
            msgwin, SLOT(slot_haveNewMessage(qlonglong)));
    connect(this, SIGNAL(signal_NewQunMsg(qlonglong)),
            msgwin, SLOT(slot_haveNewQunMessage(qlonglong)));
    connect(this, SIGNAL(signal_SysDialogMsg(int, unsigned long, qlonglong)),
            msgwin, SLOT(slot_SysDialogMsg(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_receive_nudge(qlonglong)),
            msgwin, SLOT(slot_receive_nudge(qlonglong)));

    connect(this, SIGNAL(signal_add_group(int, unsigned long, qlonglong)),
            buddyMge, SLOT(slot_add_group(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_del_group(int, unsigned long, qlonglong)),
            buddyMge, SLOT(slot_del_group(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_reName_group(int, unsigned long, qlonglong)),
            buddyMge,SLOT(slot_reName_group(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_reName_buddy(int, unsigned long, qlonglong)),
            buddyMge,SLOT(slot_reName_buddy(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_add_buddy(int, unsigned long, qlonglong)),
            buddyMge, SLOT(slot_add_buddy(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_del_buddy(int, unsigned long, qlonglong)),
            buddyMge, SLOT(slot_del_buddy(int, unsigned long, qlonglong)));
    connect(this, SIGNAL(signal_UpdateAcInfo(qlonglong)),
            buddyMge, SLOT(updateAccountInfo(qlonglong)));
    connect(this, SIGNAL(signal_MoveGroup(qlonglong, unsigned long)),
            buddyMge, SLOT(slot_MoveGroup(qlonglong, unsigned long)));

    connect(this, SIGNAL(signal_update_scheduleList()),
            scheduleSmsManager, SLOT(slot_update_scheduleList()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::schedule_SMS()
{
    FX_FUNCTION
    scheduleSms->showNormal();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::showSkinMenu()
{
    FX_FUNCTION
    this->setAutoHide(false);
    skinMenu->exec(QCursor::pos());
    this->setAutoHide(true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::showPortrait()
{
    FX_FUNCTION
    this->setAutoHide(false);
    buddySetStatusMenu->exec(QCursor::pos());
    this->setAutoHide(true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::showMenu()
{
    FX_FUNCTION
    this->setAutoHide(false);
    mainMenu->exec(QCursor::pos());
    this->setAutoHide(true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::addBuddy()
{
    FX_FUNCTION
    tmp_addBuddy = new FxAddBuddy(this);
    tmp_addBuddy->exec();
    //fix a bug when addbuddy show, and mainwindow is hide, quit the addbuddy,
    //mainwindows will quit too.
    delete tmp_addBuddy;
    tmp_addBuddy = NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::sendgroupsms()
{
    FX_FUNCTION
    FxSendGroupSMS *groupSms = new FxSendGroupSMS(this, this);
    groupSms->show();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::sendself()
{
    FX_FUNCTION
    msgwin->addAccount((qlonglong)strtol(fx_get_usr_uid(), NULL, 10), true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::about()
{
    FX_FUNCTION
    QMessageBox::about(this, tr("About Menu"), tr(
                       "The <b>Menu</b> example shows how to create "
                       "menu-bar menus and context menus."));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::setImpresa()
{
    FX_FUNCTION
    bool ok;
    QString text = QInputDialog::getText(this, tr("setImpresa"), tr(
        "please input newImpresa"), QLineEdit::Normal, "", &ok);
    if (ok)
    {
        if (text.isEmpty())
        {
            fx_set_user_impresa(NULL, NULL, NULL);
        }
        else
        {
            fx_set_user_impresa(text.toUtf8().data(), NULL, NULL);
        }
    }

    setUINickName();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::tmp_exit()
{
    FX_FUNCTION
    isQuit = true;
    close();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::setOnlineStatus()
{
    FX_FUNCTION
    fx_set_user_state(FX_STATUS_ONLINE, NULL, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::setOfflineStatus()
{
    FX_FUNCTION
    fx_set_user_state(FX_STATUS_OFFLINE, NULL, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxMainWindow::setBusyStatus()
{
    FX_FUNCTION
    fx_set_user_state(FX_STATUS_BUSY, NULL, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::setAwayStatus()
{
    FX_FUNCTION
    fx_set_user_state(FX_STATUS_AWAY, NULL, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::setacceptSMS()
{
    FX_FUNCTION
    fx_set_user_refuse_sms_day(0, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::setrefuseSMS()
{
    FX_FUNCTION
    FxRefuseSMS *xx = new FxRefuseSMS(this);
    xx->exec();
    delete xx;
}

/**************************************************************************/
/* Fetch personal information                                             */
/**************************************************************************/
void FxMainWindow::personalInfo()
{
    FX_FUNCTION
    const Fetion_Account * account =
            fx_get_account_by_id(strtol(fx_get_usr_uid(), NULL, 10));

	if (!account)
		return;

    FxContactInfo *info = new FxContactInfo(this, account);

    info->exec();
    delete info;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::checkNewVersion()
{
    FX_FUNCTION
    QString info;
    if (newVersion > CURRENT_VERSION)
    {
        info = tr("LibFetion Have New Version,"
                  "access http://www.libfetion.cn for more infomation");
    }
    else
    {
        info = tr("the current Version is the new Version");
    }

    QMessageBox::about(this, tr("LibFetion"), info);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::reportBugAct()
{
    FX_FUNCTION
    QDesktopServices::openUrl(QUrl("http://www.libfetion.cn/bbs"));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::aboutLibFetion()
{
    FX_FUNCTION
    QMessageBox messageBox(this);
    messageBox.setText(tr("Application Current Version") +
                       VERSION_NO +
                       tr("<br>This application is based on LibFetion library to writing, for more infomation access<a href=\"http://www.libfetion.cn\"> www.libfetion.cn </a> <br>""Copyright @ 2009 <b> <a href=\"mailto:dedodong@163.com\">DDD</a> (dedodong@163.com)</b>. All Rights reserved."));
    messageBox.setTextFormat(Qt::RichText);
    messageBox.setWindowTitle(tr("About LibFetion"));

    QAbstractButton *authorButton =
            messageBox.addButton(tr("Author Info"),
                                 QMessageBox::ActionRole);
    messageBox.setDefaultButton(messageBox.addButton(QMessageBox::Ok));

    messageBox.exec();

    if (messageBox.clickedButton() == authorButton)
    {
        displayAboutLibFetion();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::aboutCM()
{
    FX_FUNCTION
    QMessageBox::about(this,
                       tr("About CHINA Mobile"),
                       tr("<a href=\"http://www.fetion.com.cn\"> China Mobile Fetion</a>""China Mobile Fetion introduce"));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::menu_setmute()
{
    FX_FUNCTION
    bool currentIsMute = Settings::instance().isMute();
    Settings::instance().setMute(!currentIsMute);

    if (currentIsMute)
    {
        MuteAct->setIcon(getMenuIcon(CancelIcon));
    }
    else
    {
        MuteAct->setIcon(getMenuIcon(ApplyIcon));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::saveMsgHistroy()
{
    FX_FUNCTION
    bool ok;
    QString text = QInputDialog::getText(this,
                                        tr("saveMsgHistroy"),
                                        tr("please input password"),
                                        QLineEdit::Password,
                                        "",
                                        &ok);
    if (ok && !text.isEmpty())
    {
        if (text == QString(fx_get_usr_passwd()))
        {
            QString fileName = QFileDialog::getSaveFileName(this,
                                                 tr("saveMsgHistroy"),
                                                 QDir::homePath(),
                                                 tr("Text Files (*.htm)"));
            if (!fileName.isEmpty())
            {
                int ret = export_history(
                        (qlonglong)strtol(fx_get_usr_uid(), NULL, 10),
                        fileName.toUtf8().data());
                if (ret == 2)
                {
                    QMessageBox::critical(this, tr("save file error"),
                                          tr("the path of file can't write"));
                }
            }

        }
        else
        {
            QMessageBox::critical(this, tr("error"), tr("the password error"));
        }
    }

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::cleanMsgHistroy()
{
    FX_FUNCTION
    bool ok;
    QString text = QInputDialog::getText(this, tr("cleanMsgHistroy"),
                                         tr("please input password"),
                                         QLineEdit::Password,
                                         "",
                                         &ok);
    if (ok && !text.isEmpty())
    {
        if (text == QString(fx_get_usr_passwd()))
        {
            clean_history((qlonglong)strtol(fx_get_usr_uid(), NULL, 10));
        }
        else
        {
            QMessageBox::critical(this, tr("error"), tr("the password error"));
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::showConfigDlg()
{
    FX_FUNCTION
    show();
    /* ONLY one configure dialog is allowed */
    if (!FxConfigDia::hasInstance)
    {
        FxConfigDia *configdia = new FxConfigDia(this, this);
        configdia->show();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::menu_setautoshowmsg()
{
    FX_FUNCTION
    bool currentIsAutoShowMsg = Settings::instance().isAutoShowMsg();
    Settings::instance().setAutoShowMsg(!currentIsAutoShowMsg);

    if (currentIsAutoShowMsg)
    {
        IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::menu_setlongsms()
{
    FX_FUNCTION
    bool currentIsEnableLongSMS = Settings::instance().isEnableLongSMS();
    Settings::instance().setEnableLongSMS(!currentIsEnableLongSMS);

    if (currentIsEnableLongSMS)
    {
        SetLongSMSAct->setIcon(getMenuIcon(CancelIcon));
    }
    else
    {
        SetLongSMSAct->setIcon(getMenuIcon(ApplyIcon));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::menu_setAutoLogin()
{
    FX_FUNCTION
    bool currentIsAutoLogin = Settings::instance().isAutoLogin();
    Settings::instance().setAutoLogin(!currentIsAutoLogin);

    if (currentIsAutoLogin)
    {
        AutoLoginAct->setIcon(getMenuIcon(CancelIcon));
    }
    else
    {
        AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::initTrayIcon()
{
    FX_FUNCTION
    isHaveTray = QSystemTrayIcon::isSystemTrayAvailable();

    if (isHaveTray)
    {
        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setContextMenu(trayIconMenu);
        trayIcon->setIcon(getSysTrayIcon(fx_get_user_state()));
        trayIcon->show();

        #if WIN32
            trayIcon->setToolTip(
                    QString::fromUtf8(fx_get_usr_show_name()) + " Win Fetion");
        #else
            #ifdef Q_OS_MAC
                trayIcon->setToolTip(
                        QString::fromUtf8(fx_get_usr_show_name()) + " Mac Fetion");
            #else
                trayIcon->setToolTip(
                        QString::fromUtf8(fx_get_usr_show_name()) + " Linux Fetion");
            #endif
        #endif
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::UpdateSkinsMenu()
{
    FX_FUNCTION
    if (!skinMenu)
    {
        return ;
    }
    skinMenu->setIcon(getMenuIcon(SkinIcon));

    QList < QAction * > QAlist = skinMenu->actions();

    for (QList < QAction * > ::Iterator it = QAlist.begin();
        it != QAlist.end();
        ++it)
    {
        QAction *action = (*it);
        if (!action)
        {
            continue;
        }
        #if MS_VC6
            Skin_Info *sk_info = (Skin_Info*)(action->data().toUInt());
        #else
            Skin_Info *sk_info = action->data().value < Skin_Info * > ();
        #endif
        if (!sk_info)
        {
            continue;
        }

        if (sk_info->name == getSkinName() && \
            sk_info->skinpath == getSkinPath())
        {
            action->setIcon(getMenuIcon(ApplyIcon));
        }
        else
        {
            action->setIcon(getMenuIcon(CancelIcon));
        }

    }

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxMainWindow::UpdateSkins()
{
    FX_FUNCTION
    CHECK_SystemTiTle();
    checkSplashScreenFlag();
    msgwin->UpdateSkins();
    buddyMge->UpdateSkins();

    portrait->setIcon(getPortraitImage());
    btnAddFriend->setIcon(getAddImage());

    if (trayIcon)
    {
        trayIcon->setIcon(getSysTrayIcon(fx_get_user_state()));
    }
    if (traySetStatusMenu)
    {
        traySetStatusMenu->setIcon(getOnlineStatusIcon(fx_get_user_state()));
    }
    if (buddySetStatusMenu)
    {
        buddySetStatusMenu->setIcon(getOnlineStatusIcon(fx_get_user_state()));
    }
    if (msgHistroyMenu)
    {
        msgHistroyMenu->setIcon(getMenuIcon(HistoryIcon));
    }
    if (traySendSmsMenu)
    {
        traySendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
    }
    if (optSendSmsMenu)
    {
        optSendSmsMenu->setIcon(getMenuIcon(SMSBuddyIcon));
    }

    UpdateSkinsMenu();

    personalInfoAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
    addBuddyAct->setIcon(getMenuIcon(AddBuddyIcon));
    sendselfAct->setIcon(getMenuIcon(SMSBuddyIcon));
    sendgroupsmsAct->setIcon(getMenuIcon(SMSBuddyIcon));
    exitAct->setIcon(getMenuIcon(ExitIcon));



    OnlineAct->setIcon(getOnlineStatusIcon(FX_STATUS_ONLINE));
    OfflineAct->setIcon(getOnlineStatusIcon(FX_STATUS_OFFLINE));
    BusyAct->setIcon(getOnlineStatusIcon(FX_STATUS_BUSY));
    AwayAct->setIcon(getOnlineStatusIcon(FX_STATUS_AWAY));

    if (fx_get_user_refuse_sms_day())
    //was got from libfetion..	
    {
        refuseSMSAct->setIcon(getMenuIcon(ApplyIcon));
        acceptSMSAct->setIcon(QPixmap());
    }
    else
    {
        acceptSMSAct->setIcon(getMenuIcon(ApplyIcon));
        refuseSMSAct->setIcon(QPixmap());
    }

    if (Settings::instance().isEnableLongSMS())
    {
        SetLongSMSAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        SetLongSMSAct->setIcon(getMenuIcon(CancelIcon));
    }

    SaveMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
    CleanMsgHistroyAct->setIcon(getMenuIcon(HistoryIcon));
    ConfigAppAct->setIcon(getMenuIcon(OptionsIcon));

    CheckNewVersionAct->setIcon(getMenuIcon(AboutIcon));
    ReportBugAct->setIcon(getMenuIcon(AboutIcon));
    AboutLibFetionAct->setIcon(getMenuIcon(AboutIcon));
    AboutCMAct->setIcon(getMenuIcon(AboutIcon));

    if (isAutoLogin(NULL, NULL, NULL))
    {
        AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        AutoLoginAct->setIcon(getMenuIcon(CancelIcon));
    }

    if (Settings::instance().isMute())
    {
        MuteAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        MuteAct->setIcon(getMenuIcon(CancelIcon));
    }

    if (Settings::instance().isAutoShowMsg())
    {
        IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
    }
    else
    {
        IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));
    }
}

/*
 * Copyright (C) 2009
 *
 * Show author and coryright info of LibFetion.
 *
 * Current maintainer: DDD(dedodong@163.com)
 * Origin Author: weizhg23(weizhg23@gmail.com)
 */

#include <QFile>
#include <QDialog>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QTextStream>

void FxMainWindow::displayAboutLibFetion()
{
    QWidget *window = new QWidget;
    QListWidget *listWidget = new QListWidget;
    window->setMinimumSize(460, 220);
    window->setMaximumSize(460, 220);
    window->setWindowTitle("Libfetion Authors");
    window->setWindowFlags(Qt::WindowTitleHint | \
                           Qt::WindowSystemMenuHint | \
                           Qt::WindowStaysOnTopHint);
    window->move(Settings::instance().MainWinPos());

    QFile file(defaultResPath() + "/CREDITS.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString line;
        int item = 0;
        QListWidgetItem *newItem = NULL;
        while (!stream.atEnd())
        {
            line = stream.readLine(); // no trail '\n'
            newItem = new QListWidgetItem;
            newItem->setText(line);
            listWidget->insertItem(item, newItem);
            item++;
        }
        file.close();
    }

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(listWidget);
    window->setLayout(layout);
    window->setFocus();
    window->showNormal();
}

/*
 * Copyright (C) 2008
 *
 * A Directory Modifly State Checker used in libfetion for Mac/Linux/Windows.
 *
 * Author: YY(51test2003@gmail.com)
 */
#include <stdio.h>
#include <sys/stat.h>

#ifdef WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

int FxMainWindow::check_dir_state(const char *path)
{
    static time_t mtime = 0;
    struct stat state;
    if (stat(path, &state) ==  - 1)
     /* System call failed. Treat as be modified.*/
    {
        return 1;
    }
    if (mtime == 0)
     /* Initialize the default timestamp*/
    {
        mtime = state.st_mtime;
    } if (mtime != state.st_mtime)
     /* Directory was modified*/
    {
        //printf("Modyfied.\n");
        mtime = state.st_mtime;
        return 1;
    }

    return 0;
}
