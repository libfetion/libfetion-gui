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

#define FX_CONTACT_INFO_TAG_PREFIX    "<b style=\"color:red; \">"
#define FX_CONTACT_INFO_TAG_SUFFIX    "</b>"
#define FX_CONTACT_INFO_HTML_NEWLINE    "<br>"

#define FX_CONTACT_INFO_DATA_SET_WITH_VALUE(x, v)    do{ \
                x += htmlStyledValueString(v); \
                }while(0);

#define FX_CONTACT_INFO_DATA_SET(x)     do{ \
                x += htmlStyledValueString();\
                }while(0);

#define FX_CONTACT_INFO_NEWLINE(x)      do{ \
                x += htmlStyledNewline();    \
                }while(0);

FxContactInfo::FxContactInfo(QWidget*parent, const Fetion_Account *account):
    QDialog(parent)
{
    FX_FUNCTION
    setupUi(this);
    m_account = account;

    if ((qlonglong)strtol(fx_get_usr_uid(), NULL, 10) == m_account->id)
	{
        m_isUser = true;
	}else{
        m_isUser = false;
	}

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
    FX_FUNCTION
}

void
FxContactInfo::chang_localname()
{
    FX_FUNCTION
    fx_set_buddyinfo(m_account->id,
                     local_name->text().toUtf8().data(),
                     NULL,
                     NULL);
}

QString
FxContactInfo::htmlStyledValueString(QString data)
{
    QString val;

    val = FX_CONTACT_INFO_TAG_PREFIX + data + FX_CONTACT_INFO_TAG_SUFFIX;

    return val;
}

QString
FxContactInfo::htmlStyledNewline()
{
    QString val;

    val = FX_CONTACT_INFO_HTML_NEWLINE;

    return val;
}
/**************************************************************************/
/*                                                                        */
/**************************************************************************/

QString
FxContactInfo::getContactInfo()
{
    FX_FUNCTION
    QString info;
    QString mobile_no;
    QString fetion_no;

    if (!m_account) return NULL;

    local_name->setText(QString::fromUtf8(m_account->local_name));

    if (!fx_is_pc_user_by_account(m_account))
    {
        mobile_no = QString::fromUtf8(fx_get_original_ID(m_account->id));
        fetion_no = QString();
    }else{
        mobile_no = QString();
        fetion_no = QString("%1").arg(m_account->id);
    }

    if (m_account->personal)
    {
        info += tr("mobile_no:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                        QString::fromUtf8(m_account->personal->mobile_no));
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("fetion_no:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, fetion_no);
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("nickname:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                QString::fromUtf8(m_account->personal->nickname));
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("name:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                QString::fromUtf8(m_account->personal->name));
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("gender:");
        switch (m_account->personal->gender)
        {
            case 2:
                FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, tr("girl"));
                break;
            case 1:
                FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, tr("boy"));
                break;
            case 0:
                FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, tr("unknow"));
                break;
        }
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("impresa:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                        QString::fromUtf8(m_account->personal->impresa));
        FX_CONTACT_INFO_NEWLINE(info);

        FxLocationParser *parser = new FxLocationParser();

        info += tr("province:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                parser->getProvinceByAlias(QString::fromUtf8(m_account->personal->province)));
        FX_CONTACT_INFO_NEWLINE(info);

        info += tr("city:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                parser->getCityByCode(m_account->personal->city));
        FX_CONTACT_INFO_NEWLINE(info);

		delete parser;

    }else{
        info += tr("mobile_no:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, mobile_no);
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("fetion_no:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
                QString("%1").arg(m_account->id));
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("nickname:");
        FX_CONTACT_INFO_DATA_SET(info);
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("name:");
        FX_CONTACT_INFO_DATA_SET(info);
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("gender:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, tr("unknow"));
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("impresa:");
        FX_CONTACT_INFO_DATA_SET(info);
        FX_CONTACT_INFO_NEWLINE(info);
        info += tr("province:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, tr("unknow"));
        FX_CONTACT_INFO_NEWLINE(info);

        info += tr("city:");
        FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info, tr("unknow"));
        FX_CONTACT_INFO_NEWLINE(info);
    }


	if (m_isUser)
	{
		info += tr("score:");
		FX_CONTACT_INFO_DATA_SET_WITH_VALUE(info,
				QString("%1").arg(fx_get_usr_score()));
		FX_CONTACT_INFO_NEWLINE(info);
	}

    return info;
}
