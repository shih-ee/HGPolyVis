/********************************************************************************
** Form generated from reading UI file 'DockSelectElement.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCKSELECTELEMENT_H
#define UI_DOCKSELECTELEMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DockSelectElement
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QGroupBox *groupBox_Select;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton_Lables;
    QRadioButton *radioButton_indices;
    QLineEdit *lineEdit_SelectInput;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_SelectEntities;
    QPushButton *pushButton_SelectRelationships;
    QPushButton *pushButton_HighlightSelectedElements;

    void setupUi(QWidget *DockSelectElement)
    {
        if (DockSelectElement->objectName().isEmpty())
            DockSelectElement->setObjectName(QString::fromUtf8("DockSelectElement"));
        DockSelectElement->resize(239, 206);
        verticalLayout = new QVBoxLayout(DockSelectElement);
        verticalLayout->setSpacing(2);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        textEdit = new QTextEdit(DockSelectElement);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy);
        textEdit->setAcceptDrops(false);
        textEdit->setAutoFillBackground(true);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);

        groupBox_Select = new QGroupBox(DockSelectElement);
        groupBox_Select->setObjectName(QString::fromUtf8("groupBox_Select"));
        verticalLayout_2 = new QVBoxLayout(groupBox_Select);
        verticalLayout_2->setSpacing(2);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(2, 2, 2, 2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, -1, 0);
        radioButton_Lables = new QRadioButton(groupBox_Select);
        radioButton_Lables->setObjectName(QString::fromUtf8("radioButton_Lables"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radioButton_Lables->sizePolicy().hasHeightForWidth());
        radioButton_Lables->setSizePolicy(sizePolicy1);
        radioButton_Lables->setMaximumSize(QSize(16777215, 16777215));
        radioButton_Lables->setChecked(false);

        horizontalLayout->addWidget(radioButton_Lables);

        radioButton_indices = new QRadioButton(groupBox_Select);
        radioButton_indices->setObjectName(QString::fromUtf8("radioButton_indices"));
        sizePolicy1.setHeightForWidth(radioButton_indices->sizePolicy().hasHeightForWidth());
        radioButton_indices->setSizePolicy(sizePolicy1);
        radioButton_indices->setMaximumSize(QSize(16777215, 16777215));
        radioButton_indices->setChecked(true);

        horizontalLayout->addWidget(radioButton_indices);

        lineEdit_SelectInput = new QLineEdit(groupBox_Select);
        lineEdit_SelectInput->setObjectName(QString::fromUtf8("lineEdit_SelectInput"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEdit_SelectInput->sizePolicy().hasHeightForWidth());
        lineEdit_SelectInput->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(lineEdit_SelectInput);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, -1, 0);
        pushButton_SelectEntities = new QPushButton(groupBox_Select);
        pushButton_SelectEntities->setObjectName(QString::fromUtf8("pushButton_SelectEntities"));
        sizePolicy1.setHeightForWidth(pushButton_SelectEntities->sizePolicy().hasHeightForWidth());
        pushButton_SelectEntities->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(pushButton_SelectEntities);

        pushButton_SelectRelationships = new QPushButton(groupBox_Select);
        pushButton_SelectRelationships->setObjectName(QString::fromUtf8("pushButton_SelectRelationships"));
        sizePolicy1.setHeightForWidth(pushButton_SelectRelationships->sizePolicy().hasHeightForWidth());
        pushButton_SelectRelationships->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(pushButton_SelectRelationships);

        pushButton_HighlightSelectedElements = new QPushButton(groupBox_Select);
        pushButton_HighlightSelectedElements->setObjectName(QString::fromUtf8("pushButton_HighlightSelectedElements"));
        sizePolicy1.setHeightForWidth(pushButton_HighlightSelectedElements->sizePolicy().hasHeightForWidth());
        pushButton_HighlightSelectedElements->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(pushButton_HighlightSelectedElements);


        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout->addWidget(groupBox_Select);


        retranslateUi(DockSelectElement);

        QMetaObject::connectSlotsByName(DockSelectElement);
    } // setupUi

    void retranslateUi(QWidget *DockSelectElement)
    {
        DockSelectElement->setWindowTitle(QCoreApplication::translate("DockSelectElement", "DockSelectElement", nullptr));
        groupBox_Select->setTitle(QCoreApplication::translate("DockSelectElement", "Select Elements", nullptr));
        radioButton_Lables->setText(QCoreApplication::translate("DockSelectElement", "Labels", nullptr));
        radioButton_indices->setText(QCoreApplication::translate("DockSelectElement", "Indices", nullptr));
        pushButton_SelectEntities->setText(QCoreApplication::translate("DockSelectElement", "Select En", nullptr));
        pushButton_SelectRelationships->setText(QCoreApplication::translate("DockSelectElement", "Select Re", nullptr));
        pushButton_HighlightSelectedElements->setText(QCoreApplication::translate("DockSelectElement", "HighLight", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DockSelectElement: public Ui_DockSelectElement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCKSELECTELEMENT_H
