/********************************************************************************
** Form generated from reading UI file 'qcell.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCELL_H
#define UI_QCELL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QCell
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lblCellName;
    QSpacerItem *horizontalSpacer;
    QLineEdit *leCellValue;

    void setupUi(QWidget *QCell)
    {
        if (QCell->objectName().isEmpty())
            QCell->setObjectName(QStringLiteral("QCell"));
        QCell->resize(213, 46);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QCell->sizePolicy().hasHeightForWidth());
        QCell->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(QCell);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lblCellName = new QLabel(QCell);
        lblCellName->setObjectName(QStringLiteral("lblCellName"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblCellName->sizePolicy().hasHeightForWidth());
        lblCellName->setSizePolicy(sizePolicy1);
        lblCellName->setScaledContents(true);

        horizontalLayout->addWidget(lblCellName);

        horizontalSpacer = new QSpacerItem(1, 2, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        leCellValue = new QLineEdit(QCell);
        leCellValue->setObjectName(QStringLiteral("leCellValue"));
        sizePolicy.setHeightForWidth(leCellValue->sizePolicy().hasHeightForWidth());
        leCellValue->setSizePolicy(sizePolicy);
        leCellValue->setMaxLength(6);
        leCellValue->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(leCellValue);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(QCell);

        QMetaObject::connectSlotsByName(QCell);
    } // setupUi

    void retranslateUi(QWidget *QCell)
    {
        QCell->setWindowTitle(QApplication::translate("QCell", "Form", nullptr));
        lblCellName->setText(QApplication::translate("QCell", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QCell: public Ui_QCell {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCELL_H
