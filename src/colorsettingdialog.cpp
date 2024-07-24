#include "colorsettingdialog.h"
#include "ui_colorsettingdialog.h"
#include <QColorDialog>

ColorSettingDialog::ColorSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorSettingDialog)
{
    ui->setupUi(this);

	Color4_F color = Color4_F(0, 0, 0, 1.0);

	initUI(color);
}

ColorSettingDialog::~ColorSettingDialog()
{
    delete ui;
}

void ColorSettingDialog::initUI(Color4_F color)
{
	ui->doubleSpinBox_r->setValue(color.R);
	ui->doubleSpinBox_g->setValue(color.G);
	ui->doubleSpinBox_b->setValue(color.B);
	ui->doubleSpinBox_a->setValue(color.Alpha);

	ui->pushButton_ChangeColor->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);").
		arg(color.R*255).arg(color.G * 255).arg(color.B * 255).arg(color.Alpha * 255));
}

void ColorSettingDialog::on_buttonBox_accepted()
{
    flag_accepted = true;
    R_channel = ui->doubleSpinBox_r->value();
    G_channel = ui->doubleSpinBox_g->value();
    B_channel = ui->doubleSpinBox_b->value();
    A_channel = ui->doubleSpinBox_a->value();
}

void ColorSettingDialog::on_pushButton_ChangeColor_clicked()
{
	double br, bg, bb, ba;

	br = ui->doubleSpinBox_r->value();
	bg = ui->doubleSpinBox_g->value();
	bb = ui->doubleSpinBox_b->value();
	ba = ui->doubleSpinBox_a->value();

	QColorDialog cd(QColor(br*255, bg * 255, bb * 255, ba * 255), this);
	cd.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	int dialogCode = cd.exec();

	if (dialogCode == QDialog::Accepted) {
		QColor col = cd.currentColor();
		col.getRgbF(&br, &bg, &bb, &ba);

		ui->doubleSpinBox_r->setValue(br);
		ui->doubleSpinBox_g->setValue(bg);
		ui->doubleSpinBox_b->setValue(bb);
		ui->doubleSpinBox_a->setValue(ba);

		ui->pushButton_ChangeColor->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);").
			arg(br * 255).arg(bg * 255).arg(bb * 255).arg(ba * 255));
	}
}

void ColorSettingDialog::on_ColorSettingDialog_destroyed()
{
    flag_accepted = false;
}
