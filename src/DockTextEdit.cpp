#include "DockTextEdit.h"
#include "LogConsole.h"
#include <QFileDialog>
#include <QTextStream>

DockTextEdit::DockTextEdit(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

DockTextEdit::~DockTextEdit()
{
}


void DockTextEdit::writeColorConsole(std::string msg, QColor color)
{
	ui.textEdit->setTextColor(color);
	ui.textEdit->append(QString::fromStdString(msg));
	ui.textEdit->update();
	ui.textEdit->moveCursor(QTextCursor::End);
}

void DockTextEdit::clearConsole()
{
	ui.textEdit->clear();
}

void DockTextEdit::saveConsole(QString fileName)
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

QString DockTextEdit::getConsolePlainText()
{
	return ui.textEdit->toPlainText();
}
