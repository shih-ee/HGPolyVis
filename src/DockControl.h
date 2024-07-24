#pragma once

#include <QWidget>

#include "ui_DockControl.h"
#include "globalVariables.h"

class OGLWidget;

class DockControl : public QWidget
{
	Q_OBJECT

public:
	DockControl(OGLWidget *vis1, OGLWidget *vis2);
	~DockControl();

	void resizeEvent(QResizeEvent* event);
	void initializeColorSchemeComboBox();
	void syncUIs();

public slots:

	void HLBFGSLinesearchITERMAXChanged(int val);

	void HLBFGSUpdateNumChanged(int val);

	void on_spinBox_MaximumPolygonAreaVerticesCount_valueChanged(int arg1);

	void energyDivisionThresholdChanged(double val);

	void digonWeightChanged(double val);

	void digonScaleChanged(double val);

	void on_doubleSpinBox_PS_EXPECT_GAMMA_valueChanged(double val);

	void on_doubleSpinBox_CircleRadiusRatio_valueChanged(double arg);

	void on_doubleSpinBox_ExpectedLength_valueChanged(double arg1);

	void on_doubleSpinBox_OCBufferScale_valueChanged(double arg1);

	void nsBufferScaleChanged(double arg1);

	void multipleDigonAngleOffsetChanged(double arg1);

	void toggleOptimizationButtons(bool res);

	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////

	// Layout Tab
	void on_comboBox_LayoutSelector_currentIndexChanged(int index);


public:
	/*expose ui*/
	Ui::DockControl *ui;

private:
	OGLWidget *primeVis;
	OGLWidget *dualVis;
};
