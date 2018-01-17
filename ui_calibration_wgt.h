/********************************************************************************
** Form generated from reading UI file 'calibration_wgt.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATION_WGT_H
#define UI_CALIBRATION_WGT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_calibration_wgt
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_8;
    QLabel *lblTextCalibFactorPowerPhase_B_C;
    QLabel *lblTextCalibFactorPowerUAx;
    QLabel *lblTextCalibFactorPowerUBx;
    QLabel *lblTextCalibFactorChannel3U0;
    QLabel *lblTextCalibFactorPowerUCx;
    QLabel *lblTextCalibFactorPowerPhase_A_B;
    QLabel *lblTextCalibFactorPower3U0x;
    QLineEdit *leTextCalibFactorPowerPhase_B_C;
    QLineEdit *leTextCalibFactorPowerPhase_C_A;
    QLineEdit *leTextCalibFactorPowerPhase_A_B;
    QLineEdit *leTextCalibFactorPowerUCx;
    QLineEdit *leTextCalibFactorPower3U0x;
    QLineEdit *leTextCalibFactorChannel3Us;
    QLineEdit *leTextCalibFactorChannel3U0;
    QLineEdit *leTextCalibFactorPowerUAx;
    QLineEdit *leTextCalibFactorPowerUBx;
    QLabel *lblTextCalibFactorCurrent3I0;
    QLineEdit *leTextCalibFactorPowerPhase_B;
    QLabel *lblTextCalibFactorPowerPhase_C;
    QLineEdit *leTextCalibFactorPower3I0;
    QLabel *lblTextCalibFactorPower3U0;
    QLabel *lblTextCalibFactorPowerPhase_B;
    QLabel *lblTextCalibFactorPowerPhase_A;
    QLineEdit *leTextCalibFactorPowerPhase_A;
    QLineEdit *leTextCalibFactorPowerPhase_C;
    QLineEdit *leTextCalibFactorPowerTotal;
    QLineEdit *leTextCalibFactorCurrent3I0;
    QLabel *lblTextCalibFactorPowerTotal;
    QLabel *lblTextCalibFactorCurrentPhase_A;
    QLabel *lblTextCalibFactorCurrentPhase_B;
    QLineEdit *leTextCalibFactorCurrentPhase_A;
    QLineEdit *leTextCalibFactorCurrentPhase_B;
    QLabel *lblTextCalibFactorCurrentPhase_C;
    QLineEdit *leTextCalibFactorCurrentPhase_C;
    QLabel *lblTextCalibFactorPowerPhase_C_A;
    QLabel *lblTextCalibFactorChannel3Us;
    QSpacerItem *horizontalSpacer_24;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *calibration_wgt)
    {
        if (calibration_wgt->objectName().isEmpty())
            calibration_wgt->setObjectName(QStringLiteral("calibration_wgt"));
        calibration_wgt->resize(697, 378);
        verticalLayout = new QVBoxLayout(calibration_wgt);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(2);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setSizeConstraint(QLayout::SetMinimumSize);
        lblTextCalibFactorPowerPhase_B_C = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerPhase_B_C->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_B_C"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerPhase_B_C->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_B_C->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerPhase_B_C->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_B_C, 1, 3, 1, 1);

        lblTextCalibFactorPowerUAx = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerUAx->setObjectName(QStringLiteral("lblTextCalibFactorPowerUAx"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerUAx->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerUAx->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerUAx->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerUAx, 4, 3, 1, 1);

        lblTextCalibFactorPowerUBx = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerUBx->setObjectName(QStringLiteral("lblTextCalibFactorPowerUBx"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerUBx->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerUBx->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerUBx->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerUBx, 5, 3, 1, 1);

        lblTextCalibFactorChannel3U0 = new QLabel(calibration_wgt);
        lblTextCalibFactorChannel3U0->setObjectName(QStringLiteral("lblTextCalibFactorChannel3U0"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorChannel3U0->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorChannel3U0->setSizePolicy(sizePolicy);
        lblTextCalibFactorChannel3U0->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorChannel3U0, 7, 3, 1, 1);

        lblTextCalibFactorPowerUCx = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerUCx->setObjectName(QStringLiteral("lblTextCalibFactorPowerUCx"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerUCx->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerUCx->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerUCx->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerUCx, 6, 3, 1, 1);

        lblTextCalibFactorPowerPhase_A_B = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerPhase_A_B->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_A_B"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerPhase_A_B->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_A_B->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerPhase_A_B->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_A_B, 0, 3, 1, 1);

        lblTextCalibFactorPower3U0x = new QLabel(calibration_wgt);
        lblTextCalibFactorPower3U0x->setObjectName(QStringLiteral("lblTextCalibFactorPower3U0x"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPower3U0x->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPower3U0x->setSizePolicy(sizePolicy);
        lblTextCalibFactorPower3U0x->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPower3U0x, 3, 3, 1, 1);

        leTextCalibFactorPowerPhase_B_C = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerPhase_B_C->setObjectName(QStringLiteral("leTextCalibFactorPowerPhase_B_C"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerPhase_B_C->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerPhase_B_C->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerPhase_B_C->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerPhase_B_C->setMaxLength(6);
        leTextCalibFactorPowerPhase_B_C->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerPhase_B_C, 1, 4, 1, 1);

        leTextCalibFactorPowerPhase_C_A = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerPhase_C_A->setObjectName(QStringLiteral("leTextCalibFactorPowerPhase_C_A"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerPhase_C_A->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerPhase_C_A->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerPhase_C_A->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerPhase_C_A->setMaxLength(6);
        leTextCalibFactorPowerPhase_C_A->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerPhase_C_A, 2, 4, 1, 1);

        leTextCalibFactorPowerPhase_A_B = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerPhase_A_B->setObjectName(QStringLiteral("leTextCalibFactorPowerPhase_A_B"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerPhase_A_B->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerPhase_A_B->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerPhase_A_B->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerPhase_A_B->setMaxLength(6);
        leTextCalibFactorPowerPhase_A_B->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerPhase_A_B, 0, 4, 1, 1);

        leTextCalibFactorPowerUCx = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerUCx->setObjectName(QStringLiteral("leTextCalibFactorPowerUCx"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerUCx->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerUCx->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerUCx->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerUCx->setMaxLength(6);
        leTextCalibFactorPowerUCx->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerUCx, 6, 4, 1, 1);

        leTextCalibFactorPower3U0x = new QLineEdit(calibration_wgt);
        leTextCalibFactorPower3U0x->setObjectName(QStringLiteral("leTextCalibFactorPower3U0x"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPower3U0x->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPower3U0x->setSizePolicy(sizePolicy);
        leTextCalibFactorPower3U0x->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPower3U0x->setMaxLength(6);
        leTextCalibFactorPower3U0x->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPower3U0x, 3, 4, 1, 1);

        leTextCalibFactorChannel3Us = new QLineEdit(calibration_wgt);
        leTextCalibFactorChannel3Us->setObjectName(QStringLiteral("leTextCalibFactorChannel3Us"));
        sizePolicy.setHeightForWidth(leTextCalibFactorChannel3Us->sizePolicy().hasHeightForWidth());
        leTextCalibFactorChannel3Us->setSizePolicy(sizePolicy);
        leTextCalibFactorChannel3Us->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorChannel3Us->setMaxLength(6);
        leTextCalibFactorChannel3Us->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorChannel3Us, 8, 4, 1, 1);

        leTextCalibFactorChannel3U0 = new QLineEdit(calibration_wgt);
        leTextCalibFactorChannel3U0->setObjectName(QStringLiteral("leTextCalibFactorChannel3U0"));
        sizePolicy.setHeightForWidth(leTextCalibFactorChannel3U0->sizePolicy().hasHeightForWidth());
        leTextCalibFactorChannel3U0->setSizePolicy(sizePolicy);
        leTextCalibFactorChannel3U0->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorChannel3U0->setMaxLength(6);
        leTextCalibFactorChannel3U0->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorChannel3U0, 7, 4, 1, 1);

        leTextCalibFactorPowerUAx = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerUAx->setObjectName(QStringLiteral("leTextCalibFactorPowerUAx"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerUAx->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerUAx->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerUAx->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerUAx->setMaxLength(6);
        leTextCalibFactorPowerUAx->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerUAx, 4, 4, 1, 1);

        leTextCalibFactorPowerUBx = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerUBx->setObjectName(QStringLiteral("leTextCalibFactorPowerUBx"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerUBx->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerUBx->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerUBx->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerUBx->setMaxLength(6);
        leTextCalibFactorPowerUBx->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerUBx, 5, 4, 1, 1);

        lblTextCalibFactorCurrent3I0 = new QLabel(calibration_wgt);
        lblTextCalibFactorCurrent3I0->setObjectName(QStringLiteral("lblTextCalibFactorCurrent3I0"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorCurrent3I0->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrent3I0->setSizePolicy(sizePolicy);
        lblTextCalibFactorCurrent3I0->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrent3I0, 3, 0, 1, 1);

        leTextCalibFactorPowerPhase_B = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerPhase_B->setObjectName(QStringLiteral("leTextCalibFactorPowerPhase_B"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerPhase_B->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerPhase_B->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerPhase_B->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerPhase_B->setMaxLength(6);
        leTextCalibFactorPowerPhase_B->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerPhase_B, 5, 1, 1, 1);

        lblTextCalibFactorPowerPhase_C = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerPhase_C->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_C"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerPhase_C->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_C->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerPhase_C->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_C, 6, 0, 1, 1);

        leTextCalibFactorPower3I0 = new QLineEdit(calibration_wgt);
        leTextCalibFactorPower3I0->setObjectName(QStringLiteral("leTextCalibFactorPower3I0"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPower3I0->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPower3I0->setSizePolicy(sizePolicy);
        leTextCalibFactorPower3I0->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPower3I0->setMaxLength(6);
        leTextCalibFactorPower3I0->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPower3I0, 7, 1, 1, 1);

        lblTextCalibFactorPower3U0 = new QLabel(calibration_wgt);
        lblTextCalibFactorPower3U0->setObjectName(QStringLiteral("lblTextCalibFactorPower3U0"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPower3U0->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPower3U0->setSizePolicy(sizePolicy);
        lblTextCalibFactorPower3U0->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPower3U0, 7, 0, 1, 1);

        lblTextCalibFactorPowerPhase_B = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerPhase_B->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_B"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerPhase_B->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_B->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerPhase_B->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_B, 5, 0, 1, 1);

        lblTextCalibFactorPowerPhase_A = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerPhase_A->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_A"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerPhase_A->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_A->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerPhase_A->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_A, 4, 0, 1, 1);

        leTextCalibFactorPowerPhase_A = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerPhase_A->setObjectName(QStringLiteral("leTextCalibFactorPowerPhase_A"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerPhase_A->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerPhase_A->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerPhase_A->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerPhase_A->setMaxLength(6);
        leTextCalibFactorPowerPhase_A->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerPhase_A, 4, 1, 1, 1);

        leTextCalibFactorPowerPhase_C = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerPhase_C->setObjectName(QStringLiteral("leTextCalibFactorPowerPhase_C"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerPhase_C->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerPhase_C->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerPhase_C->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerPhase_C->setMaxLength(6);
        leTextCalibFactorPowerPhase_C->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerPhase_C, 6, 1, 1, 1);

        leTextCalibFactorPowerTotal = new QLineEdit(calibration_wgt);
        leTextCalibFactorPowerTotal->setObjectName(QStringLiteral("leTextCalibFactorPowerTotal"));
        sizePolicy.setHeightForWidth(leTextCalibFactorPowerTotal->sizePolicy().hasHeightForWidth());
        leTextCalibFactorPowerTotal->setSizePolicy(sizePolicy);
        leTextCalibFactorPowerTotal->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorPowerTotal->setMaxLength(6);
        leTextCalibFactorPowerTotal->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorPowerTotal, 8, 1, 1, 1);

        leTextCalibFactorCurrent3I0 = new QLineEdit(calibration_wgt);
        leTextCalibFactorCurrent3I0->setObjectName(QStringLiteral("leTextCalibFactorCurrent3I0"));
        sizePolicy.setHeightForWidth(leTextCalibFactorCurrent3I0->sizePolicy().hasHeightForWidth());
        leTextCalibFactorCurrent3I0->setSizePolicy(sizePolicy);
        leTextCalibFactorCurrent3I0->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorCurrent3I0->setMaxLength(6);
        leTextCalibFactorCurrent3I0->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorCurrent3I0, 3, 1, 1, 1);

        lblTextCalibFactorPowerTotal = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerTotal->setObjectName(QStringLiteral("lblTextCalibFactorPowerTotal"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerTotal->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerTotal->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerTotal->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerTotal, 8, 0, 1, 1);

        lblTextCalibFactorCurrentPhase_A = new QLabel(calibration_wgt);
        lblTextCalibFactorCurrentPhase_A->setObjectName(QStringLiteral("lblTextCalibFactorCurrentPhase_A"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorCurrentPhase_A->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrentPhase_A->setSizePolicy(sizePolicy);
        lblTextCalibFactorCurrentPhase_A->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrentPhase_A, 0, 0, 1, 1);

        lblTextCalibFactorCurrentPhase_B = new QLabel(calibration_wgt);
        lblTextCalibFactorCurrentPhase_B->setObjectName(QStringLiteral("lblTextCalibFactorCurrentPhase_B"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorCurrentPhase_B->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrentPhase_B->setSizePolicy(sizePolicy);
        lblTextCalibFactorCurrentPhase_B->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrentPhase_B, 1, 0, 1, 1);

        leTextCalibFactorCurrentPhase_A = new QLineEdit(calibration_wgt);
        leTextCalibFactorCurrentPhase_A->setObjectName(QStringLiteral("leTextCalibFactorCurrentPhase_A"));
        sizePolicy.setHeightForWidth(leTextCalibFactorCurrentPhase_A->sizePolicy().hasHeightForWidth());
        leTextCalibFactorCurrentPhase_A->setSizePolicy(sizePolicy);
        leTextCalibFactorCurrentPhase_A->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorCurrentPhase_A->setMaxLength(6);
        leTextCalibFactorCurrentPhase_A->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorCurrentPhase_A, 0, 1, 1, 1);

        leTextCalibFactorCurrentPhase_B = new QLineEdit(calibration_wgt);
        leTextCalibFactorCurrentPhase_B->setObjectName(QStringLiteral("leTextCalibFactorCurrentPhase_B"));
        sizePolicy.setHeightForWidth(leTextCalibFactorCurrentPhase_B->sizePolicy().hasHeightForWidth());
        leTextCalibFactorCurrentPhase_B->setSizePolicy(sizePolicy);
        leTextCalibFactorCurrentPhase_B->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorCurrentPhase_B->setMaxLength(6);
        leTextCalibFactorCurrentPhase_B->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorCurrentPhase_B, 1, 1, 1, 1);

        lblTextCalibFactorCurrentPhase_C = new QLabel(calibration_wgt);
        lblTextCalibFactorCurrentPhase_C->setObjectName(QStringLiteral("lblTextCalibFactorCurrentPhase_C"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorCurrentPhase_C->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrentPhase_C->setSizePolicy(sizePolicy);
        lblTextCalibFactorCurrentPhase_C->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrentPhase_C, 2, 0, 1, 1);

        leTextCalibFactorCurrentPhase_C = new QLineEdit(calibration_wgt);
        leTextCalibFactorCurrentPhase_C->setObjectName(QStringLiteral("leTextCalibFactorCurrentPhase_C"));
        sizePolicy.setHeightForWidth(leTextCalibFactorCurrentPhase_C->sizePolicy().hasHeightForWidth());
        leTextCalibFactorCurrentPhase_C->setSizePolicy(sizePolicy);
        leTextCalibFactorCurrentPhase_C->setMaximumSize(QSize(75, 16777215));
        leTextCalibFactorCurrentPhase_C->setMaxLength(6);
        leTextCalibFactorCurrentPhase_C->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leTextCalibFactorCurrentPhase_C, 2, 1, 1, 1);

        lblTextCalibFactorPowerPhase_C_A = new QLabel(calibration_wgt);
        lblTextCalibFactorPowerPhase_C_A->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_C_A"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorPowerPhase_C_A->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_C_A->setSizePolicy(sizePolicy);
        lblTextCalibFactorPowerPhase_C_A->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_C_A, 2, 3, 1, 1);

        lblTextCalibFactorChannel3Us = new QLabel(calibration_wgt);
        lblTextCalibFactorChannel3Us->setObjectName(QStringLiteral("lblTextCalibFactorChannel3Us"));
        sizePolicy.setHeightForWidth(lblTextCalibFactorChannel3Us->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorChannel3Us->setSizePolicy(sizePolicy);
        lblTextCalibFactorChannel3Us->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorChannel3Us, 8, 3, 1, 1);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_24, 0, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout_8);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 151, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(calibration_wgt);

        QMetaObject::connectSlotsByName(calibration_wgt);
    } // setupUi

    void retranslateUi(QWidget *calibration_wgt)
    {
        calibration_wgt->setWindowTitle(QApplication::translate("calibration_wgt", "Form", Q_NULLPTR));
        lblTextCalibFactorPowerPhase_B_C->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203\321\204\320\260\320\267\320\275\320\276\320\265 \320\222-\320\241", Q_NULLPTR));
        lblTextCalibFactorPowerUAx->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 UAx", Q_NULLPTR));
        lblTextCalibFactorPowerUBx->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 UBx", Q_NULLPTR));
        lblTextCalibFactorChannel3U0->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\272\320\260\320\275\320\260\320\273\320\260 3U0", Q_NULLPTR));
        lblTextCalibFactorPowerUCx->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 UCx", Q_NULLPTR));
        lblTextCalibFactorPowerPhase_A_B->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203\321\204\320\260\320\267\320\275\320\276\320\265 \320\220-\320\222", Q_NULLPTR));
        lblTextCalibFactorPower3U0x->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 3U0X", Q_NULLPTR));
        leTextCalibFactorPowerPhase_B_C->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_B_C->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_C_A->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_C_A->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_A_B->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_A_B->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerUCx->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerUCx->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPower3U0x->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPower3U0x->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorChannel3Us->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorChannel3Us->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorChannel3U0->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorChannel3U0->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerUAx->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerUAx->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerUBx->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerUBx->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        lblTextCalibFactorCurrent3I0->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 3I0", Q_NULLPTR));
        leTextCalibFactorPowerPhase_B->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_B->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        lblTextCalibFactorPowerPhase_C->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\267\320\260 \320\241", Q_NULLPTR));
        leTextCalibFactorPower3I0->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPower3I0->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        lblTextCalibFactorPower3U0->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 3U0", Q_NULLPTR));
        lblTextCalibFactorPowerPhase_B->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\267\320\260 \320\222", Q_NULLPTR));
        lblTextCalibFactorPowerPhase_A->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\267\320\260 \320\220", Q_NULLPTR));
        leTextCalibFactorPowerPhase_A->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_A->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_C->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerPhase_C->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerTotal->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorPowerTotal->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorCurrent3I0->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorCurrent3I0->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        lblTextCalibFactorPowerTotal->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\201\321\203\320\274\320\274\320\260\321\200\320\275\320\276\320\265", Q_NULLPTR));
        lblTextCalibFactorCurrentPhase_A->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 \321\204\320\260\320\267\320\260 \320\220", Q_NULLPTR));
        lblTextCalibFactorCurrentPhase_B->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 \321\204\320\260\320\267\320\260 \320\222", Q_NULLPTR));
        leTextCalibFactorCurrentPhase_A->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorCurrentPhase_A->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorCurrentPhase_B->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorCurrentPhase_B->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        lblTextCalibFactorCurrentPhase_C->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 \321\204\320\260\320\267\320\260 \320\241", Q_NULLPTR));
        leTextCalibFactorCurrentPhase_C->setInputMask(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        leTextCalibFactorCurrentPhase_C->setText(QApplication::translate("calibration_wgt", "0.0000", Q_NULLPTR));
        lblTextCalibFactorPowerPhase_C_A->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203\321\204\320\260\320\267\320\275\320\276\320\265 \320\241-\320\220", Q_NULLPTR));
        lblTextCalibFactorChannel3Us->setText(QApplication::translate("calibration_wgt", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\272\320\260\320\275\320\260\320\273\320\260 Us", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class calibration_wgt: public Ui_calibration_wgt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATION_WGT_H
