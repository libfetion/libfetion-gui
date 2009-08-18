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
#include "fxshowAccountInfo.h"

FxShowAccountInfo::FxShowAccountInfo(const Fetion_Account *account, QWidget*parent):
    QDialog(parent)
{
    setupUi(this);
    m_account = account;

    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(chang_localname()));
    setShowInfo();
    char *showname = fx_get_account_show_name(account, FALSE);
    QString show_name = QString::fromUtf8(showname);
    if (showname)
    {
        free(showname);
    }
    QString title = tr("see") + show_name + tr("info ");
    this->setWindowTitle(title);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxShowAccountInfo::~FxShowAccountInfo(){

}

void FxShowAccountInfo::chang_localname()
{
    fx_set_buddyinfo(m_account->id,
                     loacl_name->text().toUtf8().data(),
                     NULL,
                     NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxShowAccountInfo::setShowInfo()
{
    if (!m_account)
    {
        return ;
    }

    loacl_name->setText(QString::fromUtf8(m_account->local_name));

    bool hP = false;
    if (m_account->personal)
    {
        hP = true;
    }
    QString info;



    info += tr("mobile_no:");
    if (hP)
    {
        info += "<b style=\"color:red; \">" +
                QString::fromUtf8(m_account->personal->mobile_no) +
                "</b>";
    }
    else
    {
        if (!fx_is_pc_user_by_account(m_account))
        {
            char *mobile_no = fx_get_original_ID(m_account->id);
            info += "<b style=\"color:red; \">" +
                    QString(mobile_no) +
                    "</b>";
            free(mobile_no);
            mobile_no = NULL;
        }

        info += "<b style=\"color:red; \"> </b>";
    }

    AcInfo->append(info);

    if (!fx_is_pc_user_by_account(m_account))
    {
        info = tr("fetion_no:");
        info += "<b style=\"color:red; \"> </b>";
        AcInfo->append(info);
    }
    else
    {
        info = tr("fetion_no:");
        info += "<b style=\"color:red; \">" +
                QString("%1").arg(m_account->id) +
                "</b>";
        AcInfo->append(info);
    }

    info = tr("nickname:");
    if (hP)
    {
        info += "<b style=\"color:red; \">" +
                QString::fromUtf8(m_account->personal->nickname) +
                "</b>";
    }
    else
    {
        info += "<b style=\"color:red; \"> </b>";
    }
    AcInfo->append(info);

    info = tr("name:");
    if (hP)
    {
        info += "<b style=\"color:red; \">" +
                QString::fromUtf8(m_account->personal->name) +
                "</b>";
    }
    else
    {
        info += "<b style=\"color:red; \"> </b>";
    }
    AcInfo->append(info);

    info = tr("gender:");
    if (hP)
    switch (m_account->personal->gender)
    {
        case 2:
            info += "<b style=\"color:red; \">" +
                    tr("girl") +
                    "</b>";
            break;
        case 1:
            info += "<b style=\"color:red; \">" +
                    tr("boy") +
                    "</b>";
            break;
        case 0:
            info += "<b style=\"color:red; \">" +
                    tr("unknow") +
                    "</b>";
            break;
    }
    else
    {
        info += "<b style=\"color:red; \">" +
                tr("unknow") +
                "</b>";
    }
    AcInfo->append(info);

    info = tr("impresa:");
    if (hP)
    {
        info += "<b style=\"color:red; \">" +
                QString::fromUtf8(m_account->personal->impresa) +
                "</b>";
    }
    else
    {
        info += "<b style=\"color:red; \"> </b>";
    }
    AcInfo->append(info);

    /* have no other info*/
    if (!hP)
    {
        return ;
    }
}
