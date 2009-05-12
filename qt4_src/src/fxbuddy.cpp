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
#include <assert.h>
#include "fxbuddy.h"
#include "fxsendGroupSMS.h"

BuddyOpt::BuddyOpt(QTreeWidget * widget, bool isMainView)
{
	m_isMainView = isMainView;

	QunItem = NULL;
	have_zero_group = false;
	markedCount = 0;

	assert(widget);
	treeWidget = widget;

	addGroupToTree();
	addAccountToTree();

	if (m_isMainView)
		addQunToTree();
	else
	{
		connect(treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)),
				this, SLOT(updateStyles(QTreeWidgetItem *, int)));

	}
	//expandTree();
}

BuddyOpt::~BuddyOpt()
{
	freeAllGroupdata();    
}

void BuddyOpt::expandTree()
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return ;

	for(int i =0;  i< RootItem-> childCount (); i++)
		this->treeWidget->expandItem (RootItem->child(i));
}

void BuddyOpt::freeAllGroupdata()
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return ;

	int GroupCount = RootItem-> childCount ();
	QTreeWidgetItem *groupItem = NULL;
	for(int i =0;  i< GroupCount; i++)
	{
		groupItem = RootItem->child(i);
		if (!groupItem)
			continue;

		if (isQunItem(groupItem))
		{
			freeAllQundata(groupItem);
			continue;
		}

		//free all account data of this group
		freeAllAccountdata(groupItem);    

#if MS_VC6
		Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
		Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
		if( group_info) 
			delete group_info;
	}

}

