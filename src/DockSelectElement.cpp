#include "DockSelectElement.h"
#include "LogConsole.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
//#include <tchar.h>

#include <QFileDialog>
#include <QTextStream>

DockSelectElement::DockSelectElement(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DockSelectElement::~DockSelectElement()
{
}

void DockSelectElement::setContext(const char * msg, ...)
{
	char text[10000];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	ui.textEdit->setText(text);
}

void DockSelectElement::appendSeparator()
{
	ui.textEdit->moveCursor(QTextCursor::End);
	ui.textEdit->insertHtml("<hr>");
	ui.textEdit->moveCursor(QTextCursor::Start);
}

void DockSelectElement::appendLine(const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	appendLine(QString(text));
}

void DockSelectElement::appendLine(QString str)
{
	ui.textEdit->append(str);
	ui.textEdit->update();
	ui.textEdit->moveCursor(QTextCursor::Start);
}

void DockSelectElement::appendColorLine(QColor color, const char * msg, ...)
{
	char text[1024];
	va_list args;
	va_start(args, msg);
	vsnprintf(text, sizeof(text) / sizeof(char), msg, args);
	text[1023] = '\0';
	QColor cc = ui.textEdit->textColor();
	ui.textEdit->setTextColor(color);
	appendLine(QString(text));
	ui.textEdit->setTextColor(cc);
}

void DockSelectElement::appendColorLine(QColor color, QString str)
{
	QColor cc = ui.textEdit->textColor();
	ui.textEdit->setTextColor(color);
	appendLine(str);
	ui.textEdit->setTextColor(cc);
}

void DockSelectElement::appendColorLine(QColor color, QStringList str)
{
	for (auto s : str) {
		appendColorLine(color, s);
	}
}


void DockSelectElement::clearConsole()
{
	ui.textEdit->clear();
}

void DockSelectElement::saveConsole(QString fileName)
{

	if (fileName != "") {
		QFile file(fileName);

		if (file.open(QIODevice::ReadWrite)) {
			QTextStream stream(&file);
			stream << ui.textEdit->toPlainText();
			file.flush();
			file.close();
		}
		else {
			LogConsole::showError("fail to save console");
			return;
		}
	}

	LogConsole::writeConsole("finished saving");
}
