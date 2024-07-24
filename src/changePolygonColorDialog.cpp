#include "changePolygonColorDialog.h"
#include "Element.h"
#include <QGridLayout>
#include <QColorDialog>
#include <QFileDialog>
#include "mainwindow.h"
#include "LogConsole.h"
#include "ERData.h"
#include "FileIO.h"

changePolygonColorDialog::changePolygonColorDialog(ERData *erdata)
	: QDialog(NULL)
{
	ui.setupUi(this);
	mERData = erdata;

	saveColorsPushBtn = ui.pushButton_saveColorScheme;
	loadColorsPushBtn = ui.pushButton_LoadColorScheme;

	connect(saveColorsPushBtn, SIGNAL(clicked()), this, SLOT(save_Color_Scheme()));
	connect(loadColorsPushBtn, SIGNAL(clicked()), this, SLOT(load_Color_Scheme()));

	ColorButtons[0] = ui.pushButton_C_1;
	ColorButtons[1] = ui.pushButton_C_2;
	ColorButtons[2] = ui.pushButton_C_3;
	ColorButtons[3] = ui.pushButton_C_4;
	ColorButtons[4] = ui.pushButton_C_5;
	ColorButtons[5] = ui.pushButton_C_6;
	ColorButtons[6] = ui.pushButton_C_7;
	ColorButtons[7] = ui.pushButton_C_8;
	ColorButtons[8] = ui.pushButton_C_9;
	ColorButtons[9] = ui.pushButton_C_10;
	ColorButtons[10] = ui.pushButton_C_11;

	ColorSpinBoxes[0][0] = ui.spinBox_Color_R_1;		ColorSpinBoxes[0][1] = ui.spinBox_Color_G_1;		ColorSpinBoxes[0][2] = ui.spinBox_Color_B_1;		ColorSpinBoxes[0][3] = ui.spinBox_Color_A_1;
	ColorSpinBoxes[1][0] = ui.spinBox_Color_R_2;		ColorSpinBoxes[1][1] = ui.spinBox_Color_G_2;		ColorSpinBoxes[1][2] = ui.spinBox_Color_B_2;		ColorSpinBoxes[1][3] = ui.spinBox_Color_A_2;
	ColorSpinBoxes[2][0] = ui.spinBox_Color_R_3;		ColorSpinBoxes[2][1] = ui.spinBox_Color_G_3;		ColorSpinBoxes[2][2] = ui.spinBox_Color_B_3;		ColorSpinBoxes[2][3] = ui.spinBox_Color_A_3;
	ColorSpinBoxes[3][0] = ui.spinBox_Color_R_4;		ColorSpinBoxes[3][1] = ui.spinBox_Color_G_4;		ColorSpinBoxes[3][2] = ui.spinBox_Color_B_4;		ColorSpinBoxes[3][3] = ui.spinBox_Color_A_4;
	ColorSpinBoxes[4][0] = ui.spinBox_Color_R_5;		ColorSpinBoxes[4][1] = ui.spinBox_Color_G_5;		ColorSpinBoxes[4][2] = ui.spinBox_Color_B_5;		ColorSpinBoxes[4][3] = ui.spinBox_Color_A_5;
	ColorSpinBoxes[5][0] = ui.spinBox_Color_R_6;		ColorSpinBoxes[5][1] = ui.spinBox_Color_G_6;		ColorSpinBoxes[5][2] = ui.spinBox_Color_B_6;		ColorSpinBoxes[5][3] = ui.spinBox_Color_A_6;
	ColorSpinBoxes[6][0] = ui.spinBox_Color_R_7;		ColorSpinBoxes[6][1] = ui.spinBox_Color_G_7;		ColorSpinBoxes[6][2] = ui.spinBox_Color_B_7;		ColorSpinBoxes[6][3] = ui.spinBox_Color_A_7;
	ColorSpinBoxes[7][0] = ui.spinBox_Color_R_8;		ColorSpinBoxes[7][1] = ui.spinBox_Color_G_8;		ColorSpinBoxes[7][2] = ui.spinBox_Color_B_8;		ColorSpinBoxes[7][3] = ui.spinBox_Color_A_8;
	ColorSpinBoxes[8][0] = ui.spinBox_Color_R_9;		ColorSpinBoxes[8][1] = ui.spinBox_Color_G_9;		ColorSpinBoxes[8][2] = ui.spinBox_Color_B_9;		ColorSpinBoxes[8][3] = ui.spinBox_Color_A_9;
	ColorSpinBoxes[9][0] = ui.spinBox_Color_R_10;		ColorSpinBoxes[9][1] = ui.spinBox_Color_G_10;		ColorSpinBoxes[9][2] = ui.spinBox_Color_B_10;		ColorSpinBoxes[9][3] = ui.spinBox_Color_A_10;
	ColorSpinBoxes[10][0] = ui.spinBox_Color_R_11;		ColorSpinBoxes[10][1] = ui.spinBox_Color_G_11;		ColorSpinBoxes[10][2] = ui.spinBox_Color_B_11;		ColorSpinBoxes[10][3] = ui.spinBox_Color_A_11;

	for (int i = 0; i < COLORCOUNT; i++) {
		connect(ColorButtons[i], &QPushButton::clicked, this, &changePolygonColorDialog::pushButtonColorClicked);
		for(int j = 0; j<4; j++)
			connect(ColorSpinBoxes[i][j], SIGNAL(valueChanged(int)), this, SLOT(spinBoxColorChanged(int)));
	}

}

