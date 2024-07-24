/********************************************************************************
** Form generated from reading UI file 'ModifyElementDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFYELEMENTDIALOG_H
#define UI_MODIFYELEMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ModifyElementDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboBox_AllSelectedElements;
    QLineEdit *lineEdit_Label;
    QPushButton *pushButton_ChangeColor;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_X;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_Y;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_Z;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_Save;
    QPushButton *pushButton_ModifyAll;
    QPushButton *pushButton_Reset;

    void setupUi(QDialog *ModifyElementDialog)
    {
        if (ModifyElementDialog->objectName().isEmpty())
            ModifyElementDialog->setObjectName(QString::fromUtf8("ModifyElementDialog"));
        ModifyElementDialog->resize(428, 154);
        verticalLayout = new QVBoxLayout(ModifyElementDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        comboBox_AllSelectedElements = new QComboBox(ModifyElementDialog);
        comboBox_AllSelectedElements->setObjectName(QString::fromUtf8("comboBox_AllSelectedElements"));
        comboBox_AllSelectedElements->setEnabled(true);

        horizontalLayout_3->addWidget(comboBox_AllSelectedElements);


        verticalLayout->addLayout(horizontalLayout_3);

        lineEdit_Label = new QLineEdit(ModifyElementDialog);
        lineEdit_Label->setObjectName(QString::fromUtf8("lineEdit_Label"));

        verticalLayout->addWidget(lineEdit_Label);

        pushButton_ChangeColor = new QPushButton(ModifyElementDialog);
        pushButton_ChangeColor->setObjectName(QString::fromUtf8("pushButton_ChangeColor"));

        verticalLayout->addWidget(pushButton_ChangeColor);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ModifyElementDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(20, 16777215));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        doubleSpinBox_X = new QDoubleSpinBox(ModifyElementDialog);
        doubleSpinBox_X->setObjectName(QString::fromUtf8("doubleSpinBox_X"));
        doubleSpinBox_X->setDecimals(10);
        doubleSpinBox_X->setMinimum(-10000.000000000000000);
        doubleSpinBox_X->setMaximum(10000.000000000000000);
        doubleSpinBox_X->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox_X);

        label_2 = new QLabel(ModifyElementDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(20, 16777215));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        doubleSpinBox_Y = new QDoubleSpinBox(ModifyElementDialog);
        doubleSpinBox_Y->setObjectName(QString::fromUtf8("doubleSpinBox_Y"));
        doubleSpinBox_Y->setDecimals(10);
        doubleSpinBox_Y->setMinimum(-100000.000000000000000);
        doubleSpinBox_Y->setMaximum(100000.000000000000000);
        doubleSpinBox_Y->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox_Y);

        label_3 = new QLabel(ModifyElementDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(20, 16777215));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_3);

        doubleSpinBox_Z = new QDoubleSpinBox(ModifyElementDialog);
        doubleSpinBox_Z->setObjectName(QString::fromUtf8("doubleSpinBox_Z"));
        doubleSpinBox_Z->setDecimals(10);
        doubleSpinBox_Z->setMinimum(-100000.000000000000000);
        doubleSpinBox_Z->setMaximum(100000.000000000000000);
        doubleSpinBox_Z->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox_Z);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton_Save = new QPushButton(ModifyElementDialog);
        pushButton_Save->setObjectName(QString::fromUtf8("pushButton_Save"));

        horizontalLayout_2->addWidget(pushButton_Save);

        pushButton_ModifyAll = new QPushButton(ModifyElementDialog);
        pushButton_ModifyAll->setObjectName(QString::fromUtf8("pushButton_ModifyAll"));

        horizontalLayout_2->addWidget(pushButton_ModifyAll);

        pushButton_Reset = new QPushButton(ModifyElementDialog);
        pushButton_Reset->setObjectName(QString::fromUtf8("pushButton_Reset"));

        horizontalLayout_2->addWidget(pushButton_Reset);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(ModifyElementDialog);

        QMetaObject::connectSlotsByName(ModifyElementDialog);
    } // setupUi

    void retranslateUi(QDialog *ModifyElementDialog)
    {
        ModifyElementDialog->setWindowTitle(QCoreApplication::translate("ModifyElementDialog", "ModifyElementDialog", nullptr));
        pushButton_ChangeColor->setText(QCoreApplication::translate("ModifyElementDialog", "Color", nullptr));
        label->setText(QCoreApplication::translate("ModifyElementDialog", "x", nullptr));
        label_2->setText(QCoreApplication::translate("ModifyElementDialog", "y", nullptr));
        label_3->setText(QCoreApplication::translate("ModifyElementDialog", "z", nullptr));
        pushButton_Save->setText(QCoreApplication::translate("ModifyElementDialog", "Save", nullptr));
        pushButton_ModifyAll->setText(QCoreApplication::translate("ModifyElementDialog", "Modify All", nullptr));
        pushButton_Reset->setText(QCoreApplication::translate("ModifyElementDialog", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifyElementDialog: public Ui_ModifyElementDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFYELEMENTDIALOG_H
