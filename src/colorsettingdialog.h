#ifndef COLORSETTINGDIALOG_H
#define COLORSETTINGDIALOG_H

#include <QDialog>
#include "JPMath/Vector4.h"

namespace Ui {
class ColorSettingDialog;
}

class ColorSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorSettingDialog(QWidget *parent = 0);
    ~ColorSettingDialog();

	void initUI(Color4_F color);

    bool flag_accepted = false;
    double R_channel, G_channel, B_channel, A_channel;

private slots:
    void on_buttonBox_accepted();

	void on_pushButton_ChangeColor_clicked();
    void on_ColorSettingDialog_destroyed();

private:
    Ui::ColorSettingDialog *ui;
};

#endif // COLORSETTINGDIALOG_H