changePolygonColorDialog::~changePolygonColorDialog()
{
}

void changePolygonColorDialog::setUIValues(Color4_8BIT polyColor[COLORCOUNT])
{
	for (int i = 0; i < COLORCOUNT; i++) {
		ColorSpinBoxes[i][0]->setValue(polyColor[i].R);
		ColorSpinBoxes[i][1]->setValue(polyColor[i].G);
		ColorSpinBoxes[i][2]->setValue(polyColor[i].B);
		ColorSpinBoxes[i][3]->setValue(polyColor[i].Alpha);
	}

	setPushButtonColors(polyColor);
}

void changePolygonColorDialog::getUIValues(Color4_8BIT *polyColor)
{
	for (int i = 0; i < COLORCOUNT; i++) {
		polyColor[i] = Color4_8BIT(
			(double)ColorSpinBoxes[i][0]->value(),
			(double)ColorSpinBoxes[i][1]->value(),
			(double)ColorSpinBoxes[i][2]->value(),
			(double)ColorSpinBoxes[i][3]->value()
		);
	}
}

void changePolygonColorDialog::setPushButtonColors(Color4_8BIT polyColor[COLORCOUNT])
{
	for(int i = 0; i<COLORCOUNT; i++)
		ColorButtons[i]->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);").
			arg(polyColor[i].R).arg(polyColor[i].G).arg(polyColor[i].B).arg(polyColor[i].Alpha));
}

vector<int> changePolygonColorDialog::getSelectedColors()
{
	 vector<int> selectedColors;

	 if (ui.checkBox_ColorSelect_1->isChecked()) selectedColors.push_back(0);
	 if (ui.checkBox_ColorSelect_2->isChecked()) selectedColors.push_back(1);
	 if (ui.checkBox_ColorSelect_3->isChecked()) selectedColors.push_back(2);
	 if (ui.checkBox_ColorSelect_4->isChecked()) selectedColors.push_back(3);
	 if (ui.checkBox_ColorSelect_5->isChecked()) selectedColors.push_back(4);
	 if (ui.checkBox_ColorSelect_6->isChecked()) selectedColors.push_back(5);
	 if (ui.checkBox_ColorSelect_7->isChecked()) selectedColors.push_back(6);
	 if (ui.checkBox_ColorSelect_8->isChecked()) selectedColors.push_back(7);
	 if (ui.checkBox_ColorSelect_9->isChecked()) selectedColors.push_back(8);
	 if (ui.checkBox_ColorSelect_10->isChecked()) selectedColors.push_back(9);
	 if (ui.checkBox_ColorSelect_11->isChecked()) selectedColors.push_back(10);

	 return selectedColors;
}

void changePolygonColorDialog::on_buttonBox_accepted()
{
	on_pushButton_UpdateColors_clicked();
}

