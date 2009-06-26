#include <QApplication>
#include "fxeditor.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>


class Temp:public QObject{
	Q_OBJECT
public:
	Temp(FxEditor* editor){
		this->editor = editor;
	}
public slots:
	void addFaceToEditor(){
		editor->textCursor().insertText(":)");
	}
private:
	FxEditor* editor;
};

int main(int argc,char** argv){
	QApplication app(argc,argv);
	QWidget w;
	
	FxEditor fxe;
	fxe.setImagePath("./faces");
	
	Temp tmp(&fxe);
	
	QPushButton btn(":)");
	QObject::connect(&btn,SIGNAL(clicked()),&tmp,SLOT(addFaceToEditor()));
	btn.setFocusProxy(&fxe);
	
	QHBoxLayout layout(&w);
	layout.addWidget(&fxe);
	layout.addWidget(&btn);
	
	w.show();
	return app.exec();
}

#include "main.moc"