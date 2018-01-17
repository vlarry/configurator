/********************************************************************************
** Form generated from reading UI file 'versionform.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERSIONFORM_H
#define UI_VERSIONFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CVersionSoftware
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *CVersionSoftware)
    {
        if (CVersionSoftware->objectName().isEmpty())
            CVersionSoftware->setObjectName(QStringLiteral("CVersionSoftware"));
        CVersionSoftware->setWindowModality(Qt::WindowModal);
        CVersionSoftware->resize(727, 422);
        verticalLayout = new QVBoxLayout(CVersionSoftware);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEdit = new QTextEdit(CVersionSoftware);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);


        retranslateUi(CVersionSoftware);

        QMetaObject::connectSlotsByName(CVersionSoftware);
    } // setupUi

    void retranslateUi(QWidget *CVersionSoftware)
    {
        CVersionSoftware->setWindowTitle(QApplication::translate("CVersionSoftware", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CVersionSoftware: public Ui_CVersionSoftware {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERSIONFORM_H
