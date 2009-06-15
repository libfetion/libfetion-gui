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
#include "fxconfigDlg.h"
#include "appconfig.h"

FxConfigDia::FxConfigDia(FxMainWindow *wind, QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);
	mainwind= wind;
	hotKey->installEventFilter(this);

	init_state();
	init_connect();
}

FxConfigDia::~FxConfigDia()
{

}
void FxConfigDia::closeEvent(QCloseEvent *event)
{
	if (mainwind)
		mainwind->show();
	event->accept();
}

void FxConfigDia::init_state()
{
	if (Settings::instance().isDisableNudge())
		CB_DisableNudge->setCheckState(Qt::Checked);
	else
		CB_DisableNudge->setCheckState(Qt::Unchecked);

	if (Settings::instance().isMainWindowTopHint())
		CB_MainTopHit->setCheckState(Qt::Checked);
	else
		CB_MainTopHit->setCheckState(Qt::Unchecked);

#ifdef Q_OS_MAC
	CB_AutoShowMsg->setVisible(false); 
#endif
	if (Settings::instance().isAutoShowMsg())
		CB_AutoShowMsg->setCheckState(Qt::Checked);
	else
		CB_AutoShowMsg->setCheckState(Qt::Unchecked);

	if (Settings::instance().isAutoLogin())
		CB_RemberPwd->setCheckState(Qt::Checked);
	else
		CB_RemberPwd->setCheckState(Qt::Unchecked);


	if (Settings::instance().isStartHide())
		CB_MainStartHide->setCheckState(Qt::Checked);
	else
		CB_MainStartHide->setCheckState(Qt::Unchecked);

	if (Settings::instance().isMute())
		CB_Mute->setCheckState(Qt::Checked);
	else
		CB_Mute->setCheckState(Qt::Unchecked);

	if (Settings::instance().isEnableLongSMS())
		CB_LongMsg->setCheckState(Qt::Checked);
	else
		CB_LongMsg->setCheckState(Qt::Unchecked);

	if (Settings::instance().isEnterSend())
	{
		RB_EnterSend->setChecked(true);
		RB_CtrlEnterSend->setChecked(false);
	} else {
		RB_EnterSend->setChecked(false);
		RB_CtrlEnterSend->setChecked(true);
	}

	if (Settings::instance().isAutoReply())
		CB_AutoReply->setCheckState(Qt::Checked);
	else
		CB_AutoReply->setCheckState(Qt::Unchecked);

	MsgAutoRelpy->setText(Settings::instance().getAutoReply());
	RingFile->setText(Settings::instance().MsgRingPath());

#if WIN32
	if (Settings::instance().isEnableGetMsgHotKey())
	{
		CB_EnableHotKey->setCheckState(Qt::Checked);
		hotKey->setDisabled(false);
	} else {
		CB_EnableHotKey->setCheckState(Qt::Unchecked);
		hotKey->setDisabled(true);
	}

	setHotKeyValue();
#else
	CB_EnableHotKey->setDisabled(true);
	hotKey->setDisabled(true);
#endif

}

void FxConfigDia::init_connect()
{
		connect(BT_ChangeRing, SIGNAL(clicked ()), this, SLOT(slot_ChangeRing()));
		connect(BT_TestRing, SIGNAL(clicked ()), this, SLOT(slot_TestRing()));
		connect(BT_RingDefault, SIGNAL(clicked ()), this, SLOT(slot_DefaultRing()));

		connect(CB_DisableNudge, SIGNAL(clicked ()), this, SLOT(slot_DisableNudge()));
		connect(CB_MainTopHit, SIGNAL(clicked ()), this, SLOT(slot_MainTopHit()));
		connect(CB_MainStartHide, SIGNAL(clicked ()),	this, SLOT(slot_MainStartHide()));
		connect(CB_RemberPwd, SIGNAL(clicked ()), this, SLOT(slot_RemberPwd()));
		connect(CB_AutoShowMsg, SIGNAL(clicked ()), this, SLOT(slot_AutoShowMsg()));
		connect(CB_Mute, SIGNAL(clicked ()), this, SLOT(slot_Mute()));
		connect(CB_LongMsg, SIGNAL(clicked ()), this, SLOT(slot_LongMsg()));

		connect(CB_AutoReply, SIGNAL(clicked ()), this, SLOT(slot_AutoReplyMsg()));
		connect(MsgAutoRelpy, SIGNAL(textChanged()), this, SLOT(slot_LimitReplyMsgLenth()));

		connect(RB_EnterSend, SIGNAL(clicked ()), this, SLOT(slot_SendMode()));
		connect(RB_CtrlEnterSend, SIGNAL(clicked ()), this, SLOT(slot_SendMode()));
		connect(CB_EnableHotKey, SIGNAL(clicked ()), this, SLOT(slot_EnableHotKey()));
		connect(BT_SetFont, SIGNAL(clicked ()), this, SLOT(slot_SetFont()));
		connect(BT_SetDefaultFont, SIGNAL(clicked ()), this, SLOT(slot_SetDefaultFont()));
}

