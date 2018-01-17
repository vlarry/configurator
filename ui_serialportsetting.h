/********************************************************************************
** Form generated from reading UI file 'serialportsetting.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALPORTSETTING_H
#define UI_SERIALPORTSETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CSerialPortSetting
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *gboxSerialPortSettings;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *gridLayout_2;
    QLabel *lblTextParity;
    QComboBox *cboxParity;
    QLabel *lblTextStopBits;
    QLabel *lblTextDataBits;
    QComboBox *cboxStopBit;
    QComboBox *cboxDataBit;
    QGroupBox *gboxSerialPortSettings_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_4;
    QLabel *lblTextTimeout;
    QSpinBox *sboxTimeout;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_7;
    QLabel *lblTextNumRepeat;
    QSpinBox *sboxNumRepeat;
    QSpacerItem *horizontalSpacer_12;

    void setupUi(QWidget *CSerialPortSetting)
    {
        if (CSerialPortSetting->objectName().isEmpty())
            CSerialPortSetting->setObjectName(QStringLiteral("CSerialPortSetting"));
        CSerialPortSetting->setWindowModality(Qt::WindowModal);
        CSerialPortSetting->resize(492, 142);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CSerialPortSetting->sizePolicy().hasHeightForWidth());
        CSerialPortSetting->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(CSerialPortSetting);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gboxSerialPortSettings = new QGroupBox(CSerialPortSetting);
        gboxSerialPortSettings->setObjectName(QStringLiteral("gboxSerialPortSettings"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gboxSerialPortSettings->sizePolicy().hasHeightForWidth());
        gboxSerialPortSettings->setSizePolicy(sizePolicy1);
        horizontalLayout_3 = new QHBoxLayout(gboxSerialPortSettings);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 5, 5, 5);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout_2->setVerticalSpacing(0);
        lblTextParity = new QLabel(gboxSerialPortSettings);
        lblTextParity->setObjectName(QStringLiteral("lblTextParity"));
        lblTextParity->setScaledContents(true);

        gridLayout_2->addWidget(lblTextParity, 2, 0, 1, 1);

        cboxParity = new QComboBox(gboxSerialPortSettings);
        cboxParity->addItem(QString());
        cboxParity->addItem(QString());
        cboxParity->addItem(QString());
        cboxParity->addItem(QString());
        cboxParity->addItem(QString());
        cboxParity->setObjectName(QStringLiteral("cboxParity"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cboxParity->sizePolicy().hasHeightForWidth());
        cboxParity->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(cboxParity, 2, 1, 1, 1);

        lblTextStopBits = new QLabel(gboxSerialPortSettings);
        lblTextStopBits->setObjectName(QStringLiteral("lblTextStopBits"));
        lblTextStopBits->setScaledContents(true);

        gridLayout_2->addWidget(lblTextStopBits, 1, 0, 1, 1);

        lblTextDataBits = new QLabel(gboxSerialPortSettings);
        lblTextDataBits->setObjectName(QStringLiteral("lblTextDataBits"));
        lblTextDataBits->setScaledContents(true);

        gridLayout_2->addWidget(lblTextDataBits, 0, 0, 1, 1);

        cboxStopBit = new QComboBox(gboxSerialPortSettings);
        cboxStopBit->addItem(QString());
        cboxStopBit->addItem(QString());
        cboxStopBit->addItem(QString());
        cboxStopBit->setObjectName(QStringLiteral("cboxStopBit"));
        sizePolicy2.setHeightForWidth(cboxStopBit->sizePolicy().hasHeightForWidth());
        cboxStopBit->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(cboxStopBit, 1, 1, 1, 1);

        cboxDataBit = new QComboBox(gboxSerialPortSettings);
        cboxDataBit->addItem(QString());
        cboxDataBit->addItem(QString());
        cboxDataBit->addItem(QString());
        cboxDataBit->addItem(QString());
        cboxDataBit->setObjectName(QStringLiteral("cboxDataBit"));
        sizePolicy2.setHeightForWidth(cboxDataBit->sizePolicy().hasHeightForWidth());
        cboxDataBit->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(cboxDataBit, 0, 1, 1, 1);


        horizontalLayout_3->addLayout(gridLayout_2);


        horizontalLayout->addWidget(gboxSerialPortSettings);

        gboxSerialPortSettings_2 = new QGroupBox(CSerialPortSetting);
        gboxSerialPortSettings_2->setObjectName(QStringLiteral("gboxSerialPortSettings_2"));
        sizePolicy1.setHeightForWidth(gboxSerialPortSettings_2->sizePolicy().hasHeightForWidth());
        gboxSerialPortSettings_2->setSizePolicy(sizePolicy1);
        gboxSerialPortSettings_2->setAlignment(Qt::AlignCenter);
        verticalLayout = new QVBoxLayout(gboxSerialPortSettings_2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        lblTextTimeout = new QLabel(gboxSerialPortSettings_2);
        lblTextTimeout->setObjectName(QStringLiteral("lblTextTimeout"));
        lblTextTimeout->setScaledContents(true);

        gridLayout_4->addWidget(lblTextTimeout, 0, 0, 1, 1);

        sboxTimeout = new QSpinBox(gboxSerialPortSettings_2);
        sboxTimeout->setObjectName(QStringLiteral("sboxTimeout"));
        sboxTimeout->setMaximumSize(QSize(75, 75));
        sboxTimeout->setMinimum(100);
        sboxTimeout->setMaximum(1000);
        sboxTimeout->setSingleStep(100);
        sboxTimeout->setValue(500);

        gridLayout_4->addWidget(sboxTimeout, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        lblTextNumRepeat = new QLabel(gboxSerialPortSettings_2);
        lblTextNumRepeat->setObjectName(QStringLiteral("lblTextNumRepeat"));
        lblTextNumRepeat->setScaledContents(true);

        gridLayout_7->addWidget(lblTextNumRepeat, 0, 0, 1, 1);

        sboxNumRepeat = new QSpinBox(gboxSerialPortSettings_2);
        sboxNumRepeat->setObjectName(QStringLiteral("sboxNumRepeat"));
        sboxNumRepeat->setMaximumSize(QSize(75, 75));
        sboxNumRepeat->setMinimum(1);
        sboxNumRepeat->setMaximum(20);
        sboxNumRepeat->setValue(3);

        gridLayout_7->addWidget(sboxNumRepeat, 0, 2, 1, 1);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_12, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_7);


        horizontalLayout->addWidget(gboxSerialPortSettings_2);


        retranslateUi(CSerialPortSetting);

        cboxParity->setCurrentIndex(1);
        cboxStopBit->setCurrentIndex(0);
        cboxDataBit->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(CSerialPortSetting);
    } // setupUi

    void retranslateUi(QWidget *CSerialPortSetting)
    {
        CSerialPortSetting->setWindowTitle(QApplication::translate("CSerialPortSetting", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 Com-\320\277\320\276\321\200\321\202\320\260", nullptr));
        gboxSerialPortSettings->setTitle(QApplication::translate("CSerialPortSetting", "\320\237\320\276\321\201\320\273\320\265\320\264\320\276\320\262\320\260\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \320\277\320\276\321\200\321\202", nullptr));
        lblTextParity->setText(QApplication::translate("CSerialPortSetting", "\320\247\320\265\321\202\320\275\320\276\321\201\321\202\321\214", nullptr));
        cboxParity->setItemText(0, QApplication::translate("CSerialPortSetting", "No", nullptr));
        cboxParity->setItemText(1, QApplication::translate("CSerialPortSetting", "Even", nullptr));
        cboxParity->setItemText(2, QApplication::translate("CSerialPortSetting", "Odd", nullptr));
        cboxParity->setItemText(3, QApplication::translate("CSerialPortSetting", "Mark", nullptr));
        cboxParity->setItemText(4, QApplication::translate("CSerialPortSetting", "Space", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxParity->setToolTip(QApplication::translate("CSerialPortSetting", "\320\247\320\265\321\202\320\275\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextStopBits->setText(QApplication::translate("CSerialPortSetting", "\320\241\321\202\320\276\320\277 \320\261\320\270\321\202", nullptr));
        lblTextDataBits->setText(QApplication::translate("CSerialPortSetting", "\320\221\320\270\321\202 \320\264\320\260\320\275\320\275\321\213\321\205", nullptr));
        cboxStopBit->setItemText(0, QApplication::translate("CSerialPortSetting", "1", nullptr));
        cboxStopBit->setItemText(1, QApplication::translate("CSerialPortSetting", "1.5", nullptr));
        cboxStopBit->setItemText(2, QApplication::translate("CSerialPortSetting", "2", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxStopBit->setToolTip(QApplication::translate("CSerialPortSetting", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \321\201\321\202\320\276\320\277 \320\261\320\270\321\202", nullptr));
#endif // QT_NO_TOOLTIP
        cboxDataBit->setItemText(0, QApplication::translate("CSerialPortSetting", "5", nullptr));
        cboxDataBit->setItemText(1, QApplication::translate("CSerialPortSetting", "6", nullptr));
        cboxDataBit->setItemText(2, QApplication::translate("CSerialPortSetting", "7", nullptr));
        cboxDataBit->setItemText(3, QApplication::translate("CSerialPortSetting", "8", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxDataBit->setToolTip(QApplication::translate("CSerialPortSetting", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\261\320\270\321\202", nullptr));
#endif // QT_NO_TOOLTIP
        gboxSerialPortSettings_2->setTitle(QApplication::translate("CSerialPortSetting", "Modbus", nullptr));
        lblTextTimeout->setText(QApplication::translate("CSerialPortSetting", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217 \320\276\321\202\320\262\320\265\321\202\320\260, \320\274\321\201", nullptr));
#ifndef QT_NO_TOOLTIP
        sboxTimeout->setToolTip(QApplication::translate("CSerialPortSetting", "\320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265 \320\276\321\202\320\262\320\265\321\202\320\260 \320\276\321\202 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextNumRepeat->setText(QApplication::translate("CSerialPortSetting", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\277\320\276\320\277\321\213\321\202\320\276\320\272", nullptr));
#ifndef QT_NO_TOOLTIP
        sboxNumRepeat->setToolTip(QApplication::translate("CSerialPortSetting", "\320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265 \320\276\321\202\320\262\320\265\321\202\320\260 \320\276\321\202 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class CSerialPortSetting: public Ui_CSerialPortSetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALPORTSETTING_H