void BuddyOpt::freeAllAccountdata(QTreeWidgetItem *groupItem)    
{
	if (!groupItem)
		return;
	int itemCounts = groupItem->childCount();
	QTreeWidgetItem *tmpItem = NULL;

	for(int i = 0; i < itemCounts; i++)
	{
		tmpItem = groupItem->child(i);
		if(!tmpItem)
			continue;
#if MS_VC6
		Account_Info *ac_info =(Account_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
#else
		Account_Info *ac_info =tmpItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
		if(ac_info )
			delete ac_info;
	}
}

void BuddyOpt::freeAllQundata(QTreeWidgetItem *groupItem)
{
	if (!groupItem)
		return;

	int itemCounts = groupItem->childCount();
	QTreeWidgetItem *tmpItem = NULL;

	for(int i = 0; i < itemCounts; i++)
	{
		tmpItem = groupItem->child(i);
		if(!tmpItem)
			continue;
#if MS_VC6
		Qun_Info *qun_info = (Qun_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
#else
		Qun_Info *qun_info = tmpItem->data(0, Qt::UserRole).value<Qun_Info*>() ;
#endif
		if (qun_info)
			delete qun_info;
	}
}

void BuddyOpt::addQunToTree()
{
	Fetion_Qun *qun = NULL;
	Qun_Info *qun_info = NULL;

	DList *tmp_qun = (DList *)fx_get_qun();
	while(tmp_qun)
	{
		qun = (Fetion_Qun *) tmp_qun->data;
		if(qun) {
			if(!QunItem) {
				QunItem = new QTreeWidgetItem(treeWidget);
				QunItem->setText(0, tr("fetion qun"));
			}

			qun_info = new Qun_Info;

			char * qun_name = fx_get_qun_show_name(qun); 
			qun_info->qunName = QString::fromUtf8(qun_name);
			if(qun_name)
				free(qun_name);


			qun_info->qunID = qun->id; 
#if MS_VC6
			QVariant Var((uint)qun_info);
#else
			QVariant Var;
			Var.setValue (qun_info); 
#endif
			QTreeWidgetItem *item;
			item = new QTreeWidgetItem(QunItem);
			item->setText(0, qun_info->qunName);
			item->setIcon(0, getQunIcon());
			item->setData(0, Qt::UserRole, Var);
		}
		tmp_qun = d_list_next(tmp_qun);
	}
}

bool BuddyOpt::isQunItem(QTreeWidgetItem *item) 
{
	if(QunItem)
		return item == QunItem;
	else
		return false;
}

//add Group info to tree widget
void BuddyOpt::addGroupToTree()
{
	Group_Info *groupinfo = NULL; 
	Fetion_Group *group = NULL;

	DList *tmp_group = (DList *)fx_get_group();
	while(tmp_group)
	{
		group = (Fetion_Group *) tmp_group->data;
		if(group) {

			QTreeWidgetItem *item;
			item = new QTreeWidgetItem(treeWidget);

			if (!m_isMainView)
				item->setCheckState(0, Qt::Unchecked);

			QString str = QString::fromUtf8(group->name);

			groupinfo = new Group_Info;
			groupinfo->groupName = str;
			groupinfo->groupID = group->id;
			groupinfo->online_no = 0;

			item->setText(0, str+"(0/0)");
		
#if MS_VC6
				QVariant Var((uint)groupinfo);
#else
	QVariant Var;
			Var.setValue (groupinfo); 
#endif

			item->setData(0, Qt::UserRole, Var);
		}
		tmp_group = d_list_next(tmp_group);
	}

}

void BuddyOpt::delAccount_direct(qlonglong uid)
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return ;

	int GroupCount = RootItem-> childCount ();
	QTreeWidgetItem *groupItem = NULL;
	for(int i =0;  i< GroupCount; i++)
	{
		groupItem = RootItem->child(i);
		if (!groupItem || isQunItem(groupItem) )
			continue;
		int itemCounts = groupItem-> childCount();
		QTreeWidgetItem *tmpItem = NULL;
		for(int i =0;  i< itemCounts; i++)
		{
			tmpItem = groupItem->child(i);
			if(!tmpItem)
				continue;
#if MS_VC6
			Account_Info *ac_info =(Account_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
#else
			Account_Info *ac_info =tmpItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
			if (!ac_info)
				continue;
			if(ac_info->accountID == uid)
			{
				delAccount(tmpItem); 
				return;
			}
		}
	}

}

void BuddyOpt::delAccount(QTreeWidgetItem* accountItem)  
{
	if(!accountItem)
		return;

#if MS_VC6
	Account_Info *ac_info =(Account_Info*)(accountItem->data(0, Qt::UserRole).toUInt());
#else
	Account_Info *ac_info =accountItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
	if (!ac_info)
		return;

	const Fetion_Account * account = fx_get_account_by_id (ac_info->accountID);
	if(ac_info )	delete ac_info;


	QTreeWidgetItem* groupItem = accountItem->parent();
	if(!groupItem) 	return;

	groupItem->removeChild(accountItem);

	{
#if MS_VC6
		Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
		Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
		if (!group_info)
			return ;
		if (fx_is_on_line_by_account(account))
			group_info->online_no --;

#ifdef WIN32
		char online[30];
		_snprintf (online, sizeof(online)-1, "(%d/%d)", group_info->online_no, groupItem->childCount());
		QString groupShowName = group_info->groupName+ online;
#else
		char *online= NULL;
		asprintf(&online, "(%d/%d)", group_info->online_no, groupItem->childCount());
		QString groupShowName = group_info->groupName+ online;
		if (online)
			free(online);
#endif
		groupItem->setText(0, groupShowName);
	}

}

void BuddyOpt::delAccount(qlonglong uid)  
{
	const Fetion_Account * account = fx_get_account_by_id (uid);
	if(!account)  return;

	delAccount(findAccountItem(account)); 
}

void BuddyOpt::addGroup(const char* groupname, qlonglong id)
{
	Group_Info *groupinfo = new Group_Info;
	groupinfo->groupName = QString::fromUtf8((char*)groupname);
	groupinfo->groupID = id;
	groupinfo->online_no = 0;

	QTreeWidgetItem *item;
	item = new QTreeWidgetItem(treeWidget);

	if (!m_isMainView)
		item->setCheckState(0, Qt::Unchecked);

	item->setText(0, groupinfo->groupName+"(0/0)");
	
#if MS_VC6
			QVariant Var((uint)groupinfo);
#else
QVariant Var;
			Var.setValue (groupinfo); 
#endif
	item->setData(0, Qt::UserRole, Var);

}

void BuddyOpt::delGroup(qlonglong id)  
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return ;

	QTreeWidgetItem *groupItem = NULL;
	int GroupCount = RootItem->childCount ();
	for(int i =0;  i< GroupCount; i++)
	{
		groupItem = RootItem->child(i);
		if (!groupItem || isQunItem(groupItem) )
			continue;

#if MS_VC6
		Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
		Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
		if( group_info && group_info->groupID == id) 
		{
			//free all account data of this group
			freeAllAccountdata(groupItem);    
			delete group_info;
			RootItem->removeChild(groupItem); 
			return;
		}
	}

}

//add account info to tree widget
void BuddyOpt::addAccountToTree()
{
	Fetion_Account *account = NULL;
	DList *tmp_account = (DList *)fx_get_account();
	while(tmp_account) 
	{
		account = (Fetion_Account *)tmp_account->data; 
		if(account)
			addAccountToGroup(account);

		tmp_account = d_list_next(tmp_account);
	}
}

//add account to it's group item 
void BuddyOpt::addAccountToGroup(const Fetion_Account *account)  
{
	//remove the user's id on account from list...
	if (!account || account->id == (qlonglong)strtol(fx_get_usr_uid(), NULL, 10))
		return;

	int	group_no = fx_get_account_group_id(account) ;
	if(group_no <= 0)
	{
		group_no = 0;
		if( !have_zero_group)
		{
			QTreeWidgetItem *item;
			item = new QTreeWidgetItem(treeWidget);

			if (!m_isMainView)
				item->setCheckState(0, Qt::Unchecked);

			QString str = tr("un set group");

			Group_Info *groupinfo = new Group_Info;
			groupinfo->groupName = str;
			groupinfo->groupID = group_no;
			groupinfo->online_no = 0;

			item->setText(0, str);
#if MS_VC6
			QVariant Var((uint)groupinfo);
#else
			QVariant Var;
			Var.setValue (groupinfo); 
#endif
			item->setData(0, Qt::UserRole, Var);

			have_zero_group = true;
		}
	}

	char * showname = fx_get_account_show_name(account, TRUE);
	QString show_name = QString::fromUtf8(showname);
	int online_state = fx_get_online_status_by_account(account);

	addAccountToGroup(account, show_name, online_state, group_no);

	if(showname)
		free(showname);
}


void BuddyOpt::addAccountToGroup(const Fetion_Account *account, QString & name, int online_state, int group_id )
{
	//remove the user's id on account from list... fixed: it maybe have redundance
	if (!account || account->id == (qlonglong)strtol(fx_get_usr_uid(), NULL, 10))
		return;

	QTreeWidgetItem * groupItem = findGroupItemByID(group_id);
	if(!groupItem)
		return;

	Account_Info * ac_info = new Account_Info;
	ac_info->accountName = name;
	ac_info->accountID = account->id;
	ac_info->onlinestate = online_state;
    ac_info->haveUpdate = false;

	QTreeWidgetItem *accountItem = new QTreeWidgetItem;

	if (!m_isMainView)
		accountItem->setCheckState(0, Qt::Unchecked);
	accountItem->setText(0, name);

#if MS_VC6
	QVariant Var((uint)ac_info);
#else
	QVariant Var;
	Var.setValue (ac_info); //vc7 up
#endif
	accountItem->setData(0, Qt::UserRole, Var);

	accountItem->setIcon(0, getOnlineStatusIcon(ac_info->onlinestate));
	setTipsOfAccount(accountItem, account);


	groupItem->addChild(accountItem);

#if MS_VC6
	Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
	Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
	if (!group_info)
		return;

	if (fx_is_on_line_by_account (account))
	{
		groupItem->removeChild(accountItem);
		groupItem->insertChild(group_info->online_no, accountItem);
		group_info->online_no ++;
	}


#ifdef WIN32
	char online[30];
	_snprintf (online, sizeof(online)-1, "(%d/%d)", group_info->online_no, groupItem->childCount());
	QString groupShowName = group_info->groupName+ online;
#else
	char *online= NULL;
	asprintf(&online, "(%d/%d)", group_info->online_no, groupItem->childCount());
	QString groupShowName = group_info->groupName+ online;
	if(online)
		free(online);
#endif
	groupItem->setText(0, groupShowName);
}

QString BuddyOpt::createAccountTipsInfo(const Fetion_Account *account)
{
	QString tips;
	QString tmp;
	bool hP = false;
	if (account->personal)
		hP = true;

	QString info;
	info += tr("mobile_no:");
	if (hP)
	{
		tmp = QString::fromUtf8(account->personal->mobile_no);
		if (!tmp.isEmpty())
		{
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}
	} else {
		if (!fx_is_pc_user_by_account(account))
		{
			char *mobile_no = fx_get_original_ID(account->id);
			info += "<b style=\"color:red; \">"+QString(mobile_no) + "</b>"; 
			if (mobile_no)
				free(mobile_no);
		}

		info += "<b style=\"color:red; \"> </b>"; 
	tips += info +"<br>";
	}


	if (fx_is_pc_user_by_account(account)){
		info = tr("fetion_no:");
		info += "<b style=\"color:red; \">"+QString("%1").arg(account->id)+"</b>";
		tips += info +"<br>";
	}

	if(hP)
	{
		tmp = QString::fromUtf8(account->personal->nickname);
		if (!tmp.isEmpty())
		{
			info = tr("nickname:");
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}

		tmp = QString::fromUtf8(account->personal->name);
		if (!tmp.isEmpty())
		{
			info = tr("name:");
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}

		info = tr("gender:");
		switch(account->personal->gender)
		{
			case 2:
				info += "<b style=\"color:red; \">" + tr("girl") +"</b>";
				tips += info +"<br>";
				break;
			case 1:
				info += "<b style=\"color:red; \">" + tr("boy") +"</b>";
				tips += info +"<br>";
				break;
			case 0:
				info += "<b style=\"color:red; \">" + tr("unknow") +"</b>";
				break;
		}

		tmp = QString::fromUtf8(account->personal->impresa);
		if (!tmp.isEmpty())
		{
			info = tr("impresa:");
			info += "<b style=\"color:red; \">" + tmp +"</b>";
			tips += info +"<br>";
		}
	}

	//remove the last "<br>"
	tips = tips.remove( tips.size() - strlen("<br>"), strlen("<br>"));
	return tips;
}

//fix: need to....
void BuddyOpt::setTipsOfAccount(QTreeWidgetItem *accountItem, const Fetion_Account *account)
{
	if (!accountItem || !account)
		return;
	QString tips = createAccountTipsInfo(account);
	accountItem->setToolTip(0, tips); 
}

QTreeWidgetItem* BuddyOpt::findGroupItemByID(int group_id)
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if (!RootItem)
		return NULL;

	int GroupCount = RootItem-> childCount ();
	QTreeWidgetItem *groupItem = NULL;
	for(int i =0;  i< GroupCount; i++)
	{
		groupItem = RootItem->child(i);
		if (!groupItem || isQunItem(groupItem) )
			continue;

#if MS_VC6
		Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
		Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
		if (!group_info)
			continue;

		if (group_id == group_info->groupID)
			return groupItem;
	}

	return NULL;
}

