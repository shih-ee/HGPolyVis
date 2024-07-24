#pragma once

#include <QWidget>
#include <QColor>
#include <QString>
#include <string>
#include "ui_DockTextEdit.h"

class DockTextEdit : public QWidget
{
	Q_OBJECT

public:
	DockTextEdit(QWidget *parent = Q_NULLPTR);
	~DockTextEdit();

	void writeColorConsole(std::string msg, QColor color = QColor(Qt::black));

	void clearConsole();
	void saveConsole(QString filename);
	QString getConsolePlainText();

private:
	Ui::DockTextEdit ui;
};
