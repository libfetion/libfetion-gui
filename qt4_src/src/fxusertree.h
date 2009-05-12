#ifndef FXUSERTREE_H_
#define FXUSERTREE_H_

#include <QTreeWidget>
//#include "fxusertreeitem.h"
namespace fxgui{// begin namesapce fxgui

class FxUserTree: public QTreeWidget
{
Q_OBJECT
 
public:
	FxUserTree(QWidget *parent = 0):QTreeWidget(parent){
		normalSize = QSize(30,20);
		clickedSize = QSize(70,40);
		connect(this,SIGNAL(itemClicked(QTreeWidgetItem *, int)),
				this,SLOT(onItemClicked(QTreeWidgetItem *, int)));
		connect(this,SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem *)),
				this,SLOT(onCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
	}

 
private:
	QSize clickedSize;
	QSize normalSize;
 
private slots:
	void onCurrentItemChanged ( QTreeWidgetItem* current, QTreeWidgetItem * previous ){
		if(previous){
			if(previous->parent()){
				previous->setSizeHint(0,normalSize);
			}
		}
		if(current){
			if(current->parent()){
				current->setSizeHint(0,clickedSize);
			}
		}
		updateGeometries();
	}
	void onItemClicked(QTreeWidgetItem * /*item*/,int /*column*/){
	}
};

}// end  namespace fxgui 
#endif

