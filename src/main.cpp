#include "swarmvis.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SwarmVis w;
	w.show();
	return a.exec();
}