QTreeWidgetItem* BuddyOpt::findAccountItemFromGroup(QTreeWidgetItem *groupItem, const Fetion_Account *account)
{
	if (!groupItem || !account)
		return NULL;

	QTreeWidgetItem *tmpItem = NULL;
	qlonglong account_id = (qlonglong)account->id;

	int itemCounts = groupItem-> childCount ();
	for(int i =0;  i< itemCounts; i++)
	{
		tmpItem = groupItem->child(i);
		if (!tmpItem)
			continue;
#if MS_VC6
		Account_Info *ac_info =(Account_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
#else
		Account_Info *ac_info =tmpItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
		if (ac_info && account_id == ac_info->accountID)
			return tmpItem;
	}
	return NULL;
}

QTreeWidgetItem* BuddyOpt::findAccountItem(const Fetion_Account *account)
{
	if (!account)
		return NULL;
	int	group_no = fx_get_account_group_id(account) ;
	if (group_no <= 0)
		group_no = 0;

	QTreeWidgetItem* groupItem = findGroupItemByID(group_no);
	return findAccountItemFromGroup(groupItem, account);
}

static bool isonline(int state)
{
	if (state != FX_STATUS_OFFLINE && 
			state != 0 &&
			state != FX_STATUS_WAITING_AUTH &&
			state != FX_STATUS_REFUSE &&
			state != FX_STATUS_BLACK &&
			state != FX_STATUS_MOBILE ) 
		return true;
	else
		return false;

}
//this function will add to libfetion impl...
// return false should not changed
// true should changed
// state is 0 sub the online number
//       is 1 add the online number
bool BuddyOpt::isOnlineStateChanged(int old_state, int new_state, int* state)
{
	if (old_state == new_state)
		return false;

	if (isonline(old_state))  //old_state is online state
	{
		if( !isonline(new_state)) { //new state is offline state
			*state = 0;
			return true;
		}
	} else { //old_state is offline state
		if( isonline(new_state)) { //new state is online state
			*state = 1;
			return true;
		}
	}
	return false;
}

void BuddyOpt::updateAccountInfo(qlonglong account_id)
{
	const Fetion_Account * account = fx_get_account_by_id (account_id);
	if (!account)
		return;

	QTreeWidgetItem* accountItem = findAccountItem(account);

	//not find this account, so add it to Group...
	if(!accountItem) {
		printf("not find the item , i will add to the group\n");
		addAccountToGroup (account);
		return;
	}

	//update the account info
	setTipsOfAccount(accountItem, account);

	char * showname = fx_get_account_show_name(account, TRUE);
	QString show_name = QString::fromUtf8(showname);
	if (showname)
		free(showname);

#if MS_VC6
	Account_Info *ac_info =(Account_Info*)(accountItem->data(0, Qt::UserRole).toUInt());
#else
	Account_Info *ac_info = accountItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
	if (!ac_info)
		return;
	ac_info->accountName = show_name;

	int old_online_state = ac_info->onlinestate;
	int new_online_state = fx_get_online_status_by_account(account);
	ac_info->onlinestate = new_online_state;

	accountItem->setText(0, show_name);
	accountItem->setIcon(0, getOnlineStatusIcon(ac_info->onlinestate));

	int state = 0;
	if (isOnlineStateChanged(old_online_state , new_online_state , &state))
	{
		//group name ++1
		int	group_no = fx_get_account_group_id(account) ;
		if(group_no <= 0)
			group_no = 0;

		QTreeWidgetItem* groupItem = findGroupItemByID(group_no);
		if (groupItem)
		{
#if MS_VC6
	Group_Info *group_info =(Group_Info *)( groupItem->data(0, Qt::UserRole).toUInt() );
#else
	Group_Info *group_info = groupItem->data(0, Qt::UserRole).value<Group_Info *>();
#endif
	if (!group_info)
		return ;
	groupItem->removeChild(accountItem);
	
//if (groupItem->childCount() == 0)
	if (group_info->online_no - 1 > 0)
		groupItem->insertChild(group_info->online_no - 1, accountItem);
	else
		groupItem->insertChild(0, accountItem);

	if (state)			
		group_info->online_no ++;
	else 
		group_info->online_no --;

			
#ifdef WIN32
			char online[30];
			_snprintf (online, sizeof(online)-1, "(%d/%d)", group_info->online_no, groupItem->childCount());
			QString groupShowName = group_info->groupName+ online;
#else
			char *online= NULL;
			asprintf(&online, "(%d/%d)", group_info->online_no, groupItem->childCount());
			QString groupShowName = group_info->groupName+ online;
			if(online)
				free(online);
#endif

			groupItem->setText(0, groupShowName);
		}
	}
	printf("Updata new buddy.... \n");
}

void BuddyOpt::updateStyles(QTreeWidgetItem *item, int column)
{
    if (!item || column != 0)
        return;

    Qt::CheckState state = item->checkState(0);
    QTreeWidgetItem *parent = item->parent();

    if (parent) {
        // Only count style items.
        if (state == Qt::Checked)
            ++markedCount;
        else
            --markedCount;
		/*
		//bad code...
		if (markedCount > 32)
		return;
		*/
        if (state == Qt::Checked &&
            parent->checkState(0) == Qt::Unchecked) {
            // Mark parent items when child items are checked.
            parent->setCheckState(0, Qt::Checked);

        } else if (state == Qt::Unchecked &&
                   parent->checkState(0) == Qt::Checked) {

            bool marked = false;
            for (int row = 0; row < parent->childCount(); ++row) {
                if (parent->child(row)->checkState(0) == Qt::Checked) {
                    marked = true;
                    break;
                }
            }
            // Unmark parent items when all child items are unchecked.
            if (!marked)
                parent->setCheckState(0, Qt::Unchecked);
        }
    } else {
        int row;
        int number = 0;
        for (row = 0; row < item->childCount(); ++row) {
            if (item->child(row)->checkState(0) == Qt::Checked)
                ++number;
        }

        // Mark/unmark all child items when marking/unmarking top-level
        // items.
        if (state == Qt::Checked && number == 0) {
            for (row = 0; row < item->childCount(); ++row) {
                if (item->child(row)->checkState(0) == Qt::Unchecked)
                    item->child(row)->setCheckState(0, Qt::Checked);
            }
        } else if (state == Qt::Unchecked && number > 0) {
            for (row = 0; row < item->childCount(); ++row) {
                if (item->child(row)->checkState(0) == Qt::Checked)
                    item->child(row)->setCheckState(0, Qt::Unchecked);
            }
        }
    }
	emit m_itemChanged();
}



void BuddyOpt::UpdateSkins()
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return ;

	QTreeWidgetItem *groupItem = NULL;
	int GroupCount = RootItem-> childCount ();

	for(int i =0;  i< GroupCount; i++)
	{
		groupItem = RootItem->child(i);
		if (!groupItem)
			continue;

		if (isQunItem(groupItem))
		{
			UpdateSkinsForQun(groupItem);    
			continue;
		}

		//update all account icon for this group
		UpdateSkinsForAccount(groupItem);    
	}
}

