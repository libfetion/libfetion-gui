/***************************************************************************
 *   Copyright (C) 2008-2009 by DDD                                        *
 *   dedodong@163.com                                                      *
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
#include <assert.h>
#include "fxbuddyManage.h"
#include "fxmainwindow.h"
#include "fxcontactinfo.h"

BuddyMge::BuddyMge(QTreeWidget *widget, FxMainWindow *wind)
{
    FX_FUNCTION
    assert(widget);
    assert(wind);
    treeWidget = widget;
    m_mainwindow = wind;


    //add all account to main view
    buddyopt = new BuddyOpt(widget);

    initAllActions();

    connect(treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this,
            SLOT(slot_accountDoubleClicked(QTreeWidgetItem *, int)));
    connect(treeWidget, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT
            (slot_accountPressed(QTreeWidgetItem *, int)));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

BuddyMge::~BuddyMge()
{
    FX_FUNCTION
}

void BuddyMge::initAllActions()
{
    FX_FUNCTION
    AddGroupAct = new QAction(tr("add group"), getMainWindow());
    AddGroupAct->setIcon(getMenuIcon(AddGroupIcon));
    connect(AddGroupAct, SIGNAL(triggered()),
            this, SLOT(slot_addGroup()));

    DeleteGroupAct = new QAction(tr("delete group"), getMainWindow());
    DeleteGroupAct->setIcon(getMenuIcon(DeleteGroupIcon));
    connect(DeleteGroupAct, SIGNAL(triggered()),
            this, SLOT(slot_deleteGroup()));

    ReNameGroupAct = new QAction(tr("rename group"), getMainWindow());
    ReNameGroupAct->setIcon(getMenuIcon(ReNameGroupIcon));
    connect(ReNameGroupAct, SIGNAL(triggered()),
            this, SLOT(slot_renameGroup()));

    ReNameBuddyAct = new QAction(tr("rename buddy"), getMainWindow());
    ReNameBuddyAct->setIcon(getMenuIcon(ReNameBuddyIcon));
    connect(ReNameBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_renameBuddy()));

    IMBuddyAct = new QAction(tr("im buddy"), getMainWindow());
    IMBuddyAct->setIcon(getMenuIcon(IMBuddyIcon));
    connect(IMBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_imBuddy()));

    SMSBuddyAct = new QAction(tr("sms buddy"), getMainWindow());
    SMSBuddyAct->setIcon(getMenuIcon(SMSBuddyIcon));
    connect(SMSBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_smsBuddy()));

    GetInfoBuddyAct = new QAction(tr("get info buddy"), getMainWindow());
    GetInfoBuddyAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
    connect(GetInfoBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_getInfoBuddy()));

    RefreshInfoBuddyAct = new QAction(tr("update info buddy"), getMainWindow());
    RefreshInfoBuddyAct->setIcon(getMenuIcon(RefreshBuddyIcon));
    connect(RefreshInfoBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_updateInfoBuddy()));


    DeleteBuddyAct = new QAction(tr("delete buddy"), getMainWindow());
    DeleteBuddyAct->setIcon(getMenuIcon(DeleteBuddyIcon));
    connect(DeleteBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_deleteBuddy()));

    AddBlackBuddyAct = new QAction(tr("add to black list"), getMainWindow());
    AddBlackBuddyAct->setIcon(getMenuIcon(BackInBuddyIcon));
    connect(AddBlackBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_addBlackBuddy()));

    RemoveBlackBuddyAct =
            new QAction(tr("remove frome black list"), getMainWindow());
    RemoveBlackBuddyAct->setIcon(getMenuIcon(RemoveBlackIcon));
    connect(RemoveBlackBuddyAct, SIGNAL(triggered()),
            this, SLOT(slot_removeBlackBuddy()));

    IMQunAct = new QAction(tr("im qun"), getMainWindow());
    IMQunAct->setIcon(getMenuIcon(IMBuddyIcon));
    connect(IMQunAct, SIGNAL(triggered()), this, SLOT(slot_imQun()));

    SMSQunAct = new QAction(tr("sms qun"), getMainWindow());
    SMSQunAct->setIcon(getMenuIcon(SMSBuddyIcon));
    connect(SMSQunAct, SIGNAL(triggered()), this, SLOT(slot_smsQun()));

    GetInfoQunAct = new QAction(tr("getinfo qun"), getMainWindow());
    GetInfoQunAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
    connect(GetInfoQunAct, SIGNAL(triggered()),
            this, SLOT(slot_getInfoQun()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxMsgWindow *BuddyMge::getMsgWindow()
{
    FX_FUNCTION
    if (getMainWindow())
    {
        return getMainWindow()->getMsgWindow();
    }
    return NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_accountDoubleClicked(QTreeWidgetItem *item, int)
{
    FX_FUNCTION
    if (item == 0)
    {
        return ;
    }

    if (item->parent())
    //it is a account
    {
        handleAccountDoubleClicked(item);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::handleAccountDoubleClicked(QTreeWidgetItem *item)
{
    FX_FUNCTION
    if (!item)
    {
        return ;
    }
    if (buddyopt->isQunItem(item->parent()))
    //test if it is a qun
    {
        #if MS_VC6
            Qun_Info *qun_info = (Qun_Info*)(item->data(0, Qt::UserRole).toUInt
                                  ());
        #else
            Qun_Info *qun_info = item->data(0, Qt::UserRole).value < Qun_Info *
                > ();
        #endif
        if (qun_info)
        {
            showQunWindow(qun_info->qunID);
        }
    }
    else
    {
        #if MS_VC6
            Account_Info *ac_info = (Account_Info*)(item->data(0, Qt::UserRole)
                                     .toUInt());
        #else
            Account_Info *ac_info = item->data(0, Qt::UserRole).value <
                Account_Info * > ();
        #endif
        if (ac_info)
        {
            showMsgWindow(ac_info->accountID);
        }
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::showQunWindow(qlonglong qun_id)
{
    FX_FUNCTION
    getMsgWindow()->addQunWin(qun_id);

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::showMsgWindow(qlonglong account_id)
{
    getMsgWindow()->addAccount(account_id);

}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_accountPressed(QTreeWidgetItem *item, int)
{
    if (item == 0)
    {
        return ;
    }

    getMainWindow()->setAutoHide(false);

    if (qApp->mouseButtons() == Qt::RightButton)
    {
        if (item->parent())
        //it is a account or qun
        {
            if (buddyopt->isQunItem(item->parent()))
            //test if it is a qun
            {
                showQunMenu();
            }
            else
            {
                showBuddyMenu();
            }
        }
        else
            if (!buddyopt->isQunItem(item))
            {
                showGroupMenu();
            }
    }

    getMainWindow()->setAutoHide(true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::showQunMenu()
{
    QPoint pos;
    QMenu menu(getMainWindow());

    menu.addAction(IMQunAct);
    menu.addAction(SMSQunAct);
    menu.addAction(GetInfoQunAct);

    //menu.popup(QCursor::pos());
    menu.exec(QCursor::pos());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::showGroupMenu()
{
    QPoint pos;
    QMenu menu(getMainWindow());

    menu.addAction(AddGroupAct);
    menu.addAction(DeleteGroupAct);
    menu.addAction(ReNameGroupAct);

    menu.exec(QCursor::pos());
    //menu.popup(QCursor::pos());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::showBuddyMenu()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    QPoint pos;
    QMenu menu(getMainWindow());

    //if in blacklist, show remove from the blacklist
    if (!fx_is_InBlacklist_by_id(ac_info->accountID))
    {
        menu.addAction(IMBuddyAct);
        menu.addAction(SMSBuddyAct);

        menu.addSeparator();

        menu.addAction(ReNameBuddyAct);
        menu.addAction(GetInfoBuddyAct);
        menu.addAction(RefreshInfoBuddyAct);

        QMenu *groupMenu = menu.addMenu(tr("move group"));
        {
            groupMenu->setIcon(getMenuIcon(MoveIcon));
            createGroupMenu(groupMenu);
        }

        menu.addSeparator();
        menu.addAction(DeleteBuddyAct);
        menu.addAction(AddBlackBuddyAct);
    }
    else
    {
        menu.addAction(ReNameBuddyAct);
        menu.addAction(GetInfoBuddyAct);
        menu.addAction(RefreshInfoBuddyAct);

        QMenu *groupMenu = menu.addMenu(tr("move group"));
        {
            createGroupMenu(groupMenu);
        }
        menu.addSeparator();
        menu.addAction(DeleteBuddyAct);
        menu.addAction(RemoveBlackBuddyAct);
    }
    menu.exec(QCursor::pos());
    //menu.popup(QCursor::pos());
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::createGroupMenu(QMenu *groupMenu)
{
    Group_Info *groupinfo = NULL;
    Fetion_Group *group = NULL;

    Q_UNUSED(groupinfo);
    Q_UNUSED(group);

    DList *tmp_group = (DList*)fx_get_group();
    while (tmp_group)
    {
        group = (Fetion_Group*)tmp_group->data;
        if (group)
        {
            QAction *action = new QAction(QString::fromUtf8(group->name),
                getMainWindow());
            action->setIcon(getQunIcon());
            QVariant Var((int)group->id);
            action->setData(Var);
            groupMenu->addAction(action);
        }
        tmp_group = d_list_next(tmp_group);
    }
    connect(groupMenu, SIGNAL(triggered(QAction*)), this, SLOT
            (moveGroupMenutriggered(QAction*)));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_MoveGroup(qlonglong account_id, unsigned long group_id)
{
    Q_UNUSED(group_id);
    buddyopt->delAccount_direct(account_id);
    buddyopt->addAccountToGroup(fx_get_account_by_id(account_id));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::moveGroupMenutriggered(QAction *action)
{
    int group_id = action->data().toInt();

    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    fx_move_group_buddy_by_id(ac_info->accountID, group_id, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void BuddyMge::updateAccountInfo(qlonglong account_id)
{
    const Fetion_Account *account = fx_get_account_by_id(account_id);
    if (!account)
        return ;
	/* update the account list view */
    buddyopt->updateAccountInfo(account);

	/* update the chat window's info */
    getMsgWindow()->updateAccountInfo(account_id);

	/* update the account Database */
	UpdateAccountToDB(account);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_add_group(int, unsigned long newname, qlonglong id)
{
    //fix me _mainwindow->tmp_addBuddy, this is a bad code
    if (getMainWindow()->getTmpAddBuddyWindow())
    {
        QVariant Var((int)id);
        getMainWindow()->getTmpAddBuddyWindow()->CB_group->addItem(QString
                      ::fromUtf8((char*)newname), Var);
    }

    buddyopt->addGroup((const char*)newname, id);

    if (newname)
    {
        free((char*)newname);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_del_group(int, unsigned long, qlonglong id)
{
    buddyopt->delGroup(id);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_reName_group(int, unsigned long newname, qlonglong id)
{
    QTreeWidgetItem *groupItem = buddyopt->findGroupItemByID(id);
    if (!groupItem)
    {
        if (newname)
        {
            free((char*)newname);
        }
        return ;
    }

    #if MS_VC6
        Group_Info *group_info = (Group_Info*)(groupItem->data(0, Qt::UserRole)
                                  .toUInt());
    #else
        Group_Info *group_info = groupItem->data(0, Qt::UserRole).value <
            Group_Info * > ();
    #endif

    if (!group_info)
    {
        if (newname)
        {
            free((char*)newname);
        }
        return ;
    }

    group_info->groupName = QString::fromUtf8((char*)newname);

    #ifdef WIN32
        char online[30];
        _snprintf(online, sizeof(online) - 1, "(%d/%d)", group_info->online_no,
                  groupItem->childCount());
        QString groupShowName = group_info->groupName + online;
    #else
        char *online = NULL;
        (void)asprintf(&online, "(%d/%d)", group_info->online_no, groupItem
         ->childCount());
        QString groupShowName = group_info->groupName + online;
        if (online)
        {
            free(online);
        }
    #endif
    groupItem->setText(0, groupShowName);

    if (newname)
    {
        free((char*)newname);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_reName_buddy(int, unsigned long newname, qlonglong id)
{
    if (newname)
    {
        free((char*)newname);
    }
    updateAccountInfo(id);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void BuddyMge::slot_addGroup()
{
    bool ok;
    QString text = QInputDialog::getText(getMainWindow(), tr("addGroup"), tr(
        "please input group name"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        fx_add_buddylist(text.toUtf8().data(), NULL, NULL);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_deleteGroup()
{
    QTreeWidgetItem *item = getMainWindow()->view->currentItem();
    if (!item)
    {
        return ;
    }

    if (item->childCount() > 0)
    {
        QMessageBox::warning(getMainWindow(), tr("can not delete group"), tr(
                             "group is not null"), QMessageBox::Yes);
        return ;
    }

    Group_Info *group_info = getGp_InfoOfCurrentItem();
    if (!group_info)
    {
        return ;
    }


    QString msg = tr("are you sure to delete group") + group_info->groupName;

    int ret = QMessageBox::warning(getMainWindow(), tr("delete group"), msg,
                                   QMessageBox::Yes, QMessageBox::Cancel);

    if (ret == QMessageBox::Yes)
    {
        fx_delete_buddylist(group_info->groupID, NULL, NULL);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_renameGroup()
{
    Group_Info *group_info = getGp_InfoOfCurrentItem();
    if (!group_info)
    {
        return ;
    }

    bool ok;
    QString text = QInputDialog::getText(getMainWindow(), tr("renameGroup"), tr
        ("please input new group name"), QLineEdit::Normal, group_info
        ->groupName, &ok);
    if (ok && !text.isEmpty())
    {
        fx_rename_buddylist(group_info->groupID, text.toUtf8().data(), NULL,
                            NULL);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_renameBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    char *showname = fx_get_account_show_name(fx_get_account_by_id(ac_info
        ->accountID), FALSE);
    QString account_name = QString::fromUtf8(showname);
    if (showname)
    {
        free(showname);
    }

    bool ok;
    //fix me: getMainWindow()
    QString text = QInputDialog::getText(getMainWindow(), tr("renamebudd"), tr(
        "please input new buddy name"), QLineEdit::Normal, account_name, &ok);
    if (ok)
    {
        fx_set_buddyinfo(ac_info->accountID, text.isEmpty()?"":text.toUtf8().data(), NULL, NULL);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_imBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    showMsgWindow(ac_info->accountID);
    //msgwin->addAccount(ac_info->accountID);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_smsBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    getMsgWindow()->addAccount(ac_info->accountID, true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_getInfoBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }
    const Fetion_Account *account = fx_get_account_by_id(ac_info->accountID);
    if (!account)
    {
        return ;
    }
    FxContactInfo *showInfo =
            new FxContactInfo(getMainWindow(), account);
    showInfo->exec();
    delete showInfo;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_updateInfoBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    fx_update_account_info_by_id(ac_info->accountID);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_deleteBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }
    QString msg = tr("are you sure to delete buddy") + ac_info->accountName;
    int ret = QMessageBox::warning(getMainWindow(), tr("delete buddy"), msg,
                                   QMessageBox::Yes, QMessageBox::Cancel);

    if (ret == QMessageBox::Yes)
    {
        msg = tr("delete buddy can not stop it send message to you,") + "\r\n"
                 + tr("shoud you want to add it to blacklist");
        ret = QMessageBox::warning(getMainWindow(), tr("add to black"), msg,
                                   QMessageBox::Yes, QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
        {
            fx_addto_blacklist_by_id(ac_info->accountID, NULL, NULL);
        }

        fx_delete_buddy_by_id(ac_info->accountID, NULL, NULL);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_addBlackBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }
    fx_addto_blacklist_by_id(ac_info->accountID, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_removeBlackBuddy()
{
    Account_Info *ac_info = getAc_InfoOfCurrentItem();
    if (!ac_info)
    {
        return ;
    }

    fx_removefrom_blacklist_by_id(ac_info->accountID, NULL, NULL);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_imQun()
{
    Qun_Info *qun_info = getQun_InfoOfCurrentItem();
    if (!qun_info)
    {
        return ;
    }
    getMsgWindow()->addQunWin(qun_info->qunID);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_smsQun()
{
    Qun_Info *qun_info = getQun_InfoOfCurrentItem();
    if (!qun_info)
    {
        return ;
    }
    getMsgWindow()->addQunWin(qun_info->qunID, true);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_getInfoQun()
{
    Qun_Info *qun_info = getQun_InfoOfCurrentItem();
    if (!qun_info)
    {
        return ;
    }

    getMainWindow()->showNormal();
    QDialog *window = new QDialog(getMainWindow());
    window->setWindowTitle(tr("see qun info"));

    QTextEdit *AcInfo = new QTextEdit(window);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(window);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox
                                  ::Ok);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(AcInfo);
    layout->addWidget(buttonBox);
    window->setLayout(layout);
    setQunInfo(AcInfo, qun_info->qunID);
    connect(buttonBox, SIGNAL(accepted()), window, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), window, SLOT(reject()));
    window->exec();
    delete window;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::slot_add_buddy(int, unsigned long, qlonglong id)
{
    buddyopt->delAccount(id);
    buddyopt->addAccountToGroup(fx_get_account_by_id(id));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void BuddyMge::slot_del_buddy(int, unsigned long, qlonglong id)
{
    buddyopt->delAccount(id);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

Account_Info *BuddyMge::getAc_InfoOfCurrentItem()
{
    QTreeWidgetItem *item = getMainWindow()->view->currentItem();

    if (!item)
    {
        return NULL;
    }
    if (!item->parent())
    //it is a account
    {
        return NULL;
    }
    #if MS_VC6
        Account_Info *ac_info = (Account_Info*)(item->data(0, Qt::UserRole)
                                 .toUInt());
    #else
        Account_Info *ac_info = item->data(0, Qt::UserRole).value <
            Account_Info * > ();
    #endif
    return ac_info;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

Group_Info *BuddyMge::getGp_InfoOfCurrentItem()
{
    QTreeWidgetItem *item = getMainWindow()->view->currentItem();
    if (!item)
    {
        return NULL;
    }

    if (item->parent())
    //it is a account
    {
        return NULL;
    }

    #if MS_VC6
        Group_Info *group_info = (Group_Info*)(item->data(0, Qt::UserRole)
                                  .toUInt());
    #else
        Group_Info *group_info = item->data(0, Qt::UserRole).value < Group_Info
            * > ();
    #endif
    return group_info;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

Qun_Info *BuddyMge::getQun_InfoOfCurrentItem()
{
    QTreeWidgetItem *item = getMainWindow()->view->currentItem();
    if (!item)
    {
        return NULL;
    }
    if (!item->parent())
    {
        return NULL;
    }
    #if MS_VC6
        Qun_Info *qun_info = (Qun_Info*)(item->data(0, Qt::UserRole).toUInt());
    #else
        Qun_Info *qun_info = item->data(0, Qt::UserRole).value < Qun_Info * > ()
                                        ;
    #endif
    return qun_info;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::setQunInfo(QTextEdit *AcInfo, qlonglong qun_id)
{
    QString info;
    Fetion_QunInfo *quninfo = NULL;
    const Fetion_Qun *fx_qun = fx_get_qun_by_id(qun_id);
    if (fx_qun)
    {
        quninfo = fx_qun->quninfo;
    }

    if (!quninfo)
    {
        AcInfo->append(tr("qun name:"));
        AcInfo->append(tr("qun introduce:"));
        AcInfo->append(tr("qun bulletin:"));
        return ;
    }

    info = tr("qun name:");
    info += "<b style=\"color:red; \">" + QString::fromUtf8(quninfo->name) +
        "</b>";
    AcInfo->append(info);

    info = tr("qun introduce:");
    info += "<b style=\"color:red; \">" + QString::fromUtf8(quninfo->introduce)
        + "</b>";
    AcInfo->append(info);

    info = tr("qun bulletin:");
    info += "<b style=\"color:red; \">" + QString::fromUtf8(quninfo->bulletin)
        + "</b>";
    AcInfo->append(info);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BuddyMge::UpdateSkins()
{
    buddyopt->UpdateSkins();

    ReNameBuddyAct->setIcon(getMenuIcon(ReNameBuddyIcon));
    IMBuddyAct->setIcon(getMenuIcon(IMBuddyIcon));
    SMSBuddyAct->setIcon(getMenuIcon(SMSBuddyIcon));
    GetInfoBuddyAct->setIcon(getMenuIcon(GetInfoBuddyIcon));
    RefreshInfoBuddyAct->setIcon(getMenuIcon(RefreshBuddyIcon));
    DeleteBuddyAct->setIcon(getMenuIcon(DeleteBuddyIcon));
    AddBlackBuddyAct->setIcon(getMenuIcon(BackInBuddyIcon));
    RemoveBlackBuddyAct->setIcon(getMenuIcon(RemoveBlackIcon));

    AddGroupAct->setIcon(getMenuIcon(AddGroupIcon));
    DeleteGroupAct->setIcon(getMenuIcon(DeleteGroupIcon));
    ReNameGroupAct->setIcon(getMenuIcon(ReNameGroupIcon));

    IMQunAct->setIcon(getMenuIcon(IMBuddyIcon));
    SMSQunAct->setIcon(getMenuIcon(SMSBuddyIcon));
    GetInfoQunAct->setIcon(getMenuIcon(GetInfoBuddyIcon));

}
