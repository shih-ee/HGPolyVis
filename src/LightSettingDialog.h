#pragma once

#include <QDialog>
#include "ui_LightSettingDialog.h"
#include "oglwidget.h"

class LightSettingDialog : public QDialog
{
	Q_OBJECT

public:
	LightSettingDialog(OGLWidget * vis);
	~LightSettingDialog();

	void syncUI();

	void setDoubleBoxValue(QDoubleSpinBox* box, double);
	void getDoubleBoxValue(QDoubleSpinBox* box, GLfloat &value);

public slots:
	
	void updateLight(double);

	void on_checkBox_UseLIght0_clicked(bool checked);
	void on_checkBox_UseLIght1_clicked(bool checked);
	void on_checkBox_Draw_Lights_clicked(bool checked);

	void on_doubleSpinBox_lightGlyphSize_valueChanged(double arg);
	void on_doubleSpinBox_CylinderShininess_valueChanged(double arg);
	void on_doubleSpinBox_SphereShininess_valueChanged(double arg);

	void on_pushButton_ResetLightSources_clicked();

private:
	OGLWidget *focusedVis;
	Ui::LightSettingDialog ui;
};
