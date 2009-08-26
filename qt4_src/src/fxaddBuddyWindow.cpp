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
#include <QLabel>
#include <QtGui>


#include "fxaddBuddyWindow.h"
void FxAddBuddy::init()
{
    FX_FUNCTION
    init_groupItem();
    ED_usr_name->setMaxLength(10);

    QRegExp rx_port("[0-9]{0,11}");
    QValidator *validator_port = new QRegExpValidator(rx_port, this);
    ED_mobile->setValidator(validator_port);
    ED_mobile->setMaxLength(11);

    ED_fetionID->setValidator(validator_port);
    ED_fetionID->setMaxLength(9);

    ED_usr_name->setText(QString::fromUtf8(fx_get_usr_show_name()));

    connect(BT_getPersInfo, SIGNAL(clicked()), this, SLOT(getPersInfo()));
    connect(BT_newgroup, SIGNAL(clicked()), this, SLOT(newgroup()));

    connect(RB_mobile, SIGNAL(clicked()), this, SLOT(mobileRB()));
    connect(RB_fetionID, SIGNAL(clicked()), this, SLOT(fetionRB()));

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(addfriend()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxAddBuddy::FxAddBuddy(QWidget *parent): QDialog(parent)
{
    FX_FUNCTION
    setupUi(this);
    init();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxAddBuddy::FxAddBuddy(QString id,
        bool ismobile /* = false*/,
        QWidget *parent /* = 0 */)
{
    FX_FUNCTION
    Q_UNUSED(parent);
    setupUi(this);
    init();

    if (ismobile)
    {
        ED_mobile->setText(id);
    }
    else
    {
        ED_fetionID->setText(id);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxAddBuddy::~FxAddBuddy(){
    FX_FUNCTION
}

void FxAddBuddy::addfriend()
{
    FX_FUNCTION
    bool ismobile = false;
    if (RB_mobile->isChecked())
    {
        if (ED_mobile->text().size() != 11)
        {
            add_state->setText(QObject::tr("plesae input right mobile_no"));
            return ;
        }

        ismobile = true;
    }
    else
    {
        if (ED_fetionID->text().size() != 9)
        {
            add_state->setText(QObject::tr("plesae input right fetion uid"));
            return ;
        }
    }

    int groupId = CB_group->itemData(CB_group->currentIndex()).toInt();

    if (ismobile)
    {
        fx_add_buddy_by_mobile(ED_mobile->text().toUtf8().data(), ED_localName
                               ->text().toUtf8().data(), groupId, ED_usr_name
                               ->text().toUtf8().data(), NULL, NULL);
    }
    else
    {
        fx_add_buddy_by_uid(ED_fetionID->text().toUtf8().data(), ED_localName
                            ->text().toUtf8().data(), groupId, ED_usr_name
                            ->text().toUtf8().data(), NULL, NULL);
    }

    this->accept();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxAddBuddy::getPersInfo()
{
    FX_FUNCTION
    if (RB_mobile->isChecked())
    {
        ED_mobile->setEnabled(true);
        if (ED_mobile->text().size() != 11)
        {
            add_state->setText(QObject::tr("plesae input right mobile_no"));
            return ;
        }

    }
    else
    {
        if (ED_fetionID->text().size() != 9)
        {
            add_state->setText(QObject::tr("plesae input right fetion uid"));
            return ;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxAddBuddy::newgroup()
{
    FX_FUNCTION
    bool ok;
    QString text = QInputDialog::getText(this, tr("addGroup"), tr(
        "please input group name"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        fx_add_buddylist(text.toUtf8().data(), NULL, NULL);
    }
    FX_DEBUG("will create a new group");
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxAddBuddy::mobileRB()
{
    FX_FUNCTION
    setChanged();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxAddBuddy::fetionRB()
{
    FX_FUNCTION
    setChanged();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxAddBuddy::setChanged()
{
    FX_FUNCTION
    if (RB_mobile->isChecked())
    {
        RB_fetionID->setChecked(false);
        ED_mobile->setEnabled(true);
        ED_fetionID->setEnabled(false);

    }
    else
    {
        RB_mobile->setChecked(false);
        ED_fetionID->setEnabled(true);
        ED_mobile->setEnabled(false);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxAddBuddy::init_groupItem()
{
    FX_FUNCTION
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
