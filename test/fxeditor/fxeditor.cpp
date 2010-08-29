#include "fxeditor.h"
#include <QRegExp>
#include <QDebug>

// this editor will eat your newline(/n)

FxEditor::FxEditor(QWidget*parent):QTextEdit(parent)
{
	// the code bellow's generalted by my javascript code
	// so has not andy readibility.
	// use registerSymbol may be better
	// and for multi protocol's support , the list should load dynamic(maybe from an XML file?)
	// for more correct , the first element of my pair should be reg expression.
	//        for example ;) when user print &) it will be &amp;) and present as &amp<IMAGE>
    //        to make sure if the protocal escape the & to &amp; and the > to &gt; and so on. then modify them.

	strToHtmlList<<(mypair("&gt;:)","<img src=\"%path%/50.gif\" />")); 
	strToHtmlList<<(mypair("o:)","<img src=\"%path%/47.gif\" />"));
	strToHtmlList<<(mypair("*-:)","<img src=\"%path%/37.gif\" />")); 
	strToHtmlList<<(mypair(":-d","<img src=\"%path%/2.gif\" />")); 
	strToHtmlList<<(mypair(":-o","<img src=\"%path%/4.gif\" />")); 
	strToHtmlList<<(mypair(":-p","<img src=\"%path%/5.gif\" />")); 
	strToHtmlList<<(mypair("(h)","<img src=\"%path%/6.gif\" />")); 
	strToHtmlList<<(mypair(":-@","<img src=\"%path%/7.gif\" />")); 
	strToHtmlList<<(mypair(":(","<img src=\"%path%/8.gif\" />")); 
	strToHtmlList<<(mypair(":'(","<img src=\"%path%/9.gif\" />")); 
	strToHtmlList<<(mypair(":\"&gt;","<img src=\"%path%/10.gif\" />"));
	strToHtmlList<<(mypair("^o)","<img src=\"%path%/11.gif\" />")); 
	strToHtmlList<<(mypair(":&","<img src=\"%path%/12.gif\" />")); 
	strToHtmlList<<(mypair("8o|","<img src=\"%path%/13.gif\" />")); 
	strToHtmlList<<(mypair("|-)","<img src=\"%path%/14.gif\" />")); 
	strToHtmlList<<(mypair(":-#","<img src=\"%path%/15.gif\" />")); 
	strToHtmlList<<(mypair("8-)","<img src=\"%path%/16.gif\" />")); 
	strToHtmlList<<(mypair("(s)","<img src=\"%path%/17.gif\" />")); 
	strToHtmlList<<(mypair("(st)","<img src=\"%path%/18.gif\" />")); 
	strToHtmlList<<(mypair("(o)","<img src=\"%path%/19.gif\" />")); 
	strToHtmlList<<(mypair("(l)","<img src=\"%path%/20.gif\" />")); 
	strToHtmlList<<(mypair("(u)","<img src=\"%path%/21.gif\" />")); 
	strToHtmlList<<(mypair("(@)","<img src=\"%path%/22.gif\" />")); 
	strToHtmlList<<(mypair("(&)","<img src=\"%path%/23.gif\" />"));
	strToHtmlList<<(mypair("(sn)","<img src=\"%path%/24.gif\" />")); 
	strToHtmlList<<(mypair("(*)","<img src=\"%path%/25.gif\" />")); 
	strToHtmlList<<(mypair("(#)","<img src=\"%path%/26.gif\" />")); 
	strToHtmlList<<(mypair("(r)","<img src=\"%path%/27.gif\" />")); 
	strToHtmlList<<(mypair("(})","<img src=\"%path%/28.gif\" />")); 
	strToHtmlList<<(mypair("({)","<img src=\"%path%/29.gif\" />")); 
	strToHtmlList<<(mypair("(k)","<img src=\"%path%/30.gif\" />")); 
	strToHtmlList<<(mypair("(f)","<img src=\"%path%/31.gif\" />")); 
	strToHtmlList<<(mypair("(w)","<img src=\"%path%/32.gif\" />")); 
	strToHtmlList<<(mypair("(g)","<img src=\"%path%/33.gif\" />")); 
	strToHtmlList<<(mypair("(^)","<img src=\"%path%/34.gif\" />")); 
	strToHtmlList<<(mypair("-8","<img src=\"%path%/35.gif\" />")); 
	strToHtmlList<<(mypair("(i)","<img src=\"%path%/36.gif\" />")); 
	strToHtmlList<<(mypair("(c)","<img src=\"%path%/38.gif\" />")); 
	strToHtmlList<<(mypair("(um)","<img src=\"%path%/39.gif\" />")); 
	strToHtmlList<<(mypair("(mp)","<img src=\"%path%/40.gif\" />")); 
	strToHtmlList<<(mypair("(co)","<img src=\"%path%/41.gif\" />")); 
	strToHtmlList<<(mypair(":-|","<img src=\"%path%/42.gif\" />")); 
	strToHtmlList<<(mypair(":-/","<img src=\"%path%/43.gif\" />")); 
	strToHtmlList<<(mypair(":-s","<img src=\"%path%/44.gif\" />")); 
	strToHtmlList<<(mypair(")-|","<img src=\"%path%/45.gif\" />")); 
	strToHtmlList<<(mypair("(d)","<img src=\"%path%/46.gif\" />")); 
	strToHtmlList<<(mypair(":-?","<img src=\"%path%/48.gif\" />")); 
	strToHtmlList<<(mypair("(y)","<img src=\"%path%/49.gif\" />")); 
	strToHtmlList<<(mypair(":-b","<img src=\"%path%/51.gif\" />")); 
	strToHtmlList<<(mypair("b)","<img src=\"%path%/52.gif\" />")); 
	strToHtmlList<<(mypair(";)","<img src=\"%path%/3.gif\" />")); 
	strToHtmlList<<(mypair(":)","<img src=\"%path%/1.gif\" />"));
	
    connect(this,SIGNAL(textChanged()),this,SLOT(onTextChanged()));
}
void FxEditor::setImagePath(const QString& path ){
	_imagePath = path;
}
QString FxEditor::toPlainText()const
{
    QString oristr = QTextEdit::toHtml();
    //qDebug()<<oristr;
    // replace all <img ..> to str
	QList<mypair>::ConstIterator i = strToHtmlList.constBegin();
	while( i!= strToHtmlList.end() ){
		mypair k = *i;
		oristr.replace(k.second.replace("%path%",_imagePath),k.first);
		++i;
	}
	/*
    QList<QString, QString>::const_iterator i = strToHtml.constBegin();
    while (i != strToHtml.end()) {
        QString k = *i;
        oristr.replace(k.replace("%path%",_imagePath),i.key());
        ++i;
    }
	*/
    // replace all html tags
    // remove <!Do..
    oristr.remove(QRegExp("<!(^>)+>"));
    // remove stylesheet tag and its content
    oristr.remove(QRegExp("<[^>]+css\">[^<]*</style>"));
    // remove other tags
	oristr.replace(QRegExp("</p[^>]*>"),"<br />");
    oristr.remove(QRegExp("<(?!(br))[^>]+>"));
	oristr.replace(QString("<br />"), QString("\n"));
	oristr.replace(QString("<br>"), QString("\n"));
    oristr.replace("&lt;","<");
    oristr.replace("&gt;",">");
	oristr.replace("&amp;","&");
	oristr.remove(oristr.size()-1,1);

    // remove the newline
    oristr.remove(0,2);
    //qDebug()<<"\n orient plainText return :"<<QTextEdit::toPlainText()<<"\n";
    //return QTextEdit::toPlainText();
    return oristr;
}
void FxEditor::onTextChanged()
{
    static bool changeByMe = false;
    if(changeByMe)
        return;

	
	//=======================
	QString orihtml = QTextEdit::toHtml();
	orihtml.remove(QRegExp("<![^>]+>"));
    // remove stylesheet tag and its content
    orihtml.remove(QRegExp("<[^>]+css\">[^<]*</style>"));
    // remove other tags except <img>
	//qDebug()<<orihtml;
	orihtml.replace(QRegExp("</p[^>]*>"),"<br />");
    orihtml.remove(QRegExp("<(?!(img|br))[^>]+>"));
    // remove the newline
    orihtml.remove(0,2);
    orihtml.replace("&lt;","<");
    //orihtml.replace("&gt;",">");
	orihtml.replace("&amp;","&");
	orihtml.remove(orihtml.size()-6,6);
	//qDebug()<<"orihtml"<<orihtml;
	//=======================
	
	/*
	// how to get current row?
	int column = 0;//this->textCursor().columnNumber();
	if(orihtml.indexOf("<br />",brPosition,Qt::CaseInsensitive) == -1){
		lastBrPosition = -6;
		brPosition = len+1;
	}else{
		while(brPosition<len){
			qDebug()<<"br";
			lastBrPosition = brPosition;
			brPosition = orihtml.indexOf("<br />",brPosition,Qt::CaseInsensitive);
			if(brPosition == -1){
				break;
			}
			if(column == 0){
				break;
			}
			column--;
			brPosition +=6;
		}
	}*/
	//int newCursorPosition = this->textCursor().position();
	//if(this->textCursor().columnNumber() == currentColumn){ // not at this column there's not neccesary to recaculate cursor position
		//int newCursorPosition = recaculateCursor(orihtml,lastBrPosition+6,brPosition-1);
	//}
	int newCursorPosition = recaculateCursor(orihtml,0,orihtml.size()-1);

	
	QList<mypair>::ConstIterator i = strToHtmlList.constBegin();
	while( i!=strToHtmlList.end() ){
        QString k = i->second;
		//qDebug()<<"THE HTML:"<<orihtml;
        orihtml.replace(i->first,k.replace("%path%",_imagePath),Qt::CaseInsensitive);
        ++i;
	}
    //qDebug()<<QString("position:%1").arg(newCursorPosition);


    changeByMe = true;
    setHtml(orihtml);
    QTextCursor c(this->document());
	//qDebug()<<"------- position:"<<c.position();
    c.setPosition(newCursorPosition);
	//qDebug()<<"-------new position:"<<newCursorPosition;
    this->setTextCursor(c);
    changeByMe = false;
    qDebug()<<"\nplaintText:"<<toPlainText()<<"\n";
}

