/********************************************************************************
** Form generated from reading UI file 'fontsettingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FONTSETTINGDIALOG_H
#define UI_FONTSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FontSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_PrimeVis;
    QRadioButton *radioButton_DualVis;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_RFont_Bold;
    QSpinBox *spinBox_CfontSize;
    QCheckBox *checkBox_BlackRLabel;
    QSpinBox *spinBox_EfontSize;
    QLabel *label_12;
    QSpinBox *spinBox_RfontSize;
    QCheckBox *checkBox_CFont_Bold;
    QLabel *label;
    QCheckBox *checkBox_EFont_Bold;
    QCheckBox *checkBox_BlackELabel;
    QLabel *label_2;
    QCheckBox *checkBox_CenterELabel;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBox_AutoResizeELabels;
    QDoubleSpinBox *doubleSpinBox_EFontScale;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FontSettingDialog)
    {
        if (FontSettingDialog->objectName().isEmpty())
            FontSettingDialog->setObjectName(QString::fromUtf8("FontSettingDialog"));
        FontSettingDialog->resize(347, 222);
        verticalLayout = new QVBoxLayout(FontSettingDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 9);
        radioButton_PrimeVis = new QRadioButton(FontSettingDialog);
        radioButton_PrimeVis->setObjectName(QString::fromUtf8("radioButton_PrimeVis"));
        radioButton_PrimeVis->setChecked(true);

        horizontalLayout_2->addWidget(radioButton_PrimeVis);

        radioButton_DualVis = new QRadioButton(FontSettingDialog);
        radioButton_DualVis->setObjectName(QString::fromUtf8("radioButton_DualVis"));

        horizontalLayout_2->addWidget(radioButton_DualVis);


        verticalLayout->addLayout(horizontalLayout_2);

        groupBox = new QGroupBox(FontSettingDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        checkBox_RFont_Bold = new QCheckBox(groupBox);
        checkBox_RFont_Bold->setObjectName(QString::fromUtf8("checkBox_RFont_Bold"));

        gridLayout_2->addWidget(checkBox_RFont_Bold, 2, 2, 1, 1);

        spinBox_CfontSize = new QSpinBox(groupBox);
        spinBox_CfontSize->setObjectName(QString::fromUtf8("spinBox_CfontSize"));
        spinBox_CfontSize->setValue(20);

        gridLayout_2->addWidget(spinBox_CfontSize, 4, 1, 1, 1);

        checkBox_BlackRLabel = new QCheckBox(groupBox);
        checkBox_BlackRLabel->setObjectName(QString::fromUtf8("checkBox_BlackRLabel"));

        gridLayout_2->addWidget(checkBox_BlackRLabel, 2, 3, 1, 1);

        spinBox_EfontSize = new QSpinBox(groupBox);
        spinBox_EfontSize->setObjectName(QString::fromUtf8("spinBox_EfontSize"));
        spinBox_EfontSize->setValue(8);

        gridLayout_2->addWidget(spinBox_EfontSize, 1, 1, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(0, 0));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_12, 2, 0, 1, 1);

        spinBox_RfontSize = new QSpinBox(groupBox);
        spinBox_RfontSize->setObjectName(QString::fromUtf8("spinBox_RfontSize"));
        spinBox_RfontSize->setMinimumSize(QSize(80, 0));
        spinBox_RfontSize->setMinimum(1);
        spinBox_RfontSize->setMaximum(100);
        spinBox_RfontSize->setSingleStep(1);
        spinBox_RfontSize->setValue(15);

        gridLayout_2->addWidget(spinBox_RfontSize, 2, 1, 1, 1);

        checkBox_CFont_Bold = new QCheckBox(groupBox);
        checkBox_CFont_Bold->setObjectName(QString::fromUtf8("checkBox_CFont_Bold"));

        gridLayout_2->addWidget(checkBox_CFont_Bold, 4, 2, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        checkBox_EFont_Bold = new QCheckBox(groupBox);
        checkBox_EFont_Bold->setObjectName(QString::fromUtf8("checkBox_EFont_Bold"));

        gridLayout_2->addWidget(checkBox_EFont_Bold, 1, 2, 1, 1);

        checkBox_BlackELabel = new QCheckBox(groupBox);
        checkBox_BlackELabel->setObjectName(QString::fromUtf8("checkBox_BlackELabel"));

        gridLayout_2->addWidget(checkBox_BlackELabel, 1, 3, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_2, 4, 0, 1, 1);

        checkBox_CenterELabel = new QCheckBox(groupBox);
        checkBox_CenterELabel->setObjectName(QString::fromUtf8("checkBox_CenterELabel"));

        gridLayout_2->addWidget(checkBox_CenterELabel, 1, 4, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(FontSettingDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        checkBox_AutoResizeELabels = new QCheckBox(groupBox_2);
        checkBox_AutoResizeELabels->setObjectName(QString::fromUtf8("checkBox_AutoResizeELabels"));

        horizontalLayout->addWidget(checkBox_AutoResizeELabels);

        doubleSpinBox_EFontScale = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_EFontScale->setObjectName(QString::fromUtf8("doubleSpinBox_EFontScale"));
        doubleSpinBox_EFontScale->setDecimals(4);
        doubleSpinBox_EFontScale->setMaximum(5.000000000000000);
        doubleSpinBox_EFontScale->setSingleStep(0.050000000000000);

        horizontalLayout->addWidget(doubleSpinBox_EFontScale);


        verticalLayout->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(FontSettingDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(80, 0));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(FontSettingDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), FontSettingDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FontSettingDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(FontSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *FontSettingDialog)
    {
        FontSettingDialog->setWindowTitle(QCoreApplication::translate("FontSettingDialog", "Font Settings", nullptr));
        radioButton_PrimeVis->setText(QCoreApplication::translate("FontSettingDialog", "Prime Vis", nullptr));
        radioButton_DualVis->setText(QCoreApplication::translate("FontSettingDialog", "Dual Vis", nullptr));
        groupBox->setTitle(QCoreApplication::translate("FontSettingDialog", "Fonts", nullptr));
        checkBox_RFont_Bold->setText(QCoreApplication::translate("FontSettingDialog", "Bold", nullptr));
        checkBox_BlackRLabel->setText(QCoreApplication::translate("FontSettingDialog", "Black", nullptr));
        label_12->setText(QCoreApplication::translate("FontSettingDialog", "Polygon Font Size:", nullptr));
        checkBox_CFont_Bold->setText(QCoreApplication::translate("FontSettingDialog", "Bold", nullptr));
        label->setText(QCoreApplication::translate("FontSettingDialog", "Entity Font Size", nullptr));
        checkBox_EFont_Bold->setText(QCoreApplication::translate("FontSettingDialog", "Bold", nullptr));
        checkBox_BlackELabel->setText(QCoreApplication::translate("FontSettingDialog", "Black", nullptr));
        label_2->setText(QCoreApplication::translate("FontSettingDialog", "Caption Font Size", nullptr));
        checkBox_CenterELabel->setText(QCoreApplication::translate("FontSettingDialog", "Center", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("FontSettingDialog", "Auto Resize Node Label", nullptr));
        checkBox_AutoResizeELabels->setText(QCoreApplication::translate("FontSettingDialog", "Entity Font Scale", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FontSettingDialog: public Ui_FontSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FONTSETTINGDIALOG_H
