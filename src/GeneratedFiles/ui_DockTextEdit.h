/********************************************************************************
** Form generated from reading UI file 'DockTextEdit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCKTEXTEDIT_H
#define UI_DOCKTEXTEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DockTextEdit
{
public:
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *DockTextEdit)
    {
        if (DockTextEdit->objectName().isEmpty())
            DockTextEdit->setObjectName(QString::fromUtf8("DockTextEdit"));
        DockTextEdit->resize(400, 300);
        horizontalLayout = new QHBoxLayout(DockTextEdit);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        textEdit = new QTextEdit(DockTextEdit);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setLayoutDirection(Qt::LeftToRight);
        textEdit->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        textEdit->setUndoRedoEnabled(false);
        textEdit->setLineWrapMode(QTextEdit::NoWrap);
        textEdit->setReadOnly(true);

        horizontalLayout->addWidget(textEdit);


        retranslateUi(DockTextEdit);

        QMetaObject::connectSlotsByName(DockTextEdit);
    } // setupUi

    void retranslateUi(QWidget *DockTextEdit)
    {
        DockTextEdit->setWindowTitle(QCoreApplication::translate("DockTextEdit", "DockTextEdit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DockTextEdit: public Ui_DockTextEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCKTEXTEDIT_H