int FxEditor::recaculateCursor(const QString& orihtml,int begin,int end){
    int newCursorPosition = this->textCursor().position();
	int imageTagLength = 0;
	QList<mypair>::ConstIterator i = strToHtmlList.constBegin();
	/*
	qDebug()<<begin<<"==>"<<end;
	qDebug()<<this->textCursor().columnNumber()<<"====";
	while( i!=strToHtmlList.end() ){
		int foundPosition = begin;
		int len = end;
		QString _k = i->second;
		_k = _k.replace("%path%",_imagePath);
		while(foundPosition<len){
			//qDebug()<<"image";
			foundPosition = orihtml.indexOf(_k,foundPosition,Qt::CaseInsensitive);
			if(foundPosition==-1){
				break;
			}
			imageTagLength += _k.size()-1;
			foundPosition += _k.size();
		}
		++i;
	}*/
	

	i = strToHtmlList.constBegin();
	while( i!= strToHtmlList.end() ){
        //caculate new cursor position
		// a litter bug.
        int foundPosition = begin;
		int len = end;
		// recaculate the new cursor;
		while(foundPosition<len){
			//qDebug()<<"reca";
			foundPosition = orihtml.indexOf(i->first,foundPosition,Qt::CaseInsensitive);
			if(foundPosition==-1){
				break;
			}
			/*qDebug()<<"dsfdf: "<<orihtml.left(foundPosition+1);
			if( i->first.startsWith(';') && orihtml.left(foundPosition-7).contains("&amp;")){
				// HACK!!!!  for the user's input  &) will display as &amp<SMILE IMAGE :)>
				qDebug()<<":)";
				foundPosition++;
				break;
			}*/
			
			// caculate all tag before the symbol as "one" ( because only tag <img> <br /> and <p> and so on can  remained)
			QRegExp regexp("<[^>]+>");
			int _pos = 0;
			QString _s = orihtml.left(foundPosition);
			imageTagLength = 0;
			while( regexp.indexIn(_s,_pos) != -1 ){
				imageTagLength += regexp.matchedLength() - 1;
				_pos += regexp.matchedLength();
			}
			
			//qDebug()<<foundPosition<<" "<<imageTagLength<<" "<<newCursorPosition;
			if(foundPosition+i->first.size()-imageTagLength >= newCursorPosition){
				if( (i->first.indexOf("&lt;")!=-1) || (i->first.indexOf("&gt;")!=-1)){
					newCursorPosition+=3;
				}
				int _temp = foundPosition-imageTagLength+i->first.size() - newCursorPosition;
				if(_temp>0){
					//qDebug()<<"IN!";
					newCursorPosition+=_temp;
				}
				//qDebug()<<i->first<<" size "<<i->first.length();
				newCursorPosition -= i->first.size()-1;
				

				/*
				if(i->first.indexOf("&amp;")!=-1){
					qDebug()<<"&amp; found!"<<newCursorPosition;
					newCursorPosition+=4;
				}*/
			}
			foundPosition+=i->first.size();
		}
		++i;
	}
	return newCursorPosition;
}
