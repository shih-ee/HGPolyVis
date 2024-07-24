#include "LogConsole.h"
//#include <tchar.h>

#include "DockTextEdit.h"
#include <QMessageBox>


LogConsole::LogConsole()
{
}


LogConsole::~LogConsole()
{
}

void LogConsole::writeConsole(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';

	gconsole->writeColorConsole(text);
}

void LogConsole::writeConsole(QString str)
{
	gconsole->writeColorConsole(str.toStdString().c_str());
}

/*Update INFO*/
void LogConsole::writeGreenConsole(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	QColor qcolor(15, 171, 39);
	gconsole->writeColorConsole(text, qcolor);
}

/*Bad INFO*/
void LogConsole::writeRedConsole(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	QColor qcolor(255, 0, 0);
	gconsole->writeColorConsole(text, qcolor);
}

void LogConsole::writeBlueSeparator()
{
	QColor qcolor(0, 0, 255);
	gconsole->writeColorConsole("-------------------", qcolor);
}

/*INFO*/
void LogConsole::writeBlueConsole(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	QColor qcolor(0, 0, 255);
	gconsole->writeColorConsole(text, qcolor);
}

void LogConsole::writeColorConsole(LOGCOLOR color, const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';

	QColor qcolor(color.R, color.G, color.B);
	gconsole->writeColorConsole(text, qcolor);
}

void LogConsole::writeColorConsole(QColor color, const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	gconsole->writeColorConsole(text, color);
}

/*color ranges from 0 - 255*/
void LogConsole::writeColorConsole(float R, float G, float B, const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	QColor qcolor;
	qcolor.setRgbF(R, G, B);
	gconsole->writeColorConsole(text, qcolor);
}

void LogConsole::writeError(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	QString status = QString(text);
	QString str = "[ERROR!]" + status;
	writeColorConsole(LOGCOLOR(255, 0, 0), str.toStdString().c_str());
}

void LogConsole::showError(const char * msg, ...) {
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	QString status = QString(text);
	QString str = "[ERROR!]" + status;
	writeColorConsole(LOGCOLOR(255, 0, 0), str.toStdString().c_str());
}

bool LogConsole::showConfirm(const char * msg, ...) {
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);

	QString status = QString(text);

	int ret = QMessageBox::warning(NULL, QObject::tr("Warning!"), status,
		QMessageBox::Ok | QMessageBox::Cancel);

	if (ret == QMessageBox::Cancel)
		return false;
	else
		return true;
}

void LogConsole::showWarning(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	QString status = QString(text);
	QString str = "[Warning!]" + status;
	writeColorConsole(LOGCOLOR(255, 125, 0), str.toStdString().c_str());
}