void FxConfigDia::slot_DisableNudge()
{
	bool state = ! Settings::instance().isDisableNudge();
	Settings::instance().setDisableNudge(state);
}

void FxConfigDia::slot_MainTopHit()
{
	bool state = ! Settings::instance().isMainWindowTopHint();
	Settings::instance().setMainWindowTopHint(state);
}


void FxConfigDia::slot_RemberPwd()
{
	bool state = ! Settings::instance().isAutoLogin();
	Settings::instance().setAutoLogin(state);

	if (mainwind)
	{
		if (state)
			mainwind->AutoLoginAct->setIcon(getMenuIcon(ApplyIcon));
		else 
			mainwind->AutoLoginAct->setIcon(getMenuIcon(CancelIcon));
	}
}

void FxConfigDia::slot_MainStartHide()
{
	bool state = ! Settings::instance().isStartHide();
	Settings::instance().setStartHide(state);
}

void FxConfigDia::slot_AutoShowMsg()
{
	bool state = ! Settings::instance().isAutoShowMsg();
	Settings::instance().setAutoShowMsg(state);
	if (mainwind)
	{
		if (Settings::instance().isAutoShowMsg())
			mainwind->IsAutoShowMsgAct->setIcon(getMenuIcon(ApplyIcon));
		else 
			mainwind->IsAutoShowMsgAct->setIcon(getMenuIcon(CancelIcon));
	}
}
void FxConfigDia::slot_Mute()
{
	bool state = ! Settings::instance().isMute();
	Settings::instance().setMute(state);
	if (mainwind)
	{
		if (Settings::instance().isMute())
			mainwind->MuteAct->setIcon(getMenuIcon(ApplyIcon));
		else 
			mainwind->MuteAct->setIcon(getMenuIcon(CancelIcon));
	}
}

void FxConfigDia::slot_LongMsg()
{
	bool state = ! Settings::instance().isEnableLongSMS();
	Settings::instance().setEnableLongSMS(state);
}

void FxConfigDia::slot_AutoReplyMsg()
{
	if (CB_AutoReply->checkState () == Qt::Checked)
		Settings::instance().setAutoReply(true, MsgAutoRelpy->toPlainText());
	else
		Settings::instance().setAutoReply(false, MsgAutoRelpy->toPlainText());
}

void FxConfigDia::slot_SendMode()
{
	if (RB_EnterSend->isChecked())
		Settings::instance().setEnterSend(true);
	else
		Settings::instance().setEnterSend(false);
}

void FxConfigDia::slot_LimitReplyMsgLenth()
{
	QString msg = MsgAutoRelpy->toPlainText();
	if (msg.size() > 150)
	{
		msg = msg.left(150);
		MsgAutoRelpy->setPlainText(msg);
		MsgAutoRelpy->moveCursor(QTextCursor::End);
	}

	Settings::instance().setAutoReply (Settings::instance().isAutoReply(), MsgAutoRelpy->toPlainText());
}

void FxConfigDia::slot_DefaultRing()
{
	QString defaultPath = defaultSoundPath() +"/msg.wav";
	RingFile->setText(defaultPath);
	Settings::instance().setMsgRingPath(defaultPath);
}

void FxConfigDia::slot_TestRing()
{
	execPlaySound(Settings::instance().MsgRingPath());
}

void FxConfigDia::slot_ChangeRing()
{
	QFileDialog::Options options;
	QString fileName = QFileDialog::getOpenFileName(this,
			tr("get ring file"),
			QDir::homePath (),
			tr("Ring Files (*.wav);;All Files (*)"));

	if (!fileName.isEmpty())
	{
		RingFile->setText(fileName);
		Settings::instance().setMsgRingPath(fileName);
	}
}

