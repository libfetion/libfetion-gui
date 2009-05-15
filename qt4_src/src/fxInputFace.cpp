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
#include "fxmsgwindow.h" 
#include "fxmytabwidget.h" 
#include "fxInputFace.h"

static FACES_INFO _faces_info[] = {

	{"&gt;:)",    MyRect(80,  229,  118, 259), QObject::tr("naughty")}, //ÌÔÆøµÄ
	{"o:)", MyRect(280, 191,  318, 227), QObject::tr("angel")}, //ÌìÊ¹
	{"*-:)", MyRect(160, 153,  198, 189), QObject::tr("idea")}, //Ïë·¨

//	{":)",   MyRect(1,   2, 38,  39)},
	{":-d",  MyRect(40,  2, 78,  39), QObject::tr("yuk")}, //´óÐ¦,
	{";)",   MyRect(80,  2, 118, 39), QObject::tr("blink")}, //Õ£ÑÛ,
	{":-o",  MyRect(120, 2, 158, 39), QObject::tr("surprise")}, //¾ªÑÈ,
	{":-p",  MyRect(160, 2, 198, 39), QObject::tr("spit_tongue_smile")}, //ÍÂÉàÐ¦Á³,
	{"(h)",  MyRect(200, 2, 238, 39), QObject::tr("fiery_smile")}, //ÈÈÁÒÐ¦Á³,,
	{":-@",  MyRect(240, 2, 278, 39), QObject::tr("Angry")}, //ÉúÆø
	{":(",   MyRect(280, 2, 318, 39), QObject::tr("sadness")}, //±¯ÉË,


	{":'(",     MyRect(1,   40,  39,  77), QObject::tr("blubber")}, //¿ÞÆü,
	{":\"&gt;", MyRect(40,  40,  78,  77), QObject::tr("embarrass")}, //ÞÏÞÎ
	{"^o)",     MyRect(80,  40,  118, 77), QObject::tr("Ironically")}, //·í´Ì,
	//{":&amp;",  MyRect(120, 40,  158, 77)},
	{":&",	MyRect(120, 40,  158, 77), QObject::tr("Sick")}, //Éú²¡,
	{"8o|",	MyRect(160, 40,  198, 77), QObject::tr("Yaoyaqiechi")}, //Ò§ÑÀÇÐ³Ý
	{"|-)",	MyRect(200, 40,  238, 77), QObject::tr("sleepy")}, //À§
	{":-#",	MyRect(240, 40,  278, 77), QObject::tr("Privacy")}, //±£ÃÜ
	{"8-)",			MyRect(280, 40,  318, 77), QObject::tr("MomentisEyess")}, //×ª¶¯µÄÑÛ¾¦

	{"(s)",		MyRect(1,   78,  39,  113), QObject::tr("sleepyMoon")}, //³ÁË¯µÄÔÂÁÁ
	{"(st)",	MyRect(40,  78,  78,  113), QObject::tr("rain")}, //ÏÂÓê
	{"(o)",		MyRect(80,  78,  118, 113), QObject::tr("clock")}, //Ê±ÖÓ
	{"(l)",		MyRect(120, 78,  158, 113), QObject::tr("hearts")}, //ºìÐÄ
	{"(u)",		MyRect(160, 78,  198, 113), QObject::tr("crackedhearts")}, //ÆÆËéµÄÐÄ
	{"(@)",		MyRect(200, 78,  238, 113), QObject::tr("catfaces")}, //Ã¨Á³
	{"(&)",		MyRect(240, 78,  278, 113), QObject::tr("dogfaces")}, //¹·Á³
	{"(sn)",	MyRect(280, 78,  318, 113), QObject::tr("snail")}, //ÎÏÅ£


	{"(*)",MyRect(1,   115,  39,  151), QObject::tr("star")}, //ÐÇÐÇ
	{"(#)",MyRect(40,  115,  78,  151), QObject::tr("sun")}, //Ì«Ñô
	{"(r)",MyRect(80,  115,  118, 151), QObject::tr("rainbow")}, //²Êºç
	{"(})",MyRect(120, 115,  158, 151), QObject::tr("leftembrace")}, //×óÓµ±§,
	{"({)",MyRect(160, 115,  198, 151), QObject::tr("rightembrace")}, //ÓÒÓµ±§,
	{"(k)",MyRect(200, 115,  238, 151), QObject::tr("lip")}, //ºì´½
	{"(f)", MyRect(240, 115,  278, 151), QObject::tr("rose")}, //ºìÃµ¹å
	{"(w)", MyRect(280, 115,  318, 151), QObject::tr("emarcidrose")}, //µòÐ»µÄÃµ¹å


	{"(g)",  MyRect(1,   153,  39,  189), QObject::tr("giftbox")}, //ÀñÆ·ºÐ
	{"(^)",  MyRect(40,  153,  78,  189), QObject::tr("Birthdaycake")}, //ÉúÈÕµ°¸â
	{"-8",   MyRect(80,  153,  118, 189), QObject::tr("music")}, //ÒôÀÖ
	{"(i)",  MyRect(120, 153,  158, 189), QObject::tr("bulb")}, //µÆÅÝ,
//	{"*-:)", MyRect(160, 153,  198, 189)},
	{"(c)",  MyRect(200, 153,  238, 189), QObject::tr("coffee")}, //¿§·È
	{"(um)", MyRect(240, 153,  278, 189), QObject::tr("bumbershoot")}, //ÓêÉ¡
	{"b)",	 MyRect(280, 153,  318, 189), QObject::tr("withsunglass")}, //´ø×ÅÌ«Ñô¾µ


	{"(mp)",MyRect(1,   191,  39,  227), QObject::tr("mobiletelephone")}, //ÊÖ»ú
	{"(co)",MyRect(40,  191,  78,  227), QObject::tr("computer")}, //¼ÆËã»ú
	{":-|", MyRect(80,  191,  118, 227), QObject::tr("despair")}, //Ê§Íû
	{":-/", MyRect(120, 191,  158, 227), QObject::tr("confused")}, //À§»óµÄ
	{":-s", MyRect(160, 191,  198, 227), QObject::tr("afraid")}, //µ£ÐÄ
	{")-|", MyRect(200, 191,  238, 227), QObject::tr("drink")}, //ÒûÁÏ
	{"(d)", MyRect(240, 191,  278, 227), QObject::tr("goblet")}, //¸ß½Å±­
	//{"o:)", MyRect(280, 191,  318, 227)},


	{":-?",		  MyRect(1,   229,  39,  259), QObject::tr("meditation")}, //³ÁË¼
	{"(y)",		  MyRect(40,  229,  78,  259), QObject::tr("Praised")}, //Ì«°ôÁË
	//{"&gt;:)",    MyRect(80,  229,  118, 259)},
	{":-b",		  MyRect(120, 229,  158, 259), QObject::tr("spoony")}, //±¿¹Ï

	{":)",   MyRect(1,   2, 38,  39) , QObject::tr("smile")}, //Î¢Ð¦

};

