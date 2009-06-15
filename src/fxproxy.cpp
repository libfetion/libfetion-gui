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
#include <QtGui>
#include "fxproxy.h"

FxProxy::FxProxy(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);
	isQuit = false;

	proxy_item = new PROXY_ITEM;
	memset(proxy_item, 0, sizeof(PROXY_ITEM));
	//get the proxy setting
	isSetProxy(proxy_item);

	//set the proxy of libfetion
	fx_set_proxy (proxy_item);

	HostEdit->setText(proxy_item->host);
	PortEdit->setText(proxy_item->port);
	UserEdit->setText(proxy_item->name);
	PassWordEdit->setText(proxy_item->pwd);

	switch (proxy_item->type)
	{
		case PROXY_HTTP:
			connectMode->setCurrentIndex(1); 
			setDisableEdit(false);
			break;
		case PROXY_SOCKS5:
			connectMode->setCurrentIndex(2); 
			setDisableEdit(false);
			break;
		default:
			connectMode->setCurrentIndex(0); 
			setDisableEdit(true);
			break;
	}

	connect(TestButton, SIGNAL(clicked()), this, SLOT(TestNetting()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(SetProxy()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(connectMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setEdit(int)));
}

FxProxy::~FxProxy()
{
	if (proxy_item)
		free_proxt_struct (proxy_item);
}

void FxProxy::free_proxt_struct (PROXY_ITEM * p_item)
{
	if (!p_item)
		return;

	if (p_item->host)
		free (p_item->host);
	if (p_item->port)
		free (p_item->port);
	if (p_item->name)
		free (p_item->name);
	if (p_item->pwd)
		free (p_item->pwd);

	delete p_item;

	p_item = NULL;
}

void FxProxy::proxy_destroy()
{
	isQuit = true;
	close();
}

void FxProxy::closeEvent(QCloseEvent *event)
{
	hide();
	if(isQuit)
		event->accept();
	else
		event->ignore();
}


void FxProxy::setDisableEdit(bool stat)
{
    HostEdit->setDisabled(stat);
    PortEdit->setDisabled(stat);
    UserEdit->setDisabled(stat);
    PassWordEdit->setDisabled(stat);
}

int FxProxy::setEdit(int index)
{
	//if (connectMode->itemText(index) == tr("DirectConnect"))
	if (index == 0)
		setDisableEdit(true);
	else 
		setDisableEdit(false);
		return 0;
}


void FxProxy::init_get_Proxy()
{
	if (proxy_item)
		free_proxt_struct (proxy_item);

	QString state = connectMode->currentText(); 
	if (state == tr("DirectConnect"))
	{
		proxy_item = NULL;
		return;
	}

	proxy_item = new PROXY_ITEM;
	memset(proxy_item, 0, sizeof(PROXY_ITEM));

	if (state == tr("HttpProxy"))
		proxy_item->type = PROXY_HTTP; 

	if (state == tr("Socks5Proxy"))
		proxy_item->type = PROXY_SOCKS5; 

	if (HostEdit->text().size())
		proxy_item->host = strdup (HostEdit->text().toUtf8().data());
	else
		proxy_item->host = NULL;


	if (PortEdit->text().size())
		proxy_item->port = strdup (PortEdit->text().toUtf8().data());
	else
		proxy_item->port = NULL;

	if (UserEdit->text().size())
		proxy_item->name = strdup (UserEdit->text().toUtf8().data());
	else
		proxy_item->name = NULL;

	if (PassWordEdit->text().size())
		proxy_item->pwd = strdup (PassWordEdit->text().toUtf8().data());
	else
		proxy_item->pwd = NULL;
}

void FxProxy::SetProxy()
{
	init_get_Proxy();
	fx_set_proxy (proxy_item);
	setProxy(proxy_item);
}


void FxProxy::TestNetting()
{
	//connectMode
	init_get_Proxy();

	Test_Show->setText(tr("testing net"));
	printf("testing net \n");

	if (fx_test_network(proxy_item, NULL, NULL))
		Test_Show->setText(tr("net ok"));
	else
		Test_Show->setText(tr("net fail"));
	printf("testing over \n");
}

