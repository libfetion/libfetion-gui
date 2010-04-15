/***************************************************************************
 *   Copyright (C) 2008 by DDD                                             *
 *   dedodong@163.com                                                      *
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
#ifndef FXWIDGETTITLEBAR_H
#define FXWIDGETTITLEBAR_H
#include <QtGui>
#include "fxdebug.h"

namespace fxgui
{

class FxWidget;

class FxWidgetTitleBar: public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        FxWidgetTitleBar(FxWidget* parent);

    public slots:
        void on_btnMinimize_clicked();
#if 0
        void on_btnMaximize_clicked(bool checked = false);
#endif
        void on_btnClose_clicked();

    public:
        //@TO FIX
        //   for convenience of showMaximized & showNormal
        QPushButton *btnMaximize;
        void setWindowTitle(const QString&title);
        void setWindowIcon(const QIcon&icon);
        void setMinimizetoHide(bool minimizetoHide)
        {
            _isminimizetoHide = minimizetoHide;
        }

    private:
        FxWidget *_parent;
        QLabel *title;
        QToolButton *icon;
        bool _isminimizetoHide;
};

}
#endif
