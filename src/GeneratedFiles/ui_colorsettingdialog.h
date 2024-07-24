/********************************************************************************
** Form generated from reading UI file 'colorsettingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORSETTINGDIALOG_H
#define UI_COLORSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ColorSettingDialog
{
public:
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_r;
    QLabel *label_2;
    QLabel *label_3;
    QDialogButtonBox *buttonBox;
    QDoubleSpinBox *doubleSpinBox_g;
    QDoubleSpinBox *doubleSpinBox_b;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_a;
    QPushButton *pushButton_ChangeColor;

    void setupUi(QDialog *ColorSettingDialog)
    {
        if (ColorSettingDialog->objectName().isEmpty())
            ColorSettingDialog->setObjectName(QString::fromUtf8("ColorSettingDialog"));
        ColorSettingDialog->resize(174, 174);
        formLayout = new QFormLayout(ColorSettingDialog);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(ColorSettingDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(70, 16777215));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        doubleSpinBox_r = new QDoubleSpinBox(ColorSettingDialog);
        doubleSpinBox_r->setObjectName(QString::fromUtf8("doubleSpinBox_r"));
        doubleSpinBox_r->setMaximum(1.000000000000000);

        formLayout->setWidget(0, QFormLayout::FieldRole, doubleSpinBox_r);

        label_2 = new QLabel(ColorSettingDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setMaximumSize(QSize(70, 16777215));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(ColorSettingDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(0, 0));
        label_3->setMaximumSize(QSize(70, 16777215));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        buttonBox = new QDialogButtonBox(ColorSettingDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        formLayout->setWidget(5, QFormLayout::FieldRole, buttonBox);

        doubleSpinBox_g = new QDoubleSpinBox(ColorSettingDialog);
        doubleSpinBox_g->setObjectName(QString::fromUtf8("doubleSpinBox_g"));
        doubleSpinBox_g->setMaximum(1.000000000000000);

        formLayout->setWidget(1, QFormLayout::FieldRole, doubleSpinBox_g);

        doubleSpinBox_b = new QDoubleSpinBox(ColorSettingDialog);
        doubleSpinBox_b->setObjectName(QString::fromUtf8("doubleSpinBox_b"));
        doubleSpinBox_b->setMaximum(1.000000000000000);

        formLayout->setWidget(2, QFormLayout::FieldRole, doubleSpinBox_b);

        label_4 = new QLabel(ColorSettingDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(0, 0));
        label_4->setMaximumSize(QSize(70, 16777215));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        doubleSpinBox_a = new QDoubleSpinBox(ColorSettingDialog);
        doubleSpinBox_a->setObjectName(QString::fromUtf8("doubleSpinBox_a"));
        doubleSpinBox_a->setMaximum(1.000000000000000);
        doubleSpinBox_a->setValue(1.000000000000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, doubleSpinBox_a);

        pushButton_ChangeColor = new QPushButton(ColorSettingDialog);
        pushButton_ChangeColor->setObjectName(QString::fromUtf8("pushButton_ChangeColor"));

        formLayout->setWidget(4, QFormLayout::FieldRole, pushButton_ChangeColor);


        retranslateUi(ColorSettingDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ColorSettingDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ColorSettingDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ColorSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *ColorSettingDialog)
    {
        ColorSettingDialog->setWindowTitle(QCoreApplication::translate("ColorSettingDialog", "Setting Color", nullptr));
        label->setText(QCoreApplication::translate("ColorSettingDialog", "R channel", nullptr));
        label_2->setText(QCoreApplication::translate("ColorSettingDialog", "G channel", nullptr));
        label_3->setText(QCoreApplication::translate("ColorSettingDialog", "B channel", nullptr));
        label_4->setText(QCoreApplication::translate("ColorSettingDialog", "A channel", nullptr));
        pushButton_ChangeColor->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ColorSettingDialog: public Ui_ColorSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORSETTINGDIALOG_H
