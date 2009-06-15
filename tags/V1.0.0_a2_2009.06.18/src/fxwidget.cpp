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
#include "fxwidgettitlebar.h"
#include "fxwidget.h"
namespace fxgui{

FxWidget::FxWidget(QWidget *parent,Qt::WindowFlags flag):QWidget(parent,flag){

	bgScaleLeft=65;
	bgScaleRight=130;
	bgScaleBottom=58;
	bgScaleTop=135;
	
	_autoHide = false;
	_enableautoHide = false;
	_isSetSystemTitleBar = false;

	// for "editable label"
	setFocusPolicy(Qt::ClickFocus);
	
	//resizer
	QWidgetResizeHandler *qwrh = new QWidgetResizeHandler(this);
	Q_UNUSED(qwrh);
	/*
	_______________________________________________________
	| _____________________________________________ | ___ |
	| |              titleBar                     | | |s| |
	| | icon |   title      | min | max | close | | | |i| |
	| |___________________________________________| | |d| |
	| ____________________________________________  | |e| |
	| |                                           | | |B| |
	| |	                                          | | |a| |
	| |              contentWidget                | | |r| |
	| |	                                          | | |R| |
	| |	                                          | | |L| |
	| |___________________________________________| | |_| |
	|_______________________________________________|_____|
	|  ___________________________________________  |     |
	| |______________sideBarTB___________________|  |     |
	|_______________________________________________|_____|
	
	*/
	
	_mainLayout = new QGridLayout(this);
	contentWidget = new QWidget(this);
	
	titleBar = new FxWidgetTitleBar(this);
	connect(this,SIGNAL(triggleMaximizeAndNormal()),titleBar->btnMaximize,SLOT(click()));
	_mainLayout->addWidget(titleBar,0,0);
	_mainLayout->addWidget(contentWidget,1,0);
	
	sideBarRL = new QLabel(this);
	sideBarRL->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Expanding));
	sideBarRL->setMaximumWidth(3);//@To FIX  magic number
	sideBarTB = new QLabel(this);
	sideBarTB->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored));
	//sideBarRL->setMaximumHeight(3);//@To FIX  magic number
	sideBarTB->setObjectName("sideBarTB");
	sideBarRL->setObjectName("sideBarRL");
	_mainLayout->addWidget(sideBarRL,0,0,2,1);
	_mainLayout->addWidget(sideBarTB,2,0);
	sideBarRL->hide();
	sideBarTB->hide();
	
	_mainLayout->setSpacing(0);
	_mainLayout->setMargin(0);
	setContentsMargins(3,0,3,3);
	QWidget::setLayout(_mainLayout);
	orientSize = size();
	updateWindowPositionType();	
}

void FxWidget::setSystemTitleBar(bool flag)
{
	if (_isSetSystemTitleBar == flag)
	{	
		setBackground(backgroundPixmap);
		return;
	}	

	_isSetSystemTitleBar = flag;
	if (flag){
		setWindowFlags(windowFlags() ^ Qt::FramelessWindowHint);
		setWindowFlags(windowFlags() ^ Qt::WindowSystemMenuHint);

		enableAutoHide(false);
		titleBar->hide();
		sideBarRL->hide();
		sideBarTB->hide();
	} else {
		//setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
		//setWindowFlags(windowFlags() | Qt::WindowSystemMenuHint);
		/** I don't know why need to do as following... */
		setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint); 

		enableAutoHide(true);
		titleBar->show();
		sideBarRL->show();
		sideBarTB->show();
	}

	if (isVisible())
		showNormal();
	setBackground(backgroundPixmap);
}

void FxWidget::setMinimizetoHide(bool minimizetoHide){
	if (titleBar)
		titleBar->setMinimizetoHide(minimizetoHide); 
}

void FxWidget::setLayout(QLayout *layout){
	contentWidget->setLayout(layout);
}
void FxWidget::setWindowTitle(const QString &title){
	titleBar->setWindowTitle(title);
	QWidget::setWindowTitle(title);
}
void FxWidget::setFont(const QFont & font){
	titleBar->setFont(font); 
	QWidget::setFont(font); 
}
void FxWidget::setWindowIcon(const QIcon & icon){
	titleBar->setWindowIcon(icon);
	QWidget::setWindowIcon(icon);
}
void FxWidget::resizeEvent(QResizeEvent *event){
	QWidget::resizeEvent(event);
	Q_UNUSED(event);
	if( !backgroundPixmap.isNull() )
		setBackground(backgroundPixmap);
}
void FxWidget::mouseDoubleClickEvent(QMouseEvent *event){
	Q_UNUSED(event);
	emit triggleMaximizeAndNormal();
}

