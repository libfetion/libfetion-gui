#ifndef FXUSERTREE_H_
#define FXUSERTREE_H_

#include <QTreeWidget>
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
	/*
		if(previous){
			if( QPushButton *b1 = dynamic_cast<QPushButton*>(itemWidget(previous,0)) ){
				b1->setText("normal");
				previous->setSizeHint(0,normalSize);
			}
		}
		if(current){
			if( QPushButton *b1 = dynamic_cast<QPushButton*>(itemWidget(current,0)) ){
				b1->setText("normal");
				current->setSizeHint(0,clickedSize);
			}
		}
	*/
		if(previous){
			if(previous->parent()){// has a treewidgetitem as parent. not a folder
				//QPushButton *b1 = dynamic_cast<QPushButton*>(itemWidget(previous,0));
				//b1->setText("normal");
				previous->setSizeHint(0,normalSize);
			}
		}
		if(current){
			if(current->parent()){// has a treewidgetitem as parent. not a folder
				//QPushButton *b2 = dynamic_cast<QPushButton*>(itemWidget(current,0));
				//b2->setText("clicked");
				current->setSizeHint(0,clickedSize);
			}
		}
		updateGeometries();
	}
	void onItemClicked(QTreeWidgetItem * item,int column){

	}
};

}// end  namespace fxgui 
#endif

