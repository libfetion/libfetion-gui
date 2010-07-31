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
#include <QMainWindow>

#include "fxloginwindow.h"
#include "fxskinmanage.h"

FxLoginWindow::FxLoginWindow(QWidget *parent):
        FxWidget(parent),
        user_id(NULL),
        user_pwd(NULL),
        proxy(NULL),
        authcode(NULL),
        willLogin(true)
{
    FX_FUNCTION
    setObjectName("loginWindow");
    setupUi(contentWidget);
    init();
    checkAutoLogin();

    m_loginShortcut_1 =
            new QShortcut(QKeySequence(Qt::Key_Return), this);
    m_loginShortcut_2 =
            new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(m_loginShortcut_1, SIGNAL(activated()), this, SLOT(login()));
    connect(m_loginShortcut_2, SIGNAL(activated()), this, SLOT(login()));

    CHECK_SystemTiTle();

    move(Settings::instance().LoginWinPos());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxLoginWindow::~FxLoginWindow()
{
    FX_FUNCTION
    if (user_id)
    {
        free(user_id);
    }
    if (user_pwd)
    {
        free(user_pwd);
    }
    if (m_loginShortcut_1)
    { 
        delete m_loginShortcut_1;
    } 
    if (m_loginShortcut_2)
    { 
        delete m_loginShortcut_2;
    } 
    proxy->proxy_destroy();
	authcode->authcode_destroy();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::Cancel_logwin()
{
    FX_FUNCTION
    enableLoginBT();
    fx_cancel_login();
    Login_State->setText(QObject::tr("cancel login"));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::set_login_button_state(bool state)
{
    FX_FUNCTION
    willLogin = state;
    if (willLogin)
    {
        BT_Login_Ok->setText(tr("login"));
        BT_Login_Ok->setPixmap(getLogion_InImage());
    }
    else
    {
        BT_Login_Ok->setText(tr("cancel"));
        BT_Login_Ok->setPixmap(getLogin_CancelImage());
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::BT_Login_clicked()
{
    FX_FUNCTION
    if (willLogin)
    {
        login();
    }
    else
    {
        Cancel_logwin();
        set_login_button_state(true);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::Setting(const QString &)
{
    FX_FUNCTION
    proxy->show();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::setLogingState(char *ch)
{
    FX_FUNCTION
    QString str = str.fromUtf8(ch);

    Login_State->setText(str);

    update();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
/* FIXME: comment this FX_DEBUG when My_EventListener is C++ styled */
#undef FX_DEBUG
#define FX_DEBUG(x) do {printf(x);}while(0)

void My_EventListener(int message, WPARAM wParam, LPARAM lParam, void *args)
{
    FxLoginWindow *loginDlg = NULL;
    if (!args)
        return ;
    loginDlg = (FxLoginWindow*)args;
    loginDlg->handleFx_Login_Event(message, wParam, lParam);
}

void FxLoginWindow::handleFx_Login_Event(int message, WPARAM wParam, LPARAM
    lParam)
{
    Q_UNUSED(wParam);
    Q_UNUSED(lParam);
    emit signal_Login_Message((int)message, qlonglong(wParam), (unsigned long)lParam);
}

void FxLoginWindow::slots_Login_Message(int message, qlonglong wParam, unsigned long /* lParam */)
{
    switch (message)
    {
        case FX_LOGIN_URI_ERROR:
            FX_DEBUG("FX_LOGIN_URI_ERROR");
				Login_State->setText(QObject::tr("mobile_no or fetion uid error"));
				enableLoginBT();
            break;

        case FX_LOGIN_CONNECTING:
            FX_DEBUG("FX_LOGIN_CONNECTING");
            Login_State->setText(QObject::tr("connecting the fetion server"));
            break;

        case FX_LOGIN_WAIT_AUTH:
            FX_DEBUG("FX_LOGIN_WAIT_AUTH");
            Login_State->setText(QObject::tr("waiting the server auth"));
            break;

        case FX_LOGIN_AUTH_OK:
            FX_DEBUG("FX_LOGIN_WAIT_AUTH");
            Login_State->setText(QObject::tr("server auth ok"));
            break;

        case FX_LOGIN_FAIL:
            FX_DEBUG("FX_LOGIN_FAIL");
            Login_State->setText(QObject::tr("password error"));
            enableLoginBT();
            break;
        case FX_LOGIN_NETWORK_ERROR:
            FX_DEBUG("FX_LOGIN_NETWORK_ERROR");
            Login_State->setText(QObject::tr("network error"));
            cleanCacheServerAdd(); //clean the sever ip cache
            enableLoginBT();
            break;
        case FX_LOGIN_UNKOWN_ERROR:
            FX_DEBUG("FX_LOGIN_UNKOWN_ERROR");
            Login_State->setText(QObject::tr("unkown error"));
            cleanCacheServerAdd(); //clean the sever ip cache
            enableLoginBT();
            break;

        case FX_LOGIN_TIMEOUT:
            FX_DEBUG("FX_LOGIN_TIMEOUT");
            Login_State->setText(QObject::tr("login time out"));
            enableLoginBT();
            break;

        case FX_LOGIN_UNKOWN_USR:
            FX_DEBUG("FX_LOGIN_UNKOWN_USR");
            Login_State->setText(QObject::tr("unkown fetion uid"));
            enableLoginBT();
            break;

        case FX_LOGIN_GCL_GETTING:
            FX_DEBUG("FX_LOGIN_GCL_GETTING");
            break;

        case FX_LOGIN_GCL_OK:
            FX_DEBUG("FX_LOGIN_GCL_OK");
            break;

        case FX_LOGIN_GCL_FAIL:
            FX_DEBUG("FX_LOGIN_GCL_FAIL");
            Login_State->setText(QObject::tr("get account list fail"));
            enableLoginBT();
            break;

        case FX_LOGIN_GP_GETTING:
            FX_DEBUG("FX_LOGIN_GP_GETTING");
            break;

        case FX_LOGIN_GP_OK:
            FX_DEBUG("FX_LOGIN_GP_OK");
            break;
        case FX_LOGIN_GP_FAIL:
            FX_DEBUG("FX_LOGIN_GP_FAIL");
            Login_State->setText(QObject::tr("get account info fail"));
            enableLoginBT();
            break;

        case FX_LOGIN_NEED_AUTH_CODE:
            FX_DEBUG("FX_LOGIN_NEED_AUTH_CODE");
            Login_State->setText(QObject::tr(""));
            enableLoginBT();
			authcode->reset((char*)(wParam));
			authcode->show();
            break;

        case FX_LOGIN_OK:
            FX_DEBUG("FX_LOGIN_OK");
            Login_State->setText(QObject::tr("fetion login ok"));
            LoginOK();
            break;
        default:
            break;
    }

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::enableLoginBT()
{
//    FX_FUNCTION
    emit signal_enableLoginBT();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::slots_enableLonginBT()
{
//    FX_FUNCTION
    if (loginTimer.isActive())
    {
        loginTimer.stop();
    }
    set_login_button_state(true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::LoginOK()
{
//    FX_FUNCTION
    if (remPWD->checkState() == Qt::Checked)
    {
        setAutoLogin(user_id, user_pwd, loginState->currentIndex());
    }
    else
    {
        disAutoLogin();
    }

    char *serve_address = fx_get_serve_address();
    if (serve_address)
    {
        setCacheServerAdd(user_id, serve_address);
        free(serve_address);
    }
    //fixme;.....
    //fx_get_usr_uri();
    emit signal_LoginOK();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::login()
{
//    FX_FUNCTION
    #ifdef HAVE_GUI_DEBUG_ENABLED
        LoginOK();
        return ;
    #endif
    Login_State->setText(tr("logining"));


    int id_len = ED_Fetion_ID->text().size();
    int pwd_len = ED_Fetion_Pwd->text().size();

    if (id_len != 9 && id_len != 11)
    {
        Login_State->setText(tr("error usr id"));
        return ;
    }

    if (pwd_len == 0)
    {
        Login_State->setText(tr("please input password"));
        return ;
    }

    if (user_id)
    {
        free(user_id);
    }
    if (user_pwd)
    {
        free(user_pwd);
    }

    user_id = (char*)malloc(id_len + 1);
    memcpy(user_id, ED_Fetion_ID->text().toUtf8().data(), id_len);
    user_id[id_len] = '\0';

    user_pwd = (char*)malloc(pwd_len + 1);
    memcpy(user_pwd, ED_Fetion_Pwd->text().toUtf8().data(), pwd_len);
    user_pwd[pwd_len] = '\0';

    char *poxy = getCacheServerAdd(user_id);
    if (poxy)
    {
        fx_set_serve_address(poxy);
        free(poxy);
    }
    else
    {
        fx_set_unknow_serve_address();
    }


    QString state = loginState->currentText();

    if (state == tr("offline"))
    {
        fx_set_login_status(FX_STATUS_OFFLINE);
    }
    else
        if (state == tr("busy"))
        {
            fx_set_login_status(FX_STATUS_BUSY);
        }
        else
            if (state == tr("away"))
            {
                fx_set_login_status(FX_STATUS_AWAY);
            }
            else
            {
                fx_set_login_status(FX_STATUS_ONLINE);
            }

    set_login_button_state(false);

    //	fx_set_proxy("221.130.46.144:8080");
    if (!fx_login(user_id, user_pwd, My_EventListener, this))
    {
        set_login_button_state(true);
        Login_State->setText(tr("password error"));
    }

    if (loginTimer.isActive())
    {
        loginTimer.stop();
    }
    loginTimer.start(1000 *35);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::login_timer()
{
//    FX_FUNCTION
    if (loginTimer.isActive())
    {
        loginTimer.stop();
    }

    Login_State->setText(tr("login time out"));
    set_login_button_state(true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::init()
{
//    FX_FUNCTION
    proxy = new FxProxy(this);
    proxy->hide();

    authcode = new FxAuthCode(this);
    authcode->hide();

    this->installEventFilter(this);

    QRegExp rx_port("[0-9]{0,11}");
    QValidator *validator_port = new QRegExpValidator(rx_port, this);
    ED_Fetion_ID->setValidator(validator_port);

    set_login_button_state(true);

    //Login_State->setText("test version for v 0.2.0");
    connect(this, SIGNAL(signal_Login_Message(int, qlonglong, unsigned long)),
            this, SLOT(slots_Login_Message(int, qlonglong, unsigned long)));
    connect(BT_Login_Ok, SIGNAL(clicked()),
            this, SLOT(BT_Login_clicked()));
    connect(this, SIGNAL(signal_enableLoginBT()),
            this, SLOT(slots_enableLonginBT()));
    connect(Net_Setting, SIGNAL(linkActivated(const QString &)),
            this, SLOT(Setting(const QString &)));
    connect(&loginTimer, SIGNAL(timeout()),
            this, SLOT(login_timer()));

    UpdateSkins();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::checkAutoLogin()
{
//    FX_FUNCTION
    //set auto login
    int state_tmp = 0;
    if (isAutoLogin(&user_id, &user_pwd, &state_tmp) && user_id)
    {
        remPWD->setCheckState(Qt::Checked);
        ED_Fetion_ID->setText(QString::fromUtf8(user_id));
        ED_Fetion_Pwd->setText(QString::fromUtf8(user_pwd));
        loginState->setCurrentIndex(state_tmp);

        //here check is have one instance for auto login
        if (Settings::instance().isSingleInstance())
        {
            login();
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::closeEvent(QCloseEvent *event)
{
//    FX_FUNCTION
    Q_UNUSED(event);
    QApplication::quit();
    event->accept();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::moveEvent(QMoveEvent *event)
{
//    FX_FUNCTION
    Q_UNUSED(event);
    Settings::instance().setLoginWinPos(pos());
    FxWidget::moveEvent(event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::resizeEvent(QResizeEvent *event)
{
//    FX_FUNCTION
    Settings::instance().setLoginWinSize(size());
    FxWidget::resizeEvent(event);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxLoginWindow::UpdateSkins()
{
//    FX_FUNCTION
    loginState->setStyleSheet("background-color: rgb(246,243,243)");

    loginState->setItemIcon(0, getOnlineStatusIcon(FX_STATUS_ONLINE));
    loginState->setItemIcon(1, getOnlineStatusIcon(FX_STATUS_OFFLINE));
    loginState->setItemIcon(2, getOnlineStatusIcon(FX_STATUS_AWAY));
    loginState->setItemIcon(3, getOnlineStatusIcon(FX_STATUS_BUSY));

    login_image->setPixmap(getLoginImage());
    LibFetion_image->setPixmap(getLibFetionImage());
    setWindowIcon(getSysTrayIcon(0));

    if (willLogin)
    {
        BT_Login_Ok->setText(tr("login"));
        BT_Login_Ok->setPixmap(getLogion_InImage());
    }
    else
    {
        BT_Login_Ok->setText(tr("cancel"));
        BT_Login_Ok->setPixmap(getLogin_CancelImage());
    }

}
