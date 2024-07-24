#pragma once

#include <QWidget>
#include <QString>
#include <QStringList> 

#include "ui_DockSelectElement.h"

class DockSelectElement : public QWidget
{
	Q_OBJECT

public:
	DockSelectElement(QWidget *parent = Q_NULLPTR);
	~DockSelectElement();

	void setContext(const char * msg, ...);
	void appendSeparator();

	void appendLine(const char * msg, ...);
	void appendLine(QString str);

	void appendColorLine(QColor color, const char * msg, ...);
	void appendColorLine(QColor color, QString str);
	void appendColorLine(QColor color, QStringList str);

	void clearConsole();
	void saveConsole(QString filename);

public:
	Ui::DockSelectElement ui;
};
