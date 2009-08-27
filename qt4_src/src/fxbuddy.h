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
#ifndef FXBUDDY_H
#define FXBUDDY_H

#include <QTreeWidget>

#include "fxglobal.h"
#include "fxdebug.h"

class BuddyOpt: public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        BuddyOpt(QTreeWidget * Widget, bool isMainView = true);
        ~BuddyOpt();

        void UpdateSkins();

        int markedCount;
        bool isQunItem(QTreeWidgetItem *);

        void freeAllGroupdata();
        void freeAllAccountdata(QTreeWidgetItem *);
        void freeAllQundata(QTreeWidgetItem *);
        void addGroupToTree();
        void addAccountToTree();
        void addQunToTree();

        void delAccount_direct(qlonglong uid);
        void delAccount(qlonglong uid);
        void delAccount(QTreeWidgetItem* accountItem);

        void delGroup(qlonglong id);
        void addGroup(const char* groupname, qlonglong id);

        void addAccountToGroup(const Fetion_Account *account);
        void addAccountToGroup(const Fetion_Account *account,
                QString & name,
                int online_state,
                int group);

        Account_Info *fetchNoUpdateAccount();
        void updateAccountInfo(qlonglong account_id);
        QString createAccountTipsInfo(const Fetion_Account *account);
        void setTipsOfAccount(QTreeWidgetItem *accountItem,
                              const Fetion_Account *account);

        QTreeWidgetItem* findGroupItemByID(int group_id);

        void expandTree();

    signals:
        void m_itemChanged();

    private slots:
        void updateStyles(QTreeWidgetItem *item, int column);
    private:
        void UpdateSkinsForAccount(QTreeWidgetItem *groupItem);
        void UpdateSkinsForQun(QTreeWidgetItem *groupItem);
    private:
        QTreeWidgetItem* findAccountItem(const Fetion_Account *account);
        QTreeWidgetItem* findAccountItemFromGroup(QTreeWidgetItem *groupItem,
                const Fetion_Account *account);
        void create_zero_group();
        QTreeWidget *treeWidget;
        QTreeWidgetItem *QunItem;
        bool have_zero_group;
        bool isOnlineStateChanged(int old_state, int new_state, int* state);
        bool m_isMainView; //two mode: just main view and group sms send view
};

#endif