void FxWidget::setBackground(QPixmap pix){

	if (_isSetSystemTitleBar)
	{
    this->setPalette(QApplication::palette());
		return;
	}

	if(pix.isNull())
		return;
	QPalette palette;
    backgroundPixmap = pix;
    QImage image = pix.toImage();
    int left=bgScaleLeft,right=bgScaleRight,bottom=bgScaleBottom,top=bgScaleTop;
    QPixmap bgTop(width(),height());
    QPainter painter(&bgTop);
    // top
    painter.drawImage(0,0,image,0,0,left,top);
    painter.drawImage(left,0,image.copy(left,0,image.width()-left-right,image.height()).scaled(width()-left-right,image.height()),0,0,width()-left-right,top);
    painter.drawImage(width()-right,0,image,image.width()-right,0,right,top);

    //middle
    QImage image2=image.copy(0,top,image.width(),image.height()-top-bottom).scaled(image.width(),height()-top-bottom);
    painter.drawImage(0,top,image2,0,0,left,height()-top-bottom);
    painter.drawImage(left,top,image2.copy(left,0,image.width()-left-right,image2.height()).scaled(width()-left-right,height()-top-bottom),0,0,width()-left-right,height()-top-bottom);
    painter.drawImage(width()-right,top,image2,image2.width()-right,0,right,height()-bottom-top);
 
    //bottom
    int bottomSrcY=image.height()-bottom;
    int bottomDesY=height()-bottom;
    painter.drawImage(0,bottomDesY,image,0,bottomSrcY,left,bottom);
    painter.drawImage(left,bottomDesY,image.copy(left,0,image.width()-right-left,image.height()).scaled(width()-left-right,image.height()),0,bottomSrcY,width()-left-right,bottom);
    painter.drawImage(width()-right,bottomDesY,image,image.width()-right,bottomSrcY,right,bottom);

    palette.setBrush(this->backgroundRole(),QBrush(bgTop));
    this->setPalette(palette);
}

void FxWidget::turnBackNormal(){
	titleBar->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
	contentWidget->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
	titleBar->show();
	contentWidget->show();
	sideBarRL->hide();
	sideBarTB->hide();
}

void FxWidget::hideToTopBottom(){
	titleBar->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
	contentWidget->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
	titleBar->hide();
	contentWidget->hide();
	sideBarRL->hide();
	sideBarTB->show();
}
void FxWidget::hideToLeftRight(){
	titleBar->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
	contentWidget->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
	titleBar->hide();
	contentWidget->hide();
	sideBarTB->hide();
	sideBarRL->show();
}
void FxWidget::onDoubleClicked(bool checked){
//@TO BE FIXED make sure checked=maximized for css
	if(!checked){
		this->showNormal();
	}else{
		this->showFullScreen();
	}
	Q_UNUSED(checked); // avoid compiler's warnning

}
void FxWidget::updateWindowPositionType(){
	int S=3;// should be const..

	//@TO FIX
	//   在需要自动隐藏的位置最小化则，恢复时不会自动隐藏(因为只有leaveEvent才会触发隐藏(需要在恢复窗口时也判断)
	// seems fixed

	positionState = WP_NORMAL;
	if(x()-S <= 0){
		positionState = WP_LEFT;
		return;
	}
	if( x()+S >= ( (QApplication::desktop()->width()) - width() ) ){
		positionState = WP_RIGHT;
		return;
	}
	if( y()-S <= 0){
		positionState = WP_TOP;
		return;
	}
	if( y()+S >= ( (QApplication::desktop()->height())-height() ) )
		positionState = WP_BOTTOM;
}

void FxWidget::moveEvent(QMoveEvent *event){
	Q_UNUSED(event);
	if(!_autoHide){
		return;
	}
	updateWindowPositionType();
}
void FxWidget::leaveEvent(QEvent* event){
	Q_UNUSED(event);
    beginAutoHide();
}

void FxWidget::enterEvent(QEvent*){
    endAutoHide();
}

void FxWidget::beginAutoHide(){
	// the magic number 5 should be a const value --by iptton
	//if(positionState == WP_NORMAL)return;
	//setWindowFlags(Qt::FramelessWindowHint);
	//centralwidget->hide();
	//menubar->hide();
	if(!_autoHide || isMaximized())
		return;

	updateWindowPositionType();
	switch(positionState){
		case WP_NORMAL:return;break;
		case WP_LEFT:
			move(0,y());
			orientSize = size();
			positionState |= WP_HIDDEN;
			hideToLeftRight();
			//setMask(QRegion(0,0,3,height()));
			resize(5,height());
			break;
		case WP_RIGHT:
			//setMask(QRegion(0,0,3,height()));
			orientSize = size();
			positionState |= WP_HIDDEN;
			hideToLeftRight();
			resize(5,height());
			move(QApplication::desktop()->width()-8,y());
			break;
		case WP_TOP:
			move(x(),0);
			orientSize = size();
			positionState |= WP_HIDDEN;
			//setMask(QRegion(0,0,width(),3));
			hideToTopBottom();
			resize(width(),5);
			break;
		case WP_BOTTOM:
			move(x(),QApplication::desktop()->height()-5);
			orientSize = size();
			positionState |= WP_HIDDEN;
			//setMask(QRegion(0,0,width(),3));
			hideToTopBottom();
			resize(width(),5);
			break;
		default:break;
	}
}
void FxWidget::endAutoHide(){
	if(!_autoHide || isMaximized())
		return;
	if( positionState & WP_NORMAL )
		return;
	//setWindowFlags(Qt::Window);
	//centralwidget->show();
	//menubar->show();
	positionState ^= int(WP_HIDDEN);
	//resize(orientWidth,orientHeight);
	switch(positionState){
		case WP_LEFT:
			resize(orientSize);
			turnBackNormal();
			break;
		case WP_RIGHT:
			resize(orientSize);
			move(QApplication::desktop()->width()-orientSize.width(),y());
			turnBackNormal();
			break;
		case WP_TOP:
			resize(orientSize);
			turnBackNormal();
			break;
		case WP_BOTTOM:
			resize(orientSize);
			move(x(),QApplication::desktop()->height()-orientSize.height());
			turnBackNormal();
			break;
		default:break;
	}
}



}// namespace fxgui
