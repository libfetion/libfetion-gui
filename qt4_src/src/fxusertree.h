#ifndef FXUSERTREE_H_
#define FXUSERTREE_H_

#include <QTreeWidget>
#include "fxutil.h"

namespace fxgui
{// begin namesapce fxgui

class FxUserTree: public QTreeWidget
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        FxUserTree(QWidget *parent = 0) :
            QTreeWidget(parent)
        {
            normalSize = QSize(30, 16);
            clickedSize = QSize(70, 40);
        }

    private:
        QSize clickedSize;
        QSize normalSize;

    private slots:
        void onCurrentItemChanged(QTreeWidgetItem* current,
                QTreeWidgetItem * previous)
        {
            if (previous)
            {
                if (previous->parent())
                {
                    previous->setSizeHint(0, normalSize);
                }
            }
            if (current)
            {
                if (current->parent())
                {
                    current->setSizeHint(0, clickedSize);
                }
            }
            updateGeometries();
        }

        void onItemClicked(QTreeWidgetItem * /*item*/, int /*column*/)
        {
        }
};

}
#endif

