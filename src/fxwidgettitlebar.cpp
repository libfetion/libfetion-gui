#include "fxwidgettitlebar.h"
#include "fxwidget.h"

namespace fxgui
{

FxWidgetTitleBar::FxWidgetTitleBar(FxWidget *parent) :
    QWidget(parent), _parent(parent)
{
    FX_FUNCTION
    // setup ui
    QHBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);

    _isminimizetoHide = false;
    //QPushButton *icon = new QPushButton();
    icon = new QToolButton(this);
    icon->setObjectName("icon");
    title = new QLabel();
    title->setObjectName("title");
    title->setSizePolicy(
            QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    title->resize(title->width(), 17);
    QPushButton *btnMinimize = new QPushButton();
    btnMinimize->setObjectName("btnMinimize");
    /*QPushButton **/
    btnMaximize = new QPushButton();
    btnMaximize->setObjectName("btnMaximize");
    btnMaximize->setCheckable(true);
    QPushButton *btnClose = new QPushButton();
    btnClose->setObjectName("btnClose");

    layout->addWidget(icon);
    layout->addWidget(title);
    layout->addWidget(btnMinimize);
    //layout->addWidget(btnMaximize);
    layout->addWidget(btnClose);

    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    QMetaObject::connectSlotsByName(this);
}

//slots on titlebar BEGIN

//void FxWidgetTitleBar::on_icon_clicked(){
//@ToBeFinished
// icon clicked,should show which menu?
//}
void FxWidgetTitleBar::setWindowTitle(const QString &text)
{
    FX_FUNCTION
    this->title->setText(text);
}
void FxWidgetTitleBar::on_btnMinimize_clicked()
{
    FX_FUNCTION
    if (!_isminimizetoHide)
    {
        _parent->showMinimized();
    }
    // or should be hide as some application presented?
    else
    {
        _parent->hide();
    }
    // or should be hide as some application presented?
}

#if 0
void FxWidgetTitleBar::on_btnMaximize_clicked(bool checked)
{
    FX_FUNCTION
    Q_UNUSED(checked); // avoid compiler's warnning
    //@TO BE FIXED make sure checked=maximized for css
    if (!checked)
    {
        _parent->showNormal();
    }
    else
    {
        _parent->showMaximized();
        //_parent->showFullScreen();
    }
}
#endif

void FxWidgetTitleBar::on_btnClose_clicked()
{
    FX_FUNCTION
    _parent->close();
}
void FxWidgetTitleBar::setWindowIcon(const QIcon &_icon)
{
    FX_FUNCTION
    icon->setIcon(_icon);
}
//slots on titlebar END

}
