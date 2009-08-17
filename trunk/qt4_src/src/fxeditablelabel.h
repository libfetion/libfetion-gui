#ifndef FXEDITABLELABEL_H_
#define FXEDITABLELABEL_H_

#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMessageBox>
namespace fxgui{// begin namesapce fxgui

/**********************************
@author: 
	iptton@gmail.com
@des:
	use stackedWidget to build an editable label.
	
	this stackedWidget has two pages: QLabel and QLineEdit
	one for edit and one for present
	
	this widget has two states:	normal state and editing state.
	the state control the pages's switch. once user *click* the label it switch to edit stage 
	and turn back to normal state as user press ENTER or the widget lose the focus
	both of them will emit a signal textChanged(QString)
	
************************************/

// just for an signal clicked()
// eat the  mousePressedEvent 
class FxLabel : public QLabel // inherit QLabel
{
    Q_OBJECT
public:
	FxLabel(QWidget* parent=0){
		FxLabel("",parent);
	}
    FxLabel(const QString & text,QWidget * parent = 0):QLabel(text,parent) {
	};
signals:
    void clicked();
protected:
    void mouseReleaseEvent(QMouseEvent *event) { emit clicked(); event->accept();}
};

class FxEditableLabel: public QStackedWidget{
    Q_OBJECT
public:
    FxEditableLabel(QString text,QWidget *parent=0):QStackedWidget(parent){
		init(text);
    }
	FxEditableLabel(QWidget *parent=0):QStackedWidget(parent){
		init();
	}
protected:
	void init(QString text=""){
		edit = new QLineEdit(text,this);
		edit->setObjectName("edit");
		label = new FxLabel(this);
		label->setObjectName("label");
		adjustLabelText(text);
		
		addWidget(label);
		addWidget(edit);
		setCurrentWidget(label);
		setCursor(Qt::PointingHandCursor);
		
		connect(label,SIGNAL(clicked()),this,SLOT(labelClicked()));
        connect(edit,SIGNAL(editingFinished()),this,SLOT(editFinish()));
		setFocusPolicy(Qt::StrongFocus);
	}
	void resizeEvent(QResizeEvent *event){
		adjustLabelText(edit->text());
		QStackedWidget::resizeEvent(event);
	}
	void adjustLabelText(QString _text){
		QString text = _text+_suffixText;
		if(text==""){
			label->setText("");
			label->setToolTip("");
			return;
		}
		QFontMetrics labelFontMetrics(label->font());
		QString tempText;

		if( labelFontMetrics.width(text) <= width() ){
			tempText = text;
			label->resize(labelFontMetrics.width(text),label->height());
		}else{
			int dotWidth = labelFontMetrics.width("...");
			int i=0;
			while( i<text.length() && labelFontMetrics.width(text,i) < width() - dotWidth){
				++i;
			}
			tempText = text.left(i-1)+"...";
		}
		label->setText(tempText);
		label->setToolTip(text);
	}
public:
	QString text()const{
		return edit->text();
	}
signals:
	void textChanged(QString);
public slots:
	void setSuffix(const QString& text){
		_suffixText = text;
		adjustLabelText(text);
	}
    void labelClicked(){
		// change to page edit.
		setCurrentWidget(edit);
		oldText = edit->text();
		edit->selectAll();
		edit->setFocus();
    }
    void setText(QString text){
		edit->setText(text);
		adjustLabelText(text);
    }
    void editFinish(){
		if(oldText != edit->text()){
			emit(textChanged(edit->text()));
		}
		adjustLabelText(edit->text());
		setCurrentWidget(label);
    }
	void setEditState(bool editState=true){
		if(editState)
			setCurrentWidget(edit);
		else
			setCurrentWidget(label);
	}
private:
    QLineEdit *edit;
    QLabel *label;
	QString oldText;
	QString _suffixText;
};
}// end  namespace fxgui 
#endif

