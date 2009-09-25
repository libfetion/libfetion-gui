#include <QApplication>
#include "fxeditor.h"
int main(int argc,char** argv){
	QApplication app(argc,argv);
	FxEditor fxe;
#ifdef WIN32
	fxe.setImagePath("./faces");
#else
	fxe.setImagePath("./debug/faces");
#endif
	fxe.show();
	return app.exec();
}
