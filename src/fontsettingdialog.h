#ifndef FONTSETTINGDIALOG_H
#define FONTSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class FontSettingDialog;
}

class OGLWidget;

class FontSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontSettingDialog(OGLWidget *primevis, OGLWidget *dualvis);
    ~FontSettingDialog();

private slots:
    void on_spinBox_EfontSize_valueChanged(int arg1);

    void on_spinBox_RfontSize_valueChanged(int arg1);

	void on_spinBox_CfontSize_valueChanged(int arg1);

	void on_doubleSpinBox_EFontScale_valueChanged(double arg);

    void on_buttonBox_accepted();

    void on_checkBox_RFont_Bold_clicked(bool checked);

    void on_checkBox_EFont_Bold_clicked(bool checked);

	void on_checkBox_CFont_Bold_clicked(bool checked);

	void on_checkBox_BlackELabel_clicked(bool checked);

	void on_checkBox_BlackRLabel_clicked(bool checked);

	void on_checkBox_CenterELabel_clicked(bool checked);

	void on_checkBox_AutoResizeELabels_clicked(bool checked);

	void on_radioButton_PrimeVis_clicked();

	void on_radioButton_DualVis_clicked();


private:
	OGLWidget *primeVis;
	OGLWidget *dualVis;
	OGLWidget *bipartieVis = NULL;

	OGLWidget *getCurrentVis();
	void updateUIs(OGLWidget *vis);
    Ui::FontSettingDialog *ui;
};

#endif // FONTSETTINGDIALOG_H
