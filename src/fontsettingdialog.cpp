#include "fontsettingdialog.h"
#include "ui_fontsettingdialog.h"
#include "oglwidget.h"

FontSettingDialog::FontSettingDialog(OGLWidget *vis1, OGLWidget *vis2) :
    ui(new Ui::FontSettingDialog), primeVis(vis1), dualVis(vis2)
{
    ui->setupUi(this);
	ui->radioButton_PrimeVis->setChecked(true);
	updateUIs(primeVis);
}

FontSettingDialog::~FontSettingDialog()
{
    delete ui;
}

void FontSettingDialog::on_spinBox_EfontSize_valueChanged(int arg1)
{
	getCurrentVis()->EfontSize = arg1;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_spinBox_RfontSize_valueChanged(int arg1)
{
	getCurrentVis()->RfontSize = arg1;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_spinBox_CfontSize_valueChanged(int arg1)
{
	getCurrentVis()->CfontSize = arg1;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_doubleSpinBox_EFontScale_valueChanged(double arg)
{
	getCurrentVis()->EfontScale = arg;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_buttonBox_accepted()
{
	this->close();
}

void FontSettingDialog::on_checkBox_RFont_Bold_clicked(bool checked)
{
	getCurrentVis()->RfontBold = checked;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_checkBox_EFont_Bold_clicked(bool checked)
{
	getCurrentVis()->EfontBold = checked;
	getCurrentVis()->repaint();

}

void FontSettingDialog::on_checkBox_CFont_Bold_clicked(bool checked)
{
	getCurrentVis()->CfontBold = checked;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_checkBox_BlackELabel_clicked(bool checked)
{
	getCurrentVis()->blackELable = checked;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_checkBox_BlackRLabel_clicked(bool checked)
{
	getCurrentVis()->blackRLable = checked;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_checkBox_CenterELabel_clicked(bool checked)
{
	getCurrentVis()->centerELabel = checked;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_checkBox_AutoResizeELabels_clicked(bool checked)
{
	getCurrentVis()->autoresizeELabel = checked;
	getCurrentVis()->repaint();
}

void FontSettingDialog::on_radioButton_PrimeVis_clicked()
{
	updateUIs(primeVis);
}

void FontSettingDialog::on_radioButton_DualVis_clicked()
{
	updateUIs(dualVis);
}

OGLWidget * FontSettingDialog::getCurrentVis()
{
	if (ui->radioButton_PrimeVis->isChecked())
		return primeVis;
	else if (ui->radioButton_DualVis->isChecked())
		return dualVis;
	else
		return bipartieVis;
}

void FontSettingDialog::updateUIs(OGLWidget * vis)
{
	ui->spinBox_RfontSize->setValue(vis->RfontSize);
	ui->doubleSpinBox_EFontScale->setValue(vis->EfontScale);
	ui->spinBox_EfontSize->setValue(vis->EfontSize);
	ui->spinBox_CfontSize->setValue(vis->CfontSize);

	ui->checkBox_RFont_Bold->setChecked(vis->RfontBold);
	ui->checkBox_EFont_Bold->setChecked(vis->EfontBold);
	ui->checkBox_CFont_Bold->setChecked(vis->CfontBold);

	ui->checkBox_BlackELabel->setChecked(vis->blackELable);
	ui->checkBox_BlackRLabel->setChecked(vis->blackRLable);

	ui->checkBox_AutoResizeELabels->setChecked(vis->autoresizeELabel);
	ui->checkBox_CenterELabel->setChecked(vis->centerELabel);

}
