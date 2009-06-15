#ifndef FXLINEEDITFOCUS_H
#define FXLINEEDITFOCUS_H

#include <QLineEdit>
class QLineEditFocus : public QLineEdit // inherit QLineEdit
{
    Q_OBJECT
public:
    QLineEditFocus(QWidget *parent) : QLineEdit(parent) { };

signals:
    void infocus();
    void outfocus();
    void clicked();

protected:
//    void mouseReleaseEvent(QMouseEvent *) { emit clicked(); 
void    mousePressEvent ( QMouseEvent * event ) 
//	void mousePressEvent ()(QMouseEvent *) 
	{ 
		//	QLineEdit::mousePressEvent (event); 
		emit clicked(); 
		Q_UNUSED(event);
		return;
	}

	void focusInEvent ( QFocusEvent * event )
	{ 
		QLineEdit::focusInEvent (event); 
#if 0
		if (this->text() == tr("please input impresa...") 
			|| this->text() == tr("search friends...")
			) 
			this->setText("");
#endif
		emit infocus();
	 }

	void focusOutEvent ( QFocusEvent * event) 
	{
		QLineEdit::focusOutEvent (event); 
		emit outfocus();
	}
};
#endif


