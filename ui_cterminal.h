/********************************************************************************
** Form generated from reading UI file 'cterminal.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTERMINAL_H
#define UI_CTERMINAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CTerminal
{
public:
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *pteConsole;

    void setupUi(QWidget *CTerminal)
    {
        if (CTerminal->objectName().isEmpty())
            CTerminal->setObjectName(QStringLiteral("CTerminal"));
        CTerminal->resize(420, 253);
        QFont font;
        font.setFamily(QStringLiteral("Consolas"));
        font.setPointSize(10);
        CTerminal->setFont(font);
        horizontalLayout = new QHBoxLayout(CTerminal);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pteConsole = new QPlainTextEdit(CTerminal);
        pteConsole->setObjectName(QStringLiteral("pteConsole"));

        horizontalLayout->addWidget(pteConsole);


        retranslateUi(CTerminal);

        QMetaObject::connectSlotsByName(CTerminal);
    } // setupUi

    void retranslateUi(QWidget *CTerminal)
    {
        CTerminal->setWindowTitle(QApplication::translate("CTerminal", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CTerminal: public Ui_CTerminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTERMINAL_H
