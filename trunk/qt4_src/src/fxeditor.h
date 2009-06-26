#ifndef FXEDITOR_H 
#define FXEDITOR_H 
 
/******************************* 
the class FxEditor should be refactoring  
maybe should design as the commented declare function and member in this file 
 
*******************************/ 
 
#include <QTextEdit> 
namespace fxgui{
typedef QPair<QString,QString> mypair; 
class FxEditor : public QTextEdit 
{ 
    Q_OBJECT 
public: 
    FxEditor(QWidget*parent = 0); 
    QString toPlainText () const; 
	// for farther feature: permit user to edit rich text such as <i> <b> and so on. 
	//QString toHtml() const; 
public slots: 
    void onTextChanged(); 
public: 
	void setImagePath(const QString& path); 
	//QString imagePath() const; 
	 
private: 
 
	int recaculateCursor(const QString& orihtml,int begin,int end); 
/* 
	int registerSymbol(const QString& symbol,const QString& html); 
	QString translateSymbol(const QString& text); 
	QString revertToSymbol(const QString& text)£» 
*/ 
    // do not use map 
    // because it sort the key by char order 
    //QMap<QString,QString> strToHtml; 
    QList<mypair> strToHtmlList; 
	// the image's path 
	QString _imagePath; 
}; 
 
}// namespace fxgui
#endif // FXEDITOR_H
