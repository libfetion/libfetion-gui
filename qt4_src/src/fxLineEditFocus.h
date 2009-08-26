#ifndef FXLINEEDITFOCUS_H
#define FXLINEEDITFOCUS_H

#include <QLineEdit>
#include "fxutil.h"

class QLineEditFocus : public QLineEdit // inherit QLineEdit
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        QLineEditFocus(QWidget *parent) : QLineEdit(parent) { };

    signals:
        void infocus();
        void outfocus();
        void clicked();

    protected:
        void mousePressEvent ( QMouseEvent * event )
        {
            //	QLineEdit::mousePressEvent (event);
            emit clicked();
            Q_UNUSED(event);
            return;
        }

        void focusInEvent ( QFocusEvent * event )
        {
            QLineEdit::focusInEvent (event);
            emit infocus();
         }

        void focusOutEvent ( QFocusEvent * event)
        {
            QLineEdit::focusOutEvent (event);
            emit outfocus();
        }
};
#endif


