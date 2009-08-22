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
#include <QtDebug>

#include "fxcontactinfo.h"
#include "fxlocationparser.h"

FxContactInfo::FxContactInfo(QWidget*parent, const Fetion_Account *account):
    QDialog(parent)
{
    setupUi(this);
    m_account = account;

    char *showname = fx_get_account_show_name(account, FALSE);
    QString show_name = QString::fromUtf8(showname);
    if (showname)
    {
        free(showname);
    }
    QString title = tr("see") + show_name + tr("info ");
    this->setWindowTitle(title);

    this->AcInfo->setHtml(getContactInfo());

    this->label->setText(tr("fx_contact_info_tips"));
    this->pushButton->setText(tr("fx_contact_info_modify"));

    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(chang_localname()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxContactInfo::~FxContactInfo(){

}

void
FxContactInfo::chang_localname()
{
    fx_set_buddyinfo(m_account->id,
                     local_name->text().toUtf8().data(),
                     NULL,
                     NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString
FxContactInfo::getContactInfo()
{
    QString info;

    if (!m_account)
    {
        return NULL;
    }

    local_name->setText(QString::fromUtf8(m_account->local_name));

    bool hP = false;
    if (m_account->personal)
    {
        hP = true;
    }

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
    info += "<br>";

    if (!fx_is_pc_user_by_account(m_account))
    {
        info += tr("fetion_no:");
        info += "<b style=\"color:red; \"> </b>";
    }
    else
    {
        info += tr("fetion_no:");
        info += "<b style=\"color:red; \">" +
                QString("%1").arg(m_account->id) +
                "</b>";
    }
    info += "<br>";

    info += tr("nickname:");
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
    info += "<br>";

    info += tr("name:");
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
    info += "<br>";

    info += tr("gender:");
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
    info += "<br>";

    info += tr("score:");
    info += "<b style=\"color:red; \">" +
            QString("%1").arg(fx_get_usr_score()) +
            "</b>";
    info += "<br>";

    info += tr("impresa:");
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
    info += "<br>";

    FxLocationParser *parser = new FxLocationParser();
    if (hP)
    {
        info += tr("province:");
        info += "<b style=\"color:red; \">" +
                parser->getProvinceByAlias(QString::fromUtf8(m_account->personal->province)) +
                "</b>";
        info += "<br>";

        info += tr("city:");
        info += "<b style=\"color:red; \">" +
                parser->getCityByCode(m_account->personal->city) +
                "</b>";
        info += "<br>";
    }

    qDebug() << "Account info : " << info;
    return info;
}