void BuddyOpt::UpdateSkinsForAccount(QTreeWidgetItem *groupItem)
{
	if (!groupItem)
		return;

	int itemCounts = groupItem->childCount();
	QTreeWidgetItem *tmpItem = NULL;

	for(int i = 0; i < itemCounts; i++)
	{
		tmpItem = groupItem->child(i);
		if(!tmpItem)
			continue;
#if MS_VC6
		Account_Info *ac_info =(Account_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
#else
		Account_Info *ac_info =tmpItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
		if (!ac_info)
			continue;

		tmpItem->setIcon(0, getOnlineStatusIcon(ac_info->onlinestate));
	}
}

void BuddyOpt::UpdateSkinsForQun(QTreeWidgetItem *groupItem)
{
	if (!groupItem)
		return;

	QTreeWidgetItem *tmpItem = NULL;
	int itemCounts = groupItem->childCount();

	for(int i = 0; i < itemCounts; i++)
	{
		tmpItem = groupItem->child(i);
		if (!tmpItem)
			continue;
		tmpItem->setIcon(0, getQunIcon());
	}
}

Account_Info *BuddyOpt::fetchNoUpdateAccount()
{
	QTreeWidgetItem *RootItem = this->treeWidget->invisibleRootItem();
	if(!RootItem)
		return NULL;

	int GroupCount = RootItem-> childCount ();
	QTreeWidgetItem *groupItem = NULL;
	for(int i =0;  i< GroupCount; i++)
	{
		groupItem = RootItem->child(i);
		if (!groupItem || isQunItem(groupItem) )
			continue;
		int itemCounts = groupItem-> childCount();
		QTreeWidgetItem *tmpItem = NULL;
		for(int i =0;  i< itemCounts; i++)
		{
			tmpItem = groupItem->child(i);
			if(!tmpItem)
				continue;
#if MS_VC6
			Account_Info *ac_info =(Account_Info*)(tmpItem->data(0, Qt::UserRole).toUInt());
#else
			Account_Info *ac_info =tmpItem->data(0, Qt::UserRole).value<Account_Info*>() ;
#endif
			if (!ac_info)
				continue;
            if(!ac_info->haveUpdate)
                return ac_info;
		}
	}

    return NULL;
}
