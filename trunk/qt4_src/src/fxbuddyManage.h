/***************************************************************************
 *   Copyright (C) 2008-2009 by DDD                                          *
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
#ifndef FXBUDDYMANAGE_H
#define FXBUDDYMANAGE_H

#include <qobject.h>
#include <QMainWindow>
#include <QTreeWidget>
#include <QTextEdit>

#include "appconfig.h"

#include "fxbuddy.h"

#include "fxutil.h"
class FxMainWindow;
class FxMsgWindow; 

class BuddyMge : public QObject
{
	Q_OBJECT
	LOG4QT_DECLARE_QCLASS_LOGGER
	public:
		BuddyMge(QTreeWidget * Widget, FxMainWindow *wind);
		~BuddyMge();

        Account_Info *fetchNoUpdateAccount() 
        {
            if (buddyopt) 
                return buddyopt->fetchNoUpdateAccount();
            return NULL;
        };

        void handleAccountDoubleClicked(QTreeWidgetItem * item);
        void UpdateSkins();

        void showQunWindow(qlonglong qun_id);
        void showMsgWindow(qlonglong account_id);
protected:

    FxMainWindow *getMainWindow() {return m_mainwindow;}
    FxMsgWindow *getMsgWindow(); 

	void showGroupMenu();
	void showBuddyMenu();
	void showQunMenu();
	void createGroupMenu(QMenu *groupMenu);		
	void createSkinMenu(QMenu *skinMenu);		


	//this function will beremoved in later version
	void setQunInfo(QTextEdit *AcInfo, qlonglong qun_id);

	Account_Info *getAc_InfoOfCurrentItem();
	Group_Info *getGp_InfoOfCurrentItem();
	Qun_Info* getQun_InfoOfCurrentItem();
private:
    void initAllActions();

signals:

public slots:

	void slot_accountDoubleClicked (QTreeWidgetItem * item, int column );
	void slot_accountPressed (QTreeWidgetItem * item, int column );
	void moveGroupMenutriggered(QAction *action);
	void updateAccountInfo (qlonglong account_id);

	void slot_MoveGroup(qlonglong account_id, int group_id); 

	void slot_add_group(int, int, qlonglong);
	void slot_del_group(int, int, qlonglong);
	void slot_reName_group(int, int, qlonglong);
	void slot_reName_buddy(int, int, qlonglong);


    void slot_addGroup();
    void slot_deleteGroup();
    void slot_renameGroup();

    void slot_renameBuddy();
    void slot_imBuddy();
    void slot_smsBuddy();
    void slot_getInfoBuddy();
    void slot_updateInfoBuddy();
    void slot_deleteBuddy();
    void slot_addBlackBuddy();
    void slot_removeBlackBuddy();


    void slot_imQun();
    void slot_smsQun();
    void slot_getInfoQun();

	void slot_del_buddy(int, int, qlonglong);
	void slot_add_buddy(int, int, qlonglong);
private:

    QAction *ReNameBuddyAct;
    QAction *IMBuddyAct;
    QAction *SMSBuddyAct;
    QAction *GetInfoBuddyAct;
    QAction *RefreshInfoBuddyAct;
    QAction *DeleteBuddyAct;
    QAction *AddBlackBuddyAct;

    QAction *RemoveBlackBuddyAct;

    QAction *AddGroupAct;
    QAction *DeleteGroupAct;
    QAction *ReNameGroupAct;

    QAction *IMQunAct;
    QAction *SMSQunAct;
    QAction *GetInfoQunAct;
private:
    FxMainWindow *m_mainwindow;
    QTreeWidget *treeWidget;
    BuddyOpt *buddyopt; 
	QMenu *buddyMenu;
};

#endif
