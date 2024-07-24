#include "helpdialog.h"
#include "ui_helpdialog.h"
#include <QFile>
#include "LogConsole.h"
#include <QTextStream>

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

	QString fileName = "../README.md";
	if (!fileName.isEmpty()) {
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			LogConsole::writeError("Could not open the user manual %s", fileName.toStdString().c_str());
			return;
		}
		QTextStream in(&file);
		ui->textEdit->setText(in.readAll());
		file.close();
	}
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
