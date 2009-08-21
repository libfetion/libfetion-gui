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

static FACES_INFO _faces_info[] =
{

    {
        "&gt;:)", MyRect(80, 229, 118, 259), QObject::tr("naughty")
    }
    ,  //�����
    {
        "o:)", MyRect(280, 191, 318, 227), QObject::tr("angel")
    }
    ,  //��ʹ
    {
        "*-:)", MyRect(160, 153, 198, 189), QObject::tr("idea")
    }
    ,  //�뷨

    //	{":)",   MyRect(1,   2, 38,  39)},
    {
        ":-d", MyRect(40, 2, 78, 39), QObject::tr("yuk")
    }
    ,  //��Ц,
    {
        ";)", MyRect(80, 2, 118, 39), QObject::tr("blink")
    }
    ,  //գ��,
    {
        ":-o", MyRect(120, 2, 158, 39), QObject::tr("surprise")
    }
    ,  //����,
    {
        ":-p", MyRect(160, 2, 198, 39), QObject::tr("spit_tongue_smile")
    }
    ,  //����Ц��,
    {
        "(h)", MyRect(200, 2, 238, 39), QObject::tr("fiery_smile")
    }
    ,  //����Ц��,,
    {
        ":-@", MyRect(240, 2, 278, 39), QObject::tr("Angry")
    }
    ,  //����
    {
        ":(", MyRect(280, 2, 318, 39), QObject::tr("sadness")
    }
    ,  //����,


    {
        ":'(", MyRect(1, 40, 39, 77), QObject::tr("blubber")
    }
    ,  //����,
    {
        ":\"&gt;", MyRect(40, 40, 78, 77), QObject::tr("embarrass")
    }
    ,  //����
    {
        "^o)", MyRect(80, 40, 118, 77), QObject::tr("Ironically")
    }
    ,  //���,
    //{":&amp;",  MyRect(120, 40,  158, 77)},
    {
        ":&", MyRect(120, 40, 158, 77), QObject::tr("Sick")
    }
    ,  //��,
    {
        "8o|", MyRect(160, 40, 198, 77), QObject::tr("Yaoyaqiechi")
    }
    ,  //ҧ���г�
    {
        "|-)", MyRect(200, 40, 238, 77), QObject::tr("sleepy")
    }
    ,  //��
    {
        ":-#", MyRect(240, 40, 278, 77), QObject::tr("Privacy")
    }
    ,  //����
    {
        "8-)", MyRect(280, 40, 318, 77), QObject::tr("MomentisEyess")
    }
    ,  //ת�����۾�

    {
        "(s)", MyRect(1, 78, 39, 113), QObject::tr("sleepyMoon")
    }
    ,  //��˯������
    {
        "(st)", MyRect(40, 78, 78, 113), QObject::tr("rain")
    }
    ,  //����
    {
        "(o)", MyRect(80, 78, 118, 113), QObject::tr("clock")
    }
    ,  //ʱ��
    {
        "(l)", MyRect(120, 78, 158, 113), QObject::tr("hearts")
    }
    ,  //����
    {
        "(u)", MyRect(160, 78, 198, 113), QObject::tr("crackedhearts")
    }
    ,  //�������
    {
        "(@)", MyRect(200, 78, 238, 113), QObject::tr("catfaces")
    }
    ,  //è��
    {
        "(&)", MyRect(240, 78, 278, 113), QObject::tr("dogfaces")
    }
    ,  //����
    {
        "(sn)", MyRect(280, 78, 318, 113), QObject::tr("snail")
    }
    ,  //��ţ


    {
        "(*)", MyRect(1, 115, 39, 151), QObject::tr("star")
    }
    ,  //����
    {
        "(#)", MyRect(40, 115, 78, 151), QObject::tr("sun")
    }
    ,  //̫��
    {
        "(r)", MyRect(80, 115, 118, 151), QObject::tr("rainbow")
    }
    ,  //�ʺ�
    {
        "(})", MyRect(120, 115, 158, 151), QObject::tr("leftembrace")
    }
    ,  //��ӵ��,
    {
        "({)", MyRect(160, 115, 198, 151), QObject::tr("rightembrace")
    }
    ,  //��ӵ��,
    {
        "(k)", MyRect(200, 115, 238, 151), QObject::tr("lip")
    }
    ,  //�촽
    {
        "(f)", MyRect(240, 115, 278, 151), QObject::tr("rose")
    }
    ,  //��õ��
    {
        "(w)", MyRect(280, 115, 318, 151), QObject::tr("emarcidrose")
    }
    ,  //��л��õ��


    {
        "(g)", MyRect(1, 153, 39, 189), QObject::tr("giftbox")
    }
    ,  //��Ʒ��
    {
        "(^)", MyRect(40, 153, 78, 189), QObject::tr("Birthdaycake")
    }
    ,  //���յ���
    {
        "-8", MyRect(80, 153, 118, 189), QObject::tr("music")
    }
    ,  //����
    {
        "(i)", MyRect(120, 153, 158, 189), QObject::tr("bulb")
    }
    ,  //����,
    //	{"*-:)", MyRect(160, 153,  198, 189)},
    {
        "(c)", MyRect(200, 153, 238, 189), QObject::tr("coffee")
    }
    ,  //����
    {
        "(um)", MyRect(240, 153, 278, 189), QObject::tr("bumbershoot")
    }
    ,  //��ɡ
    {
        "b)", MyRect(280, 153, 318, 189), QObject::tr("withsunglass")
    }
    ,  //����̫����


    {
        "(mp)", MyRect(1, 191, 39, 227), QObject::tr("mobiletelephone")
    }
    ,  //�ֻ�
    {
        "(co)", MyRect(40, 191, 78, 227), QObject::tr("computer")
    }
    ,  //�����
    {
        ":-|", MyRect(80, 191, 118, 227), QObject::tr("despair")
    }
    ,  //ʧ��
    {
        ":-/", MyRect(120, 191, 158, 227), QObject::tr("confused")
    }
    ,  //�����
    {
        ":-s", MyRect(160, 191, 198, 227), QObject::tr("afraid")
    }
    ,  //����
    {
        ")-|", MyRect(200, 191, 238, 227), QObject::tr("drink")
    }
    ,  //����
    {
        "(d)", MyRect(240, 191, 278, 227), QObject::tr("goblet")
    }
    ,  //�߽ű�
    //{"o:)", MyRect(280, 191,  318, 227)},


    {
        ":-?", MyRect(1, 229, 39, 259), QObject::tr("meditation")
    }
    ,  //��˼
    {
        "(y)", MyRect(40, 229, 78, 259), QObject::tr("Praised")
    }
    ,  //̫����
    //{"&gt;:)",    MyRect(80,  229,  118, 259)},
    {
        ":-b", MyRect(120, 229, 158, 259), QObject::tr("spoony")
    }
    ,  //����

    {
        ":)", MyRect(1, 2, 38, 39), QObject::tr("smile")
    }
    ,  //΢Ц

};

