#include "mainwindow.h"
#include "oglwidget.h"
#include <iostream>
#include <QApplication>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
#ifdef USE_OPENMP
	int num_cores = omp_get_num_procs();
	omp_set_num_threads(num_cores-1);
#endif

	QApplication app(argc, argv);

	MainWindow w;
	w.show();

    return app.exec();
}
