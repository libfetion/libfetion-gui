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
#include "fxscheduleSmsManage.h"
#include "fxscheduleSms.h"

FxScheduleSMSManage::FxScheduleSMSManage(FxMainWindow * wind,QWidget *parent)
    : QMainWindow(parent)
{
	setupUi(this);
	mainwind = wind;

    init_UI();
	
	QSize dt_size =	QApplication::desktop()->size();
	this->move(dt_size.width()/3, dt_size.height()/3);
}

FxScheduleSMSManage::~FxScheduleSMSManage()
{

}

void FxScheduleSMSManage::init_UI()
{
	connect(CreateSCM, SIGNAL(clicked()), this, SLOT(createItemOfSCMList()));
	connect(DeleteSCM, SIGNAL(clicked()), this, SLOT(deleteItemOfSCMList()));
	connect(RefreshSCM, SIGNAL(clicked()), this, SLOT(refreshSCMList()));
	connect(ChooseAllSCM, SIGNAL(clicked()), this, SLOT(chooseAllSCMList()));

	QStringList labels;
	labels << tr("receiver") << tr("send timer") << tr("message content");

	view->horizontalHeader()->setDefaultSectionSize(90);
	view->setColumnCount(3);
	view->setHorizontalHeaderLabels(labels);
#if 0
	view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
	view->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	view->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
#endif
	view->verticalHeader()->hide();

	loadALlSCMList();
}

void FxScheduleSMSManage::createItemOfSCMList()
{
    if (mainwind && mainwind->getScheduleSmsWindow())
        mainwind->getScheduleSmsWindow()->showNormal();
}

void FxScheduleSMSManage::deleteItemOfSCMList()
{
	QTableWidgetItem * item = NULL;
	int row = view->rowCount();
	for (int i = row; i >= 0; i--)
	{
		item = view->item(i, 0); 
		if (!item)
			continue;

		if (item->checkState() == Qt::Checked)
		{
#if DEBUG_GUI
			printf ("FxScheduleSMSManage::deleteItemOfSCMList() %d \n", 
					item->data(Qt::UserRole).toInt());
#else
			fx_delete_schedule_sms(item->data(Qt::UserRole).toInt(), NULL, NULL);
#endif
			view->removeRow(i);//fix bug?
		}
	}
}

void FxScheduleSMSManage::slot_update_scheduleList()
{
	loadALlSCMList();
}

void FxScheduleSMSManage::refreshSCMList()
{
	loadALlSCMList();
}

void FxScheduleSMSManage::chooseAllSCMList()
{
	QTableWidgetItem * item = NULL;
	int row = view->rowCount();
	for (int i = 0; i < row; i++)
	{
		item = view->item(i, 0); 
		if (!item)
			continue;
		item->setCheckState(Qt::Checked);
	}
}

void FxScheduleSMSManage::loadALlSCMList()
{
	view->setRowCount(0);
#if DEBUG_GUI
QString receiver = "ddd";
QString send_time= "3224234";
QString message = "32242349324324";
addItemToSCMList(receiver, send_time, message, 0);
addItemToSCMList(receiver, send_time, message, 2);
#else
	Fetion_Schedule_SMS *sch_sms = NULL;
	DList *tmp = (DList *)fx_data_get_smlist();

	while(tmp)
	{
		sch_sms = (Fetion_Schedule_SMS *) tmp->data;
		addItemToSCMList(sch_sms);
		tmp = d_list_next(tmp);
	}
#endif
}

void FxScheduleSMSManage::addItemToSCMList(Fetion_Schedule_SMS* scm)
{
	if (!scm)
		return;
	char * name = fx_covert_schedule_receiver_to_string(scm->receivers);
	if (!name)
		return;
	QString receiver = QString::fromUtf8(name);

	QString send_time= QString::fromUtf8(scm->send_time);
	QString message = QString::fromUtf8(scm->message);
	addItemToSCMList(receiver, send_time, message, scm->id);
	free(name);
}

void FxScheduleSMSManage::addItemToSCMList(QString &receiver, QString& send_time, QString &message,
	   	int scm_id )
{
	int row = view->rowCount();
	view->setRowCount(row + 1);

	QTableWidgetItem *item0 = new QTableWidgetItem(receiver);
	item0->setData(Qt::UserRole, scm_id);
	item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);

	QTableWidgetItem *item1 = new QTableWidgetItem(send_time);
	item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
	QTableWidgetItem *item2 = new QTableWidgetItem(message);
	item2->setFlags(item2->flags() & ~Qt::ItemIsEditable);
							
	view->setItem(row, 0, item0);
	view->setItem(row, 1, item1);
	view->setItem(row, 2, item2);

	item0->setCheckState(Qt::Unchecked);

}

void FxScheduleSMSManage::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if (mainwind)
		mainwind->showNormal();
    this->hide();
//	event->accept();
}