bool FxInputFace::getFaces(int x, int y, QString &face, QString &str)
{
    int i;
    for (i = 0; i < sizeof(_faces_info) / sizeof(_faces_info[0]); i++)
    if (_faces_info[i].rect.contains(x, y))
    {
        face = _faces_info[i].face;
        str = _faces_info[i].str;
        return true;
    }

    face = "";
    str = "";
    return false;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxInputFace::FxInputFace(QWidget *parent): QDialog(parent)
{
    msgWind = NULL;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

FxInputFace::~FxInputFace(){

}

void FxInputFace::setMsgWindow(FxMsgWindow *Wind)
{
    msgWind = Wind;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

#ifdef Q_OS_MAC
    static bool shouldHide = false;
#endif

void FxInputFace::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    #ifdef Q_OS_MAC
        if (shouldHide)
        {
            shouldHide = false;
            hide();
            return ;
        }

        shouldHide = true;
    #else
        hide();
    #endif
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void FxInputFace::mouseReleaseEvent(QMouseEvent *event)
{
    #ifdef Q_OS_MAC
        shouldHide = false;
    #endif
    hide();

    //releaseMouse();

    if (!msgWind)
    {
        return ;
    }

    FxMyTabWidget *tabWidget = msgWind->tabWidget;
    if (!tabWidget)
    {
        return ;
    }

    AccountTab *accountTab = (AccountTab*)(tabWidget->widget(tabWidget
                              ->currentIndex()));
    if (!accountTab)
    {
        return ;
    }

    /*
    QString symbol;
    QString str;
    QString face = getFaces (event->x(), event->y());
     */

    QString face;
    QString str;
    if (!getFaces(event->x(), event->y(), face, str))
    {
        return ;
    }
    QString msg = accountTab->msgSend->MsgEdit->toPlainText();
    QString newmsg = msg + face;

    accountTab->msgSend->MsgEdit->setText(newmsg);
    accountTab->msgSend->MsgEdit->moveCursor(QTextCursor::End);
    accountTab->msgSend->MsgEdit->setFocus();
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


void FxInputFace::mouseMoveEvent(QMouseEvent *event)
{
    QString face;
    QString str;
    if (!getFaces(event->x(), event->y(), face, str))
    {
        return ;
    }

    QString text = str + "  " + face;
    QToolTip::showText(event->globalPos(), text, this);
}