bool FxInputFace::getFaces(int x, int y, QString &face, QString &str)
{
	int i;
	for (i = 0; i < sizeof(_faces_info)/sizeof(_faces_info[0]); i++) 
		if (_faces_info[i].rect.contains (x, y)) {
			face = _faces_info[i].face;
			str = _faces_info[i].str;
			return true;
		}

		face = "";
		str = "";
		return false;
}

FxInputFace::FxInputFace(QWidget *parent)
    : QDialog(parent)
{
	msgWind = NULL;
}

FxInputFace::~FxInputFace()
{

}

void FxInputFace::setMsgWindow(FxMsgWindow *Wind)
{
	msgWind = Wind;
}

#ifdef Q_OS_MAC
static bool shouldHide = false;
#endif

void FxInputFace::focusOutEvent ( QFocusEvent * event ) 
{
#ifdef Q_OS_MAC
	if (shouldHide)
	{
		shouldHide = false;
		hide();
		return;
	}

	shouldHide = true;
#else
	hide();
#endif
}

void FxInputFace::mouseReleaseEvent ( QMouseEvent * event ) 
{
#ifdef Q_OS_MAC
	shouldHide = false;
#endif
	hide();

	//releaseMouse();

	if (!msgWind)
		return;

	FxMyTabWidget *tabWidget = msgWind->tabWidget;
	if (!tabWidget)
		return;

	AccountTab *accountTab = (AccountTab*)(tabWidget->widget(tabWidget->currentIndex()));
	if (!accountTab)
		return;

	/*
	 QString symbol;
		 QString str;
	QString face = getFaces (event->x(), event->y());
*/

	QString face;
	QString str;
	if (!getFaces (event->x(), event->y(), face, str))
		return;
	QString msg = accountTab->msgSend->MsgEdit->toPlainText();
	QString newmsg = msg + face;

	accountTab->msgSend->MsgEdit->setText(newmsg);
	accountTab->msgSend->MsgEdit->moveCursor(QTextCursor::End);
	accountTab->msgSend->MsgEdit->setFocus();
}


void FxInputFace::mouseMoveEvent(QMouseEvent *event)
{
	QString face;
	QString str;
	if (!getFaces (event->x(), event->y(), face, str))
		return;

	QString text = str + "  " + face;
	QToolTip::showText(event->globalPos(), text, this);
}

