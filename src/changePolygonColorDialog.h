#pragma once

#include <QDialog>
#include <QPushButton>
#include <QSpinBox>

#include "ColorPalette.h"
#include "JPMath/Vector4.h"
#include "ui_changePolygonColorDialog.h"

class ERData;

class changePolygonColorDialog : public QDialog
{
	Q_OBJECT

public:
	changePolygonColorDialog(ERData *erdata);
	~changePolygonColorDialog();

	void setUIValues(Color4_8BIT polyColor[COLORCOUNT]);
	void getUIValues(Color4_8BIT* polyColor);
	
	void setPushButtonColors(Color4_8BIT polyColor[COLORCOUNT]);

	QPushButton *saveColorsPushBtn;
	QPushButton *loadColorsPushBtn;

	vector<int> getSelectedColors();
	

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

	void on_pushButton_SetPolygonColorsAsSelected_clicked();

	void on_pushButton_UpdateColors_clicked();

	void on_pushButton_resetToDefaultColors_clicked();

	void on_pushButton_SwapSelectedColors_clicked();

	void on_pushButton_OutputColorInfo_clicked();

	void on_pushButton_addToColorPalette_clicked();

	void pushButtonColorClicked();
	void spinBoxColorChanged(int arg1);

	void load_Color_Scheme();
	void save_Color_Scheme();


private:
	ERData *mERData = NULL;
	QSpinBox *ColorSpinBoxes[COLORCOUNT][4];
	QPushButton *ColorButtons[COLORCOUNT];
	Ui::changePolygonColorDialog ui;
};
