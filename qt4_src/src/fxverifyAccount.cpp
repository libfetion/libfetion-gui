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
#include "fxverifyAccount.h"

FxVerifyAccount::FxVerifyAccount(FxMainWindow *wind,
                                 char *uri,
                                 char *desc,
                                 QWidget *parent): QDialog(parent)
{
    setupUi(this);

    mainwind = wind;
    m_uri = uri;
    m_desc = QString::fromUtf8(desc);
    init_groupItem();
    Label_desc->setText(m_desc);
    Label_FetionID->setText(uri);

    Local_name->setText(m_desc);

    connect(RB_Accept, SIGNAL(clicked()),
            this, SLOT(verifyRB()));
    connect(RB_Refuse, SIGNAL(clicked()),
            this, SLOT(verifyRB()));
    connect(buttonBox, SIGNAL(accepted()),
            this, SLOT(verifyAccount()));
    connect(buttonBox, SIGNAL(rejected()),
            this, SLOT(close()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxVerifyAccount::~FxVerifyAccount()
{
    if (mainwind)
    {
        mainwind->show();
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxVerifyAccount::verifyAccount()
{
    if (RB_Accept->isChecked())
    {
        int groupId = CB_group->itemData(CB_group->currentIndex()).toInt();
        fx_handleContactRequest(m_uri,
                                1,
                                groupId,
                                Local_name->text().toUtf8().data());
    }
    else
    {
        fx_handleContactRequest(m_uri, 0, 0, "refuse");
        if (CB_ADDBlack->isChecked())
        {
            fx_addto_blacklist_by_uri(m_uri, NULL, NULL);
        }
    }
    close();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxVerifyAccount::verifyRB()
{
    if (RB_Accept->isChecked())
    {
        RB_Refuse->setChecked(false);
        CB_ADDBlack->setEnabled(false);
        Local_name->setEnabled(true);
        CB_group->setEnabled(true);
    }
    else
    {
        RB_Refuse->setChecked(true);
        CB_ADDBlack->setEnabled(true);
        Local_name->setEnabled(false);
        CB_group->setEnabled(false);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxVerifyAccount::init_groupItem()
{
    Fetion_Group *group = NULL;

    DList *tmp_group = (DList*)fx_get_group();
    while (tmp_group)
    {
        group = (Fetion_Group*)tmp_group->data;
        if (group)
        {
            QVariant Var((int)group->id);
            CB_group->addItem(QString::fromUtf8(group->name), Var);
        }
        tmp_group = d_list_next(tmp_group);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxVerifyAccount::closeEvent(QCloseEvent *event)
{
    if (mainwind)
    {
        mainwind->show();
    }
    event->accept();
}
