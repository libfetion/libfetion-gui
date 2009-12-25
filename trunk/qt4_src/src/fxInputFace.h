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
#ifndef _FXINPUTFACE_H
#define _FXINPUTFACE_H

#include <QDialog>

#include "fxglobal.h"
#include "fxdebug.h"

class FxMsgWindow;

class FxInputFace: public QDialog
{
    public:
        /* FIXME: need re-write here */
        static QString parseSmileySymbol(QString newmsg){
            newmsg.replace("&gt;:)", "<img src='" + FxFacePath() + "/50.gif'>");
            newmsg.replace("o:)", "<img src='" + FxFacePath() + "/47.gif'>");
            newmsg.replace("O:)", "<img src='" + FxFacePath() + "/47.gif'>");
            newmsg.replace("*-:)", "<img src='" + FxFacePath() + "/37.gif'>");

            newmsg.replace(":-d", "<img src='" + FxFacePath() + "/2.gif'>");
            newmsg.replace(":-D", "<img src='" + FxFacePath() + "/2.gif'>");
            newmsg.replace(":-o", "<img src='" + FxFacePath() + "/4.gif'>");
            newmsg.replace(":-O", "<img src='" + FxFacePath() + "/4.gif'>");
            newmsg.replace(":-p", "<img src='" + FxFacePath() + "/5.gif'>");
            newmsg.replace(":-P", "<img src='" + FxFacePath() + "/5.gif'>");
            newmsg.replace("(h)", "<img src='" + FxFacePath() + "/6.gif'>");
            newmsg.replace("(H)", "<img src='" + FxFacePath() + "/6.gif'>");
            newmsg.replace(":-@", "<img src='" + FxFacePath() + "/7.gif'>");
            newmsg.replace(":(", "<img src='" + FxFacePath() + "/8.gif'>");
            newmsg.replace(":'(", "<img src='" + FxFacePath() + "/9.gif'>");
            newmsg.replace(":\"&gt;", "<img src='" + FxFacePath() + "/10.gif'>");
            newmsg.replace("^o)", "<img src='" + FxFacePath() + "/11.gif'>");
            newmsg.replace("^O)", "<img src='" + FxFacePath() + "/11.gif'>");

            newmsg.replace(":&", "<img src='" + FxFacePath() + "/12.gif'>");

            newmsg.replace("8o|", "<img src='" + FxFacePath() + "/13.gif'>");
            newmsg.replace("8O|", "<img src='" + FxFacePath() + "/13.gif'>");
            newmsg.replace("|-)", "<img src='" + FxFacePath() + "/14.gif'>");
            newmsg.replace(":-#", "<img src='" + FxFacePath() + "/15.gif'>");
            newmsg.replace("8-)", "<img src='" + FxFacePath() + "/16.gif'>");
            newmsg.replace("(s)", "<img src='" + FxFacePath() + "/17.gif'>");
            newmsg.replace("(S)", "<img src='" + FxFacePath() + "/17.gif'>");
            newmsg.replace("(st)", "<img src='" + FxFacePath() + "/18.gif'>");
            newmsg.replace("(ST)", "<img src='" + FxFacePath() + "/18.gif'>");
            newmsg.replace("(o)", "<img src='" + FxFacePath() + "/19.gif'>");
            newmsg.replace("(O)", "<img src='" + FxFacePath() + "/19.gif'>");
            newmsg.replace("(l)", "<img src='" + FxFacePath() + "/20.gif'>");
            newmsg.replace("(L)", "<img src='" + FxFacePath() + "/20.gif'>");
            newmsg.replace("(u)", "<img src='" + FxFacePath() + "/21.gif'>");
            newmsg.replace("(U)", "<img src='" + FxFacePath() + "/21.gif'>");
            newmsg.replace("(@)", "<img src='" + FxFacePath() + "/22.gif'>");
            newmsg.replace("(&)", "<img src='" + FxFacePath() + "/23.gif'>");
            newmsg.replace("(sn)", "<img src='" + FxFacePath() + "/24.gif'>");
            newmsg.replace("(SN)", "<img src='" + FxFacePath() + "/24.gif'>");
            newmsg.replace("(*)", "<img src='" + FxFacePath() + "/25.gif'>");
            newmsg.replace("(#)", "<img src='" + FxFacePath() + "/26.gif'>");
            newmsg.replace("(r)", "<img src='" + FxFacePath() + "/27.gif'>");
            newmsg.replace("(R)", "<img src='" + FxFacePath() + "/27.gif'>");
            newmsg.replace("(})", "<img src='" + FxFacePath() + "/28.gif'>");
            newmsg.replace("({)", "<img src='" + FxFacePath() + "/29.gif'>");
            newmsg.replace("(k)", "<img src='" + FxFacePath() + "/30.gif'>");
            newmsg.replace("(K)", "<img src='" + FxFacePath() + "/30.gif'>");
            newmsg.replace("(f)", "<img src='" + FxFacePath() + "/31.gif'>");
            newmsg.replace("(F)", "<img src='" + FxFacePath() + "/31.gif'>");
            newmsg.replace("(w)", "<img src='" + FxFacePath() + "/32.gif'>");
            newmsg.replace("(W)", "<img src='" + FxFacePath() + "/32.gif'>");
            newmsg.replace("(g)", "<img src='" + FxFacePath() + "/33.gif'>");
            newmsg.replace("(G)", "<img src='" + FxFacePath() + "/33.gif'>");
            newmsg.replace("(^)", "<img src='" + FxFacePath() + "/34.gif'>");
            newmsg.replace("-8", "<img src='" + FxFacePath() + "/35.gif'>");
            newmsg.replace("(i)", "<img src='" + FxFacePath() + "/36.gif'>");
            newmsg.replace("(I)", "<img src='" + FxFacePath() + "/36.gif'>");
            newmsg.replace("(c)", "<img src='" + FxFacePath() + "/38.gif'>");
            newmsg.replace("(C)", "<img src='" + FxFacePath() + "/38.gif'>");
            newmsg.replace("(um)", "<img src='" + FxFacePath() + "/39.gif'>");
            newmsg.replace("(UM)", "<img src='" + FxFacePath() + "/39.gif'>");
            newmsg.replace("(mp)", "<img src='" + FxFacePath() + "/40.gif'>");
            newmsg.replace("(MP)", "<img src='" + FxFacePath() + "/40.gif'>");
            newmsg.replace("(co)", "<img src='" + FxFacePath() + "/41.gif'>");
            newmsg.replace("(CO)", "<img src='" + FxFacePath() + "/41.gif'>");
            newmsg.replace(":-|", "<img src='" + FxFacePath() + "/42.gif'>");
            newmsg.replace(":-/", "<img src='" + FxFacePath() + "/43.gif'>");
            newmsg.replace(":-s", "<img src='" + FxFacePath() + "/44.gif'>");
            newmsg.replace(":-S", "<img src='" + FxFacePath() + "/44.gif'>");
            newmsg.replace(")-|", "<img src='" + FxFacePath() + "/45.gif'>");
            newmsg.replace("(d)", "<img src='" + FxFacePath() + "/46.gif'>");
            newmsg.replace("(D)", "<img src='" + FxFacePath() + "/46.gif'>");
            newmsg.replace(":-?", "<img src='" + FxFacePath() + "/48.gif'>");
            newmsg.replace("(y)", "<img src='" + FxFacePath() + "/49.gif'>");
            newmsg.replace("(Y)", "<img src='" + FxFacePath() + "/49.gif'>");
            newmsg.replace(":-b", "<img src='" + FxFacePath() + "/51.gif'>");
            newmsg.replace(":-B", "<img src='" + FxFacePath() + "/51.gif'>");
            newmsg.replace("b)", "<img src='" + FxFacePath() + "/52.gif'>");
            newmsg.replace("B)", "<img src='" + FxFacePath() + "/52.gif'>");

            newmsg.replace(";)", "<img src='" + FxFacePath() + "/3.gif'>");
            newmsg.replace(":)", "<img src='" + FxFacePath() + "/1.gif'>");

            return newmsg;
            };

    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
    public:
        FxInputFace(QWidget *parent = 0);
        ~FxInputFace();
        void setMsgWindow(FxMsgWindow *msgWind);

    protected:
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent * event);
        void focusOutEvent(QFocusEvent * event);
    private slots:

    private:
        FxMsgWindow *msgWind;
        bool getFaces(int x, int y, QString &face, QString &str);
};

class MyRect
{
    public:
        MyRect(int x, int y, int width, int buttom)
        {
            m_x = x;
            m_y = y;
            m_width = width;
            m_buttom = buttom;
        }
        ;
        bool contains(int x, int y)
        {
            if (x > m_x && y > m_y && x < m_width && y < m_buttom)
                return true;
            else
                return false;
        }
    private:
        int m_x, m_y, m_width, m_buttom;
};

typedef struct _FACES_INFO
{
        QString face;
        class MyRect rect;
        QString str;
} FACES_INFO;

#endif
