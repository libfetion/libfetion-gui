#ifndef FXWIDGETTITLEBAR_H
#define FXWIDGETTITLEBAR_H
#include <QtGui>
namespace fxgui{

    class FxWidget;

    class FxWidgetTitleBar:public QWidget{
        Q_OBJECT
        public:
            FxWidgetTitleBar(FxWidget* parent);

        public slots:
            void on_btnMinimize_clicked();
            void on_btnMaximize_clicked(bool checked = false);
            void on_btnClose_clicked();

        public:
            //@TO FIX
            //   for convenience of showMaximized & showNormal
            QPushButton *btnMaximize;
            void setWindowTitle(const QString&title);
            void setWindowIcon(const QIcon&icon);
            void setMinimizetoHide(bool minimizetoHide) {_isminimizetoHide = minimizetoHide;}

        private:
            FxWidget *_parent;
            QLabel *title;
            QToolButton *icon;
            bool _isminimizetoHide;
    };

}
#endif
