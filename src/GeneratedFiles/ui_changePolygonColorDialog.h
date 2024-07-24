/********************************************************************************
** Form generated from reading UI file 'changePolygonColorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEPOLYGONCOLORDIALOG_H
#define UI_CHANGEPOLYGONCOLORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_changePolygonColorDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_PolygonColor;
    QGridLayout *gridLayout;
    QLabel *label_15;
    QLabel *label_14;
    QSpinBox *spinBox_Color_G_9;
    QSpinBox *spinBox_Color_A_9;
    QLabel *label_11;
    QSpinBox *spinBox_Color_B_9;
    QPushButton *pushButton_C_9;
    QSpinBox *spinBox_Color_R_9;
    QSpinBox *spinBox_Color_R_10;
    QSpinBox *spinBox_Color_G_10;
    QSpinBox *spinBox_Color_B_10;
    QSpinBox *spinBox_Color_B_7;
    QSpinBox *spinBox_Color_A_6;
    QLabel *label_9;
    QSpinBox *spinBox_Color_A_7;
    QSpinBox *spinBox_Color_G_8;
    QLabel *label_10;
    QSpinBox *spinBox_Color_G_7;
    QLabel *label_8;
    QPushButton *pushButton_C_8;
    QPushButton *pushButton_C_10;
    QSpinBox *spinBox_Color_G_11;
    QSpinBox *spinBox_Color_A_11;
    QSpinBox *spinBox_Color_R_11;
    QLabel *label_12;
    QSpinBox *spinBox_Color_B_11;
    QPushButton *pushButton_C_3;
    QSpinBox *spinBox_Color_G_4;
    QPushButton *pushButton_C_11;
    QLabel *label_5;
    QSpinBox *spinBox_Color_G_5;
    QPushButton *pushButton_C_6;
    QLabel *label_7;
    QSpinBox *spinBox_Color_B_8;
    QSpinBox *spinBox_Color_B_5;
    QSpinBox *spinBox_Color_A_8;
    QSpinBox *spinBox_Color_R_7;
    QSpinBox *spinBox_Color_A_5;
    QLabel *label_6;
    QPushButton *pushButton_C_7;
    QSpinBox *spinBox_Color_A_4;
    QSpinBox *spinBox_Color_G_6;
    QSpinBox *spinBox_Color_R_5;
    QSpinBox *spinBox_Color_B_4;
    QSpinBox *spinBox_Color_B_6;
    QSpinBox *spinBox_Color_R_8;
    QSpinBox *spinBox_Color_R_4;
    QPushButton *pushButton_C_5;
    QPushButton *pushButton_C_4;
    QSpinBox *spinBox_Color_R_6;
    QSpinBox *spinBox_Color_G_3;
    QSpinBox *spinBox_Color_A_3;
    QLabel *label_2;
    QSpinBox *spinBox_Color_R_1;
    QSpinBox *spinBox_Color_G_2;
    QSpinBox *spinBox_Color_B_3;
    QLabel *label_4;
    QLabel *label_3;
    QSpinBox *spinBox_Color_B_1;
    QSpinBox *spinBox_Color_R_2;
    QSpinBox *spinBox_Color_A_1;
    QSpinBox *spinBox_Color_B_2;
    QSpinBox *spinBox_Color_A_10;
    QLabel *label_13;
    QLabel *label;
    QSpinBox *spinBox_Color_A_2;
    QPushButton *pushButton_C_1;
    QPushButton *pushButton_C_2;
    QSpinBox *spinBox_Color_R_3;
    QSpinBox *spinBox_Color_G_1;
    QCheckBox *checkBox_ColorSelect_1;
    QCheckBox *checkBox_ColorSelect_2;
    QCheckBox *checkBox_ColorSelect_3;
    QCheckBox *checkBox_ColorSelect_4;
    QCheckBox *checkBox_ColorSelect_5;
    QCheckBox *checkBox_ColorSelect_6;
    QCheckBox *checkBox_ColorSelect_7;
    QCheckBox *checkBox_ColorSelect_8;
    QCheckBox *checkBox_ColorSelect_9;
    QCheckBox *checkBox_ColorSelect_10;
    QCheckBox *checkBox_ColorSelect_11;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_SetPolygonColorsAsSelected;
    QPushButton *pushButton_SwapSelectedColors;
    QPushButton *pushButton_OutputColorInfo;
    QPushButton *pushButton_UpdateColors;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_saveColorScheme;
    QPushButton *pushButton_LoadColorScheme;
    QPushButton *pushButton_resetToDefaultColors;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *changePolygonColorDialog)
    {
        if (changePolygonColorDialog->objectName().isEmpty())
            changePolygonColorDialog->setObjectName(QString::fromUtf8("changePolygonColorDialog"));
        changePolygonColorDialog->resize(309, 466);
        verticalLayout = new QVBoxLayout(changePolygonColorDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_PolygonColor = new QGroupBox(changePolygonColorDialog);
        groupBox_PolygonColor->setObjectName(QString::fromUtf8("groupBox_PolygonColor"));
        gridLayout = new QGridLayout(groupBox_PolygonColor);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        label_15 = new QLabel(groupBox_PolygonColor);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 0, 4, 1, 1);

        label_14 = new QLabel(groupBox_PolygonColor);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 0, 3, 1, 1);

        spinBox_Color_G_9 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_9->setObjectName(QString::fromUtf8("spinBox_Color_G_9"));
        spinBox_Color_G_9->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_9, 9, 2, 1, 1);

        spinBox_Color_A_9 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_9->setObjectName(QString::fromUtf8("spinBox_Color_A_9"));
        spinBox_Color_A_9->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_9, 9, 4, 1, 1);

        label_11 = new QLabel(groupBox_PolygonColor);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_11, 10, 0, 1, 1);

        spinBox_Color_B_9 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_9->setObjectName(QString::fromUtf8("spinBox_Color_B_9"));
        spinBox_Color_B_9->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_9, 9, 3, 1, 1);

        pushButton_C_9 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_9->setObjectName(QString::fromUtf8("pushButton_C_9"));
        pushButton_C_9->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_9, 9, 5, 1, 1);

        spinBox_Color_R_9 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_9->setObjectName(QString::fromUtf8("spinBox_Color_R_9"));
        spinBox_Color_R_9->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_9, 9, 1, 1, 1);

        spinBox_Color_R_10 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_10->setObjectName(QString::fromUtf8("spinBox_Color_R_10"));
        spinBox_Color_R_10->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_10, 10, 1, 1, 1);

        spinBox_Color_G_10 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_10->setObjectName(QString::fromUtf8("spinBox_Color_G_10"));
        spinBox_Color_G_10->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_10, 10, 2, 1, 1);

        spinBox_Color_B_10 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_10->setObjectName(QString::fromUtf8("spinBox_Color_B_10"));
        spinBox_Color_B_10->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_10, 10, 3, 1, 1);

        spinBox_Color_B_7 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_7->setObjectName(QString::fromUtf8("spinBox_Color_B_7"));
        spinBox_Color_B_7->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_7, 7, 3, 1, 1);

        spinBox_Color_A_6 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_6->setObjectName(QString::fromUtf8("spinBox_Color_A_6"));
        spinBox_Color_A_6->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_6, 6, 4, 1, 1);

        label_9 = new QLabel(groupBox_PolygonColor);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 8, 0, 1, 1);

        spinBox_Color_A_7 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_7->setObjectName(QString::fromUtf8("spinBox_Color_A_7"));
        spinBox_Color_A_7->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_7, 7, 4, 1, 1);

        spinBox_Color_G_8 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_8->setObjectName(QString::fromUtf8("spinBox_Color_G_8"));
        spinBox_Color_G_8->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_8, 8, 2, 1, 1);

        label_10 = new QLabel(groupBox_PolygonColor);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_10, 9, 0, 1, 1);

        spinBox_Color_G_7 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_7->setObjectName(QString::fromUtf8("spinBox_Color_G_7"));
        spinBox_Color_G_7->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_7, 7, 2, 1, 1);

        label_8 = new QLabel(groupBox_PolygonColor);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 7, 0, 1, 1);

        pushButton_C_8 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_8->setObjectName(QString::fromUtf8("pushButton_C_8"));
        pushButton_C_8->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_8, 8, 5, 1, 1);

        pushButton_C_10 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_10->setObjectName(QString::fromUtf8("pushButton_C_10"));
        pushButton_C_10->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_10, 10, 5, 1, 1);

        spinBox_Color_G_11 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_11->setObjectName(QString::fromUtf8("spinBox_Color_G_11"));
        spinBox_Color_G_11->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_11, 11, 2, 1, 1);

        spinBox_Color_A_11 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_11->setObjectName(QString::fromUtf8("spinBox_Color_A_11"));
        spinBox_Color_A_11->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_11, 11, 4, 1, 1);

        spinBox_Color_R_11 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_11->setObjectName(QString::fromUtf8("spinBox_Color_R_11"));
        spinBox_Color_R_11->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_11, 11, 1, 1, 1);

        label_12 = new QLabel(groupBox_PolygonColor);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_12, 11, 0, 1, 1);

        spinBox_Color_B_11 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_11->setObjectName(QString::fromUtf8("spinBox_Color_B_11"));
        spinBox_Color_B_11->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_11, 11, 3, 1, 1);

        pushButton_C_3 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_3->setObjectName(QString::fromUtf8("pushButton_C_3"));
        pushButton_C_3->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_3, 3, 5, 1, 1);

        spinBox_Color_G_4 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_4->setObjectName(QString::fromUtf8("spinBox_Color_G_4"));
        spinBox_Color_G_4->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_4, 4, 2, 1, 1);

        pushButton_C_11 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_11->setObjectName(QString::fromUtf8("pushButton_C_11"));
        pushButton_C_11->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_11, 11, 5, 1, 1);

        label_5 = new QLabel(groupBox_PolygonColor);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        spinBox_Color_G_5 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_5->setObjectName(QString::fromUtf8("spinBox_Color_G_5"));
        spinBox_Color_G_5->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_5, 5, 2, 1, 1);

        pushButton_C_6 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_6->setObjectName(QString::fromUtf8("pushButton_C_6"));
        pushButton_C_6->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_6, 6, 5, 1, 1);

        label_7 = new QLabel(groupBox_PolygonColor);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        spinBox_Color_B_8 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_8->setObjectName(QString::fromUtf8("spinBox_Color_B_8"));
        spinBox_Color_B_8->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_8, 8, 3, 1, 1);

        spinBox_Color_B_5 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_5->setObjectName(QString::fromUtf8("spinBox_Color_B_5"));
        spinBox_Color_B_5->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_5, 5, 3, 1, 1);

        spinBox_Color_A_8 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_8->setObjectName(QString::fromUtf8("spinBox_Color_A_8"));
        spinBox_Color_A_8->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_8, 8, 4, 1, 1);

        spinBox_Color_R_7 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_7->setObjectName(QString::fromUtf8("spinBox_Color_R_7"));
        spinBox_Color_R_7->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_7, 7, 1, 1, 1);

        spinBox_Color_A_5 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_5->setObjectName(QString::fromUtf8("spinBox_Color_A_5"));
        spinBox_Color_A_5->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_5, 5, 4, 1, 1);

        label_6 = new QLabel(groupBox_PolygonColor);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        pushButton_C_7 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_7->setObjectName(QString::fromUtf8("pushButton_C_7"));
        pushButton_C_7->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_7, 7, 5, 1, 1);

        spinBox_Color_A_4 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_4->setObjectName(QString::fromUtf8("spinBox_Color_A_4"));
        spinBox_Color_A_4->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_4, 4, 4, 1, 1);

        spinBox_Color_G_6 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_6->setObjectName(QString::fromUtf8("spinBox_Color_G_6"));
        spinBox_Color_G_6->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_6, 6, 2, 1, 1);

        spinBox_Color_R_5 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_5->setObjectName(QString::fromUtf8("spinBox_Color_R_5"));
        spinBox_Color_R_5->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_5, 5, 1, 1, 1);

        spinBox_Color_B_4 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_4->setObjectName(QString::fromUtf8("spinBox_Color_B_4"));
        spinBox_Color_B_4->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_4, 4, 3, 1, 1);

        spinBox_Color_B_6 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_6->setObjectName(QString::fromUtf8("spinBox_Color_B_6"));
        spinBox_Color_B_6->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_6, 6, 3, 1, 1);

        spinBox_Color_R_8 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_8->setObjectName(QString::fromUtf8("spinBox_Color_R_8"));
        spinBox_Color_R_8->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_8, 8, 1, 1, 1);

        spinBox_Color_R_4 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_4->setObjectName(QString::fromUtf8("spinBox_Color_R_4"));
        spinBox_Color_R_4->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_4, 4, 1, 1, 1);

        pushButton_C_5 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_5->setObjectName(QString::fromUtf8("pushButton_C_5"));
        pushButton_C_5->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_5, 5, 5, 1, 1);

        pushButton_C_4 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_4->setObjectName(QString::fromUtf8("pushButton_C_4"));
        pushButton_C_4->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_4, 4, 5, 1, 1);

        spinBox_Color_R_6 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_6->setObjectName(QString::fromUtf8("spinBox_Color_R_6"));
        spinBox_Color_R_6->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_6, 6, 1, 1, 1);

        spinBox_Color_G_3 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_3->setObjectName(QString::fromUtf8("spinBox_Color_G_3"));
        spinBox_Color_G_3->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_3, 3, 2, 1, 1);

        spinBox_Color_A_3 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_3->setObjectName(QString::fromUtf8("spinBox_Color_A_3"));
        spinBox_Color_A_3->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_3, 3, 4, 1, 1);

        label_2 = new QLabel(groupBox_PolygonColor);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        spinBox_Color_R_1 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_1->setObjectName(QString::fromUtf8("spinBox_Color_R_1"));
        spinBox_Color_R_1->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_1, 1, 1, 1, 1);

        spinBox_Color_G_2 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_2->setObjectName(QString::fromUtf8("spinBox_Color_G_2"));
        spinBox_Color_G_2->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_2, 2, 2, 1, 1);

        spinBox_Color_B_3 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_3->setObjectName(QString::fromUtf8("spinBox_Color_B_3"));
        spinBox_Color_B_3->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_3, 3, 3, 1, 1);

        label_4 = new QLabel(groupBox_PolygonColor);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_3 = new QLabel(groupBox_PolygonColor);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        spinBox_Color_B_1 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_1->setObjectName(QString::fromUtf8("spinBox_Color_B_1"));
        spinBox_Color_B_1->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_1, 1, 3, 1, 1);

        spinBox_Color_R_2 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_2->setObjectName(QString::fromUtf8("spinBox_Color_R_2"));
        spinBox_Color_R_2->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_2, 2, 1, 1, 1);

        spinBox_Color_A_1 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_1->setObjectName(QString::fromUtf8("spinBox_Color_A_1"));
        spinBox_Color_A_1->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_1, 1, 4, 1, 1);

        spinBox_Color_B_2 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_B_2->setObjectName(QString::fromUtf8("spinBox_Color_B_2"));
        spinBox_Color_B_2->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_B_2, 2, 3, 1, 1);

        spinBox_Color_A_10 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_10->setObjectName(QString::fromUtf8("spinBox_Color_A_10"));
        spinBox_Color_A_10->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_10, 10, 4, 1, 1);

        label_13 = new QLabel(groupBox_PolygonColor);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 0, 2, 1, 1);

        label = new QLabel(groupBox_PolygonColor);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        spinBox_Color_A_2 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_A_2->setObjectName(QString::fromUtf8("spinBox_Color_A_2"));
        spinBox_Color_A_2->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_A_2, 2, 4, 1, 1);

        pushButton_C_1 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_1->setObjectName(QString::fromUtf8("pushButton_C_1"));
        pushButton_C_1->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_1, 1, 5, 1, 1);

        pushButton_C_2 = new QPushButton(groupBox_PolygonColor);
        pushButton_C_2->setObjectName(QString::fromUtf8("pushButton_C_2"));
        pushButton_C_2->setMaximumSize(QSize(23, 23));

        gridLayout->addWidget(pushButton_C_2, 2, 5, 1, 1);

        spinBox_Color_R_3 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_R_3->setObjectName(QString::fromUtf8("spinBox_Color_R_3"));
        spinBox_Color_R_3->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_R_3, 3, 1, 1, 1);

        spinBox_Color_G_1 = new QSpinBox(groupBox_PolygonColor);
        spinBox_Color_G_1->setObjectName(QString::fromUtf8("spinBox_Color_G_1"));
        spinBox_Color_G_1->setMaximum(255);

        gridLayout->addWidget(spinBox_Color_G_1, 1, 2, 1, 1);

        checkBox_ColorSelect_1 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_1->setObjectName(QString::fromUtf8("checkBox_ColorSelect_1"));

        gridLayout->addWidget(checkBox_ColorSelect_1, 1, 6, 1, 1);

        checkBox_ColorSelect_2 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_2->setObjectName(QString::fromUtf8("checkBox_ColorSelect_2"));

        gridLayout->addWidget(checkBox_ColorSelect_2, 2, 6, 1, 1);

        checkBox_ColorSelect_3 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_3->setObjectName(QString::fromUtf8("checkBox_ColorSelect_3"));

        gridLayout->addWidget(checkBox_ColorSelect_3, 3, 6, 1, 1);

        checkBox_ColorSelect_4 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_4->setObjectName(QString::fromUtf8("checkBox_ColorSelect_4"));

        gridLayout->addWidget(checkBox_ColorSelect_4, 4, 6, 1, 1);

        checkBox_ColorSelect_5 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_5->setObjectName(QString::fromUtf8("checkBox_ColorSelect_5"));

        gridLayout->addWidget(checkBox_ColorSelect_5, 5, 6, 1, 1);

        checkBox_ColorSelect_6 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_6->setObjectName(QString::fromUtf8("checkBox_ColorSelect_6"));

        gridLayout->addWidget(checkBox_ColorSelect_6, 6, 6, 1, 1);

        checkBox_ColorSelect_7 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_7->setObjectName(QString::fromUtf8("checkBox_ColorSelect_7"));

        gridLayout->addWidget(checkBox_ColorSelect_7, 7, 6, 1, 1);

        checkBox_ColorSelect_8 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_8->setObjectName(QString::fromUtf8("checkBox_ColorSelect_8"));

        gridLayout->addWidget(checkBox_ColorSelect_8, 8, 6, 1, 1);

        checkBox_ColorSelect_9 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_9->setObjectName(QString::fromUtf8("checkBox_ColorSelect_9"));

        gridLayout->addWidget(checkBox_ColorSelect_9, 9, 6, 1, 1);

        checkBox_ColorSelect_10 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_10->setObjectName(QString::fromUtf8("checkBox_ColorSelect_10"));

        gridLayout->addWidget(checkBox_ColorSelect_10, 10, 6, 1, 1);

        checkBox_ColorSelect_11 = new QCheckBox(groupBox_PolygonColor);
        checkBox_ColorSelect_11->setObjectName(QString::fromUtf8("checkBox_ColorSelect_11"));

        gridLayout->addWidget(checkBox_ColorSelect_11, 11, 6, 1, 1);


        verticalLayout->addWidget(groupBox_PolygonColor);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, 0, -1, -1);
        pushButton_SetPolygonColorsAsSelected = new QPushButton(changePolygonColorDialog);
        pushButton_SetPolygonColorsAsSelected->setObjectName(QString::fromUtf8("pushButton_SetPolygonColorsAsSelected"));

        gridLayout_2->addWidget(pushButton_SetPolygonColorsAsSelected, 0, 0, 1, 1);

        pushButton_SwapSelectedColors = new QPushButton(changePolygonColorDialog);
        pushButton_SwapSelectedColors->setObjectName(QString::fromUtf8("pushButton_SwapSelectedColors"));

        gridLayout_2->addWidget(pushButton_SwapSelectedColors, 0, 1, 1, 1);

        pushButton_OutputColorInfo = new QPushButton(changePolygonColorDialog);
        pushButton_OutputColorInfo->setObjectName(QString::fromUtf8("pushButton_OutputColorInfo"));

        gridLayout_2->addWidget(pushButton_OutputColorInfo, 1, 1, 1, 1);

        pushButton_UpdateColors = new QPushButton(changePolygonColorDialog);
        pushButton_UpdateColors->setObjectName(QString::fromUtf8("pushButton_UpdateColors"));

        gridLayout_2->addWidget(pushButton_UpdateColors, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        pushButton_saveColorScheme = new QPushButton(changePolygonColorDialog);
        pushButton_saveColorScheme->setObjectName(QString::fromUtf8("pushButton_saveColorScheme"));

        horizontalLayout->addWidget(pushButton_saveColorScheme);

        pushButton_LoadColorScheme = new QPushButton(changePolygonColorDialog);
        pushButton_LoadColorScheme->setObjectName(QString::fromUtf8("pushButton_LoadColorScheme"));

        horizontalLayout->addWidget(pushButton_LoadColorScheme);

        pushButton_resetToDefaultColors = new QPushButton(changePolygonColorDialog);
        pushButton_resetToDefaultColors->setObjectName(QString::fromUtf8("pushButton_resetToDefaultColors"));

        horizontalLayout->addWidget(pushButton_resetToDefaultColors);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(changePolygonColorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(changePolygonColorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), changePolygonColorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), changePolygonColorDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(changePolygonColorDialog);
    } // setupUi

    void retranslateUi(QDialog *changePolygonColorDialog)
    {
        changePolygonColorDialog->setWindowTitle(QCoreApplication::translate("changePolygonColorDialog", "changePolygonColorDialog", nullptr));
        groupBox_PolygonColor->setTitle(QCoreApplication::translate("changePolygonColorDialog", "PolygonColor", nullptr));
        label_15->setText(QCoreApplication::translate("changePolygonColorDialog", "Alpha", nullptr));
        label_14->setText(QCoreApplication::translate("changePolygonColorDialog", "B", nullptr));
        label_11->setText(QCoreApplication::translate("changePolygonColorDialog", "Decagon", nullptr));
        pushButton_C_9->setText(QString());
        label_9->setText(QCoreApplication::translate("changePolygonColorDialog", "Octagon", nullptr));
        label_10->setText(QCoreApplication::translate("changePolygonColorDialog", "Enneagon", nullptr));
        label_8->setText(QCoreApplication::translate("changePolygonColorDialog", "Heptagon", nullptr));
        pushButton_C_8->setText(QString());
        pushButton_C_10->setText(QString());
        label_12->setText(QCoreApplication::translate("changePolygonColorDialog", "Else", nullptr));
        pushButton_C_3->setText(QString());
        pushButton_C_11->setText(QString());
        label_5->setText(QCoreApplication::translate("changePolygonColorDialog", "Quadrangle", nullptr));
        pushButton_C_6->setText(QString());
        label_7->setText(QCoreApplication::translate("changePolygonColorDialog", "Hexgon", nullptr));
        label_6->setText(QCoreApplication::translate("changePolygonColorDialog", "Pentagon", nullptr));
        pushButton_C_7->setText(QString());
        pushButton_C_5->setText(QString());
        pushButton_C_4->setText(QString());
        label_2->setText(QCoreApplication::translate("changePolygonColorDialog", "R", nullptr));
        label_4->setText(QCoreApplication::translate("changePolygonColorDialog", "Triangle", nullptr));
        label_3->setText(QCoreApplication::translate("changePolygonColorDialog", "Digon", nullptr));
        label_13->setText(QCoreApplication::translate("changePolygonColorDialog", "G", nullptr));
        label->setText(QCoreApplication::translate("changePolygonColorDialog", "Monogon", nullptr));
        pushButton_C_1->setText(QString());
        pushButton_C_2->setText(QString());
        checkBox_ColorSelect_1->setText(QString());
        checkBox_ColorSelect_2->setText(QString());
        checkBox_ColorSelect_3->setText(QString());
        checkBox_ColorSelect_4->setText(QString());
        checkBox_ColorSelect_5->setText(QString());
        checkBox_ColorSelect_6->setText(QString());
        checkBox_ColorSelect_7->setText(QString());
        checkBox_ColorSelect_8->setText(QString());
        checkBox_ColorSelect_9->setText(QString());
        checkBox_ColorSelect_10->setText(QString());
        checkBox_ColorSelect_11->setText(QString());
        pushButton_SetPolygonColorsAsSelected->setText(QCoreApplication::translate("changePolygonColorDialog", "Set All Color as Selected", nullptr));
        pushButton_SwapSelectedColors->setText(QCoreApplication::translate("changePolygonColorDialog", "Swap Selected Colors", nullptr));
        pushButton_OutputColorInfo->setText(QCoreApplication::translate("changePolygonColorDialog", "Color Info", nullptr));
        pushButton_UpdateColors->setText(QCoreApplication::translate("changePolygonColorDialog", "Update Colors", nullptr));
        pushButton_saveColorScheme->setText(QCoreApplication::translate("changePolygonColorDialog", "Save Colors...", nullptr));
        pushButton_LoadColorScheme->setText(QCoreApplication::translate("changePolygonColorDialog", "Load Colors...", nullptr));
        pushButton_resetToDefaultColors->setText(QCoreApplication::translate("changePolygonColorDialog", "Reset to default", nullptr));
    } // retranslateUi

};

namespace Ui {
    class changePolygonColorDialog: public Ui_changePolygonColorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPOLYGONCOLORDIALOG_H
