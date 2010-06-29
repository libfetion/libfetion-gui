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
#ifndef FXWIDGET_H
#define FXWIDGET_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QWidget>
#include <QGridLayout>
#include <QLayout>
#include <QLabel>
#include <QResizeEvent>

#include <QtGui>
#include <QDebug>
#include "qwidgetresizehandler_p.h"

#include "fxglobal.h"
#include "fxdebug.h"

namespace fxgui
{
class FxWidgetTitleBar;
class FxWidget: public QWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    Q_PROPERTY(QPixmap background READ getBackground WRITE setBackground);
    Q_PROPERTY(int scaleLeft READ getBgScaleLeft WRITE setBgScaleLeft);
    Q_PROPERTY(int scaleRight READ getBgScaleRight WRITE setBgScaleRight);
    Q_PROPERTY(int scaleBottom READ getBgScaleBottom WRITE setBgScaleBottom);
    Q_PROPERTY(int scaleTop READ getBgScaleTop WRITE setBgScaleTop);
    //Q_PROPERTY(QPixmap hiddenBar WRITE setHiddenBar)
    public:
        FxWidget(QWidget *parent = 0,
                Qt::WindowFlags flag = (Qt::Window |
                           Qt::FramelessWindowHint |
                           Qt::WindowSystemMenuHint));
    public slots:
        void beginAutoHide();
        void endAutoHide();

    signals:
        void triggleMaximizeAndNormal();

    protected:
        void resizeEvent(QResizeEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
        void enterEvent(QEvent *event);
        void leaveEvent(QEvent *event);
        void moveEvent(QMoveEvent *event);
        void updateWindowPositionType();

    private:
        QPushButton *btnMaximize;
        QGridLayout *_mainLayout;
        QLabel *sideBarRL; // sideBar right
        QLabel *sideBarTB; // sideBar bottom
        int bgScaleLeft;
        int bgScaleRight;
        int bgScaleTop;
        int bgScaleBottom;
        QPixmap backgroundPixmap;
        //QPixmap hiddenBarPixmap;
        bool _autoHide;
        bool _enableautoHide;
        enum WINDOW_POS
        {
            WP_NORMAL = 0,
            WP_LEFT = 1,
            WP_RIGHT = 2,
            WP_TOP = 4,
            WP_BOTTOM = 8,
            WP_HIDDEN = 16,
        };
        int positionState;
        QSize orientSize;
        bool _isSetSystemTitleBar;

    public:
        void setSystemTitleBar(bool flag = true);
        void setMinimizetoHide(bool minimizetoHide = false);
        void setBackground(QPixmap pix);
        QPixmap getBackground() { return backgroundPixmap; }

        int getBgScaleLeft() { return bgScaleLeft; }
        void setBgScaleLeft(int p)
        {
            bgScaleLeft = p;
        }

        int getBgScaleRight() { return bgScaleRight;}
        void setBgScaleRight(int p)
        {
            bgScaleRight = p;
        }
        int getBgScaleBottom() { return bgScaleBottom; }
        void setBgScaleBottom(int p)
        {
            bgScaleBottom = p;
        }
        int getBgScaleTop() { return bgScaleTop; }
        void setBgScaleTop(int p)
        {
            bgScaleTop = p;
        }
        void setLayout(QLayout*);
        void setWindowTitle(const QString & title);

        void setTitleBarIcon(const QIcon & icon);

        bool isAutoHide() const
        {
            return _autoHide;
        }

    public:
        FxWidgetTitleBar *titleBar;
        QWidget *contentWidget;
    public slots:
#ifdef Q_OS_MAC
        //on mac platform, we didn't support autohide function
        void enableAutoHide(bool flag=true)
	{
		Q_UNUSED(flag);
	}
        void setAutoHide(bool autoHide=true)
	{
		Q_UNUSED(autoHide);
	}
#else
        void enableAutoHide(bool flag = true)
        {
            _enableautoHide = flag;
            _autoHide = flag;
        }

        void setAutoHide(bool autoHide = true)
        {
            if (!_enableautoHide) return;
            _autoHide = autoHide;
            if (_autoHide)
            {
                setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
                if (!isVisible())
                {
                    show();
                }
                updateWindowPositionType();
            }
        }
#endif
        void turnBackNormal();
        void hideToTopBottom();
        void hideToLeftRight();
        void onDoubleClicked(bool checked);
};//class FxWidget

}// namespace fxgui
#endif
