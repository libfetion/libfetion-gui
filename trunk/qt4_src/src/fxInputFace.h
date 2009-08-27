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