void FxConfigDia::slot_EnableHotKey()
{
	bool state = ! Settings::instance().isEnableGetMsgHotKey();
	Settings::instance().setEnableGetMsgHotKey(state);
	if (state)
		hotKey->setDisabled(false);
	else
		hotKey->setDisabled(true);
}

void FxConfigDia::slot_SetFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok,
					Settings::instance().getCurrentFont(), this);
	if (ok)
		SetAllFont(font);
}

void FxConfigDia::slot_SetDefaultFont()
{
	SetAllFont( Settings::instance().getSyetemDefualFont() );
}

void FxConfigDia::SetAllFont(const QFont & font)
{
	Settings::instance().setFont(font); 
	QApplication::setFont(font);

	this->setFont(font);

	if (mainwind)
		mainwind->SetAllFont(font);


	{ //update current dialog contorl font. very bad code, haha
    groupBox->setFont(font);
    RingFile->setFont(font);
    BT_ChangeRing->setFont(font);
    BT_TestRing->setFont(font);
    BT_RingDefault->setFont(font);
    groupBox_2->setFont(font);
    MsgAutoRelpy->setFont(font);
    CB_AutoReply->setFont(font);
    groupBox_3->setFont(font);
    RB_EnterSend->setFont(font);
    RB_CtrlEnterSend->setFont(font);
    groupBox_4->setFont(font);
    CB_MainTopHit->setFont(font);
    CB_AutoShowMsg->setFont(font);
    CB_RemberPwd->setFont(font);
    CB_MainStartHide->setFont(font);
    CB_Mute->setFont(font);
    CB_LongMsg->setFont(font);
    CB_DisableNudge->setFont(font);
    BT_SetDefaultFont->setFont(font);
    BT_SetFont->setFont(font);
    groupBox_5->setFont(font);
    hotKey->setFont(font);
    CB_EnableHotKey->setFont(font);
    registedHotKeyState->setFont(font);
	}

	this->repaint();
}

bool FxConfigDia::eventFilter(QObject *target, QEvent *event)
{
	if (event->type() == QEvent::KeyPress )
	{
		getHotKeyValue((QKeyEvent *)event);
		return true;
	}
	return QObject::eventFilter(target, event);
}

//return 'a' should ignore the key
QChar keyToChar(int key)
{
	if ((key >= Qt::Key_0 && key <= Qt::Key_9))
		return '0'+ (key - Qt::Key_0);
		
	if (key >= Qt::Key_A && key <= Qt::Key_Z) 
		return 'a'+ (key - Qt::Key_A);

	if (key == 0x21) 
		return '1';
	if (key == 0x40)
		return '2';
	if (key == 0x23)
		return '3';
	if (key == 0x24)
		return '4';
	if (key == 0x25)
		return '5';
	if (key == 0x5e)
		return '6';
	if (key == 0x26)
		return '7';
	if (key == 0x2a)
		return '8';
	if (key == 0x28)
		return '9';
	if (key == 0x29)
		return '0';

	return 'A';  
}

bool FxConfigDia::getHotKeyValue(QKeyEvent *keyEvent)
{
	bool ret = false;
	QChar value = keyToChar(keyEvent->key());
	if (QChar('A') != value)  //filter the input character 
	{
		Settings::instance().setGetMsgHotKey(value, keyEvent->modifiers());
		ret = true;
	}
	setHotKeyValue();
	return ret;
}

void FxConfigDia::setHotKeyValue()
{
	QString Modifiy;
	bool isHaveModifie = false;

	QChar keyValue = Settings::instance().GetMsgHotKey();
	Qt::KeyboardModifiers keyMod = Settings::instance().GetMsgHotKeyMod(); 
	if (keyMod& Qt::ControlModifier)
	{
		Modifiy += "Ctrl + ";
		isHaveModifie = true;
	}
	if (keyMod & Qt::ShiftModifier)
	{
		Modifiy += "Shift + ";
		isHaveModifie = true;
	}
	if (keyMod & Qt::AltModifier)
	{
		Modifiy += "Alt + ";
		isHaveModifie = true;
	}
	if (!isHaveModifie)
		Modifiy = "Ctrl + Alt + ";

	hotKey->setText(QString(Modifiy + keyValue).toUpper());

	if (Settings::instance().isRegistedGetMsgHotKey())
		registedHotKeyState->setText("");
	else
		registedHotKeyState->setText(tr("registed fail"));
}
