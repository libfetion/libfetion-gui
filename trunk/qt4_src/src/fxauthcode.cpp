/***************************************************************************
 *   Copyright (C) 2010 by DDD                                          *
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
#include "fxauthcode.h"

FxAuthCode::FxAuthCode(QWidget *parent): QDialog(parent)
{
    FX_FUNCTION
    setupUi(this);
    isQuit = false;
	verfy = NULL;

    connect(buttonBox, SIGNAL(accepted()),
            this, SLOT(SetAuthCode()));
    connect(buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
FxAuthCode::~FxAuthCode()
{
    FX_FUNCTION
	verfy = NULL;
}


/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxAuthCode::authcode_destroy()
{
    FX_FUNCTION
    isQuit = true;
    close();
}

void FxAuthCode::reset(char * guid)
{
    FX_FUNCTION
	if (!guid)
		return;
	verfy = fx_get_auth_code(guid);
	if (!verfy || !verfy->pic)
		return;

	if (verfy->tips)
		auth_tips->setText(QString::fromUtf8(verfy->tips));
	else
		auth_tips->setText("");

	if (verfy->text)
		auth_text->setText(QString::fromUtf8(verfy->text));
	else
		auth_text->setText("");

	auth_pic.loadFromData(verfy->pic, verfy->pic_len);
	auth_image->setPixmap(auth_pic);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxAuthCode::closeEvent(QCloseEvent *event)
{
    FX_FUNCTION
    hide();
    if (isQuit)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


/**************************************************************************/
/*                                                                        */
/**************************************************************************/
void FxAuthCode::SetAuthCode()
{
    FX_FUNCTION
	if (auth_code->text().isEmpty())
		return;
	if (!verfy)
		return;
	fx_set_auth_code(auth_code->text().toUtf8().data(), verfy);
	verfy = NULL;
}

