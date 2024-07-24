/********************************************************************************
** Form generated from reading UI file 'LightSettingDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHTSETTINGDIALOG_H
#define UI_LIGHTSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LightSettingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_ResetLightSources;
    QCheckBox *checkBox_Draw_Lights;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_lightGlyphSize;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_CylinderShininess;
    QDoubleSpinBox *doubleSpinBox_SphereShininess;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_19;
    QDoubleSpinBox *doubleSpinBox_LightZ;
    QDoubleSpinBox *doubleSpinBox_AmbientR;
    QDoubleSpinBox *doubleSpinBox_LightY;
    QDoubleSpinBox *doubleSpinBox_LightX;
    QCheckBox *checkBox_UseLIght0;
    QDoubleSpinBox *doubleSpinBox_AmbientA;
    QDoubleSpinBox *doubleSpinBox_AmbientG;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_AmbientB;
    QLabel *label_2;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_DiffuseR;
    QDoubleSpinBox *doubleSpinBox_DiffuseG;
    QDoubleSpinBox *doubleSpinBox_DiffuseB;
    QDoubleSpinBox *doubleSpinBox_DiffuseA;
    QDoubleSpinBox *doubleSpinBox_SpecularR;
    QDoubleSpinBox *doubleSpinBox_SpecularG;
    QDoubleSpinBox *doubleSpinBox_SpecularB;
    QDoubleSpinBox *doubleSpinBox_SpecularA;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_LightZ_2;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_LightX_2;
    QDoubleSpinBox *doubleSpinBox_LightY_2;
    QLabel *label_5;
    QCheckBox *checkBox_UseLIght1;
    QDoubleSpinBox *doubleSpinBox_AmbientB_2;
    QDoubleSpinBox *doubleSpinBox_AmbientR_2;
    QDoubleSpinBox *doubleSpinBox_AmbientG_2;
    QDoubleSpinBox *doubleSpinBox_AmbientA_2;
    QDoubleSpinBox *doubleSpinBox_DiffuseR_2;
    QDoubleSpinBox *doubleSpinBox_DiffuseG_2;
    QDoubleSpinBox *doubleSpinBox_DiffuseB_2;
    QDoubleSpinBox *doubleSpinBox_DiffuseA_2;
    QDoubleSpinBox *doubleSpinBox_SpecularR_2;
    QDoubleSpinBox *doubleSpinBox_SpecularG_2;
    QDoubleSpinBox *doubleSpinBox_SpecularB_2;
    QDoubleSpinBox *doubleSpinBox_SpecularA_2;

    void setupUi(QWidget *LightSettingDialog)
    {
        if (LightSettingDialog->objectName().isEmpty())
            LightSettingDialog->setObjectName(QString::fromUtf8("LightSettingDialog"));
        LightSettingDialog->resize(387, 307);
        verticalLayout = new QVBoxLayout(LightSettingDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        pushButton_ResetLightSources = new QPushButton(LightSettingDialog);
        pushButton_ResetLightSources->setObjectName(QString::fromUtf8("pushButton_ResetLightSources"));
        pushButton_ResetLightSources->setFocusPolicy(Qt::ClickFocus);

        horizontalLayout->addWidget(pushButton_ResetLightSources);

        checkBox_Draw_Lights = new QCheckBox(LightSettingDialog);
        checkBox_Draw_Lights->setObjectName(QString::fromUtf8("checkBox_Draw_Lights"));

        horizontalLayout->addWidget(checkBox_Draw_Lights);

        label_7 = new QLabel(LightSettingDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_7);

        doubleSpinBox_lightGlyphSize = new QDoubleSpinBox(LightSettingDialog);
        doubleSpinBox_lightGlyphSize->setObjectName(QString::fromUtf8("doubleSpinBox_lightGlyphSize"));
        doubleSpinBox_lightGlyphSize->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox_lightGlyphSize);


        verticalLayout->addLayout(horizontalLayout);

        groupBox_3 = new QGroupBox(LightSettingDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 0, 2, 1, 1);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_9, 0, 0, 1, 1);

        doubleSpinBox_CylinderShininess = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_CylinderShininess->setObjectName(QString::fromUtf8("doubleSpinBox_CylinderShininess"));
        doubleSpinBox_CylinderShininess->setMaximum(128.000000000000000);

        gridLayout_2->addWidget(doubleSpinBox_CylinderShininess, 0, 3, 1, 1);

        doubleSpinBox_SphereShininess = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_SphereShininess->setObjectName(QString::fromUtf8("doubleSpinBox_SphereShininess"));
        doubleSpinBox_SphereShininess->setMaximum(128.000000000000000);

        gridLayout_2->addWidget(doubleSpinBox_SphereShininess, 0, 1, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        groupBox = new QGroupBox(LightSettingDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_19 = new QGridLayout(groupBox);
        gridLayout_19->setSpacing(2);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        gridLayout_19->setContentsMargins(2, 2, 2, 2);
        doubleSpinBox_LightZ = new QDoubleSpinBox(groupBox);
        doubleSpinBox_LightZ->setObjectName(QString::fromUtf8("doubleSpinBox_LightZ"));
        doubleSpinBox_LightZ->setMinimum(-10000.000000000000000);
        doubleSpinBox_LightZ->setMaximum(10000.000000000000000);
        doubleSpinBox_LightZ->setSingleStep(0.100000000000000);
        doubleSpinBox_LightZ->setValue(3.000000000000000);

        gridLayout_19->addWidget(doubleSpinBox_LightZ, 0, 3, 1, 1);

        doubleSpinBox_AmbientR = new QDoubleSpinBox(groupBox);
        doubleSpinBox_AmbientR->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientR"));
        doubleSpinBox_AmbientR->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientR->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientR->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_AmbientR, 1, 1, 1, 1);

        doubleSpinBox_LightY = new QDoubleSpinBox(groupBox);
        doubleSpinBox_LightY->setObjectName(QString::fromUtf8("doubleSpinBox_LightY"));
        doubleSpinBox_LightY->setMinimum(-10000.000000000000000);
        doubleSpinBox_LightY->setMaximum(10000.000000000000000);
        doubleSpinBox_LightY->setSingleStep(0.100000000000000);
        doubleSpinBox_LightY->setValue(3.000000000000000);

        gridLayout_19->addWidget(doubleSpinBox_LightY, 0, 2, 1, 1);

        doubleSpinBox_LightX = new QDoubleSpinBox(groupBox);
        doubleSpinBox_LightX->setObjectName(QString::fromUtf8("doubleSpinBox_LightX"));
        doubleSpinBox_LightX->setMinimum(-10000.000000000000000);
        doubleSpinBox_LightX->setMaximum(10000.000000000000000);
        doubleSpinBox_LightX->setSingleStep(0.100000000000000);
        doubleSpinBox_LightX->setValue(3.000000000000000);

        gridLayout_19->addWidget(doubleSpinBox_LightX, 0, 1, 1, 1);

        checkBox_UseLIght0 = new QCheckBox(groupBox);
        checkBox_UseLIght0->setObjectName(QString::fromUtf8("checkBox_UseLIght0"));

        gridLayout_19->addWidget(checkBox_UseLIght0, 0, 0, 1, 1);

        doubleSpinBox_AmbientA = new QDoubleSpinBox(groupBox);
        doubleSpinBox_AmbientA->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientA"));
        doubleSpinBox_AmbientA->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientA->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientA->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_AmbientA, 1, 4, 1, 1);

        doubleSpinBox_AmbientG = new QDoubleSpinBox(groupBox);
        doubleSpinBox_AmbientG->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientG"));
        doubleSpinBox_AmbientG->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientG->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientG->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_AmbientG, 1, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(label_3, 3, 0, 1, 1);

        doubleSpinBox_AmbientB = new QDoubleSpinBox(groupBox);
        doubleSpinBox_AmbientB->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientB"));
        doubleSpinBox_AmbientB->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientB->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientB->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_AmbientB, 1, 3, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_19->addWidget(label, 1, 0, 1, 1);

        doubleSpinBox_DiffuseR = new QDoubleSpinBox(groupBox);
        doubleSpinBox_DiffuseR->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseR"));
        doubleSpinBox_DiffuseR->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseR->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseR->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_DiffuseR, 2, 1, 1, 1);

        doubleSpinBox_DiffuseG = new QDoubleSpinBox(groupBox);
        doubleSpinBox_DiffuseG->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseG"));
        doubleSpinBox_DiffuseG->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseG->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseG->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_DiffuseG, 2, 2, 1, 1);

        doubleSpinBox_DiffuseB = new QDoubleSpinBox(groupBox);
        doubleSpinBox_DiffuseB->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseB"));
        doubleSpinBox_DiffuseB->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseB->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseB->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_DiffuseB, 2, 3, 1, 1);

        doubleSpinBox_DiffuseA = new QDoubleSpinBox(groupBox);
        doubleSpinBox_DiffuseA->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseA"));
        doubleSpinBox_DiffuseA->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseA->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseA->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_DiffuseA, 2, 4, 1, 1);

        doubleSpinBox_SpecularR = new QDoubleSpinBox(groupBox);
        doubleSpinBox_SpecularR->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularR"));
        doubleSpinBox_SpecularR->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularR->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularR->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_SpecularR, 3, 1, 1, 1);

        doubleSpinBox_SpecularG = new QDoubleSpinBox(groupBox);
        doubleSpinBox_SpecularG->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularG"));
        doubleSpinBox_SpecularG->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularG->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularG->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_SpecularG, 3, 2, 1, 1);

        doubleSpinBox_SpecularB = new QDoubleSpinBox(groupBox);
        doubleSpinBox_SpecularB->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularB"));
        doubleSpinBox_SpecularB->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularB->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularB->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_SpecularB, 3, 3, 1, 1);

        doubleSpinBox_SpecularA = new QDoubleSpinBox(groupBox);
        doubleSpinBox_SpecularA->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularA"));
        doubleSpinBox_SpecularA->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularA->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularA->setSingleStep(0.100000000000000);

        gridLayout_19->addWidget(doubleSpinBox_SpecularA, 3, 4, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(LightSettingDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        doubleSpinBox_LightZ_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_LightZ_2->setObjectName(QString::fromUtf8("doubleSpinBox_LightZ_2"));
        doubleSpinBox_LightZ_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_LightZ_2->setMaximum(10000.000000000000000);
        doubleSpinBox_LightZ_2->setSingleStep(0.100000000000000);
        doubleSpinBox_LightZ_2->setValue(1.000000000000000);

        gridLayout->addWidget(doubleSpinBox_LightZ_2, 0, 3, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        doubleSpinBox_LightX_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_LightX_2->setObjectName(QString::fromUtf8("doubleSpinBox_LightX_2"));
        doubleSpinBox_LightX_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_LightX_2->setMaximum(10000.000000000000000);
        doubleSpinBox_LightX_2->setSingleStep(0.100000000000000);
        doubleSpinBox_LightX_2->setValue(2.000000000000000);

        gridLayout->addWidget(doubleSpinBox_LightX_2, 0, 1, 1, 1);

        doubleSpinBox_LightY_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_LightY_2->setObjectName(QString::fromUtf8("doubleSpinBox_LightY_2"));
        doubleSpinBox_LightY_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_LightY_2->setMaximum(10000.000000000000000);
        doubleSpinBox_LightY_2->setSingleStep(0.100000000000000);
        doubleSpinBox_LightY_2->setValue(-2.000000000000000);

        gridLayout->addWidget(doubleSpinBox_LightY_2, 0, 2, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        checkBox_UseLIght1 = new QCheckBox(groupBox_2);
        checkBox_UseLIght1->setObjectName(QString::fromUtf8("checkBox_UseLIght1"));

        gridLayout->addWidget(checkBox_UseLIght1, 0, 0, 1, 1);

        doubleSpinBox_AmbientB_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_AmbientB_2->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientB_2"));
        doubleSpinBox_AmbientB_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientB_2->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientB_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_AmbientB_2, 1, 3, 1, 1);

        doubleSpinBox_AmbientR_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_AmbientR_2->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientR_2"));
        doubleSpinBox_AmbientR_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientR_2->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientR_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_AmbientR_2, 1, 1, 1, 1);

        doubleSpinBox_AmbientG_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_AmbientG_2->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientG_2"));
        doubleSpinBox_AmbientG_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientG_2->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientG_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_AmbientG_2, 1, 2, 1, 1);

        doubleSpinBox_AmbientA_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_AmbientA_2->setObjectName(QString::fromUtf8("doubleSpinBox_AmbientA_2"));
        doubleSpinBox_AmbientA_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_AmbientA_2->setMaximum(10000.000000000000000);
        doubleSpinBox_AmbientA_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_AmbientA_2, 1, 4, 1, 1);

        doubleSpinBox_DiffuseR_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_DiffuseR_2->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseR_2"));
        doubleSpinBox_DiffuseR_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseR_2->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseR_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_DiffuseR_2, 2, 1, 1, 1);

        doubleSpinBox_DiffuseG_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_DiffuseG_2->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseG_2"));
        doubleSpinBox_DiffuseG_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseG_2->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseG_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_DiffuseG_2, 2, 2, 1, 1);

        doubleSpinBox_DiffuseB_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_DiffuseB_2->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseB_2"));
        doubleSpinBox_DiffuseB_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseB_2->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseB_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_DiffuseB_2, 2, 3, 1, 1);

        doubleSpinBox_DiffuseA_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_DiffuseA_2->setObjectName(QString::fromUtf8("doubleSpinBox_DiffuseA_2"));
        doubleSpinBox_DiffuseA_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_DiffuseA_2->setMaximum(10000.000000000000000);
        doubleSpinBox_DiffuseA_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_DiffuseA_2, 2, 4, 1, 1);

        doubleSpinBox_SpecularR_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_SpecularR_2->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularR_2"));
        doubleSpinBox_SpecularR_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularR_2->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularR_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_SpecularR_2, 3, 1, 1, 1);

        doubleSpinBox_SpecularG_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_SpecularG_2->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularG_2"));
        doubleSpinBox_SpecularG_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularG_2->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularG_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_SpecularG_2, 3, 2, 1, 1);

        doubleSpinBox_SpecularB_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_SpecularB_2->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularB_2"));
        doubleSpinBox_SpecularB_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularB_2->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularB_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_SpecularB_2, 3, 3, 1, 1);

        doubleSpinBox_SpecularA_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_SpecularA_2->setObjectName(QString::fromUtf8("doubleSpinBox_SpecularA_2"));
        doubleSpinBox_SpecularA_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_SpecularA_2->setMaximum(10000.000000000000000);
        doubleSpinBox_SpecularA_2->setSingleStep(0.100000000000000);

        gridLayout->addWidget(doubleSpinBox_SpecularA_2, 3, 4, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(LightSettingDialog);

        QMetaObject::connectSlotsByName(LightSettingDialog);
    } // setupUi

    void retranslateUi(QWidget *LightSettingDialog)
    {
        LightSettingDialog->setWindowTitle(QCoreApplication::translate("LightSettingDialog", "LightSettingDialog", nullptr));
        pushButton_ResetLightSources->setText(QCoreApplication::translate("LightSettingDialog", "Reset Lights", nullptr));
        checkBox_Draw_Lights->setText(QCoreApplication::translate("LightSettingDialog", "Draw Lights", nullptr));
        label_7->setText(QCoreApplication::translate("LightSettingDialog", "Glyph Size:", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("LightSettingDialog", "Shininess", nullptr));
        label_8->setText(QCoreApplication::translate("LightSettingDialog", "Cylinder", nullptr));
        label_9->setText(QCoreApplication::translate("LightSettingDialog", "Sphere", nullptr));
        groupBox->setTitle(QCoreApplication::translate("LightSettingDialog", "Light Setting", nullptr));
        checkBox_UseLIght0->setText(QCoreApplication::translate("LightSettingDialog", "Light 0", nullptr));
        label_3->setText(QCoreApplication::translate("LightSettingDialog", "Specular", nullptr));
        label_2->setText(QCoreApplication::translate("LightSettingDialog", "Diffuse", nullptr));
        label->setText(QCoreApplication::translate("LightSettingDialog", "Ambient", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("LightSettingDialog", "GroupBox", nullptr));
        label_4->setText(QCoreApplication::translate("LightSettingDialog", "Diffuse", nullptr));
        label_6->setText(QCoreApplication::translate("LightSettingDialog", "Specular", nullptr));
        label_5->setText(QCoreApplication::translate("LightSettingDialog", "Ambient", nullptr));
        checkBox_UseLIght1->setText(QCoreApplication::translate("LightSettingDialog", "Light 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LightSettingDialog: public Ui_LightSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTSETTINGDIALOG_H