void changePolygonColorDialog::on_buttonBox_rejected()
{
}

void changePolygonColorDialog::on_pushButton_SetPolygonColorsAsSelected_clicked()
{

	vector<int> selectedColor = getSelectedColors();
	if (selectedColor.size() != 1) {
		LogConsole::showError("please pick one color");
		return;
	}

	int selected = selectedColor[0];

	int R = ColorSpinBoxes[selected][0]->value();
	int G = ColorSpinBoxes[selected][1]->value();
	int B = ColorSpinBoxes[selected][2]->value();
	int A = ColorSpinBoxes[selected][3]->value();

	for (int i = 0; i < COLORCOUNT; i++) {
		ColorSpinBoxes[i][0]->setValue(R);
		ColorSpinBoxes[i][1]->setValue(G);
		ColorSpinBoxes[i][2]->setValue(B);
		ColorSpinBoxes[i][3]->setValue(A);
	}
}

void changePolygonColorDialog::on_pushButton_UpdateColors_clicked()
{
	getUIValues(Element::polyColor);
	mERData->assignRelationshipColors();
	gmainwindow->updateWidgets();
}

void changePolygonColorDialog::on_pushButton_SwapSelectedColors_clicked()
{
	vector<int> selectedColor = getSelectedColors();
	if (selectedColor.size() != 2) {
		LogConsole::showError("please pick two color");
		return;
	}

	int selected1 = selectedColor[0];
	int selected2 = selectedColor[1];

	int R1 = ColorSpinBoxes[selected1][0]->value();
	int G1 = ColorSpinBoxes[selected1][1]->value();
	int B1 = ColorSpinBoxes[selected1][2]->value();
	int A1 = ColorSpinBoxes[selected1][3]->value();

	int R2 = ColorSpinBoxes[selected2][0]->value();
	int G2 = ColorSpinBoxes[selected2][1]->value();
	int B2 = ColorSpinBoxes[selected2][2]->value();
	int A2 = ColorSpinBoxes[selected2][3]->value();

	ColorSpinBoxes[selected1][0]->setValue(R2);
	ColorSpinBoxes[selected1][1]->setValue(G2);
	ColorSpinBoxes[selected1][2]->setValue(B2);
	ColorSpinBoxes[selected1][3]->setValue(A2);

	ColorSpinBoxes[selected2][0]->setValue(R1);
	ColorSpinBoxes[selected2][1]->setValue(G1);
	ColorSpinBoxes[selected2][2]->setValue(B1);
	ColorSpinBoxes[selected2][3]->setValue(A1);

}

void changePolygonColorDialog::on_pushButton_OutputColorInfo_clicked()
{

	vector<Color4_8BIT> colors;
	for (int i = 0; i < COLORCOUNT; i++) {

		int R = ColorSpinBoxes[i][0]->value();
		int G = ColorSpinBoxes[i][1]->value();
		int B = ColorSpinBoxes[i][2]->value();
		int A = ColorSpinBoxes[i][3]->value();

		colors.push_back(Color4_8BIT(R, G, B, A));

		QColor color = QColor(R, G, B, A);
		LogConsole::writeColorConsole(color, "RGBA(%d, %d, %d, %d)", R, G, B, A);

		int H, S, V, alpha;
		color.getHsv(&H, &S, &V, &alpha);
		LogConsole::writeColorConsole(color, "HSVA(%d, %d, %d, %d)", H, S, V, alpha);

		int hsl_H, hsl_S, hsl_L, hsl_A;
		color.getHsl(&hsl_H, &hsl_S, &hsl_L, &hsl_A);
		LogConsole::writeColorConsole(color, "HSLA(%d, %d, %d, %d)", hsl_H, hsl_S, hsl_L, hsl_A);
	}
}

void changePolygonColorDialog::on_pushButton_addToColorPalette_clicked()
{
}

void changePolygonColorDialog::on_pushButton_resetToDefaultColors_clicked()
{
	Element::initPolyColors(ColorBrewer2);
	setUIValues(Element::polyColor);
	gmainwindow->updateWidgets();
}


void changePolygonColorDialog::pushButtonColorClicked()
{
	QPushButton* buttonSender = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked

	int btnId = -1;
	for (int i = 0; i < COLORCOUNT; i++) {
		if (buttonSender == ColorButtons[i]) {
			btnId = i;
			break;
		}
	}

	if (btnId != -1) {
		int br, bg, bb, ba;
		QColor curColor = buttonSender->palette().color(QPalette::Background);
		curColor.getRgb(&br, &bg, &bb, &ba);

		QColorDialog cd(QColor(br, bg, bb, ba), this);
		cd.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
		int dialogCode = cd.exec();

		if (dialogCode == QDialog::Accepted) {
			QColor col = cd.currentColor();
			col.getRgb(&br, &bg, &bb, &ba);

			ColorSpinBoxes[btnId][0]->setValue(br);
			ColorSpinBoxes[btnId][1]->setValue(bg);
			ColorSpinBoxes[btnId][2]->setValue(bb);
			ColorSpinBoxes[btnId][3]->setValue(ba);
		}
	}
}

void changePolygonColorDialog::spinBoxColorChanged(int arg1)
{
	QSpinBox* spinBox = qobject_cast<QSpinBox*>(sender()); // retrieve the button you have clicked
	
	int spinBoxId = -1;
	int spinBoxComp = -1;
	for (int i = 0; i < COLORCOUNT; i++) {
		for (int j = 0; j < 4; j++) {
			if (spinBox == ColorSpinBoxes[i][j])
			{
				spinBoxId = i;
				spinBoxComp = j;
				break;
			}
		}

		if (spinBoxId != -1 && spinBoxComp != -1) {
			break;
		}
	}

	int R = ColorSpinBoxes[spinBoxId][0]->value();
	int G = ColorSpinBoxes[spinBoxId][1]->value();
	int B = ColorSpinBoxes[spinBoxId][2]->value();
	int A = ColorSpinBoxes[spinBoxId][3]->value();

	ColorButtons[spinBoxId]->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
		.arg(R).arg(G).arg(B).arg(A));

}

void changePolygonColorDialog::save_Color_Scheme()
{
	QString filter = "Polygon Colors Scheme (*.PolyColors)";

	QString pathrelative = QString::fromStdString(gmainwindow->m_PATH);
	QString pathOftext = QFileDialog::getSaveFileName(this, "Save Color Scheme to a txt...", pathrelative, filter);

	string str = pathOftext.toStdString();

	if (!str.size())
		return;

	vector<Color4_8BIT> colors;
	for (int i = 0; i < COLORCOUNT; i++) {
		int R = ColorSpinBoxes[i][0]->value();
		int G = ColorSpinBoxes[i][1]->value();
		int B = ColorSpinBoxes[i][2]->value();
		int A = ColorSpinBoxes[i][3]->value();

		colors.push_back(Vec4(R,G,B,A));
	}

	/*write to file*/
	FileIO::writePolygonColorsFile(str.c_str(), colors);

	LogConsole::writeConsole("Energies are saved");
}

void changePolygonColorDialog::load_Color_Scheme()
{
	QString filters("Polygon Colors Scheme (*.PolyColors)");
	QString defaultFilter("Polygon Colors Scheme (*.PolyColors)");

	QString pathrelative = QString::fromStdString(gmainwindow->m_PATH);
	QString fileName = QFileDialog::getOpenFileName(0, "Open Color Scheme file", pathrelative, filters, &defaultFilter);

	if (fileName.isEmpty())
		return;

	string str = fileName.toStdString();
	const char* path = str.c_str();

	vector<Color4_8BIT> colors = FileIO::readPolygonColorsFile(path);
	if (colors.size() != COLORCOUNT) {
		LogConsole::showError("Incorrect colors number");
	}
	else {

		for (int i = 0; i < colors.size(); i++) {
			ColorSpinBoxes[i][0]->setValue(colors[i].R);
			ColorSpinBoxes[i][1]->setValue(colors[i].G);
			ColorSpinBoxes[i][2]->setValue(colors[i].B);
			ColorSpinBoxes[i][3]->setValue(colors[i].Alpha);
		}
	}

}