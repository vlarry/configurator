/********************************************************************************
** Form generated from reading UI file 'configuratorwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGURATORWINDOW_H
#define UI_CONFIGURATORWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_ConfiguratorWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_131;
    QTabWidget *tabwgtMenu;
    QWidget *tabFile;
    QHBoxLayout *horizontalLayout_111;
    QGroupBox *groupboxMenuFile;
    QHBoxLayout *horizontalLayout_110;
    QGridLayout *gridLayout_2;
    QPushButton *pbtnMenuNewCreate;
    QPushButton *pbtnMenuSave;
    QPushButton *pbtnMenuOpen;
    QPushButton *pbtnMenuSaveAs;
    QPushButton *pbtnMenuExit;
    QSpacerItem *horizontalSpacer_53;
    QWidget *tabExport;
    QHBoxLayout *horizontalLayout_112;
    QVBoxLayout *verticalLayout_124;
    QPushButton *pbtnMenuExportToPDF;
    QPushButton *pbtnExportPurpose;
    QPushButton *pushbtnExportEventJournalDb;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_120;
    QPushButton *pbtnImportPurpose;
    QPushButton *pushbtnImportEventDb;
    QSpacerItem *verticalSpacer_74;
    QSpacerItem *horizontalSpacer_57;
    QWidget *tabView;
    QHBoxLayout *horizontalLayout_113;
    QVBoxLayout *verticalLayout_128;
    QPushButton *pbtnMenuPanelMenuCtrl;
    QPushButton *pbtnMenuPanelVariableCtrl;
    QSpacerItem *verticalSpacer_20;
    QSpacerItem *horizontalSpacer_59;
    QWidget *tabSettings;
    QHBoxLayout *horizontalLayout_114;
    QGroupBox *gboxMenuSerialPort;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QToolButton *tbtnPortRefresh;
    QComboBox *cboxPortName;
    QComboBox *cboxBaudrate;
    QLabel *lblTextPortName;
    QLabel *lblTextPortSpeed;
    QPushButton *pbtnPortCtrl;
    QPushButton *pbtnSerialPortSettings;
    QGroupBox *groupboxMenuCommunication;
    QHBoxLayout *horizontalLayout_115;
    QVBoxLayout *verticalLayout_129;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblTextSlaveID;
    QSpinBox *sboxSlaveID;
    QCheckBox *chboxTerminal;
    QGroupBox *groupboxSyncDevice;
    QVBoxLayout *verticalLayout_134;
    QHBoxLayout *horizontalLayout_123;
    QSpinBox *spinboxSyncTime;
    QLabel *label_16;
    QSpacerItem *verticalSpacer_75;
    QSpacerItem *horizontalSpacer;
    QWidget *tabHelp;
    QHBoxLayout *horizontalLayout_116;
    QVBoxLayout *verticalLayout_130;
    QPushButton *pbtnMenuReference;
    QPushButton *pbtnVersionSoftware;
    QSpacerItem *verticalSpacer_73;
    QSpacerItem *horizontalSpacer_61;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_5;
    QCheckBox *checkboxCalibTimeout;
    QSpinBox *sboxTimeoutCalc;
    QWidget *centralWgt;
    QVBoxLayout *verticalLayout_133;
    QStackedWidget *stwgtMain;
    QWidget *pageInAnalogMain;
    QVBoxLayout *verticalLayout_74;
    QHBoxLayout *horizontalLayout_52;
    QGroupBox *gboxMain;
    QVBoxLayout *verticalLayout_65;
    QHBoxLayout *horizontalLayout_51;
    QVBoxLayout *verticalLayout_60;
    QLabel *lblTextMainKtt;
    QLabel *lblTextMainKt0;
    QLabel *lblTextMainKtn;
    QLabel *lblMainOnTN;
    QVBoxLayout *verticalLayout_64;
    QLineEdit *leM01;
    QLineEdit *leM02;
    QLineEdit *leM03;
    QComboBox *cboxM04;
    QSpacerItem *horizontalSpacer_65;
    QSpacerItem *verticalSpacer_61;
    QWidget *pageInAnalogCalibration;
    QVBoxLayout *verticalLayout_59;
    QHBoxLayout *horizontalLayout_5;
    QGridLayout *gridLayout_8;
    QLabel *lblTextCalibFactorPowerPhase_B_C;
    QLabel *lblTextCalibFactorPowerUAx;
    QLabel *lblTextCalibFactorPowerUBx;
    QLabel *lblTextCalibFactorChannel3U0;
    QLabel *lblTextCalibFactorPowerUCx;
    QLabel *lblTextCalibFactorPowerPhase_A_B;
    QLabel *lblTextCalibFactorPower3U0x;
    QLineEdit *leKUBC;
    QLineEdit *leKUCA;
    QLineEdit *leKUAB;
    QLineEdit *leKUCX;
    QLineEdit *leK3U0X;
    QLineEdit *leKU0X_;
    QLineEdit *leKU0X;
    QLineEdit *leKUAX;
    QLineEdit *leKUBX;
    QLabel *lblTextCalibFactorCurrent3I0;
    QLineEdit *leKUB;
    QLabel *lblTextCalibFactorPowerPhase_C;
    QLineEdit *leK3U0;
    QLabel *lblTextCalibFactorPower3U0;
    QLabel *lblTextCalibFactorPowerPhase_B;
    QLabel *lblTextCalibFactorPowerPhase_A;
    QLineEdit *leKUA;
    QLineEdit *leKUC;
    QLineEdit *leKUS;
    QLineEdit *leK3I0;
    QLabel *lblTextCalibFactorPowerTotal;
    QLabel *lblTextCalibFactorCurrentPhase_A;
    QLabel *lblTextCalibFactorCurrentPhase_B;
    QLineEdit *leKIA;
    QLineEdit *leKIB;
    QLabel *lblTextCalibFactorCurrentPhase_C;
    QLineEdit *leKIC;
    QLabel *lblTextCalibFactorPowerPhase_C_A;
    QLabel *lblTextCalibFactorChannel3Us;
    QSpacerItem *horizontalSpacer_24;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_3;
    QWidget *pageProtectionImax;
    QHBoxLayout *horizontalLayout_77;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_53;
    QVBoxLayout *verticalLayout_12;
    QPushButton *pbtnProtectionMTZ1;
    QPushButton *pbtnProtectionMTZ2;
    QPushButton *pbtnProtectionMTZ3;
    QPushButton *pbtnProtectionMTZ4;
    QSpacerItem *verticalSpacer_5;
    QStackedWidget *stwgtProtectionPropertiesMTZ;
    QWidget *pageProtectionPropertiesMTZ1;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *gboxProtectionPropertiesMTZ1;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_11;
    QLabel *lblTextProtectionMTZ_Ctrl;
    QLabel *lblTextProtectionMTZ_StartCurrent;
    QLabel *lblTextProtectionMTZ_Timeout;
    QLabel *lblTextProtectionMTZ_EnterBlocking;
    QLabel *lblTextProtectionMTZ_EnterBlocking_5;
    QVBoxLayout *vltProtectionMTZ31;
    QComboBox *cboxM05;
    QLineEdit *leM06;
    QLineEdit *leM08;
    QLineEdit *leK31;
    QLineEdit *leX01;
    QSpacerItem *horizontalSpacer_13;
    QSpacerItem *verticalSpacer_23;
    QWidget *pageProtectionPropertiesMTZ2;
    QVBoxLayout *verticalLayout_88;
    QHBoxLayout *horizontalLayout_17;
    QGroupBox *gboxProtectionPropertiesMTZ2;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_13;
    QLabel *lblTextProtectionMTZ_Ctrl_2;
    QLabel *lblTextProtectionMTZ_StartCurrent_2;
    QLabel *lblTextProtectionMTZ_Timeout_2;
    QLabel *lblTextProtectionMTZ_Acceleration;
    QLabel *lblTextProtectionMTZ_Acceleration_2;
    QVBoxLayout *vltProtectionMTZ31_2;
    QComboBox *cboxM09;
    QLineEdit *leM10;
    QLineEdit *leM11;
    QLineEdit *leM12;
    QLineEdit *leX03;
    QSpacerItem *horizontalSpacer_34;
    QSpacerItem *verticalSpacer_24;
    QWidget *pageProtectionPropertiesMTZ3;
    QVBoxLayout *verticalLayout_89;
    QHBoxLayout *horizontalLayout_76;
    QGroupBox *gboxProtectionPropertiesMTZ3;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_14;
    QLabel *lblTextProtectionMTZ_Ctrl_3;
    QLabel *lblTextProtectionMTZ_StartCurrent_3;
    QLabel *lblTextProtectionMTZ_Timeout_3;
    QLabel *lblTextProtectionMTZ_Factor;
    QLabel *lblTextProtectionMTZ_Factor_2;
    QVBoxLayout *vltProtectionMTZ31_3;
    QComboBox *cboxM13;
    QLineEdit *leM14;
    QLineEdit *leK22;
    QLineEdit *leM15;
    QLineEdit *leX04;
    QSpacerItem *horizontalSpacer_35;
    QSpacerItem *verticalSpacer_25;
    QWidget *pageProtectionPropertiesMTZ4;
    QVBoxLayout *verticalLayout_91;
    QHBoxLayout *horizontalLayout_78;
    QGroupBox *gboxProtectionPropertiesMTZ4;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_15;
    QLabel *lblTextProtectionMTZ_Ctrl_4;
    QLabel *lblTextProtectionMTZ_StartCurrent_4;
    QLabel *lblTextProtectionMTZ_DirStart;
    QLabel *lblTextProtectionMTZ_Angle;
    QLabel *lblTextProtectionMTZ_Angle_2;
    QLabel *lblTextProtectionMTZ_Angle_3;
    QLabel *lblTextProtectionMTZ_Angle_4;
    QVBoxLayout *vltProtectionMTZ31_4;
    QComboBox *cboxM16;
    QLineEdit *leM17;
    QLineEdit *leM07;
    QLineEdit *leK20;
    QLineEdit *leM18;
    QLineEdit *leX05;
    QLineEdit *leX05a;
    QSpacerItem *horizontalSpacer_36;
    QSpacerItem *verticalSpacer_26;
    QSpacerItem *verticalSpacer_21;
    QCustomPlot *wgtChartMTZ;
    QWidget *pageProtectionEarthy;
    QVBoxLayout *verticalLayout_90;
    QHBoxLayout *horizontalLayout_23;
    QVBoxLayout *verticalLayout_20;
    QPushButton *pbtnProtectionEarthly_OZZ1;
    QPushButton *pbtnProtectionEarthly_OZZ2;
    QPushButton *pbtnProtectionEarthly_NZZ1;
    QPushButton *pbtnProtectionEarthly_NZZ2;
    QSpacerItem *verticalSpacer_27;
    QStackedWidget *stwgtProtectionPropertiesEarthly;
    QWidget *pageProtectionPropertiesOZZ1;
    QVBoxLayout *verticalLayout_92;
    QHBoxLayout *horizontalLayout_79;
    QGroupBox *gboxProtectionPropertiesEarthly_OZZ1;
    QHBoxLayout *horizontalLayout_19;
    QVBoxLayout *verticalLayout_21;
    QLabel *lblTextProtectionEarthly_OZZ1_Ctrl;
    QLabel *lblTextProtectionEarthly_OZZ1_StartCurrent;
    QLabel *lblTextProtectionEarthly_OZZ1_Pause;
    QLabel *lblTextProtectionEarthly_OZZ1_KvzCurrent;
    QVBoxLayout *vltProtectionMTZ31_7;
    QComboBox *cboxM22;
    QLineEdit *leM23;
    QLineEdit *leM24;
    QLineEdit *leX07;
    QSpacerItem *horizontalSpacer_38;
    QSpacerItem *verticalSpacer_28;
    QWidget *pageProtectionPropertiesOZZ2;
    QVBoxLayout *verticalLayout_93;
    QHBoxLayout *horizontalLayout_80;
    QGroupBox *gboxProtectionPropertiesEarthly_OZZ2;
    QHBoxLayout *horizontalLayout_20;
    QVBoxLayout *verticalLayout_22;
    QLabel *lblTextProtectionEarthly_OZZ2_Ctrl;
    QLabel *lblTextProtectionEarthly_OZZ2_StartCurrent;
    QLabel *lblTextProtectionEarthly_OZZ2_Pause;
    QLabel *lblTextProtectionEarthly_OZZ2_KvzCurrent;
    QVBoxLayout *vltProtectionMTZ31_8;
    QComboBox *cboxK23;
    QLineEdit *leK24;
    QLineEdit *leK25;
    QLineEdit *leX07a;
    QSpacerItem *horizontalSpacer_39;
    QSpacerItem *verticalSpacer_29;
    QWidget *pageProtectionPropertiesNZZ1;
    QVBoxLayout *verticalLayout_94;
    QHBoxLayout *horizontalLayout_81;
    QGroupBox *gboxProtectionPropertiesEarthly_NZZ1;
    QHBoxLayout *horizontalLayout_21;
    QVBoxLayout *verticalLayout_23;
    QLabel *lblTextProtectionEarthly_NZZ1_Ctrl;
    QLabel *lblTextProtectionEarthly_NZZ1_StartCurrent;
    QLabel *lblTextProtectionEarthly_NZZ1_StartPower;
    QLabel *lblTextProtectionEarthly_NZZ1_Angle;
    QLabel *lblTextProtectionEarthly_NZZ1_Pause;
    QLabel *lblTextProtectionEarthly_NZZ1_KvzCurrent;
    QLabel *lblTextProtectionEarthly_NZZ1_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_9;
    QComboBox *cboxM25;
    QLineEdit *leM26;
    QLineEdit *leM27;
    QLineEdit *leK21;
    QLineEdit *leM28;
    QLineEdit *leX08;
    QLineEdit *leX09;
    QSpacerItem *horizontalSpacer_40;
    QSpacerItem *verticalSpacer_30;
    QWidget *pageProtectionPropertiesNZZ2;
    QVBoxLayout *verticalLayout_25;
    QHBoxLayout *horizontalLayout_18;
    QGroupBox *gboxProtectionPropertiesEarthly_NZZ2;
    QHBoxLayout *horizontalLayout_22;
    QVBoxLayout *verticalLayout_24;
    QLabel *lblTextProtectionEarthly_NZZ2_Ctrl;
    QLabel *lblTextProtectionEarthly_NZZ2_StartCurrent;
    QLabel *lblTextProtectionEarthly_NZZ2_StartPower;
    QLabel *lblTextProtectionEarthly_NZZ2_Angle;
    QLabel *lblTextProtectionEarthly_NZZ2_Pause;
    QLabel *lblTextProtectionEarthly_NZZ2_KvzCurrent;
    QLabel *lblTextProtectionEarthly_NZZ2_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_10;
    QComboBox *cboxK26;
    QLineEdit *leK27;
    QLineEdit *leK28;
    QLineEdit *leK29;
    QLineEdit *leK30;
    QLineEdit *leX08a;
    QLineEdit *leX09a;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *verticalSpacer_62;
    QSpacerItem *horizontalSpacer_22;
    QSpacerItem *verticalSpacer_8;
    QWidget *pageProtectionPower;
    QVBoxLayout *verticalLayout_95;
    QHBoxLayout *horizontalLayout_24;
    QVBoxLayout *verticalLayout_26;
    QPushButton *pbtnProtectionPower_Umax1;
    QPushButton *pbtnProtectionPower_Umax2;
    QPushButton *pbtnProtectionPower_Umin1;
    QPushButton *pbtnProtectionPower_Umin2;
    QPushButton *pbtnProtectionPower_3UO;
    QSpacerItem *verticalSpacer_9;
    QStackedWidget *stwgtProtectionPropertiesPower;
    QWidget *pageProtectionPropertiesUmax1;
    QVBoxLayout *verticalLayout_32;
    QHBoxLayout *horizontalLayout_82;
    QGroupBox *gboxProtectionPropertiesPower_Umax1;
    QHBoxLayout *horizontalLayout_25;
    QVBoxLayout *verticalLayout_27;
    QLabel *lblTextProtectionPower_Umax1_Ctrl;
    QLabel *lblTextProtectionPower_Umax1_StartPower;
    QLabel *lblTextProtectionPower_Umax1_Pause;
    QLabel *lblTextProtectionPower_Umax1_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_11;
    QComboBox *cboxM32;
    QLineEdit *leM33;
    QLineEdit *leM34;
    QLineEdit *leX11;
    QSpacerItem *horizontalSpacer_42;
    QSpacerItem *verticalSpacer_31;
    QWidget *pageProtectionPropertiesUmax2;
    QVBoxLayout *verticalLayout_96;
    QHBoxLayout *horizontalLayout_83;
    QGroupBox *gboxProtectionPropertiesPower_Umax2;
    QHBoxLayout *horizontalLayout_26;
    QVBoxLayout *verticalLayout_28;
    QLabel *lblTextProtectionPower_Umax2_Ctrl;
    QLabel *lblTextProtectionPower_Umax2_StartPower;
    QLabel *lblTextProtectionPower_Umax2_Pause;
    QLabel *lblTextProtectionPower_Umax2_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_12;
    QComboBox *cboxM35;
    QLineEdit *leM36;
    QLineEdit *leM37;
    QLineEdit *leX12;
    QSpacerItem *horizontalSpacer_43;
    QSpacerItem *verticalSpacer_32;
    QWidget *pageProtectionPropertiesUmin1;
    QVBoxLayout *verticalLayout_97;
    QHBoxLayout *horizontalLayout_84;
    QGroupBox *gboxProtectionPropertiesPower_Umin1;
    QHBoxLayout *horizontalLayout_27;
    QVBoxLayout *verticalLayout_29;
    QLabel *lblTextProtectionPower_Umin1_Ctrl;
    QLabel *lblTextProtectionPower_Umin1_Logic;
    QLabel *lblTextProtectionPower_Umin1_StartCtrl;
    QLabel *lblTextProtectionPower_Umin1_StartPower;
    QLabel *lblTextProtectionPower_Umin1_StartPower_2;
    QLabel *lblTextProtectionPower_Umin1_StartPower_3;
    QVBoxLayout *vltProtectionMTZ31_13;
    QComboBox *cboxM38;
    QComboBox *cboxM39;
    QComboBox *cboxM40;
    QLineEdit *leM41;
    QLineEdit *leM42;
    QLineEdit *leX13;
    QSpacerItem *horizontalSpacer_44;
    QSpacerItem *verticalSpacer_33;
    QWidget *pageProtectionPropertiesUmin2;
    QVBoxLayout *verticalLayout_98;
    QHBoxLayout *horizontalLayout_85;
    QGroupBox *gboxProtectionPropertiesPower_Umin2;
    QHBoxLayout *horizontalLayout_28;
    QVBoxLayout *verticalLayout_30;
    QLabel *lblTextProtectionPower_Umin2_Ctrl;
    QLabel *lblTextProtectionPower_Umin2_Logic;
    QLabel *lblTextProtectionPower_Umin2_StartCtrl;
    QLabel *lblTextProtectionPower_Umin2_StartPower;
    QLabel *lblTextProtectionPower_Umin2_StartPower_2;
    QLabel *lblTextProtectionPower_Umin2_StartPower_3;
    QVBoxLayout *vltProtectionMTZ31_14;
    QComboBox *cboxM43;
    QComboBox *cboxM44;
    QComboBox *cboxM45;
    QLineEdit *leM46;
    QLineEdit *leM47;
    QLineEdit *leX14;
    QSpacerItem *horizontalSpacer_45;
    QSpacerItem *verticalSpacer_34;
    QWidget *pageProtectionProperties3UO;
    QVBoxLayout *verticalLayout_99;
    QHBoxLayout *horizontalLayout_86;
    QGroupBox *gboxProtectionPropertiesPower_3UO;
    QHBoxLayout *horizontalLayout_29;
    QVBoxLayout *verticalLayout_31;
    QLabel *lblTextProtectionPower_3UO_Ctrl;
    QLabel *lblTextProtectionPower_3UO_StartCurrent;
    QLabel *lblTextProtectionPower_3UO_Pause;
    QLabel *lblTextProtectionPower_3UO_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_15;
    QComboBox *cboxM48;
    QLineEdit *leM49;
    QLineEdit *leM50;
    QLineEdit *leX15;
    QSpacerItem *horizontalSpacer_46;
    QSpacerItem *verticalSpacer_35;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *verticalSpacer_63;
    QWidget *pageProtectionMotor;
    QVBoxLayout *verticalLayout_100;
    QHBoxLayout *horizontalLayout_30;
    QVBoxLayout *verticalLayout_18;
    QPushButton *pbtnProtectionMotorStarting;
    QPushButton *pbtnProtectionMotorImin;
    QSpacerItem *verticalSpacer_6;
    QStackedWidget *stwgtProtectionPropertiesMotor;
    QWidget *pagePotectionPropertiesStarting;
    QVBoxLayout *verticalLayout_101;
    QHBoxLayout *horizontalLayout_16;
    QGroupBox *gboxProtectionPropertiesMotor_StartingCurrent;
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_16;
    QLabel *lblTextProtectionMotor_Ctrl;
    QLabel *lblTextProtectionMotor_StartCurrent;
    QLabel *lblTextProtectionMotor_Pause;
    QLabel *lblTextProtectionMotor_Kvz_Current;
    QVBoxLayout *vltProtectionMTZ31_5;
    QComboBox *cboxM19;
    QLineEdit *leM20;
    QLineEdit *leM21;
    QLineEdit *leX06;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer_7;
    QWidget *pageProtectionPropertiesImin;
    QVBoxLayout *verticalLayout_19;
    QHBoxLayout *horizontalLayout_15;
    QGroupBox *gboxProtectionPropertiesMotor_Imin;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_17;
    QLabel *lblTextProtectionMotor_Imin_Ctrl;
    QLabel *lblTextProtectionMotor_StartPower;
    QLabel *lblTextProtectionMotor_Pause_2;
    QLabel *lblTextProtectionMotor_Kvz_Power;
    QVBoxLayout *vltProtectionMTZ31_6;
    QComboBox *cboxM29;
    QLineEdit *leM30;
    QLineEdit *leM31;
    QLineEdit *leX10;
    QSpacerItem *horizontalSpacer_47;
    QSpacerItem *verticalSpacer_36;
    QSpacerItem *horizontalSpacer_25;
    QSpacerItem *verticalSpacer_64;
    QWidget *pageProtectionFrequency;
    QVBoxLayout *verticalLayout_121;
    QHBoxLayout *horizontalLayout_88;
    QVBoxLayout *verticalLayout_33;
    QPushButton *pbtnProtectionFrequency_ACR1;
    QPushButton *pbtnProtectionFrequency_ACR2;
    QPushButton *pbtnProtectionFrequency_ACR3;
    QSpacerItem *verticalSpacer_10;
    QStackedWidget *stwgtProtectionPropertiesFrequency;
    QWidget *pageProtectionPropertiesACHR1;
    QVBoxLayout *verticalLayout_102;
    QHBoxLayout *horizontalLayout_35;
    QGroupBox *gboxProtectionPropertiesFrequency_ACR1;
    QHBoxLayout *horizontalLayout_32;
    QVBoxLayout *verticalLayout_34;
    QLabel *lblTextProtectionFrequency_ACR1_Ctrl;
    QLabel *lblTextProtectionFrequency_ACR1_StartFreq;
    QLabel *lblTextProtectionFrequency_ACR1_Pause;
    QLabel *lblTextProtectionFrequency_ACR1_Umin;
    QLabel *lblTextProtectionFrequency_ACR1_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_16;
    QComboBox *cboxM51;
    QLineEdit *leM52;
    QLineEdit *leM53;
    QLineEdit *leM54;
    QLineEdit *leX16;
    QSpacerItem *horizontalSpacer_18;
    QSpacerItem *verticalSpacer_37;
    QWidget *pageProtectionPropertiesACHR2;
    QVBoxLayout *verticalLayout_103;
    QHBoxLayout *horizontalLayout_87;
    QGroupBox *gboxProtectionPropertiesFrequency_ACR2;
    QHBoxLayout *horizontalLayout_33;
    QVBoxLayout *verticalLayout_35;
    QLabel *lblTextProtectionFrequency_ACR2_Ctrl;
    QLabel *lblTextProtectionFrequency_ACR2_StartFreq;
    QLabel *lblTextProtectionFrequency_ACR2_Pause;
    QLabel *lblTextProtectionFrequency_ACR2_Umin;
    QLabel *lblTextProtectionFrequency_ACR2_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_17;
    QComboBox *cboxM55;
    QLineEdit *leM56;
    QLineEdit *leM57;
    QLineEdit *leM58;
    QLineEdit *leX17;
    QSpacerItem *horizontalSpacer_49;
    QSpacerItem *verticalSpacer_38;
    QWidget *pageProtectionPropertiesACHR3;
    QVBoxLayout *verticalLayout_37;
    QHBoxLayout *horizontalLayout_31;
    QGroupBox *gboxProtectionPropertiesFrequency_ACR3;
    QHBoxLayout *horizontalLayout_34;
    QVBoxLayout *verticalLayout_36;
    QLabel *lblTextProtectionFrequency_ACR3_Ctrl;
    QLabel *lblTextProtectionFrequency_ACR3_StartFreq;
    QLabel *lblTextProtectionFrequency_ACR3_Pause;
    QLabel *lblTextProtectionFrequency_ACR3_Umin;
    QLabel *lblTextProtectionFrequency_ACR3_KvzPower;
    QVBoxLayout *vltProtectionMTZ31_18;
    QComboBox *cboxM59;
    QLineEdit *leM60;
    QLineEdit *leM61;
    QLineEdit *leM62;
    QLineEdit *leX18;
    QSpacerItem *horizontalSpacer_52;
    QSpacerItem *verticalSpacer_39;
    QSpacerItem *horizontalSpacer_26;
    QSpacerItem *verticalSpacer_65;
    QWidget *pageProtectionExternal;
    QVBoxLayout *verticalLayout_122;
    QHBoxLayout *horizontalLayout_95;
    QVBoxLayout *verticalLayout_38;
    QPushButton *pbtnProtectionExternal_Arc;
    QPushButton *pbtnProtectionExternal1;
    QPushButton *pbtnProtectionExternal2;
    QPushButton *pbtnProtectionExternal3;
    QSpacerItem *verticalSpacer_13;
    QStackedWidget *stwgtProtectionPropertiesExternal;
    QWidget *pageProtectionPropertiesArc;
    QVBoxLayout *verticalLayout_47;
    QVBoxLayout *verticalLayout_45;
    QHBoxLayout *horizontalLayout_94;
    QGroupBox *gboxProtectionPropertiesExternalArc;
    QHBoxLayout *horizontalLayout_40;
    QHBoxLayout *horizontalLayout_36;
    QVBoxLayout *verticalLayout_39;
    QLabel *lblTextProtectionExternal_Arc_Ctrl;
    QLabel *lblTextProtectionExternal_Arc_StartCurrent;
    QLabel *lblTextProtectionExtermal_Arc_KvzCurrent;
    QVBoxLayout *vltProtectionMTZ31_19;
    QComboBox *cboxM63;
    QLineEdit *leM64;
    QLineEdit *leX19;
    QSpacerItem *horizontalSpacer_56;
    QSpacerItem *verticalSpacer_40;
    QSpacerItem *verticalSpacer_16;
    QWidget *pageProtectionPropertiesExternal1;
    QVBoxLayout *verticalLayout_48;
    QVBoxLayout *verticalLayout_40;
    QHBoxLayout *horizontalLayout_41;
    QGroupBox *gboxProtectionPropertiesExternal1;
    QHBoxLayout *horizontalLayout_89;
    QHBoxLayout *horizontalLayout_37;
    QVBoxLayout *verticalLayout_42;
    QLabel *lblTextProtectionExternal_Ext1_Ctrl;
    QLabel *lblTextProtectionExternal_Ext1_Pause;
    QVBoxLayout *vltProtectionMTZ31_20;
    QComboBox *cboxM71;
    QLineEdit *leM72;
    QSpacerItem *horizontalSpacer_19;
    QSpacerItem *verticalSpacer_41;
    QSpacerItem *verticalSpacer_11;
    QWidget *pageProtectionPropertiesExternal2;
    QVBoxLayout *verticalLayout_49;
    QVBoxLayout *verticalLayout_41;
    QHBoxLayout *horizontalLayout_92;
    QGroupBox *gboxProtectionPropertiesExternal2;
    QHBoxLayout *horizontalLayout_90;
    QHBoxLayout *horizontalLayout_38;
    QVBoxLayout *verticalLayout_44;
    QLabel *lblTextProtectionExternal_Ext2_Ctrl;
    QLabel *lblTextProtectionExternal_Ext2_Pause;
    QVBoxLayout *vltProtectionMTZ31_21;
    QComboBox *cboxM73;
    QLineEdit *leM74;
    QSpacerItem *horizontalSpacer_54;
    QSpacerItem *verticalSpacer_42;
    QSpacerItem *verticalSpacer_12;
    QWidget *pageProtectionPropertiesExternal3;
    QVBoxLayout *verticalLayout_50;
    QVBoxLayout *verticalLayout_43;
    QHBoxLayout *horizontalLayout_93;
    QGroupBox *gboxProtectionPropertiesExternal3;
    QHBoxLayout *horizontalLayout_91;
    QHBoxLayout *horizontalLayout_39;
    QVBoxLayout *verticalLayout_46;
    QLabel *lblTextProtectionExternal_Ext3_Ctrl;
    QLabel *lblTextProtectionExternal_Ext3_Pause;
    QVBoxLayout *vltProtectionMTZ31_22;
    QComboBox *cboxM75;
    QLineEdit *leM76;
    QSpacerItem *horizontalSpacer_55;
    QSpacerItem *verticalSpacer_49;
    QSpacerItem *verticalSpacer_15;
    QSpacerItem *horizontalSpacer_37;
    QSpacerItem *verticalSpacer_66;
    QWidget *pageProtectionTemperature;
    QVBoxLayout *verticalLayout_123;
    QHBoxLayout *horizontalLayout_96;
    QVBoxLayout *verticalLayout_53;
    QVBoxLayout *verticalLayout_51;
    QPushButton *pbtnProtectionTemp1;
    QPushButton *pbtnProtectionTemp2;
    QSpacerItem *verticalSpacer_14;
    QStackedWidget *stwgtProtectionPropertiesTemperature;
    QWidget *pageProtectionPropertiesTemp1;
    QVBoxLayout *verticalLayout_55;
    QHBoxLayout *horizontalLayout_45;
    QGroupBox *gboxProtectionPropertiesTemperature1;
    QHBoxLayout *horizontalLayout_43;
    QVBoxLayout *verticalLayout_52;
    QLabel *lblTextProtectionTemp1_Sensor1;
    QLabel *lblTextProtectionTemp1_Sensor2;
    QLabel *lblTextProtectionTemp1_StartTemp;
    QLabel *lblTextProtectionTemp1_Pause;
    QLabel *lblTextProtectionTemp1_KvzTemp;
    QVBoxLayout *vltProtectionMTZ31_23;
    QComboBox *cboxM65;
    QComboBox *cboxM66;
    QLineEdit *leM67;
    QLineEdit *leM69;
    QLineEdit *leX20;
    QSpacerItem *horizontalSpacer_20;
    QSpacerItem *verticalSpacer_17;
    QWidget *pageProtectionPropertiesTemp2;
    QVBoxLayout *verticalLayout_104;
    QHBoxLayout *horizontalLayout_42;
    QGroupBox *gboxProtectionPropertiesTemperature2;
    QHBoxLayout *horizontalLayout_44;
    QVBoxLayout *verticalLayout_54;
    QLabel *lblTextProtectionTemp2_Sensor1;
    QLabel *lblTextProtectionTemp2_Sensor2;
    QLabel *lblTextProtectionTemp2_StartTemp;
    QLabel *lblTextProtectionTemp2_Pause;
    QLabel *lblTextProtectionTemp2_KvzTemp;
    QVBoxLayout *vltProtectionMTZ31_24;
    QComboBox *cboxProtectionTemp2_Sensor1;
    QComboBox *cboxProtectionTemp2_Sensor2;
    QLineEdit *leM68;
    QLineEdit *leM70;
    QLineEdit *leX21;
    QSpacerItem *horizontalSpacer_58;
    QSpacerItem *verticalSpacer_50;
    QSpacerItem *horizontalSpacer_41;
    QSpacerItem *verticalSpacer_67;
    QWidget *pageProtectionLevel;
    QVBoxLayout *verticalLayout_114;
    QHBoxLayout *horizontalLayout_61;
    QVBoxLayout *verticalLayout_57;
    QPushButton *pbtnProtectionLevel1;
    QPushButton *pbtnProtectionLevel2;
    QPushButton *pbtnProtectionLevelSignStart;
    QSpacerItem *verticalSpacer_18;
    QStackedWidget *stwgtProtectionPropertiesLevel;
    QWidget *pageProtectionPropertiesLevel1;
    QVBoxLayout *verticalLayout_106;
    QVBoxLayout *verticalLayout_61;
    QHBoxLayout *horizontalLayout_49;
    QGroupBox *gboxProtectionPropertiesLevel1;
    QHBoxLayout *horizontalLayout_47;
    QVBoxLayout *verticalLayout_58;
    QLabel *lblTextProtectionLevel1_Ctrl;
    QLabel *lblTextProtectionLevel1_Pause;
    QVBoxLayout *vltProtectionMTZ31_25;
    QComboBox *cboxM77;
    QLineEdit *leM78;
    QSpacerItem *horizontalSpacer_60;
    QSpacerItem *verticalSpacer_51;
    QSpacerItem *verticalSpacer_53;
    QWidget *pageProtectionPropertiesLevel2;
    QVBoxLayout *verticalLayout_107;
    QVBoxLayout *verticalLayout_105;
    QHBoxLayout *horizontalLayout_46;
    QGroupBox *gboxProtectionPropertiesLevel2;
    QHBoxLayout *horizontalLayout_48;
    QVBoxLayout *verticalLayout_62;
    QLabel *lblTextProtectionLevel2_Ctrl;
    QLabel *lblTextProtectionLevel2_Pause;
    QVBoxLayout *vltProtectionMTZ31_26;
    QComboBox *cboxProtectionLeve2_Ctrl;
    QLineEdit *leM79;
    QSpacerItem *horizontalSpacer_21;
    QSpacerItem *verticalSpacer_52;
    QSpacerItem *verticalSpacer_54;
    QWidget *pageProtectionLevelSignStart;
    QVBoxLayout *verticalLayout_113;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_56;
    QVBoxLayout *verticalLayout_66;
    QLabel *lblTextStartSignal_1;
    QLabel *lblTextStartSignal_2;
    QLabel *lblTextStartSignal_3;
    QLabel *lblTextStartSignal_4;
    QLabel *lblTextStartSignal_5;
    QLabel *lblTextStartSignal_6;
    QLabel *lblTextStartSignal_7;
    QLabel *lblTextStartSignal_8;
    QVBoxLayout *verticalLayout_63;
    QComboBox *cboxMeasBRU;
    QComboBox *cboxfA;
    QComboBox *cboxfB;
    QComboBox *cboxfC;
    QComboBox *cboxWorkVak;
    QComboBox *cboxWorkBRU;
    QComboBox *cboxOnBRU;
    QComboBox *cboxRmeasOK;
    QFrame *line;
    QHBoxLayout *horizontalLayout_57;
    QVBoxLayout *verticalLayout_68;
    QLabel *lblTextStartSignal_9;
    QLabel *lblTextStartSignal_10;
    QLabel *lblTextStartSignal_11;
    QLabel *lblTextStartSignal_12;
    QLabel *lblTextStartSignal_13;
    QLabel *lblTextStartSignal_14;
    QLabel *lblTextStartSignal_15;
    QLabel *lblTextStartSignal_16;
    QVBoxLayout *verticalLayout_67;
    QComboBox *cboxOnBRU_2;
    QComboBox *cboxOffMTZ1;
    QComboBox *cboxOffMTZ2;
    QComboBox *cboxOffMTZ3;
    QComboBox *cboxOffMTZ4;
    QComboBox *cboxOffStart;
    QComboBox *cboxOffOZZ1;
    QComboBox *cboxOffNZZ1;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_58;
    QVBoxLayout *verticalLayout_70;
    QLabel *lblTextStartSignal_17;
    QLabel *lblTextStartSignal_18;
    QLabel *lblTextStartSignal_19;
    QLabel *lblTextStartSignal_20;
    QLabel *lblTextStartSignal_21;
    QLabel *lblTextStartSignal_22;
    QLabel *lblTextStartSignal_23;
    QLabel *lblTextStartSignal_24;
    QVBoxLayout *verticalLayout_69;
    QComboBox *cboxOffImin;
    QComboBox *cboxOffImax1;
    QComboBox *cboxOffImax2;
    QComboBox *cboxOffImin1;
    QComboBox *cboxOffImin2;
    QComboBox *cboxOffI3UO;
    QComboBox *cboxOffAVR;
    QComboBox *cboxOffACR1;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_59;
    QVBoxLayout *verticalLayout_71;
    QLabel *lblTextStartSignal_25;
    QLabel *lblTextStartSignal_26;
    QLabel *lblTextStartSignal_27;
    QLabel *lblTextStartSignal_28;
    QLabel *lblTextStartSignal_29;
    QLabel *lblTextStartSignal_30;
    QLabel *lblTextStartSignal_31;
    QLabel *lblTextStartSignal_32;
    QVBoxLayout *verticalLayout_72;
    QComboBox *cboxOffACR2;
    QComboBox *cboxOffACR3;
    QComboBox *cboxOffTemp1;
    QComboBox *cboxOffTemp2;
    QComboBox *cboxOffExt1;
    QComboBox *cboxOffExt2;
    QComboBox *cboxOffExt3;
    QComboBox *cboxOffLevel1;
    QFrame *line_4;
    QVBoxLayout *verticalLayout_73;
    QHBoxLayout *horizontalLayout_60;
    QLabel *lblTextStartSignal_33;
    QComboBox *cboxOffLevel2;
    QSpacerItem *verticalSpacer_19;
    QSpacerItem *verticalSpacer_68;
    QSpacerItem *horizontalSpacer_48;
    QSpacerItem *verticalSpacer_69;
    QWidget *pageProtectBRU;
    QVBoxLayout *verticalLayout_112;
    QHBoxLayout *horizontalLayout_63;
    QGroupBox *groupBox_11;
    QHBoxLayout *horizontalLayout_102;
    QGridLayout *gridLayout_10;
    QLabel *lblTextProtectionMTZ_Ctrl_5;
    QLabel *lblTextProtectionMTZ_Timeout_4;
    QComboBox *cboxM93;
    QLineEdit *leM96;
    QLabel *lblTextProtectionMTZ_StartCurrent_5;
    QLineEdit *leM97;
    QLabel *lblTextProtectionMTZ_EnterBlocking_3;
    QLabel *lblTextProtectionMTZ_EnterBlocking_2;
    QComboBox *cboxM95;
    QLineEdit *leM98;
    QLabel *lblTextProtectionMTZ_EnterBlocking_4;
    QLineEdit *leM99;
    QSpacerItem *horizontalSpacer_10;
    QSpacerItem *verticalSpacer_59;
    QWidget *pageProtectVacuum;
    QVBoxLayout *verticalLayout_115;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_12;
    QHBoxLayout *horizontalLayout_103;
    QGridLayout *gridLayout_11;
    QLineEdit *leM91;
    QLabel *lblTextProtectionMTZ_Ctrl_8;
    QComboBox *cboxM90;
    QLabel *lblTextProtectionMTZ_Ctrl_6;
    QLabel *lblTextProtectionMTZ_Ctrl_7;
    QLineEdit *leM92;
    QLabel *lblTextProtectionMTZ_Ctrl_9;
    QLineEdit *leX23;
    QSpacerItem *verticalSpacer_70;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_60;
    QWidget *pageSwitchDevice;
    QHBoxLayout *horizontalLayout_55;
    QVBoxLayout *verticalLayout_75;
    QPushButton *pbtnSwDevBreaker;
    QPushButton *pbtnSwDevBlock;
    QPushButton *pbtnSwDevSCHR;
    QPushButton *pbtnSwDevLR;
    QPushButton *pbtnSwDevZR;
    QPushButton *pbtnSwDevTruck;
    QPushButton *pbtnSwDevCtrl;
    QSpacerItem *verticalSpacer_22;
    QStackedWidget *stwgtSwitchDevice;
    QWidget *pageSwDevBreaker;
    QVBoxLayout *verticalLayout_125;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_76;
    QGridLayout *gridLayout_6;
    QLabel *lblTextSwitchON;
    QLabel *lblCtrlOT;
    QLabel *lblT_KCU;
    QLineEdit *leK09;
    QComboBox *cboxK06;
    QLabel *lblKcu;
    QLabel *lblPosVV;
    QComboBox *cboxK32;
    QLabel *lblTextSwBreakerModeCtrl;
    QSpacerItem *horizontalSpacer_11;
    QComboBox *cboxK17;
    QLabel *lblTextSwitchOFF;
    QComboBox *cboxK07;
    QComboBox *cboxK01;
    QLineEdit *leK02;
    QLineEdit *leK04;
    QComboBox *cboxK03;
    QLabel *lblTextSwitchON_T;
    QLabel *lblTextSwitchOFF_T;
    QLabel *lblTotv;
    QLineEdit *leK05;
    QLabel *lblI_VV_on;
    QLabel *lblKv_KCU_I;
    QLineEdit *leX22;
    QLineEdit *leK08;
    QSpacerItem *verticalSpacer_4;
    QWidget *pageSwDevDisconnect;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_70;
    QGroupBox *gboxSwDevDisconnect;
    QHBoxLayout *horizontalLayout_54;
    QVBoxLayout *verticalLayout_77;
    QLabel *lblTextVk_VV_tel;
    QLabel *lblTextVk_VV_tel_2;
    QLabel *lblTextVk_VV_tel_3;
    QVBoxLayout *vltProtectionMTZ31_28;
    QComboBox *cboxK13;
    QComboBox *cboxK14;
    QComboBox *cboxK15;
    QSpacerItem *horizontalSpacer_27;
    QSpacerItem *verticalSpacer_43;
    QWidget *pageSwDevSCHR;
    QVBoxLayout *verticalLayout_84;
    QHBoxLayout *horizontalLayout_71;
    QGroupBox *gboxSwDevSCHR;
    QHBoxLayout *horizontalLayout_64;
    QVBoxLayout *verticalLayout_78;
    QLabel *lblTextSCHR_Ctrl;
    QLabel *lblTextSCHR_Cmd;
    QLabel *lblTextSCHR_Timp;
    QLabel *lblTextSCHR_Toff;
    QVBoxLayout *vltProtectionMTZ31_29;
    QComboBox *cboxK34;
    QComboBox *cboxK38;
    QLineEdit *leK42;
    QLineEdit *leK46;
    QSpacerItem *horizontalSpacer_28;
    QSpacerItem *verticalSpacer_44;
    QWidget *pageSwDevLR;
    QVBoxLayout *verticalLayout_85;
    QHBoxLayout *horizontalLayout_72;
    QGroupBox *gboxSwDevLR;
    QHBoxLayout *horizontalLayout_65;
    QVBoxLayout *verticalLayout_79;
    QLabel *lblTextLR_Ctrl;
    QLabel *lblTextLR_Cmd;
    QLabel *lblTextLR_Timp;
    QLabel *lblTextLR_Toff;
    QVBoxLayout *vltProtectionMTZ31_30;
    QComboBox *cboxK35;
    QComboBox *cboxK39;
    QLineEdit *leK43;
    QLineEdit *leK47;
    QSpacerItem *horizontalSpacer_30;
    QSpacerItem *verticalSpacer_45;
    QWidget *pageSwDevZR;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_73;
    QGroupBox *gboxSwDevZR;
    QHBoxLayout *horizontalLayout_66;
    QVBoxLayout *verticalLayout_80;
    QLabel *lblTextZR_Ctrl;
    QLabel *lblTextZR_Cmd;
    QLabel *lblTextZR_Timp;
    QLabel *lblTextZR_Toff;
    QVBoxLayout *vltProtectionMTZ31_31;
    QComboBox *cboxK36;
    QComboBox *cboxK40;
    QLineEdit *leK44;
    QLineEdit *leK48;
    QSpacerItem *horizontalSpacer_31;
    QSpacerItem *verticalSpacer_46;
    QWidget *pageSwDevTruck;
    QVBoxLayout *verticalLayout_86;
    QHBoxLayout *horizontalLayout_74;
    QGroupBox *gboxSwDevTruck;
    QHBoxLayout *horizontalLayout_67;
    QVBoxLayout *verticalLayout_81;
    QLabel *lblTextTruck_Ctrl;
    QLabel *lblTextTruck_Cmd;
    QLabel *lblTextTruck_Timp;
    QLabel *lblTextTruck_Toff;
    QVBoxLayout *vltProtectionMTZ31_32;
    QComboBox *cboxK37;
    QComboBox *cboxK41;
    QLineEdit *leK45;
    QLineEdit *leK49;
    QSpacerItem *horizontalSpacer_32;
    QSpacerItem *verticalSpacer_47;
    QWidget *pageSwDev_TN_Ctrl;
    QVBoxLayout *verticalLayout_87;
    QHBoxLayout *horizontalLayout_50;
    QVBoxLayout *verticalLayout_83;
    QGroupBox *gboxSwDevCtrl;
    QHBoxLayout *horizontalLayout_68;
    QVBoxLayout *verticalLayout_82;
    QLabel *lblTextCtrl_Ctrl;
    QLabel *lblTextTruck_Toff_2;
    QVBoxLayout *vltProtectionMTZ31_33;
    QComboBox *cboxK18;
    QLineEdit *leT02;
    QSpacerItem *verticalSpacer_72;
    QSpacerItem *horizontalSpacer_33;
    QSpacerItem *verticalSpacer_48;
    QSpacerItem *horizontalSpacer_3;
    QWidget *pageAutomation;
    QHBoxLayout *horizontalLayout_75;
    QVBoxLayout *verticalLayout_108;
    QPushButton *pbtnAddAVR;
    QPushButton *pbtnAddAVRSection;
    QPushButton *pbtnAddAPV;
    QPushButton *pbtnAddAPV_Start;
    QSpacerItem *verticalSpacer_55;
    QStackedWidget *stwgtAdditional;
    QWidget *pageAdditionalPropertiesAVRInput;
    QVBoxLayout *verticalLayout_109;
    QHBoxLayout *horizontalLayout_104;
    QHBoxLayout *horizontalLayout_97;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_111;
    QGridLayout *gridLayout_12;
    QLabel *label_4;
    QLineEdit *leM84;
    QLineEdit *leM82;
    QLabel *label;
    QComboBox *cboxM81;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *leM83;
    QLabel *label_5;
    QLineEdit *leM85;
    QSpacerItem *horizontalSpacer_62;
    QSpacerItem *horizontalSpacer_71;
    QSpacerItem *verticalSpacer_56;
    QWidget *pageAdditionalAVRSection;
    QVBoxLayout *verticalLayout_117;
    QHBoxLayout *horizontalLayout_98;
    QGridLayout *gridLayout_14;
    QLabel *label_12;
    QComboBox *cboxK33;
    QSpacerItem *horizontalSpacer_23;
    QSpacerItem *verticalSpacer_71;
    QWidget *pageAdditionalPropertiesAPV;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_105;
    QHBoxLayout *horizontalLayout_100;
    QVBoxLayout *verticalLayout_110;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_99;
    QGridLayout *gridLayout_13;
    QLabel *label_7;
    QComboBox *cboxM87;
    QLabel *label_6;
    QLineEdit *leM88;
    QLabel *label_8;
    QLineEdit *leM89;
    QSpacerItem *verticalSpacer_58;
    QSpacerItem *horizontalSpacer_63;
    QSpacerItem *horizontalSpacer_72;
    QSpacerItem *verticalSpacer_57;
    QWidget *pageAdditionalPropertiesAPVStart;
    QVBoxLayout *verticalLayout_127;
    QGroupBox *groupBox_10;
    QVBoxLayout *verticalLayout_126;
    QGridLayout *gridLayout_25;
    QSpacerItem *horizontalSpacer_184;
    QComboBox *cboxAdditionalAPV_StartRmeasOK;
    QComboBox *cboxAdditionalAPV_StartBlockOnBRU;
    QLabel *label_39;
    QComboBox *cboxAdditionalAPV_StartOffExt1;
    QLabel *label_38;
    QComboBox *cboxAdditionalAPV_StartOffExt3;
    QComboBox *cboxAdditionalAPV_StartOffExt2;
    QLabel *label_40;
    QComboBox *cboxAdditionalAPV_StartOffAVR;
    QComboBox *cboxAdditionalAPV_StartOffACHR3;
    QComboBox *cboxAdditionalAPV_StartOffTemp1;
    QSpacerItem *horizontalSpacer_185;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_28;
    QComboBox *cboxAdditionalAPV_StartOffUmax1;
    QComboBox *cboxAdditionalAPV_StartOffUmax2;
    QComboBox *cboxAdditionalAPV_StartOffUmin1;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QComboBox *cboxAdditionalAPV_StartBRUWork;
    QComboBox *cboxAdditionalAPV_StartVacWork;
    QComboBox *cboxAdditionalAPV_StartOffLev1;
    QComboBox *cboxAdditionalAPV_StartOffACHR1;
    QComboBox *cboxAdditionalAPV_StartOff3UO;
    QSpacerItem *horizontalSpacer_186;
    QSpacerItem *horizontalSpacer_187;
    QLabel *label_32;
    QLabel *label_33;
    QComboBox *cboxAdditionalAPV_StartOffMTZ1;
    QComboBox *cboxAdditionalAPV_StartOffMTZ2;
    QComboBox *cboxAdditionalAPV_StartOffStart;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QComboBox *cboxAdditionalAPV_StartOffUmin2;
    QLabel *label_37;
    QLabel *label_41;
    QComboBox *cboxAdditionalAPV_StartOffOZZ1;
    QComboBox *cboxAdditionalAPV_StartOffNZZ1;
    QLabel *label_42;
    QLabel *label_43;
    QComboBox *cboxAdditionalAPV_StartOffMTZ4;
    QLabel *label_44;
    QComboBox *cboxAdditionalAPV_StartOnFromBRU;
    QLabel *label_45;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QLabel *label_49;
    QComboBox *cboxAdditionalAPV_StartPskVacPhaseC;
    QComboBox *cboxAdditionalAPV_StartMeasBRU;
    QLabel *label_50;
    QComboBox *cboxAdditionalAPV_StartPskVacPhaseA;
    QComboBox *cboxAdditionalAPV_StartPskVacPhaseB;
    QComboBox *cboxAdditionalAPV_StartOffACHR2;
    QLabel *label_51;
    QLabel *label_52;
    QLabel *label_53;
    QComboBox *cboxAdditionalAPV_StartOffMTZ3;
    QLabel *label_54;
    QComboBox *cboxAdditionalAPV_StartOffImin;
    QLabel *label_55;
    QLabel *label_56;
    QLabel *label_57;
    QComboBox *cboxAdditionalAPV_StartOffTemp2;
    QComboBox *cboxAdditionalAPV_StartOffLev2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_29;
    QWidget *pageJournalCrash;
    QWidget *pageJournalEvent;
    QVBoxLayout *verticalLayout_132;
    QHBoxLayout *horizontalLayout_122;
    QVBoxLayout *verticalLayout_7;
    QPushButton *pbtnEventJournalReadToTable;
    QPushButton *pbtnEventJournalTableClear;
    QCheckBox *checkboxEventJournalScrollTable;
    QGroupBox *groupboxEventJournalReadInterval;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_117;
    QRadioButton *radiobtnEventJournalInterval;
    QRadioButton *radiobtnEventJournalDate;
    QSpacerItem *horizontalSpacer_64;
    QHBoxLayout *horizontalLayout_121;
    QWidget *wgtEventJournalRange;
    QHBoxLayout *horizontalLayout_119;
    QHBoxLayout *horizontalLayout_101;
    QLabel *label_13;
    QSpinBox *spinBoxEventJournalReadBegin;
    QLabel *label_14;
    QSpinBox *spinBoxEvenJournalReadCount;
    QWidget *wgtEventJournalCalendar;
    QHBoxLayout *horizontalLayout_120;
    QHBoxLayout *horizontalLayout_118;
    QLineEdit *leJournalEventDate;
    QToolButton *toolbtnEventJournalCalendarOpen;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_11;
    QLineEdit *leEventCount;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_15;
    QLineEdit *leEventProcessTime;
    QTableWidget *tablewgtEventJournal;
    QWidget *pageJournalOscilloscope;
    QWidget *pageMeasurePrimaryValues;
    QWidget *pageMeasureSecondaryValues;
    QWidget *pageMeasureElectricPower;
    QWidget *pageMonitorInputDiscret;
    QWidget *pageMonitorOutputDiscret;
    QWidget *pageSetLedPurpose;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_106;
    QPushButton *pbtnClearLedOutput;
    QSpacerItem *horizontalSpacer_12;
    QTableView *tablewgtLedPurpose;
    QWidget *pageSetDiscretInputPurpose;
    QVBoxLayout *verticalLayout_56;
    QHBoxLayout *horizontalLayout_107;
    QPushButton *pbtnClearDiscreteInput;
    QSpacerItem *horizontalSpacer_14;
    QTableView *tablewgtDiscreteInputPurpose;
    QWidget *pageSetRelayPurpose;
    QVBoxLayout *verticalLayout_118;
    QHBoxLayout *horizontalLayout_108;
    QPushButton *pbtnClearRelayOutput;
    QSpacerItem *horizontalSpacer_50;
    QTableView *tablewgtRelayPurpose;
    QWidget *pageSetKeyboardPurpose;
    QVBoxLayout *verticalLayout_119;
    QHBoxLayout *horizontalLayout_109;
    QPushButton *pbtnClearKeyboardPurpose;
    QSpacerItem *horizontalSpacer_51;
    QTableView *tablewgtKeyboardPurpose;
    QHBoxLayout *horizontalLayout_62;
    QLabel *label_9;
    QPushButton *pbtnReadCurrentBlock;
    QPushButton *pbtnWriteCurrentBlock;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_10;
    QPushButton *pbtnReadAllBlock;
    QPushButton *pbtnWriteAllBlock;
    QStatusBar *statusBar;
    QDockWidget *dockwgtDeviceMenu;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_116;
    QHBoxLayout *horizontalLayout_69;
    QSpacerItem *horizontalSpacer_6;
    QToolButton *tbntExpandItems;
    QTreeWidget *treewgtDeviceMenu;

    void setupUi(QMainWindow *ConfiguratorWindow)
    {
        if (ConfiguratorWindow->objectName().isEmpty())
            ConfiguratorWindow->setObjectName(QStringLiteral("ConfiguratorWindow"));
        ConfiguratorWindow->resize(1029, 677);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConfiguratorWindow->sizePolicy().hasHeightForWidth());
        ConfiguratorWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(ConfiguratorWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_131 = new QVBoxLayout(centralWidget);
        verticalLayout_131->setSpacing(6);
        verticalLayout_131->setContentsMargins(11, 11, 11, 11);
        verticalLayout_131->setObjectName(QStringLiteral("verticalLayout_131"));
        tabwgtMenu = new QTabWidget(centralWidget);
        tabwgtMenu->setObjectName(QStringLiteral("tabwgtMenu"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabwgtMenu->sizePolicy().hasHeightForWidth());
        tabwgtMenu->setSizePolicy(sizePolicy1);
        tabFile = new QWidget();
        tabFile->setObjectName(QStringLiteral("tabFile"));
        horizontalLayout_111 = new QHBoxLayout(tabFile);
        horizontalLayout_111->setSpacing(6);
        horizontalLayout_111->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_111->setObjectName(QStringLiteral("horizontalLayout_111"));
        groupboxMenuFile = new QGroupBox(tabFile);
        groupboxMenuFile->setObjectName(QStringLiteral("groupboxMenuFile"));
        horizontalLayout_110 = new QHBoxLayout(groupboxMenuFile);
        horizontalLayout_110->setSpacing(6);
        horizontalLayout_110->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_110->setObjectName(QStringLiteral("horizontalLayout_110"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pbtnMenuNewCreate = new QPushButton(groupboxMenuFile);
        pbtnMenuNewCreate->setObjectName(QStringLiteral("pbtnMenuNewCreate"));

        gridLayout_2->addWidget(pbtnMenuNewCreate, 0, 0, 1, 1);

        pbtnMenuSave = new QPushButton(groupboxMenuFile);
        pbtnMenuSave->setObjectName(QStringLiteral("pbtnMenuSave"));

        gridLayout_2->addWidget(pbtnMenuSave, 0, 1, 1, 1);

        pbtnMenuOpen = new QPushButton(groupboxMenuFile);
        pbtnMenuOpen->setObjectName(QStringLiteral("pbtnMenuOpen"));

        gridLayout_2->addWidget(pbtnMenuOpen, 1, 0, 1, 1);

        pbtnMenuSaveAs = new QPushButton(groupboxMenuFile);
        pbtnMenuSaveAs->setObjectName(QStringLiteral("pbtnMenuSaveAs"));

        gridLayout_2->addWidget(pbtnMenuSaveAs, 1, 1, 1, 1);

        pbtnMenuExit = new QPushButton(groupboxMenuFile);
        pbtnMenuExit->setObjectName(QStringLiteral("pbtnMenuExit"));

        gridLayout_2->addWidget(pbtnMenuExit, 1, 2, 1, 1);


        horizontalLayout_110->addLayout(gridLayout_2);


        horizontalLayout_111->addWidget(groupboxMenuFile);

        horizontalSpacer_53 = new QSpacerItem(515, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_111->addItem(horizontalSpacer_53);

        tabwgtMenu->addTab(tabFile, QString());
        tabExport = new QWidget();
        tabExport->setObjectName(QStringLiteral("tabExport"));
        horizontalLayout_112 = new QHBoxLayout(tabExport);
        horizontalLayout_112->setSpacing(6);
        horizontalLayout_112->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_112->setObjectName(QStringLiteral("horizontalLayout_112"));
        verticalLayout_124 = new QVBoxLayout();
        verticalLayout_124->setSpacing(6);
        verticalLayout_124->setObjectName(QStringLiteral("verticalLayout_124"));
        pbtnMenuExportToPDF = new QPushButton(tabExport);
        pbtnMenuExportToPDF->setObjectName(QStringLiteral("pbtnMenuExportToPDF"));

        verticalLayout_124->addWidget(pbtnMenuExportToPDF);

        pbtnExportPurpose = new QPushButton(tabExport);
        pbtnExportPurpose->setObjectName(QStringLiteral("pbtnExportPurpose"));

        verticalLayout_124->addWidget(pbtnExportPurpose);

        pushbtnExportEventJournalDb = new QPushButton(tabExport);
        pushbtnExportEventJournalDb->setObjectName(QStringLiteral("pushbtnExportEventJournalDb"));

        verticalLayout_124->addWidget(pushbtnExportEventJournalDb);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_124->addItem(verticalSpacer);


        horizontalLayout_112->addLayout(verticalLayout_124);

        verticalLayout_120 = new QVBoxLayout();
        verticalLayout_120->setSpacing(6);
        verticalLayout_120->setObjectName(QStringLiteral("verticalLayout_120"));
        pbtnImportPurpose = new QPushButton(tabExport);
        pbtnImportPurpose->setObjectName(QStringLiteral("pbtnImportPurpose"));

        verticalLayout_120->addWidget(pbtnImportPurpose);

        pushbtnImportEventDb = new QPushButton(tabExport);
        pushbtnImportEventDb->setObjectName(QStringLiteral("pushbtnImportEventDb"));

        verticalLayout_120->addWidget(pushbtnImportEventDb);

        verticalSpacer_74 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_120->addItem(verticalSpacer_74);


        horizontalLayout_112->addLayout(verticalLayout_120);

        horizontalSpacer_57 = new QSpacerItem(752, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_112->addItem(horizontalSpacer_57);

        tabwgtMenu->addTab(tabExport, QString());
        tabView = new QWidget();
        tabView->setObjectName(QStringLiteral("tabView"));
        horizontalLayout_113 = new QHBoxLayout(tabView);
        horizontalLayout_113->setSpacing(6);
        horizontalLayout_113->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_113->setObjectName(QStringLiteral("horizontalLayout_113"));
        verticalLayout_128 = new QVBoxLayout();
        verticalLayout_128->setSpacing(6);
        verticalLayout_128->setObjectName(QStringLiteral("verticalLayout_128"));
        pbtnMenuPanelMenuCtrl = new QPushButton(tabView);
        pbtnMenuPanelMenuCtrl->setObjectName(QStringLiteral("pbtnMenuPanelMenuCtrl"));

        verticalLayout_128->addWidget(pbtnMenuPanelMenuCtrl);

        pbtnMenuPanelVariableCtrl = new QPushButton(tabView);
        pbtnMenuPanelVariableCtrl->setObjectName(QStringLiteral("pbtnMenuPanelVariableCtrl"));

        verticalLayout_128->addWidget(pbtnMenuPanelVariableCtrl);

        verticalSpacer_20 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_128->addItem(verticalSpacer_20);


        horizontalLayout_113->addLayout(verticalLayout_128);

        horizontalSpacer_59 = new QSpacerItem(660, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_113->addItem(horizontalSpacer_59);

        tabwgtMenu->addTab(tabView, QString());
        tabSettings = new QWidget();
        tabSettings->setObjectName(QStringLiteral("tabSettings"));
        horizontalLayout_114 = new QHBoxLayout(tabSettings);
        horizontalLayout_114->setSpacing(6);
        horizontalLayout_114->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_114->setObjectName(QStringLiteral("horizontalLayout_114"));
        gboxMenuSerialPort = new QGroupBox(tabSettings);
        gboxMenuSerialPort->setObjectName(QStringLiteral("gboxMenuSerialPort"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(gboxMenuSerialPort->sizePolicy().hasHeightForWidth());
        gboxMenuSerialPort->setSizePolicy(sizePolicy2);
        gboxMenuSerialPort->setAlignment(Qt::AlignCenter);
        horizontalLayout = new QHBoxLayout(gboxMenuSerialPort);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout->setVerticalSpacing(0);
        tbtnPortRefresh = new QToolButton(gboxMenuSerialPort);
        tbtnPortRefresh->setObjectName(QStringLiteral("tbtnPortRefresh"));
        tbtnPortRefresh->setMinimumSize(QSize(22, 22));
        tbtnPortRefresh->setMaximumSize(QSize(22, 22));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/resource/images/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbtnPortRefresh->setIcon(icon);
        tbtnPortRefresh->setIconSize(QSize(22, 22));

        gridLayout->addWidget(tbtnPortRefresh, 0, 2, 1, 1);

        cboxPortName = new QComboBox(gboxMenuSerialPort);
        cboxPortName->setObjectName(QStringLiteral("cboxPortName"));

        gridLayout->addWidget(cboxPortName, 0, 1, 1, 1);

        cboxBaudrate = new QComboBox(gboxMenuSerialPort);
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->addItem(QString());
        cboxBaudrate->setObjectName(QStringLiteral("cboxBaudrate"));

        gridLayout->addWidget(cboxBaudrate, 1, 1, 1, 1);

        lblTextPortName = new QLabel(gboxMenuSerialPort);
        lblTextPortName->setObjectName(QStringLiteral("lblTextPortName"));
        lblTextPortName->setScaledContents(true);

        gridLayout->addWidget(lblTextPortName, 0, 0, 1, 1);

        lblTextPortSpeed = new QLabel(gboxMenuSerialPort);
        lblTextPortSpeed->setObjectName(QStringLiteral("lblTextPortSpeed"));
        lblTextPortSpeed->setScaledContents(true);

        gridLayout->addWidget(lblTextPortSpeed, 1, 0, 1, 1);

        pbtnPortCtrl = new QPushButton(gboxMenuSerialPort);
        pbtnPortCtrl->setObjectName(QStringLiteral("pbtnPortCtrl"));
        pbtnPortCtrl->setCheckable(true);

        gridLayout->addWidget(pbtnPortCtrl, 0, 3, 1, 1);

        pbtnSerialPortSettings = new QPushButton(gboxMenuSerialPort);
        pbtnSerialPortSettings->setObjectName(QStringLiteral("pbtnSerialPortSettings"));

        gridLayout->addWidget(pbtnSerialPortSettings, 1, 3, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        horizontalLayout_114->addWidget(gboxMenuSerialPort);

        groupboxMenuCommunication = new QGroupBox(tabSettings);
        groupboxMenuCommunication->setObjectName(QStringLiteral("groupboxMenuCommunication"));
        groupboxMenuCommunication->setAlignment(Qt::AlignCenter);
        horizontalLayout_115 = new QHBoxLayout(groupboxMenuCommunication);
        horizontalLayout_115->setSpacing(6);
        horizontalLayout_115->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_115->setObjectName(QStringLiteral("horizontalLayout_115"));
        verticalLayout_129 = new QVBoxLayout();
        verticalLayout_129->setSpacing(6);
        verticalLayout_129->setObjectName(QStringLiteral("verticalLayout_129"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lblTextSlaveID = new QLabel(groupboxMenuCommunication);
        lblTextSlaveID->setObjectName(QStringLiteral("lblTextSlaveID"));
        lblTextSlaveID->setScaledContents(true);

        horizontalLayout_2->addWidget(lblTextSlaveID);

        sboxSlaveID = new QSpinBox(groupboxMenuCommunication);
        sboxSlaveID->setObjectName(QStringLiteral("sboxSlaveID"));
        sboxSlaveID->setMinimum(1);
        sboxSlaveID->setMaximum(247);

        horizontalLayout_2->addWidget(sboxSlaveID);


        verticalLayout_129->addLayout(horizontalLayout_2);

        chboxTerminal = new QCheckBox(groupboxMenuCommunication);
        chboxTerminal->setObjectName(QStringLiteral("chboxTerminal"));

        verticalLayout_129->addWidget(chboxTerminal);


        horizontalLayout_115->addLayout(verticalLayout_129);


        horizontalLayout_114->addWidget(groupboxMenuCommunication);

        groupboxSyncDevice = new QGroupBox(tabSettings);
        groupboxSyncDevice->setObjectName(QStringLiteral("groupboxSyncDevice"));
        groupboxSyncDevice->setAlignment(Qt::AlignCenter);
        verticalLayout_134 = new QVBoxLayout(groupboxSyncDevice);
        verticalLayout_134->setSpacing(6);
        verticalLayout_134->setContentsMargins(11, 11, 11, 11);
        verticalLayout_134->setObjectName(QStringLiteral("verticalLayout_134"));
        horizontalLayout_123 = new QHBoxLayout();
        horizontalLayout_123->setSpacing(6);
        horizontalLayout_123->setObjectName(QStringLiteral("horizontalLayout_123"));
        spinboxSyncTime = new QSpinBox(groupboxSyncDevice);
        spinboxSyncTime->setObjectName(QStringLiteral("spinboxSyncTime"));
        spinboxSyncTime->setAlignment(Qt::AlignCenter);
        spinboxSyncTime->setMinimum(500);
        spinboxSyncTime->setMaximum(10000);
        spinboxSyncTime->setSingleStep(100);
        spinboxSyncTime->setValue(1000);

        horizontalLayout_123->addWidget(spinboxSyncTime);

        label_16 = new QLabel(groupboxSyncDevice);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setScaledContents(true);

        horizontalLayout_123->addWidget(label_16);


        verticalLayout_134->addLayout(horizontalLayout_123);

        verticalSpacer_75 = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_134->addItem(verticalSpacer_75);


        horizontalLayout_114->addWidget(groupboxSyncDevice);

        horizontalSpacer = new QSpacerItem(570, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_114->addItem(horizontalSpacer);

        tabwgtMenu->addTab(tabSettings, QString());
        tabHelp = new QWidget();
        tabHelp->setObjectName(QStringLiteral("tabHelp"));
        horizontalLayout_116 = new QHBoxLayout(tabHelp);
        horizontalLayout_116->setSpacing(6);
        horizontalLayout_116->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_116->setObjectName(QStringLiteral("horizontalLayout_116"));
        verticalLayout_130 = new QVBoxLayout();
        verticalLayout_130->setSpacing(6);
        verticalLayout_130->setObjectName(QStringLiteral("verticalLayout_130"));
        pbtnMenuReference = new QPushButton(tabHelp);
        pbtnMenuReference->setObjectName(QStringLiteral("pbtnMenuReference"));

        verticalLayout_130->addWidget(pbtnMenuReference);

        pbtnVersionSoftware = new QPushButton(tabHelp);
        pbtnVersionSoftware->setObjectName(QStringLiteral("pbtnVersionSoftware"));

        verticalLayout_130->addWidget(pbtnVersionSoftware);

        verticalSpacer_73 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_130->addItem(verticalSpacer_73);


        horizontalLayout_116->addLayout(verticalLayout_130);

        horizontalSpacer_61 = new QSpacerItem(752, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_116->addItem(horizontalSpacer_61);

        tabwgtMenu->addTab(tabHelp, QString());

        verticalLayout_131->addWidget(tabwgtMenu);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 5, -1, 5);
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        checkboxCalibTimeout = new QCheckBox(widget);
        checkboxCalibTimeout->setObjectName(QStringLiteral("checkboxCalibTimeout"));
        checkboxCalibTimeout->setChecked(true);

        gridLayout_5->addWidget(checkboxCalibTimeout, 0, 0, 1, 1);

        sboxTimeoutCalc = new QSpinBox(widget);
        sboxTimeoutCalc->setObjectName(QStringLiteral("sboxTimeoutCalc"));
        sboxTimeoutCalc->setMaximumSize(QSize(75, 75));
        sboxTimeoutCalc->setMinimum(100);
        sboxTimeoutCalc->setMaximum(5000);
        sboxTimeoutCalc->setValue(1000);

        gridLayout_5->addWidget(sboxTimeoutCalc, 0, 1, 1, 1);


        horizontalLayout_7->addLayout(gridLayout_5);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout_131->addWidget(widget);

        centralWgt = new QWidget(centralWidget);
        centralWgt->setObjectName(QStringLiteral("centralWgt"));
        verticalLayout_133 = new QVBoxLayout(centralWgt);
        verticalLayout_133->setSpacing(6);
        verticalLayout_133->setContentsMargins(11, 11, 11, 11);
        verticalLayout_133->setObjectName(QStringLiteral("verticalLayout_133"));
        stwgtMain = new QStackedWidget(centralWgt);
        stwgtMain->setObjectName(QStringLiteral("stwgtMain"));
        stwgtMain->setFrameShape(QFrame::StyledPanel);
        pageInAnalogMain = new QWidget();
        pageInAnalogMain->setObjectName(QStringLiteral("pageInAnalogMain"));
        verticalLayout_74 = new QVBoxLayout(pageInAnalogMain);
        verticalLayout_74->setSpacing(6);
        verticalLayout_74->setContentsMargins(11, 11, 11, 11);
        verticalLayout_74->setObjectName(QStringLiteral("verticalLayout_74"));
        horizontalLayout_52 = new QHBoxLayout();
        horizontalLayout_52->setSpacing(6);
        horizontalLayout_52->setObjectName(QStringLiteral("horizontalLayout_52"));
        gboxMain = new QGroupBox(pageInAnalogMain);
        gboxMain->setObjectName(QStringLiteral("gboxMain"));
        verticalLayout_65 = new QVBoxLayout(gboxMain);
        verticalLayout_65->setSpacing(6);
        verticalLayout_65->setContentsMargins(11, 11, 11, 11);
        verticalLayout_65->setObjectName(QStringLiteral("verticalLayout_65"));
        horizontalLayout_51 = new QHBoxLayout();
        horizontalLayout_51->setSpacing(6);
        horizontalLayout_51->setObjectName(QStringLiteral("horizontalLayout_51"));
        verticalLayout_60 = new QVBoxLayout();
        verticalLayout_60->setSpacing(6);
        verticalLayout_60->setObjectName(QStringLiteral("verticalLayout_60"));
        lblTextMainKtt = new QLabel(gboxMain);
        lblTextMainKtt->setObjectName(QStringLiteral("lblTextMainKtt"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(lblTextMainKtt->sizePolicy().hasHeightForWidth());
        lblTextMainKtt->setSizePolicy(sizePolicy4);
        lblTextMainKtt->setScaledContents(true);

        verticalLayout_60->addWidget(lblTextMainKtt);

        lblTextMainKt0 = new QLabel(gboxMain);
        lblTextMainKt0->setObjectName(QStringLiteral("lblTextMainKt0"));
        sizePolicy4.setHeightForWidth(lblTextMainKt0->sizePolicy().hasHeightForWidth());
        lblTextMainKt0->setSizePolicy(sizePolicy4);
        lblTextMainKt0->setScaledContents(true);

        verticalLayout_60->addWidget(lblTextMainKt0);

        lblTextMainKtn = new QLabel(gboxMain);
        lblTextMainKtn->setObjectName(QStringLiteral("lblTextMainKtn"));
        sizePolicy4.setHeightForWidth(lblTextMainKtn->sizePolicy().hasHeightForWidth());
        lblTextMainKtn->setSizePolicy(sizePolicy4);
        lblTextMainKtn->setScaledContents(true);

        verticalLayout_60->addWidget(lblTextMainKtn);

        lblMainOnTN = new QLabel(gboxMain);
        lblMainOnTN->setObjectName(QStringLiteral("lblMainOnTN"));
        sizePolicy4.setHeightForWidth(lblMainOnTN->sizePolicy().hasHeightForWidth());
        lblMainOnTN->setSizePolicy(sizePolicy4);
        lblMainOnTN->setScaledContents(true);

        verticalLayout_60->addWidget(lblMainOnTN);


        horizontalLayout_51->addLayout(verticalLayout_60);

        verticalLayout_64 = new QVBoxLayout();
        verticalLayout_64->setSpacing(6);
        verticalLayout_64->setObjectName(QStringLiteral("verticalLayout_64"));
        leM01 = new QLineEdit(gboxMain);
        leM01->setObjectName(QStringLiteral("leM01"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(leM01->sizePolicy().hasHeightForWidth());
        leM01->setSizePolicy(sizePolicy5);
        leM01->setMaximumSize(QSize(75, 16777215));
        leM01->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        leM01->setMaxLength(10);
        leM01->setAlignment(Qt::AlignCenter);

        verticalLayout_64->addWidget(leM01);

        leM02 = new QLineEdit(gboxMain);
        leM02->setObjectName(QStringLiteral("leM02"));
        sizePolicy5.setHeightForWidth(leM02->sizePolicy().hasHeightForWidth());
        leM02->setSizePolicy(sizePolicy5);
        leM02->setMaximumSize(QSize(75, 16777215));
        leM02->setMaxLength(10);
        leM02->setAlignment(Qt::AlignCenter);

        verticalLayout_64->addWidget(leM02);

        leM03 = new QLineEdit(gboxMain);
        leM03->setObjectName(QStringLiteral("leM03"));
        sizePolicy5.setHeightForWidth(leM03->sizePolicy().hasHeightForWidth());
        leM03->setSizePolicy(sizePolicy5);
        leM03->setMaximumSize(QSize(75, 16777215));
        leM03->setMaxLength(10);
        leM03->setAlignment(Qt::AlignCenter);

        verticalLayout_64->addWidget(leM03);

        cboxM04 = new QComboBox(gboxMain);
        cboxM04->addItem(QString());
        cboxM04->addItem(QString());
        cboxM04->addItem(QString());
        cboxM04->addItem(QString());
        cboxM04->setObjectName(QStringLiteral("cboxM04"));

        verticalLayout_64->addWidget(cboxM04);


        horizontalLayout_51->addLayout(verticalLayout_64);


        verticalLayout_65->addLayout(horizontalLayout_51);


        horizontalLayout_52->addWidget(gboxMain);

        horizontalSpacer_65 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_52->addItem(horizontalSpacer_65);


        verticalLayout_74->addLayout(horizontalLayout_52);

        verticalSpacer_61 = new QSpacerItem(20, 204, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_74->addItem(verticalSpacer_61);

        stwgtMain->addWidget(pageInAnalogMain);
        pageInAnalogCalibration = new QWidget();
        pageInAnalogCalibration->setObjectName(QStringLiteral("pageInAnalogCalibration"));
        verticalLayout_59 = new QVBoxLayout(pageInAnalogCalibration);
        verticalLayout_59->setSpacing(6);
        verticalLayout_59->setContentsMargins(11, 11, 11, 11);
        verticalLayout_59->setObjectName(QStringLiteral("verticalLayout_59"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(2);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setSizeConstraint(QLayout::SetMinimumSize);
        lblTextCalibFactorPowerPhase_B_C = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerPhase_B_C->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_B_C"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerPhase_B_C->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_B_C->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerPhase_B_C->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_B_C, 1, 3, 1, 1);

        lblTextCalibFactorPowerUAx = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerUAx->setObjectName(QStringLiteral("lblTextCalibFactorPowerUAx"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerUAx->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerUAx->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerUAx->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerUAx, 4, 3, 1, 1);

        lblTextCalibFactorPowerUBx = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerUBx->setObjectName(QStringLiteral("lblTextCalibFactorPowerUBx"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerUBx->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerUBx->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerUBx->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerUBx, 5, 3, 1, 1);

        lblTextCalibFactorChannel3U0 = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorChannel3U0->setObjectName(QStringLiteral("lblTextCalibFactorChannel3U0"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorChannel3U0->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorChannel3U0->setSizePolicy(sizePolicy5);
        lblTextCalibFactorChannel3U0->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorChannel3U0, 7, 3, 1, 1);

        lblTextCalibFactorPowerUCx = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerUCx->setObjectName(QStringLiteral("lblTextCalibFactorPowerUCx"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerUCx->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerUCx->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerUCx->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerUCx, 6, 3, 1, 1);

        lblTextCalibFactorPowerPhase_A_B = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerPhase_A_B->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_A_B"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerPhase_A_B->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_A_B->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerPhase_A_B->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_A_B, 0, 3, 1, 1);

        lblTextCalibFactorPower3U0x = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPower3U0x->setObjectName(QStringLiteral("lblTextCalibFactorPower3U0x"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPower3U0x->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPower3U0x->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPower3U0x->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPower3U0x, 3, 3, 1, 1);

        leKUBC = new QLineEdit(pageInAnalogCalibration);
        leKUBC->setObjectName(QStringLiteral("leKUBC"));
        sizePolicy5.setHeightForWidth(leKUBC->sizePolicy().hasHeightForWidth());
        leKUBC->setSizePolicy(sizePolicy5);
        leKUBC->setMaximumSize(QSize(75, 16777215));
        leKUBC->setMaxLength(10);
        leKUBC->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUBC, 1, 4, 1, 1);

        leKUCA = new QLineEdit(pageInAnalogCalibration);
        leKUCA->setObjectName(QStringLiteral("leKUCA"));
        sizePolicy5.setHeightForWidth(leKUCA->sizePolicy().hasHeightForWidth());
        leKUCA->setSizePolicy(sizePolicy5);
        leKUCA->setMaximumSize(QSize(75, 16777215));
        leKUCA->setMaxLength(10);
        leKUCA->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUCA, 2, 4, 1, 1);

        leKUAB = new QLineEdit(pageInAnalogCalibration);
        leKUAB->setObjectName(QStringLiteral("leKUAB"));
        sizePolicy5.setHeightForWidth(leKUAB->sizePolicy().hasHeightForWidth());
        leKUAB->setSizePolicy(sizePolicy5);
        leKUAB->setMaximumSize(QSize(75, 16777215));
        leKUAB->setMaxLength(10);
        leKUAB->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUAB, 0, 4, 1, 1);

        leKUCX = new QLineEdit(pageInAnalogCalibration);
        leKUCX->setObjectName(QStringLiteral("leKUCX"));
        sizePolicy5.setHeightForWidth(leKUCX->sizePolicy().hasHeightForWidth());
        leKUCX->setSizePolicy(sizePolicy5);
        leKUCX->setMaximumSize(QSize(75, 16777215));
        leKUCX->setMaxLength(10);
        leKUCX->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUCX, 6, 4, 1, 1);

        leK3U0X = new QLineEdit(pageInAnalogCalibration);
        leK3U0X->setObjectName(QStringLiteral("leK3U0X"));
        sizePolicy5.setHeightForWidth(leK3U0X->sizePolicy().hasHeightForWidth());
        leK3U0X->setSizePolicy(sizePolicy5);
        leK3U0X->setMaximumSize(QSize(75, 16777215));
        leK3U0X->setMaxLength(10);
        leK3U0X->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leK3U0X, 3, 4, 1, 1);

        leKU0X_ = new QLineEdit(pageInAnalogCalibration);
        leKU0X_->setObjectName(QStringLiteral("leKU0X_"));
        sizePolicy5.setHeightForWidth(leKU0X_->sizePolicy().hasHeightForWidth());
        leKU0X_->setSizePolicy(sizePolicy5);
        leKU0X_->setMaximumSize(QSize(75, 16777215));
        leKU0X_->setMaxLength(10);
        leKU0X_->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKU0X_, 8, 4, 1, 1);

        leKU0X = new QLineEdit(pageInAnalogCalibration);
        leKU0X->setObjectName(QStringLiteral("leKU0X"));
        sizePolicy5.setHeightForWidth(leKU0X->sizePolicy().hasHeightForWidth());
        leKU0X->setSizePolicy(sizePolicy5);
        leKU0X->setMaximumSize(QSize(75, 16777215));
        leKU0X->setMaxLength(10);
        leKU0X->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKU0X, 7, 4, 1, 1);

        leKUAX = new QLineEdit(pageInAnalogCalibration);
        leKUAX->setObjectName(QStringLiteral("leKUAX"));
        sizePolicy5.setHeightForWidth(leKUAX->sizePolicy().hasHeightForWidth());
        leKUAX->setSizePolicy(sizePolicy5);
        leKUAX->setMaximumSize(QSize(75, 16777215));
        leKUAX->setMaxLength(10);
        leKUAX->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUAX, 4, 4, 1, 1);

        leKUBX = new QLineEdit(pageInAnalogCalibration);
        leKUBX->setObjectName(QStringLiteral("leKUBX"));
        sizePolicy5.setHeightForWidth(leKUBX->sizePolicy().hasHeightForWidth());
        leKUBX->setSizePolicy(sizePolicy5);
        leKUBX->setMaximumSize(QSize(75, 16777215));
        leKUBX->setMaxLength(10);
        leKUBX->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUBX, 5, 4, 1, 1);

        lblTextCalibFactorCurrent3I0 = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorCurrent3I0->setObjectName(QStringLiteral("lblTextCalibFactorCurrent3I0"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorCurrent3I0->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrent3I0->setSizePolicy(sizePolicy5);
        lblTextCalibFactorCurrent3I0->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrent3I0, 3, 0, 1, 1);

        leKUB = new QLineEdit(pageInAnalogCalibration);
        leKUB->setObjectName(QStringLiteral("leKUB"));
        sizePolicy5.setHeightForWidth(leKUB->sizePolicy().hasHeightForWidth());
        leKUB->setSizePolicy(sizePolicy5);
        leKUB->setMaximumSize(QSize(75, 16777215));
        leKUB->setMaxLength(10);
        leKUB->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUB, 5, 1, 1, 1);

        lblTextCalibFactorPowerPhase_C = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerPhase_C->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_C"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerPhase_C->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_C->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerPhase_C->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_C, 6, 0, 1, 1);

        leK3U0 = new QLineEdit(pageInAnalogCalibration);
        leK3U0->setObjectName(QStringLiteral("leK3U0"));
        sizePolicy5.setHeightForWidth(leK3U0->sizePolicy().hasHeightForWidth());
        leK3U0->setSizePolicy(sizePolicy5);
        leK3U0->setMaximumSize(QSize(75, 16777215));
        leK3U0->setMaxLength(10);
        leK3U0->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leK3U0, 7, 1, 1, 1);

        lblTextCalibFactorPower3U0 = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPower3U0->setObjectName(QStringLiteral("lblTextCalibFactorPower3U0"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPower3U0->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPower3U0->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPower3U0->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPower3U0, 7, 0, 1, 1);

        lblTextCalibFactorPowerPhase_B = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerPhase_B->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_B"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerPhase_B->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_B->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerPhase_B->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_B, 5, 0, 1, 1);

        lblTextCalibFactorPowerPhase_A = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerPhase_A->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_A"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerPhase_A->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_A->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerPhase_A->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_A, 4, 0, 1, 1);

        leKUA = new QLineEdit(pageInAnalogCalibration);
        leKUA->setObjectName(QStringLiteral("leKUA"));
        sizePolicy5.setHeightForWidth(leKUA->sizePolicy().hasHeightForWidth());
        leKUA->setSizePolicy(sizePolicy5);
        leKUA->setMaximumSize(QSize(75, 16777215));
        leKUA->setMaxLength(10);
        leKUA->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUA, 4, 1, 1, 1);

        leKUC = new QLineEdit(pageInAnalogCalibration);
        leKUC->setObjectName(QStringLiteral("leKUC"));
        sizePolicy5.setHeightForWidth(leKUC->sizePolicy().hasHeightForWidth());
        leKUC->setSizePolicy(sizePolicy5);
        leKUC->setMaximumSize(QSize(75, 16777215));
        leKUC->setMaxLength(10);
        leKUC->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUC, 6, 1, 1, 1);

        leKUS = new QLineEdit(pageInAnalogCalibration);
        leKUS->setObjectName(QStringLiteral("leKUS"));
        sizePolicy5.setHeightForWidth(leKUS->sizePolicy().hasHeightForWidth());
        leKUS->setSizePolicy(sizePolicy5);
        leKUS->setMaximumSize(QSize(75, 16777215));
        leKUS->setMaxLength(10);
        leKUS->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKUS, 8, 1, 1, 1);

        leK3I0 = new QLineEdit(pageInAnalogCalibration);
        leK3I0->setObjectName(QStringLiteral("leK3I0"));
        sizePolicy5.setHeightForWidth(leK3I0->sizePolicy().hasHeightForWidth());
        leK3I0->setSizePolicy(sizePolicy5);
        leK3I0->setMaximumSize(QSize(75, 16777215));
        leK3I0->setMaxLength(10);
        leK3I0->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leK3I0, 3, 1, 1, 1);

        lblTextCalibFactorPowerTotal = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerTotal->setObjectName(QStringLiteral("lblTextCalibFactorPowerTotal"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerTotal->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerTotal->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerTotal->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerTotal, 8, 0, 1, 1);

        lblTextCalibFactorCurrentPhase_A = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorCurrentPhase_A->setObjectName(QStringLiteral("lblTextCalibFactorCurrentPhase_A"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorCurrentPhase_A->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrentPhase_A->setSizePolicy(sizePolicy5);
        lblTextCalibFactorCurrentPhase_A->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrentPhase_A, 0, 0, 1, 1);

        lblTextCalibFactorCurrentPhase_B = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorCurrentPhase_B->setObjectName(QStringLiteral("lblTextCalibFactorCurrentPhase_B"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorCurrentPhase_B->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrentPhase_B->setSizePolicy(sizePolicy5);
        lblTextCalibFactorCurrentPhase_B->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrentPhase_B, 1, 0, 1, 1);

        leKIA = new QLineEdit(pageInAnalogCalibration);
        leKIA->setObjectName(QStringLiteral("leKIA"));
        sizePolicy5.setHeightForWidth(leKIA->sizePolicy().hasHeightForWidth());
        leKIA->setSizePolicy(sizePolicy5);
        leKIA->setMaximumSize(QSize(75, 16777215));
        leKIA->setMaxLength(10);
        leKIA->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKIA, 0, 1, 1, 1);

        leKIB = new QLineEdit(pageInAnalogCalibration);
        leKIB->setObjectName(QStringLiteral("leKIB"));
        sizePolicy5.setHeightForWidth(leKIB->sizePolicy().hasHeightForWidth());
        leKIB->setSizePolicy(sizePolicy5);
        leKIB->setMaximumSize(QSize(75, 16777215));
        leKIB->setMaxLength(10);
        leKIB->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKIB, 1, 1, 1, 1);

        lblTextCalibFactorCurrentPhase_C = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorCurrentPhase_C->setObjectName(QStringLiteral("lblTextCalibFactorCurrentPhase_C"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorCurrentPhase_C->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorCurrentPhase_C->setSizePolicy(sizePolicy5);
        lblTextCalibFactorCurrentPhase_C->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorCurrentPhase_C, 2, 0, 1, 1);

        leKIC = new QLineEdit(pageInAnalogCalibration);
        leKIC->setObjectName(QStringLiteral("leKIC"));
        sizePolicy5.setHeightForWidth(leKIC->sizePolicy().hasHeightForWidth());
        leKIC->setSizePolicy(sizePolicy5);
        leKIC->setMaximumSize(QSize(75, 16777215));
        leKIC->setMaxLength(10);
        leKIC->setAlignment(Qt::AlignCenter);

        gridLayout_8->addWidget(leKIC, 2, 1, 1, 1);

        lblTextCalibFactorPowerPhase_C_A = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorPowerPhase_C_A->setObjectName(QStringLiteral("lblTextCalibFactorPowerPhase_C_A"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorPowerPhase_C_A->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorPowerPhase_C_A->setSizePolicy(sizePolicy5);
        lblTextCalibFactorPowerPhase_C_A->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorPowerPhase_C_A, 2, 3, 1, 1);

        lblTextCalibFactorChannel3Us = new QLabel(pageInAnalogCalibration);
        lblTextCalibFactorChannel3Us->setObjectName(QStringLiteral("lblTextCalibFactorChannel3Us"));
        sizePolicy5.setHeightForWidth(lblTextCalibFactorChannel3Us->sizePolicy().hasHeightForWidth());
        lblTextCalibFactorChannel3Us->setSizePolicy(sizePolicy5);
        lblTextCalibFactorChannel3Us->setScaledContents(true);

        gridLayout_8->addWidget(lblTextCalibFactorChannel3Us, 8, 3, 1, 1);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_8->addItem(horizontalSpacer_24, 0, 2, 1, 1);


        horizontalLayout_5->addLayout(gridLayout_8);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);


        verticalLayout_59->addLayout(horizontalLayout_5);

        verticalSpacer_3 = new QSpacerItem(20, 128, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_59->addItem(verticalSpacer_3);

        stwgtMain->addWidget(pageInAnalogCalibration);
        pageProtectionImax = new QWidget();
        pageProtectionImax->setObjectName(QStringLiteral("pageProtectionImax"));
        horizontalLayout_77 = new QHBoxLayout(pageProtectionImax);
        horizontalLayout_77->setSpacing(6);
        horizontalLayout_77->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_77->setObjectName(QStringLiteral("horizontalLayout_77"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_53 = new QHBoxLayout();
        horizontalLayout_53->setSpacing(6);
        horizontalLayout_53->setObjectName(QStringLiteral("horizontalLayout_53"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        pbtnProtectionMTZ1 = new QPushButton(pageProtectionImax);
        pbtnProtectionMTZ1->setObjectName(QStringLiteral("pbtnProtectionMTZ1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionMTZ1->sizePolicy().hasHeightForWidth());
        pbtnProtectionMTZ1->setSizePolicy(sizePolicy5);
        pbtnProtectionMTZ1->setCheckable(false);

        verticalLayout_12->addWidget(pbtnProtectionMTZ1);

        pbtnProtectionMTZ2 = new QPushButton(pageProtectionImax);
        pbtnProtectionMTZ2->setObjectName(QStringLiteral("pbtnProtectionMTZ2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionMTZ2->sizePolicy().hasHeightForWidth());
        pbtnProtectionMTZ2->setSizePolicy(sizePolicy5);
        pbtnProtectionMTZ2->setCheckable(false);

        verticalLayout_12->addWidget(pbtnProtectionMTZ2);

        pbtnProtectionMTZ3 = new QPushButton(pageProtectionImax);
        pbtnProtectionMTZ3->setObjectName(QStringLiteral("pbtnProtectionMTZ3"));
        sizePolicy5.setHeightForWidth(pbtnProtectionMTZ3->sizePolicy().hasHeightForWidth());
        pbtnProtectionMTZ3->setSizePolicy(sizePolicy5);
        pbtnProtectionMTZ3->setCheckable(false);

        verticalLayout_12->addWidget(pbtnProtectionMTZ3);

        pbtnProtectionMTZ4 = new QPushButton(pageProtectionImax);
        pbtnProtectionMTZ4->setObjectName(QStringLiteral("pbtnProtectionMTZ4"));
        sizePolicy5.setHeightForWidth(pbtnProtectionMTZ4->sizePolicy().hasHeightForWidth());
        pbtnProtectionMTZ4->setSizePolicy(sizePolicy5);
        pbtnProtectionMTZ4->setCheckable(false);

        verticalLayout_12->addWidget(pbtnProtectionMTZ4);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_5);


        horizontalLayout_53->addLayout(verticalLayout_12);

        stwgtProtectionPropertiesMTZ = new QStackedWidget(pageProtectionImax);
        stwgtProtectionPropertiesMTZ->setObjectName(QStringLiteral("stwgtProtectionPropertiesMTZ"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesMTZ->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesMTZ->setSizePolicy(sizePolicy);
        pageProtectionPropertiesMTZ1 = new QWidget();
        pageProtectionPropertiesMTZ1->setObjectName(QStringLiteral("pageProtectionPropertiesMTZ1"));
        verticalLayout_10 = new QVBoxLayout(pageProtectionPropertiesMTZ1);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        gboxProtectionPropertiesMTZ1 = new QGroupBox(pageProtectionPropertiesMTZ1);
        gboxProtectionPropertiesMTZ1->setObjectName(QStringLiteral("gboxProtectionPropertiesMTZ1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesMTZ1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesMTZ1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesMTZ1->setAlignment(Qt::AlignCenter);
        horizontalLayout_6 = new QHBoxLayout(gboxProtectionPropertiesMTZ1);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        lblTextProtectionMTZ_Ctrl = new QLabel(gboxProtectionPropertiesMTZ1);
        lblTextProtectionMTZ_Ctrl->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(lblTextProtectionMTZ_Ctrl->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Ctrl->setSizePolicy(sizePolicy6);
        lblTextProtectionMTZ_Ctrl->setScaledContents(true);

        verticalLayout_11->addWidget(lblTextProtectionMTZ_Ctrl);

        lblTextProtectionMTZ_StartCurrent = new QLabel(gboxProtectionPropertiesMTZ1);
        lblTextProtectionMTZ_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionMTZ_StartCurrent"));
        sizePolicy6.setHeightForWidth(lblTextProtectionMTZ_StartCurrent->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_StartCurrent->setSizePolicy(sizePolicy6);
        lblTextProtectionMTZ_StartCurrent->setScaledContents(true);

        verticalLayout_11->addWidget(lblTextProtectionMTZ_StartCurrent);

        lblTextProtectionMTZ_Timeout = new QLabel(gboxProtectionPropertiesMTZ1);
        lblTextProtectionMTZ_Timeout->setObjectName(QStringLiteral("lblTextProtectionMTZ_Timeout"));
        sizePolicy6.setHeightForWidth(lblTextProtectionMTZ_Timeout->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Timeout->setSizePolicy(sizePolicy6);
        lblTextProtectionMTZ_Timeout->setScaledContents(true);

        verticalLayout_11->addWidget(lblTextProtectionMTZ_Timeout);

        lblTextProtectionMTZ_EnterBlocking = new QLabel(gboxProtectionPropertiesMTZ1);
        lblTextProtectionMTZ_EnterBlocking->setObjectName(QStringLiteral("lblTextProtectionMTZ_EnterBlocking"));
        sizePolicy6.setHeightForWidth(lblTextProtectionMTZ_EnterBlocking->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_EnterBlocking->setSizePolicy(sizePolicy6);
        lblTextProtectionMTZ_EnterBlocking->setScaledContents(true);

        verticalLayout_11->addWidget(lblTextProtectionMTZ_EnterBlocking);

        lblTextProtectionMTZ_EnterBlocking_5 = new QLabel(gboxProtectionPropertiesMTZ1);
        lblTextProtectionMTZ_EnterBlocking_5->setObjectName(QStringLiteral("lblTextProtectionMTZ_EnterBlocking_5"));
        sizePolicy6.setHeightForWidth(lblTextProtectionMTZ_EnterBlocking_5->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_EnterBlocking_5->setSizePolicy(sizePolicy6);
        lblTextProtectionMTZ_EnterBlocking_5->setScaledContents(true);

        verticalLayout_11->addWidget(lblTextProtectionMTZ_EnterBlocking_5);


        horizontalLayout_6->addLayout(verticalLayout_11);

        vltProtectionMTZ31 = new QVBoxLayout();
        vltProtectionMTZ31->setSpacing(6);
        vltProtectionMTZ31->setObjectName(QStringLiteral("vltProtectionMTZ31"));
        cboxM05 = new QComboBox(gboxProtectionPropertiesMTZ1);
        cboxM05->addItem(QString());
        cboxM05->addItem(QString());
        cboxM05->addItem(QString());
        cboxM05->addItem(QString());
        cboxM05->setObjectName(QStringLiteral("cboxM05"));
        cboxM05->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31->addWidget(cboxM05);

        leM06 = new QLineEdit(gboxProtectionPropertiesMTZ1);
        leM06->setObjectName(QStringLiteral("leM06"));
        sizePolicy5.setHeightForWidth(leM06->sizePolicy().hasHeightForWidth());
        leM06->setSizePolicy(sizePolicy5);
        leM06->setMaximumSize(QSize(100, 100));
        leM06->setMaxLength(8);
        leM06->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31->addWidget(leM06);

        leM08 = new QLineEdit(gboxProtectionPropertiesMTZ1);
        leM08->setObjectName(QStringLiteral("leM08"));
        sizePolicy5.setHeightForWidth(leM08->sizePolicy().hasHeightForWidth());
        leM08->setSizePolicy(sizePolicy5);
        leM08->setMaximumSize(QSize(100, 100));
        leM08->setMaxLength(8);
        leM08->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31->addWidget(leM08);

        leK31 = new QLineEdit(gboxProtectionPropertiesMTZ1);
        leK31->setObjectName(QStringLiteral("leK31"));
        sizePolicy5.setHeightForWidth(leK31->sizePolicy().hasHeightForWidth());
        leK31->setSizePolicy(sizePolicy5);
        leK31->setMaximumSize(QSize(100, 100));
        leK31->setMaxLength(8);
        leK31->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31->addWidget(leK31);

        leX01 = new QLineEdit(gboxProtectionPropertiesMTZ1);
        leX01->setObjectName(QStringLiteral("leX01"));
        sizePolicy5.setHeightForWidth(leX01->sizePolicy().hasHeightForWidth());
        leX01->setSizePolicy(sizePolicy5);
        leX01->setMaximumSize(QSize(100, 100));
        leX01->setMaxLength(8);
        leX01->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31->addWidget(leX01);


        horizontalLayout_6->addLayout(vltProtectionMTZ31);


        horizontalLayout_8->addWidget(gboxProtectionPropertiesMTZ1);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_13);


        verticalLayout_10->addLayout(horizontalLayout_8);

        verticalSpacer_23 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_10->addItem(verticalSpacer_23);

        stwgtProtectionPropertiesMTZ->addWidget(pageProtectionPropertiesMTZ1);
        pageProtectionPropertiesMTZ2 = new QWidget();
        pageProtectionPropertiesMTZ2->setObjectName(QStringLiteral("pageProtectionPropertiesMTZ2"));
        verticalLayout_88 = new QVBoxLayout(pageProtectionPropertiesMTZ2);
        verticalLayout_88->setSpacing(6);
        verticalLayout_88->setContentsMargins(11, 11, 11, 11);
        verticalLayout_88->setObjectName(QStringLiteral("verticalLayout_88"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        gboxProtectionPropertiesMTZ2 = new QGroupBox(pageProtectionPropertiesMTZ2);
        gboxProtectionPropertiesMTZ2->setObjectName(QStringLiteral("gboxProtectionPropertiesMTZ2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesMTZ2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesMTZ2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesMTZ2->setAlignment(Qt::AlignCenter);
        horizontalLayout_10 = new QHBoxLayout(gboxProtectionPropertiesMTZ2);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        lblTextProtectionMTZ_Ctrl_2 = new QLabel(gboxProtectionPropertiesMTZ2);
        lblTextProtectionMTZ_Ctrl_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_2"));
        lblTextProtectionMTZ_Ctrl_2->setScaledContents(true);

        verticalLayout_13->addWidget(lblTextProtectionMTZ_Ctrl_2);

        lblTextProtectionMTZ_StartCurrent_2 = new QLabel(gboxProtectionPropertiesMTZ2);
        lblTextProtectionMTZ_StartCurrent_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_StartCurrent_2"));
        lblTextProtectionMTZ_StartCurrent_2->setScaledContents(true);

        verticalLayout_13->addWidget(lblTextProtectionMTZ_StartCurrent_2);

        lblTextProtectionMTZ_Timeout_2 = new QLabel(gboxProtectionPropertiesMTZ2);
        lblTextProtectionMTZ_Timeout_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_Timeout_2"));
        lblTextProtectionMTZ_Timeout_2->setScaledContents(true);

        verticalLayout_13->addWidget(lblTextProtectionMTZ_Timeout_2);

        lblTextProtectionMTZ_Acceleration = new QLabel(gboxProtectionPropertiesMTZ2);
        lblTextProtectionMTZ_Acceleration->setObjectName(QStringLiteral("lblTextProtectionMTZ_Acceleration"));
        lblTextProtectionMTZ_Acceleration->setScaledContents(true);

        verticalLayout_13->addWidget(lblTextProtectionMTZ_Acceleration);

        lblTextProtectionMTZ_Acceleration_2 = new QLabel(gboxProtectionPropertiesMTZ2);
        lblTextProtectionMTZ_Acceleration_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_Acceleration_2"));
        lblTextProtectionMTZ_Acceleration_2->setScaledContents(true);

        verticalLayout_13->addWidget(lblTextProtectionMTZ_Acceleration_2);


        horizontalLayout_10->addLayout(verticalLayout_13);

        vltProtectionMTZ31_2 = new QVBoxLayout();
        vltProtectionMTZ31_2->setSpacing(6);
        vltProtectionMTZ31_2->setObjectName(QStringLiteral("vltProtectionMTZ31_2"));
        cboxM09 = new QComboBox(gboxProtectionPropertiesMTZ2);
        cboxM09->addItem(QString());
        cboxM09->addItem(QString());
        cboxM09->addItem(QString());
        cboxM09->addItem(QString());
        cboxM09->setObjectName(QStringLiteral("cboxM09"));
        cboxM09->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_2->addWidget(cboxM09);

        leM10 = new QLineEdit(gboxProtectionPropertiesMTZ2);
        leM10->setObjectName(QStringLiteral("leM10"));
        sizePolicy5.setHeightForWidth(leM10->sizePolicy().hasHeightForWidth());
        leM10->setSizePolicy(sizePolicy5);
        leM10->setMaximumSize(QSize(100, 100));
        leM10->setMaxLength(10);
        leM10->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_2->addWidget(leM10);

        leM11 = new QLineEdit(gboxProtectionPropertiesMTZ2);
        leM11->setObjectName(QStringLiteral("leM11"));
        sizePolicy5.setHeightForWidth(leM11->sizePolicy().hasHeightForWidth());
        leM11->setSizePolicy(sizePolicy5);
        leM11->setMaximumSize(QSize(100, 100));
        leM11->setMaxLength(10);
        leM11->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_2->addWidget(leM11);

        leM12 = new QLineEdit(gboxProtectionPropertiesMTZ2);
        leM12->setObjectName(QStringLiteral("leM12"));
        sizePolicy5.setHeightForWidth(leM12->sizePolicy().hasHeightForWidth());
        leM12->setSizePolicy(sizePolicy5);
        leM12->setMaximumSize(QSize(100, 100));
        leM12->setMaxLength(10);
        leM12->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_2->addWidget(leM12);

        leX03 = new QLineEdit(gboxProtectionPropertiesMTZ2);
        leX03->setObjectName(QStringLiteral("leX03"));
        sizePolicy5.setHeightForWidth(leX03->sizePolicy().hasHeightForWidth());
        leX03->setSizePolicy(sizePolicy5);
        leX03->setMaximumSize(QSize(100, 100));
        leX03->setMaxLength(10);
        leX03->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_2->addWidget(leX03);


        horizontalLayout_10->addLayout(vltProtectionMTZ31_2);


        horizontalLayout_17->addWidget(gboxProtectionPropertiesMTZ2);

        horizontalSpacer_34 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_34);


        verticalLayout_88->addLayout(horizontalLayout_17);

        verticalSpacer_24 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_88->addItem(verticalSpacer_24);

        stwgtProtectionPropertiesMTZ->addWidget(pageProtectionPropertiesMTZ2);
        pageProtectionPropertiesMTZ3 = new QWidget();
        pageProtectionPropertiesMTZ3->setObjectName(QStringLiteral("pageProtectionPropertiesMTZ3"));
        verticalLayout_89 = new QVBoxLayout(pageProtectionPropertiesMTZ3);
        verticalLayout_89->setSpacing(6);
        verticalLayout_89->setContentsMargins(11, 11, 11, 11);
        verticalLayout_89->setObjectName(QStringLiteral("verticalLayout_89"));
        horizontalLayout_76 = new QHBoxLayout();
        horizontalLayout_76->setSpacing(6);
        horizontalLayout_76->setObjectName(QStringLiteral("horizontalLayout_76"));
        gboxProtectionPropertiesMTZ3 = new QGroupBox(pageProtectionPropertiesMTZ3);
        gboxProtectionPropertiesMTZ3->setObjectName(QStringLiteral("gboxProtectionPropertiesMTZ3"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesMTZ3->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesMTZ3->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesMTZ3->setAlignment(Qt::AlignCenter);
        horizontalLayout_11 = new QHBoxLayout(gboxProtectionPropertiesMTZ3);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        lblTextProtectionMTZ_Ctrl_3 = new QLabel(gboxProtectionPropertiesMTZ3);
        lblTextProtectionMTZ_Ctrl_3->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_3"));
        lblTextProtectionMTZ_Ctrl_3->setScaledContents(true);

        verticalLayout_14->addWidget(lblTextProtectionMTZ_Ctrl_3);

        lblTextProtectionMTZ_StartCurrent_3 = new QLabel(gboxProtectionPropertiesMTZ3);
        lblTextProtectionMTZ_StartCurrent_3->setObjectName(QStringLiteral("lblTextProtectionMTZ_StartCurrent_3"));
        lblTextProtectionMTZ_StartCurrent_3->setScaledContents(true);

        verticalLayout_14->addWidget(lblTextProtectionMTZ_StartCurrent_3);

        lblTextProtectionMTZ_Timeout_3 = new QLabel(gboxProtectionPropertiesMTZ3);
        lblTextProtectionMTZ_Timeout_3->setObjectName(QStringLiteral("lblTextProtectionMTZ_Timeout_3"));
        lblTextProtectionMTZ_Timeout_3->setScaledContents(true);

        verticalLayout_14->addWidget(lblTextProtectionMTZ_Timeout_3);

        lblTextProtectionMTZ_Factor = new QLabel(gboxProtectionPropertiesMTZ3);
        lblTextProtectionMTZ_Factor->setObjectName(QStringLiteral("lblTextProtectionMTZ_Factor"));
        lblTextProtectionMTZ_Factor->setScaledContents(true);

        verticalLayout_14->addWidget(lblTextProtectionMTZ_Factor);

        lblTextProtectionMTZ_Factor_2 = new QLabel(gboxProtectionPropertiesMTZ3);
        lblTextProtectionMTZ_Factor_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_Factor_2"));
        lblTextProtectionMTZ_Factor_2->setScaledContents(true);

        verticalLayout_14->addWidget(lblTextProtectionMTZ_Factor_2);


        horizontalLayout_11->addLayout(verticalLayout_14);

        vltProtectionMTZ31_3 = new QVBoxLayout();
        vltProtectionMTZ31_3->setSpacing(6);
        vltProtectionMTZ31_3->setObjectName(QStringLiteral("vltProtectionMTZ31_3"));
        cboxM13 = new QComboBox(gboxProtectionPropertiesMTZ3);
        cboxM13->addItem(QString());
        cboxM13->addItem(QString());
        cboxM13->addItem(QString());
        cboxM13->addItem(QString());
        cboxM13->addItem(QString());
        cboxM13->addItem(QString());
        cboxM13->setObjectName(QStringLiteral("cboxM13"));
        cboxM13->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_3->addWidget(cboxM13);

        leM14 = new QLineEdit(gboxProtectionPropertiesMTZ3);
        leM14->setObjectName(QStringLiteral("leM14"));
        sizePolicy5.setHeightForWidth(leM14->sizePolicy().hasHeightForWidth());
        leM14->setSizePolicy(sizePolicy5);
        leM14->setMaximumSize(QSize(100, 100));
        leM14->setMaxLength(10);
        leM14->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_3->addWidget(leM14);

        leK22 = new QLineEdit(gboxProtectionPropertiesMTZ3);
        leK22->setObjectName(QStringLiteral("leK22"));
        sizePolicy5.setHeightForWidth(leK22->sizePolicy().hasHeightForWidth());
        leK22->setSizePolicy(sizePolicy5);
        leK22->setMaximumSize(QSize(100, 100));
        leK22->setMaxLength(10);
        leK22->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_3->addWidget(leK22);

        leM15 = new QLineEdit(gboxProtectionPropertiesMTZ3);
        leM15->setObjectName(QStringLiteral("leM15"));
        sizePolicy5.setHeightForWidth(leM15->sizePolicy().hasHeightForWidth());
        leM15->setSizePolicy(sizePolicy5);
        leM15->setMaximumSize(QSize(100, 100));
        leM15->setMaxLength(10);
        leM15->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_3->addWidget(leM15);

        leX04 = new QLineEdit(gboxProtectionPropertiesMTZ3);
        leX04->setObjectName(QStringLiteral("leX04"));
        sizePolicy5.setHeightForWidth(leX04->sizePolicy().hasHeightForWidth());
        leX04->setSizePolicy(sizePolicy5);
        leX04->setMaximumSize(QSize(100, 100));
        leX04->setMaxLength(10);
        leX04->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_3->addWidget(leX04);


        horizontalLayout_11->addLayout(vltProtectionMTZ31_3);


        horizontalLayout_76->addWidget(gboxProtectionPropertiesMTZ3);

        horizontalSpacer_35 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_76->addItem(horizontalSpacer_35);


        verticalLayout_89->addLayout(horizontalLayout_76);

        verticalSpacer_25 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_89->addItem(verticalSpacer_25);

        stwgtProtectionPropertiesMTZ->addWidget(pageProtectionPropertiesMTZ3);
        pageProtectionPropertiesMTZ4 = new QWidget();
        pageProtectionPropertiesMTZ4->setObjectName(QStringLiteral("pageProtectionPropertiesMTZ4"));
        verticalLayout_91 = new QVBoxLayout(pageProtectionPropertiesMTZ4);
        verticalLayout_91->setSpacing(6);
        verticalLayout_91->setContentsMargins(11, 11, 11, 11);
        verticalLayout_91->setObjectName(QStringLiteral("verticalLayout_91"));
        horizontalLayout_78 = new QHBoxLayout();
        horizontalLayout_78->setSpacing(6);
        horizontalLayout_78->setObjectName(QStringLiteral("horizontalLayout_78"));
        gboxProtectionPropertiesMTZ4 = new QGroupBox(pageProtectionPropertiesMTZ4);
        gboxProtectionPropertiesMTZ4->setObjectName(QStringLiteral("gboxProtectionPropertiesMTZ4"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesMTZ4->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesMTZ4->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesMTZ4->setAlignment(Qt::AlignCenter);
        horizontalLayout_12 = new QHBoxLayout(gboxProtectionPropertiesMTZ4);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        lblTextProtectionMTZ_Ctrl_4 = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_Ctrl_4->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_4"));
        lblTextProtectionMTZ_Ctrl_4->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_Ctrl_4);

        lblTextProtectionMTZ_StartCurrent_4 = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_StartCurrent_4->setObjectName(QStringLiteral("lblTextProtectionMTZ_StartCurrent_4"));
        lblTextProtectionMTZ_StartCurrent_4->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_StartCurrent_4);

        lblTextProtectionMTZ_DirStart = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_DirStart->setObjectName(QStringLiteral("lblTextProtectionMTZ_DirStart"));
        lblTextProtectionMTZ_DirStart->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_DirStart);

        lblTextProtectionMTZ_Angle = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_Angle->setObjectName(QStringLiteral("lblTextProtectionMTZ_Angle"));
        lblTextProtectionMTZ_Angle->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_Angle);

        lblTextProtectionMTZ_Angle_2 = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_Angle_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_Angle_2"));
        lblTextProtectionMTZ_Angle_2->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_Angle_2);

        lblTextProtectionMTZ_Angle_3 = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_Angle_3->setObjectName(QStringLiteral("lblTextProtectionMTZ_Angle_3"));
        lblTextProtectionMTZ_Angle_3->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_Angle_3);

        lblTextProtectionMTZ_Angle_4 = new QLabel(gboxProtectionPropertiesMTZ4);
        lblTextProtectionMTZ_Angle_4->setObjectName(QStringLiteral("lblTextProtectionMTZ_Angle_4"));
        lblTextProtectionMTZ_Angle_4->setScaledContents(true);

        verticalLayout_15->addWidget(lblTextProtectionMTZ_Angle_4);


        horizontalLayout_12->addLayout(verticalLayout_15);

        vltProtectionMTZ31_4 = new QVBoxLayout();
        vltProtectionMTZ31_4->setSpacing(6);
        vltProtectionMTZ31_4->setObjectName(QStringLiteral("vltProtectionMTZ31_4"));
        cboxM16 = new QComboBox(gboxProtectionPropertiesMTZ4);
        cboxM16->addItem(QString());
        cboxM16->addItem(QString());
        cboxM16->addItem(QString());
        cboxM16->addItem(QString());
        cboxM16->setObjectName(QStringLiteral("cboxM16"));
        cboxM16->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_4->addWidget(cboxM16);

        leM17 = new QLineEdit(gboxProtectionPropertiesMTZ4);
        leM17->setObjectName(QStringLiteral("leM17"));
        sizePolicy5.setHeightForWidth(leM17->sizePolicy().hasHeightForWidth());
        leM17->setSizePolicy(sizePolicy5);
        leM17->setMaximumSize(QSize(100, 100));
        leM17->setMaxLength(10);
        leM17->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_4->addWidget(leM17);

        leM07 = new QLineEdit(gboxProtectionPropertiesMTZ4);
        leM07->setObjectName(QStringLiteral("leM07"));
        sizePolicy5.setHeightForWidth(leM07->sizePolicy().hasHeightForWidth());
        leM07->setSizePolicy(sizePolicy5);
        leM07->setMaximumSize(QSize(100, 100));
        leM07->setMaxLength(10);
        leM07->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_4->addWidget(leM07);

        leK20 = new QLineEdit(gboxProtectionPropertiesMTZ4);
        leK20->setObjectName(QStringLiteral("leK20"));
        sizePolicy5.setHeightForWidth(leK20->sizePolicy().hasHeightForWidth());
        leK20->setSizePolicy(sizePolicy5);
        leK20->setMaximumSize(QSize(100, 100));
        leK20->setMaxLength(4);
        leK20->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_4->addWidget(leK20);

        leM18 = new QLineEdit(gboxProtectionPropertiesMTZ4);
        leM18->setObjectName(QStringLiteral("leM18"));
        sizePolicy5.setHeightForWidth(leM18->sizePolicy().hasHeightForWidth());
        leM18->setSizePolicy(sizePolicy5);
        leM18->setMaximumSize(QSize(100, 100));
        leM18->setMaxLength(10);
        leM18->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_4->addWidget(leM18);

        leX05 = new QLineEdit(gboxProtectionPropertiesMTZ4);
        leX05->setObjectName(QStringLiteral("leX05"));
        sizePolicy5.setHeightForWidth(leX05->sizePolicy().hasHeightForWidth());
        leX05->setSizePolicy(sizePolicy5);
        leX05->setMaximumSize(QSize(100, 100));
        leX05->setMaxLength(10);
        leX05->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_4->addWidget(leX05);

        leX05a = new QLineEdit(gboxProtectionPropertiesMTZ4);
        leX05a->setObjectName(QStringLiteral("leX05a"));
        sizePolicy5.setHeightForWidth(leX05a->sizePolicy().hasHeightForWidth());
        leX05a->setSizePolicy(sizePolicy5);
        leX05a->setMaximumSize(QSize(100, 100));
        leX05a->setMaxLength(10);
        leX05a->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_4->addWidget(leX05a);


        horizontalLayout_12->addLayout(vltProtectionMTZ31_4);


        horizontalLayout_78->addWidget(gboxProtectionPropertiesMTZ4);

        horizontalSpacer_36 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_78->addItem(horizontalSpacer_36);


        verticalLayout_91->addLayout(horizontalLayout_78);

        verticalSpacer_26 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_91->addItem(verticalSpacer_26);

        stwgtProtectionPropertiesMTZ->addWidget(pageProtectionPropertiesMTZ4);

        horizontalLayout_53->addWidget(stwgtProtectionPropertiesMTZ);


        verticalLayout_6->addLayout(horizontalLayout_53);

        verticalSpacer_21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_21);


        horizontalLayout_77->addLayout(verticalLayout_6);

        wgtChartMTZ = new QCustomPlot(pageProtectionImax);
        wgtChartMTZ->setObjectName(QStringLiteral("wgtChartMTZ"));
        sizePolicy.setHeightForWidth(wgtChartMTZ->sizePolicy().hasHeightForWidth());
        wgtChartMTZ->setSizePolicy(sizePolicy);

        horizontalLayout_77->addWidget(wgtChartMTZ);

        stwgtMain->addWidget(pageProtectionImax);
        pageProtectionEarthy = new QWidget();
        pageProtectionEarthy->setObjectName(QStringLiteral("pageProtectionEarthy"));
        verticalLayout_90 = new QVBoxLayout(pageProtectionEarthy);
        verticalLayout_90->setSpacing(6);
        verticalLayout_90->setContentsMargins(11, 11, 11, 11);
        verticalLayout_90->setObjectName(QStringLiteral("verticalLayout_90"));
        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(0);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        pbtnProtectionEarthly_OZZ1 = new QPushButton(pageProtectionEarthy);
        pbtnProtectionEarthly_OZZ1->setObjectName(QStringLiteral("pbtnProtectionEarthly_OZZ1"));
        pbtnProtectionEarthly_OZZ1->setCheckable(false);

        verticalLayout_20->addWidget(pbtnProtectionEarthly_OZZ1);

        pbtnProtectionEarthly_OZZ2 = new QPushButton(pageProtectionEarthy);
        pbtnProtectionEarthly_OZZ2->setObjectName(QStringLiteral("pbtnProtectionEarthly_OZZ2"));
        pbtnProtectionEarthly_OZZ2->setCheckable(false);

        verticalLayout_20->addWidget(pbtnProtectionEarthly_OZZ2);

        pbtnProtectionEarthly_NZZ1 = new QPushButton(pageProtectionEarthy);
        pbtnProtectionEarthly_NZZ1->setObjectName(QStringLiteral("pbtnProtectionEarthly_NZZ1"));
        pbtnProtectionEarthly_NZZ1->setCheckable(false);

        verticalLayout_20->addWidget(pbtnProtectionEarthly_NZZ1);

        pbtnProtectionEarthly_NZZ2 = new QPushButton(pageProtectionEarthy);
        pbtnProtectionEarthly_NZZ2->setObjectName(QStringLiteral("pbtnProtectionEarthly_NZZ2"));
        pbtnProtectionEarthly_NZZ2->setCheckable(false);

        verticalLayout_20->addWidget(pbtnProtectionEarthly_NZZ2);

        verticalSpacer_27 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer_27);


        horizontalLayout_23->addLayout(verticalLayout_20);

        stwgtProtectionPropertiesEarthly = new QStackedWidget(pageProtectionEarthy);
        stwgtProtectionPropertiesEarthly->setObjectName(QStringLiteral("stwgtProtectionPropertiesEarthly"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesEarthly->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesEarthly->setSizePolicy(sizePolicy);
        pageProtectionPropertiesOZZ1 = new QWidget();
        pageProtectionPropertiesOZZ1->setObjectName(QStringLiteral("pageProtectionPropertiesOZZ1"));
        verticalLayout_92 = new QVBoxLayout(pageProtectionPropertiesOZZ1);
        verticalLayout_92->setSpacing(6);
        verticalLayout_92->setContentsMargins(11, 11, 11, 11);
        verticalLayout_92->setObjectName(QStringLiteral("verticalLayout_92"));
        horizontalLayout_79 = new QHBoxLayout();
        horizontalLayout_79->setSpacing(6);
        horizontalLayout_79->setObjectName(QStringLiteral("horizontalLayout_79"));
        gboxProtectionPropertiesEarthly_OZZ1 = new QGroupBox(pageProtectionPropertiesOZZ1);
        gboxProtectionPropertiesEarthly_OZZ1->setObjectName(QStringLiteral("gboxProtectionPropertiesEarthly_OZZ1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesEarthly_OZZ1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesEarthly_OZZ1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesEarthly_OZZ1->setAlignment(Qt::AlignCenter);
        horizontalLayout_19 = new QHBoxLayout(gboxProtectionPropertiesEarthly_OZZ1);
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        lblTextProtectionEarthly_OZZ1_Ctrl = new QLabel(gboxProtectionPropertiesEarthly_OZZ1);
        lblTextProtectionEarthly_OZZ1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ1_Ctrl"));
        lblTextProtectionEarthly_OZZ1_Ctrl->setScaledContents(true);

        verticalLayout_21->addWidget(lblTextProtectionEarthly_OZZ1_Ctrl);

        lblTextProtectionEarthly_OZZ1_StartCurrent = new QLabel(gboxProtectionPropertiesEarthly_OZZ1);
        lblTextProtectionEarthly_OZZ1_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ1_StartCurrent"));
        lblTextProtectionEarthly_OZZ1_StartCurrent->setScaledContents(true);

        verticalLayout_21->addWidget(lblTextProtectionEarthly_OZZ1_StartCurrent);

        lblTextProtectionEarthly_OZZ1_Pause = new QLabel(gboxProtectionPropertiesEarthly_OZZ1);
        lblTextProtectionEarthly_OZZ1_Pause->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ1_Pause"));
        lblTextProtectionEarthly_OZZ1_Pause->setScaledContents(true);

        verticalLayout_21->addWidget(lblTextProtectionEarthly_OZZ1_Pause);

        lblTextProtectionEarthly_OZZ1_KvzCurrent = new QLabel(gboxProtectionPropertiesEarthly_OZZ1);
        lblTextProtectionEarthly_OZZ1_KvzCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ1_KvzCurrent"));
        lblTextProtectionEarthly_OZZ1_KvzCurrent->setScaledContents(true);

        verticalLayout_21->addWidget(lblTextProtectionEarthly_OZZ1_KvzCurrent);


        horizontalLayout_19->addLayout(verticalLayout_21);

        vltProtectionMTZ31_7 = new QVBoxLayout();
        vltProtectionMTZ31_7->setSpacing(6);
        vltProtectionMTZ31_7->setObjectName(QStringLiteral("vltProtectionMTZ31_7"));
        cboxM22 = new QComboBox(gboxProtectionPropertiesEarthly_OZZ1);
        cboxM22->addItem(QString());
        cboxM22->addItem(QString());
        cboxM22->addItem(QString());
        cboxM22->setObjectName(QStringLiteral("cboxM22"));
        cboxM22->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_7->addWidget(cboxM22);

        leM23 = new QLineEdit(gboxProtectionPropertiesEarthly_OZZ1);
        leM23->setObjectName(QStringLiteral("leM23"));
        sizePolicy5.setHeightForWidth(leM23->sizePolicy().hasHeightForWidth());
        leM23->setSizePolicy(sizePolicy5);
        leM23->setMaximumSize(QSize(100, 100));
        leM23->setMaxLength(10);
        leM23->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_7->addWidget(leM23);

        leM24 = new QLineEdit(gboxProtectionPropertiesEarthly_OZZ1);
        leM24->setObjectName(QStringLiteral("leM24"));
        sizePolicy5.setHeightForWidth(leM24->sizePolicy().hasHeightForWidth());
        leM24->setSizePolicy(sizePolicy5);
        leM24->setMaximumSize(QSize(100, 100));
        leM24->setMaxLength(10);
        leM24->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_7->addWidget(leM24);

        leX07 = new QLineEdit(gboxProtectionPropertiesEarthly_OZZ1);
        leX07->setObjectName(QStringLiteral("leX07"));
        sizePolicy5.setHeightForWidth(leX07->sizePolicy().hasHeightForWidth());
        leX07->setSizePolicy(sizePolicy5);
        leX07->setMaximumSize(QSize(100, 100));
        leX07->setMaxLength(10);
        leX07->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_7->addWidget(leX07);


        horizontalLayout_19->addLayout(vltProtectionMTZ31_7);


        horizontalLayout_79->addWidget(gboxProtectionPropertiesEarthly_OZZ1);

        horizontalSpacer_38 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_79->addItem(horizontalSpacer_38);


        verticalLayout_92->addLayout(horizontalLayout_79);

        verticalSpacer_28 = new QSpacerItem(20, 118, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_92->addItem(verticalSpacer_28);

        stwgtProtectionPropertiesEarthly->addWidget(pageProtectionPropertiesOZZ1);
        pageProtectionPropertiesOZZ2 = new QWidget();
        pageProtectionPropertiesOZZ2->setObjectName(QStringLiteral("pageProtectionPropertiesOZZ2"));
        verticalLayout_93 = new QVBoxLayout(pageProtectionPropertiesOZZ2);
        verticalLayout_93->setSpacing(6);
        verticalLayout_93->setContentsMargins(11, 11, 11, 11);
        verticalLayout_93->setObjectName(QStringLiteral("verticalLayout_93"));
        horizontalLayout_80 = new QHBoxLayout();
        horizontalLayout_80->setSpacing(6);
        horizontalLayout_80->setObjectName(QStringLiteral("horizontalLayout_80"));
        gboxProtectionPropertiesEarthly_OZZ2 = new QGroupBox(pageProtectionPropertiesOZZ2);
        gboxProtectionPropertiesEarthly_OZZ2->setObjectName(QStringLiteral("gboxProtectionPropertiesEarthly_OZZ2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesEarthly_OZZ2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesEarthly_OZZ2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesEarthly_OZZ2->setAlignment(Qt::AlignCenter);
        horizontalLayout_20 = new QHBoxLayout(gboxProtectionPropertiesEarthly_OZZ2);
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        lblTextProtectionEarthly_OZZ2_Ctrl = new QLabel(gboxProtectionPropertiesEarthly_OZZ2);
        lblTextProtectionEarthly_OZZ2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ2_Ctrl"));
        lblTextProtectionEarthly_OZZ2_Ctrl->setScaledContents(true);

        verticalLayout_22->addWidget(lblTextProtectionEarthly_OZZ2_Ctrl);

        lblTextProtectionEarthly_OZZ2_StartCurrent = new QLabel(gboxProtectionPropertiesEarthly_OZZ2);
        lblTextProtectionEarthly_OZZ2_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ2_StartCurrent"));
        lblTextProtectionEarthly_OZZ2_StartCurrent->setScaledContents(true);

        verticalLayout_22->addWidget(lblTextProtectionEarthly_OZZ2_StartCurrent);

        lblTextProtectionEarthly_OZZ2_Pause = new QLabel(gboxProtectionPropertiesEarthly_OZZ2);
        lblTextProtectionEarthly_OZZ2_Pause->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ2_Pause"));
        lblTextProtectionEarthly_OZZ2_Pause->setScaledContents(true);

        verticalLayout_22->addWidget(lblTextProtectionEarthly_OZZ2_Pause);

        lblTextProtectionEarthly_OZZ2_KvzCurrent = new QLabel(gboxProtectionPropertiesEarthly_OZZ2);
        lblTextProtectionEarthly_OZZ2_KvzCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_OZZ2_KvzCurrent"));
        lblTextProtectionEarthly_OZZ2_KvzCurrent->setScaledContents(true);

        verticalLayout_22->addWidget(lblTextProtectionEarthly_OZZ2_KvzCurrent);


        horizontalLayout_20->addLayout(verticalLayout_22);

        vltProtectionMTZ31_8 = new QVBoxLayout();
        vltProtectionMTZ31_8->setSpacing(6);
        vltProtectionMTZ31_8->setObjectName(QStringLiteral("vltProtectionMTZ31_8"));
        cboxK23 = new QComboBox(gboxProtectionPropertiesEarthly_OZZ2);
        cboxK23->addItem(QString());
        cboxK23->addItem(QString());
        cboxK23->addItem(QString());
        cboxK23->setObjectName(QStringLiteral("cboxK23"));
        cboxK23->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_8->addWidget(cboxK23);

        leK24 = new QLineEdit(gboxProtectionPropertiesEarthly_OZZ2);
        leK24->setObjectName(QStringLiteral("leK24"));
        sizePolicy5.setHeightForWidth(leK24->sizePolicy().hasHeightForWidth());
        leK24->setSizePolicy(sizePolicy5);
        leK24->setMaximumSize(QSize(100, 100));
        leK24->setMaxLength(10);
        leK24->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_8->addWidget(leK24);

        leK25 = new QLineEdit(gboxProtectionPropertiesEarthly_OZZ2);
        leK25->setObjectName(QStringLiteral("leK25"));
        sizePolicy5.setHeightForWidth(leK25->sizePolicy().hasHeightForWidth());
        leK25->setSizePolicy(sizePolicy5);
        leK25->setMaximumSize(QSize(100, 100));
        leK25->setMaxLength(10);
        leK25->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_8->addWidget(leK25);

        leX07a = new QLineEdit(gboxProtectionPropertiesEarthly_OZZ2);
        leX07a->setObjectName(QStringLiteral("leX07a"));
        sizePolicy5.setHeightForWidth(leX07a->sizePolicy().hasHeightForWidth());
        leX07a->setSizePolicy(sizePolicy5);
        leX07a->setMaximumSize(QSize(100, 100));
        leX07a->setMaxLength(10);
        leX07a->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_8->addWidget(leX07a);


        horizontalLayout_20->addLayout(vltProtectionMTZ31_8);


        horizontalLayout_80->addWidget(gboxProtectionPropertiesEarthly_OZZ2);

        horizontalSpacer_39 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_80->addItem(horizontalSpacer_39);


        verticalLayout_93->addLayout(horizontalLayout_80);

        verticalSpacer_29 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_93->addItem(verticalSpacer_29);

        stwgtProtectionPropertiesEarthly->addWidget(pageProtectionPropertiesOZZ2);
        pageProtectionPropertiesNZZ1 = new QWidget();
        pageProtectionPropertiesNZZ1->setObjectName(QStringLiteral("pageProtectionPropertiesNZZ1"));
        verticalLayout_94 = new QVBoxLayout(pageProtectionPropertiesNZZ1);
        verticalLayout_94->setSpacing(6);
        verticalLayout_94->setContentsMargins(11, 11, 11, 11);
        verticalLayout_94->setObjectName(QStringLiteral("verticalLayout_94"));
        horizontalLayout_81 = new QHBoxLayout();
        horizontalLayout_81->setSpacing(6);
        horizontalLayout_81->setObjectName(QStringLiteral("horizontalLayout_81"));
        gboxProtectionPropertiesEarthly_NZZ1 = new QGroupBox(pageProtectionPropertiesNZZ1);
        gboxProtectionPropertiesEarthly_NZZ1->setObjectName(QStringLiteral("gboxProtectionPropertiesEarthly_NZZ1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesEarthly_NZZ1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesEarthly_NZZ1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesEarthly_NZZ1->setAlignment(Qt::AlignCenter);
        horizontalLayout_21 = new QHBoxLayout(gboxProtectionPropertiesEarthly_NZZ1);
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setSpacing(6);
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        lblTextProtectionEarthly_NZZ1_Ctrl = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_Ctrl"));
        lblTextProtectionEarthly_NZZ1_Ctrl->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_Ctrl);

        lblTextProtectionEarthly_NZZ1_StartCurrent = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_StartCurrent"));
        lblTextProtectionEarthly_NZZ1_StartCurrent->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_StartCurrent);

        lblTextProtectionEarthly_NZZ1_StartPower = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_StartPower->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_StartPower"));
        lblTextProtectionEarthly_NZZ1_StartPower->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_StartPower);

        lblTextProtectionEarthly_NZZ1_Angle = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_Angle->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_Angle"));
        lblTextProtectionEarthly_NZZ1_Angle->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_Angle);

        lblTextProtectionEarthly_NZZ1_Pause = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_Pause->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_Pause"));
        lblTextProtectionEarthly_NZZ1_Pause->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_Pause);

        lblTextProtectionEarthly_NZZ1_KvzCurrent = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_KvzCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_KvzCurrent"));
        lblTextProtectionEarthly_NZZ1_KvzCurrent->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_KvzCurrent);

        lblTextProtectionEarthly_NZZ1_KvzPower = new QLabel(gboxProtectionPropertiesEarthly_NZZ1);
        lblTextProtectionEarthly_NZZ1_KvzPower->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ1_KvzPower"));
        lblTextProtectionEarthly_NZZ1_KvzPower->setScaledContents(true);

        verticalLayout_23->addWidget(lblTextProtectionEarthly_NZZ1_KvzPower);


        horizontalLayout_21->addLayout(verticalLayout_23);

        vltProtectionMTZ31_9 = new QVBoxLayout();
        vltProtectionMTZ31_9->setSpacing(6);
        vltProtectionMTZ31_9->setObjectName(QStringLiteral("vltProtectionMTZ31_9"));
        cboxM25 = new QComboBox(gboxProtectionPropertiesEarthly_NZZ1);
        cboxM25->addItem(QString());
        cboxM25->addItem(QString());
        cboxM25->addItem(QString());
        cboxM25->setObjectName(QStringLiteral("cboxM25"));
        cboxM25->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_9->addWidget(cboxM25);

        leM26 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ1);
        leM26->setObjectName(QStringLiteral("leM26"));
        sizePolicy5.setHeightForWidth(leM26->sizePolicy().hasHeightForWidth());
        leM26->setSizePolicy(sizePolicy5);
        leM26->setMaximumSize(QSize(100, 100));
        leM26->setMaxLength(10);
        leM26->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_9->addWidget(leM26);

        leM27 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ1);
        leM27->setObjectName(QStringLiteral("leM27"));
        sizePolicy5.setHeightForWidth(leM27->sizePolicy().hasHeightForWidth());
        leM27->setSizePolicy(sizePolicy5);
        leM27->setMaximumSize(QSize(100, 100));
        leM27->setMaxLength(10);
        leM27->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_9->addWidget(leM27);

        leK21 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ1);
        leK21->setObjectName(QStringLiteral("leK21"));
        sizePolicy5.setHeightForWidth(leK21->sizePolicy().hasHeightForWidth());
        leK21->setSizePolicy(sizePolicy5);
        leK21->setMaximumSize(QSize(100, 100));
        leK21->setMaxLength(4);
        leK21->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_9->addWidget(leK21);

        leM28 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ1);
        leM28->setObjectName(QStringLiteral("leM28"));
        sizePolicy5.setHeightForWidth(leM28->sizePolicy().hasHeightForWidth());
        leM28->setSizePolicy(sizePolicy5);
        leM28->setMaximumSize(QSize(100, 100));
        leM28->setMaxLength(10);
        leM28->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_9->addWidget(leM28);

        leX08 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ1);
        leX08->setObjectName(QStringLiteral("leX08"));
        sizePolicy5.setHeightForWidth(leX08->sizePolicy().hasHeightForWidth());
        leX08->setSizePolicy(sizePolicy5);
        leX08->setMaximumSize(QSize(100, 100));
        leX08->setMaxLength(10);
        leX08->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_9->addWidget(leX08);

        leX09 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ1);
        leX09->setObjectName(QStringLiteral("leX09"));
        sizePolicy5.setHeightForWidth(leX09->sizePolicy().hasHeightForWidth());
        leX09->setSizePolicy(sizePolicy5);
        leX09->setMaximumSize(QSize(100, 100));
        leX09->setMaxLength(10);
        leX09->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_9->addWidget(leX09);


        horizontalLayout_21->addLayout(vltProtectionMTZ31_9);


        horizontalLayout_81->addWidget(gboxProtectionPropertiesEarthly_NZZ1);

        horizontalSpacer_40 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_81->addItem(horizontalSpacer_40);


        verticalLayout_94->addLayout(horizontalLayout_81);

        verticalSpacer_30 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_94->addItem(verticalSpacer_30);

        stwgtProtectionPropertiesEarthly->addWidget(pageProtectionPropertiesNZZ1);
        pageProtectionPropertiesNZZ2 = new QWidget();
        pageProtectionPropertiesNZZ2->setObjectName(QStringLiteral("pageProtectionPropertiesNZZ2"));
        verticalLayout_25 = new QVBoxLayout(pageProtectionPropertiesNZZ2);
        verticalLayout_25->setSpacing(6);
        verticalLayout_25->setContentsMargins(11, 11, 11, 11);
        verticalLayout_25->setObjectName(QStringLiteral("verticalLayout_25"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        gboxProtectionPropertiesEarthly_NZZ2 = new QGroupBox(pageProtectionPropertiesNZZ2);
        gboxProtectionPropertiesEarthly_NZZ2->setObjectName(QStringLiteral("gboxProtectionPropertiesEarthly_NZZ2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesEarthly_NZZ2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesEarthly_NZZ2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesEarthly_NZZ2->setAlignment(Qt::AlignCenter);
        horizontalLayout_22 = new QHBoxLayout(gboxProtectionPropertiesEarthly_NZZ2);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setSpacing(6);
        verticalLayout_24->setObjectName(QStringLiteral("verticalLayout_24"));
        lblTextProtectionEarthly_NZZ2_Ctrl = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_Ctrl"));
        lblTextProtectionEarthly_NZZ2_Ctrl->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_Ctrl);

        lblTextProtectionEarthly_NZZ2_StartCurrent = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_StartCurrent"));
        lblTextProtectionEarthly_NZZ2_StartCurrent->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_StartCurrent);

        lblTextProtectionEarthly_NZZ2_StartPower = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_StartPower->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_StartPower"));
        lblTextProtectionEarthly_NZZ2_StartPower->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_StartPower);

        lblTextProtectionEarthly_NZZ2_Angle = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_Angle->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_Angle"));
        lblTextProtectionEarthly_NZZ2_Angle->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_Angle);

        lblTextProtectionEarthly_NZZ2_Pause = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_Pause->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_Pause"));
        lblTextProtectionEarthly_NZZ2_Pause->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_Pause);

        lblTextProtectionEarthly_NZZ2_KvzCurrent = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_KvzCurrent->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_KvzCurrent"));
        lblTextProtectionEarthly_NZZ2_KvzCurrent->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_KvzCurrent);

        lblTextProtectionEarthly_NZZ2_KvzPower = new QLabel(gboxProtectionPropertiesEarthly_NZZ2);
        lblTextProtectionEarthly_NZZ2_KvzPower->setObjectName(QStringLiteral("lblTextProtectionEarthly_NZZ2_KvzPower"));
        lblTextProtectionEarthly_NZZ2_KvzPower->setScaledContents(true);

        verticalLayout_24->addWidget(lblTextProtectionEarthly_NZZ2_KvzPower);


        horizontalLayout_22->addLayout(verticalLayout_24);

        vltProtectionMTZ31_10 = new QVBoxLayout();
        vltProtectionMTZ31_10->setSpacing(6);
        vltProtectionMTZ31_10->setObjectName(QStringLiteral("vltProtectionMTZ31_10"));
        cboxK26 = new QComboBox(gboxProtectionPropertiesEarthly_NZZ2);
        cboxK26->addItem(QString());
        cboxK26->addItem(QString());
        cboxK26->addItem(QString());
        cboxK26->setObjectName(QStringLiteral("cboxK26"));
        cboxK26->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_10->addWidget(cboxK26);

        leK27 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ2);
        leK27->setObjectName(QStringLiteral("leK27"));
        sizePolicy5.setHeightForWidth(leK27->sizePolicy().hasHeightForWidth());
        leK27->setSizePolicy(sizePolicy5);
        leK27->setMaximumSize(QSize(100, 100));
        leK27->setMaxLength(10);
        leK27->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_10->addWidget(leK27);

        leK28 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ2);
        leK28->setObjectName(QStringLiteral("leK28"));
        sizePolicy5.setHeightForWidth(leK28->sizePolicy().hasHeightForWidth());
        leK28->setSizePolicy(sizePolicy5);
        leK28->setMaximumSize(QSize(100, 100));
        leK28->setMaxLength(10);
        leK28->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_10->addWidget(leK28);

        leK29 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ2);
        leK29->setObjectName(QStringLiteral("leK29"));
        sizePolicy5.setHeightForWidth(leK29->sizePolicy().hasHeightForWidth());
        leK29->setSizePolicy(sizePolicy5);
        leK29->setMaximumSize(QSize(100, 100));
        leK29->setMaxLength(4);
        leK29->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_10->addWidget(leK29);

        leK30 = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ2);
        leK30->setObjectName(QStringLiteral("leK30"));
        sizePolicy5.setHeightForWidth(leK30->sizePolicy().hasHeightForWidth());
        leK30->setSizePolicy(sizePolicy5);
        leK30->setMaximumSize(QSize(100, 100));
        leK30->setMaxLength(10);
        leK30->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_10->addWidget(leK30);

        leX08a = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ2);
        leX08a->setObjectName(QStringLiteral("leX08a"));
        sizePolicy5.setHeightForWidth(leX08a->sizePolicy().hasHeightForWidth());
        leX08a->setSizePolicy(sizePolicy5);
        leX08a->setMaximumSize(QSize(100, 100));
        leX08a->setMaxLength(10);
        leX08a->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_10->addWidget(leX08a);

        leX09a = new QLineEdit(gboxProtectionPropertiesEarthly_NZZ2);
        leX09a->setObjectName(QStringLiteral("leX09a"));
        sizePolicy5.setHeightForWidth(leX09a->sizePolicy().hasHeightForWidth());
        leX09a->setSizePolicy(sizePolicy5);
        leX09a->setMaximumSize(QSize(100, 100));
        leX09a->setMaxLength(10);
        leX09a->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_10->addWidget(leX09a);


        horizontalLayout_22->addLayout(vltProtectionMTZ31_10);


        horizontalLayout_18->addWidget(gboxProtectionPropertiesEarthly_NZZ2);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_16);


        verticalLayout_25->addLayout(horizontalLayout_18);

        verticalSpacer_62 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_25->addItem(verticalSpacer_62);

        stwgtProtectionPropertiesEarthly->addWidget(pageProtectionPropertiesNZZ2);

        horizontalLayout_23->addWidget(stwgtProtectionPropertiesEarthly);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_23->addItem(horizontalSpacer_22);


        verticalLayout_90->addLayout(horizontalLayout_23);

        verticalSpacer_8 = new QSpacerItem(20, 62, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_90->addItem(verticalSpacer_8);

        stwgtMain->addWidget(pageProtectionEarthy);
        pageProtectionPower = new QWidget();
        pageProtectionPower->setObjectName(QStringLiteral("pageProtectionPower"));
        verticalLayout_95 = new QVBoxLayout(pageProtectionPower);
        verticalLayout_95->setSpacing(6);
        verticalLayout_95->setContentsMargins(11, 11, 11, 11);
        verticalLayout_95->setObjectName(QStringLiteral("verticalLayout_95"));
        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        verticalLayout_26 = new QVBoxLayout();
        verticalLayout_26->setSpacing(0);
        verticalLayout_26->setObjectName(QStringLiteral("verticalLayout_26"));
        verticalLayout_26->setSizeConstraint(QLayout::SetMinimumSize);
        pbtnProtectionPower_Umax1 = new QPushButton(pageProtectionPower);
        pbtnProtectionPower_Umax1->setObjectName(QStringLiteral("pbtnProtectionPower_Umax1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionPower_Umax1->sizePolicy().hasHeightForWidth());
        pbtnProtectionPower_Umax1->setSizePolicy(sizePolicy5);
        pbtnProtectionPower_Umax1->setCheckable(false);

        verticalLayout_26->addWidget(pbtnProtectionPower_Umax1);

        pbtnProtectionPower_Umax2 = new QPushButton(pageProtectionPower);
        pbtnProtectionPower_Umax2->setObjectName(QStringLiteral("pbtnProtectionPower_Umax2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionPower_Umax2->sizePolicy().hasHeightForWidth());
        pbtnProtectionPower_Umax2->setSizePolicy(sizePolicy5);
        pbtnProtectionPower_Umax2->setCheckable(false);

        verticalLayout_26->addWidget(pbtnProtectionPower_Umax2);

        pbtnProtectionPower_Umin1 = new QPushButton(pageProtectionPower);
        pbtnProtectionPower_Umin1->setObjectName(QStringLiteral("pbtnProtectionPower_Umin1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionPower_Umin1->sizePolicy().hasHeightForWidth());
        pbtnProtectionPower_Umin1->setSizePolicy(sizePolicy5);
        pbtnProtectionPower_Umin1->setCheckable(false);

        verticalLayout_26->addWidget(pbtnProtectionPower_Umin1);

        pbtnProtectionPower_Umin2 = new QPushButton(pageProtectionPower);
        pbtnProtectionPower_Umin2->setObjectName(QStringLiteral("pbtnProtectionPower_Umin2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionPower_Umin2->sizePolicy().hasHeightForWidth());
        pbtnProtectionPower_Umin2->setSizePolicy(sizePolicy5);
        pbtnProtectionPower_Umin2->setCheckable(false);

        verticalLayout_26->addWidget(pbtnProtectionPower_Umin2);

        pbtnProtectionPower_3UO = new QPushButton(pageProtectionPower);
        pbtnProtectionPower_3UO->setObjectName(QStringLiteral("pbtnProtectionPower_3UO"));
        sizePolicy5.setHeightForWidth(pbtnProtectionPower_3UO->sizePolicy().hasHeightForWidth());
        pbtnProtectionPower_3UO->setSizePolicy(sizePolicy5);
        pbtnProtectionPower_3UO->setCheckable(false);

        verticalLayout_26->addWidget(pbtnProtectionPower_3UO);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_26->addItem(verticalSpacer_9);


        horizontalLayout_24->addLayout(verticalLayout_26);

        stwgtProtectionPropertiesPower = new QStackedWidget(pageProtectionPower);
        stwgtProtectionPropertiesPower->setObjectName(QStringLiteral("stwgtProtectionPropertiesPower"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesPower->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesPower->setSizePolicy(sizePolicy);
        pageProtectionPropertiesUmax1 = new QWidget();
        pageProtectionPropertiesUmax1->setObjectName(QStringLiteral("pageProtectionPropertiesUmax1"));
        verticalLayout_32 = new QVBoxLayout(pageProtectionPropertiesUmax1);
        verticalLayout_32->setSpacing(6);
        verticalLayout_32->setContentsMargins(11, 11, 11, 11);
        verticalLayout_32->setObjectName(QStringLiteral("verticalLayout_32"));
        horizontalLayout_82 = new QHBoxLayout();
        horizontalLayout_82->setSpacing(6);
        horizontalLayout_82->setObjectName(QStringLiteral("horizontalLayout_82"));
        gboxProtectionPropertiesPower_Umax1 = new QGroupBox(pageProtectionPropertiesUmax1);
        gboxProtectionPropertiesPower_Umax1->setObjectName(QStringLiteral("gboxProtectionPropertiesPower_Umax1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesPower_Umax1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesPower_Umax1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesPower_Umax1->setAlignment(Qt::AlignCenter);
        horizontalLayout_25 = new QHBoxLayout(gboxProtectionPropertiesPower_Umax1);
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        verticalLayout_27 = new QVBoxLayout();
        verticalLayout_27->setSpacing(6);
        verticalLayout_27->setObjectName(QStringLiteral("verticalLayout_27"));
        lblTextProtectionPower_Umax1_Ctrl = new QLabel(gboxProtectionPropertiesPower_Umax1);
        lblTextProtectionPower_Umax1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionPower_Umax1_Ctrl"));
        lblTextProtectionPower_Umax1_Ctrl->setScaledContents(true);

        verticalLayout_27->addWidget(lblTextProtectionPower_Umax1_Ctrl);

        lblTextProtectionPower_Umax1_StartPower = new QLabel(gboxProtectionPropertiesPower_Umax1);
        lblTextProtectionPower_Umax1_StartPower->setObjectName(QStringLiteral("lblTextProtectionPower_Umax1_StartPower"));
        lblTextProtectionPower_Umax1_StartPower->setScaledContents(true);

        verticalLayout_27->addWidget(lblTextProtectionPower_Umax1_StartPower);

        lblTextProtectionPower_Umax1_Pause = new QLabel(gboxProtectionPropertiesPower_Umax1);
        lblTextProtectionPower_Umax1_Pause->setObjectName(QStringLiteral("lblTextProtectionPower_Umax1_Pause"));
        lblTextProtectionPower_Umax1_Pause->setScaledContents(true);

        verticalLayout_27->addWidget(lblTextProtectionPower_Umax1_Pause);

        lblTextProtectionPower_Umax1_KvzPower = new QLabel(gboxProtectionPropertiesPower_Umax1);
        lblTextProtectionPower_Umax1_KvzPower->setObjectName(QStringLiteral("lblTextProtectionPower_Umax1_KvzPower"));
        lblTextProtectionPower_Umax1_KvzPower->setScaledContents(true);

        verticalLayout_27->addWidget(lblTextProtectionPower_Umax1_KvzPower);


        horizontalLayout_25->addLayout(verticalLayout_27);

        vltProtectionMTZ31_11 = new QVBoxLayout();
        vltProtectionMTZ31_11->setSpacing(6);
        vltProtectionMTZ31_11->setObjectName(QStringLiteral("vltProtectionMTZ31_11"));
        cboxM32 = new QComboBox(gboxProtectionPropertiesPower_Umax1);
        cboxM32->addItem(QString());
        cboxM32->addItem(QString());
        cboxM32->addItem(QString());
        cboxM32->addItem(QString());
        cboxM32->setObjectName(QStringLiteral("cboxM32"));
        cboxM32->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_11->addWidget(cboxM32);

        leM33 = new QLineEdit(gboxProtectionPropertiesPower_Umax1);
        leM33->setObjectName(QStringLiteral("leM33"));
        sizePolicy5.setHeightForWidth(leM33->sizePolicy().hasHeightForWidth());
        leM33->setSizePolicy(sizePolicy5);
        leM33->setMaximumSize(QSize(100, 100));
        leM33->setMaxLength(10);
        leM33->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_11->addWidget(leM33);

        leM34 = new QLineEdit(gboxProtectionPropertiesPower_Umax1);
        leM34->setObjectName(QStringLiteral("leM34"));
        sizePolicy5.setHeightForWidth(leM34->sizePolicy().hasHeightForWidth());
        leM34->setSizePolicy(sizePolicy5);
        leM34->setMaximumSize(QSize(100, 100));
        leM34->setMaxLength(10);
        leM34->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_11->addWidget(leM34);

        leX11 = new QLineEdit(gboxProtectionPropertiesPower_Umax1);
        leX11->setObjectName(QStringLiteral("leX11"));
        sizePolicy5.setHeightForWidth(leX11->sizePolicy().hasHeightForWidth());
        leX11->setSizePolicy(sizePolicy5);
        leX11->setMaximumSize(QSize(100, 100));
        leX11->setMaxLength(10);
        leX11->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_11->addWidget(leX11);


        horizontalLayout_25->addLayout(vltProtectionMTZ31_11);


        horizontalLayout_82->addWidget(gboxProtectionPropertiesPower_Umax1);

        horizontalSpacer_42 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_82->addItem(horizontalSpacer_42);


        verticalLayout_32->addLayout(horizontalLayout_82);

        verticalSpacer_31 = new QSpacerItem(20, 102, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_32->addItem(verticalSpacer_31);

        stwgtProtectionPropertiesPower->addWidget(pageProtectionPropertiesUmax1);
        pageProtectionPropertiesUmax2 = new QWidget();
        pageProtectionPropertiesUmax2->setObjectName(QStringLiteral("pageProtectionPropertiesUmax2"));
        verticalLayout_96 = new QVBoxLayout(pageProtectionPropertiesUmax2);
        verticalLayout_96->setSpacing(6);
        verticalLayout_96->setContentsMargins(11, 11, 11, 11);
        verticalLayout_96->setObjectName(QStringLiteral("verticalLayout_96"));
        horizontalLayout_83 = new QHBoxLayout();
        horizontalLayout_83->setSpacing(6);
        horizontalLayout_83->setObjectName(QStringLiteral("horizontalLayout_83"));
        gboxProtectionPropertiesPower_Umax2 = new QGroupBox(pageProtectionPropertiesUmax2);
        gboxProtectionPropertiesPower_Umax2->setObjectName(QStringLiteral("gboxProtectionPropertiesPower_Umax2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesPower_Umax2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesPower_Umax2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesPower_Umax2->setAlignment(Qt::AlignCenter);
        horizontalLayout_26 = new QHBoxLayout(gboxProtectionPropertiesPower_Umax2);
        horizontalLayout_26->setSpacing(6);
        horizontalLayout_26->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setSpacing(6);
        verticalLayout_28->setObjectName(QStringLiteral("verticalLayout_28"));
        lblTextProtectionPower_Umax2_Ctrl = new QLabel(gboxProtectionPropertiesPower_Umax2);
        lblTextProtectionPower_Umax2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionPower_Umax2_Ctrl"));
        lblTextProtectionPower_Umax2_Ctrl->setScaledContents(true);

        verticalLayout_28->addWidget(lblTextProtectionPower_Umax2_Ctrl);

        lblTextProtectionPower_Umax2_StartPower = new QLabel(gboxProtectionPropertiesPower_Umax2);
        lblTextProtectionPower_Umax2_StartPower->setObjectName(QStringLiteral("lblTextProtectionPower_Umax2_StartPower"));
        lblTextProtectionPower_Umax2_StartPower->setScaledContents(true);

        verticalLayout_28->addWidget(lblTextProtectionPower_Umax2_StartPower);

        lblTextProtectionPower_Umax2_Pause = new QLabel(gboxProtectionPropertiesPower_Umax2);
        lblTextProtectionPower_Umax2_Pause->setObjectName(QStringLiteral("lblTextProtectionPower_Umax2_Pause"));
        lblTextProtectionPower_Umax2_Pause->setScaledContents(true);

        verticalLayout_28->addWidget(lblTextProtectionPower_Umax2_Pause);

        lblTextProtectionPower_Umax2_KvzPower = new QLabel(gboxProtectionPropertiesPower_Umax2);
        lblTextProtectionPower_Umax2_KvzPower->setObjectName(QStringLiteral("lblTextProtectionPower_Umax2_KvzPower"));
        lblTextProtectionPower_Umax2_KvzPower->setScaledContents(true);

        verticalLayout_28->addWidget(lblTextProtectionPower_Umax2_KvzPower);


        horizontalLayout_26->addLayout(verticalLayout_28);

        vltProtectionMTZ31_12 = new QVBoxLayout();
        vltProtectionMTZ31_12->setSpacing(6);
        vltProtectionMTZ31_12->setObjectName(QStringLiteral("vltProtectionMTZ31_12"));
        cboxM35 = new QComboBox(gboxProtectionPropertiesPower_Umax2);
        cboxM35->addItem(QString());
        cboxM35->addItem(QString());
        cboxM35->addItem(QString());
        cboxM35->addItem(QString());
        cboxM35->setObjectName(QStringLiteral("cboxM35"));
        cboxM35->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_12->addWidget(cboxM35);

        leM36 = new QLineEdit(gboxProtectionPropertiesPower_Umax2);
        leM36->setObjectName(QStringLiteral("leM36"));
        sizePolicy5.setHeightForWidth(leM36->sizePolicy().hasHeightForWidth());
        leM36->setSizePolicy(sizePolicy5);
        leM36->setMaximumSize(QSize(100, 100));
        leM36->setMaxLength(10);
        leM36->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_12->addWidget(leM36);

        leM37 = new QLineEdit(gboxProtectionPropertiesPower_Umax2);
        leM37->setObjectName(QStringLiteral("leM37"));
        sizePolicy5.setHeightForWidth(leM37->sizePolicy().hasHeightForWidth());
        leM37->setSizePolicy(sizePolicy5);
        leM37->setMaximumSize(QSize(100, 100));
        leM37->setMaxLength(10);
        leM37->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_12->addWidget(leM37);

        leX12 = new QLineEdit(gboxProtectionPropertiesPower_Umax2);
        leX12->setObjectName(QStringLiteral("leX12"));
        sizePolicy5.setHeightForWidth(leX12->sizePolicy().hasHeightForWidth());
        leX12->setSizePolicy(sizePolicy5);
        leX12->setMaximumSize(QSize(100, 100));
        leX12->setMaxLength(10);
        leX12->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_12->addWidget(leX12);


        horizontalLayout_26->addLayout(vltProtectionMTZ31_12);


        horizontalLayout_83->addWidget(gboxProtectionPropertiesPower_Umax2);

        horizontalSpacer_43 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_83->addItem(horizontalSpacer_43);


        verticalLayout_96->addLayout(horizontalLayout_83);

        verticalSpacer_32 = new QSpacerItem(20, 102, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_96->addItem(verticalSpacer_32);

        stwgtProtectionPropertiesPower->addWidget(pageProtectionPropertiesUmax2);
        pageProtectionPropertiesUmin1 = new QWidget();
        pageProtectionPropertiesUmin1->setObjectName(QStringLiteral("pageProtectionPropertiesUmin1"));
        verticalLayout_97 = new QVBoxLayout(pageProtectionPropertiesUmin1);
        verticalLayout_97->setSpacing(6);
        verticalLayout_97->setContentsMargins(11, 11, 11, 11);
        verticalLayout_97->setObjectName(QStringLiteral("verticalLayout_97"));
        horizontalLayout_84 = new QHBoxLayout();
        horizontalLayout_84->setSpacing(6);
        horizontalLayout_84->setObjectName(QStringLiteral("horizontalLayout_84"));
        gboxProtectionPropertiesPower_Umin1 = new QGroupBox(pageProtectionPropertiesUmin1);
        gboxProtectionPropertiesPower_Umin1->setObjectName(QStringLiteral("gboxProtectionPropertiesPower_Umin1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesPower_Umin1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesPower_Umin1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesPower_Umin1->setAlignment(Qt::AlignCenter);
        horizontalLayout_27 = new QHBoxLayout(gboxProtectionPropertiesPower_Umin1);
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        verticalLayout_29 = new QVBoxLayout();
        verticalLayout_29->setSpacing(6);
        verticalLayout_29->setObjectName(QStringLiteral("verticalLayout_29"));
        lblTextProtectionPower_Umin1_Ctrl = new QLabel(gboxProtectionPropertiesPower_Umin1);
        lblTextProtectionPower_Umin1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionPower_Umin1_Ctrl"));
        lblTextProtectionPower_Umin1_Ctrl->setScaledContents(true);

        verticalLayout_29->addWidget(lblTextProtectionPower_Umin1_Ctrl);

        lblTextProtectionPower_Umin1_Logic = new QLabel(gboxProtectionPropertiesPower_Umin1);
        lblTextProtectionPower_Umin1_Logic->setObjectName(QStringLiteral("lblTextProtectionPower_Umin1_Logic"));
        lblTextProtectionPower_Umin1_Logic->setScaledContents(true);

        verticalLayout_29->addWidget(lblTextProtectionPower_Umin1_Logic);

        lblTextProtectionPower_Umin1_StartCtrl = new QLabel(gboxProtectionPropertiesPower_Umin1);
        lblTextProtectionPower_Umin1_StartCtrl->setObjectName(QStringLiteral("lblTextProtectionPower_Umin1_StartCtrl"));
        lblTextProtectionPower_Umin1_StartCtrl->setScaledContents(true);

        verticalLayout_29->addWidget(lblTextProtectionPower_Umin1_StartCtrl);

        lblTextProtectionPower_Umin1_StartPower = new QLabel(gboxProtectionPropertiesPower_Umin1);
        lblTextProtectionPower_Umin1_StartPower->setObjectName(QStringLiteral("lblTextProtectionPower_Umin1_StartPower"));
        lblTextProtectionPower_Umin1_StartPower->setScaledContents(true);

        verticalLayout_29->addWidget(lblTextProtectionPower_Umin1_StartPower);

        lblTextProtectionPower_Umin1_StartPower_2 = new QLabel(gboxProtectionPropertiesPower_Umin1);
        lblTextProtectionPower_Umin1_StartPower_2->setObjectName(QStringLiteral("lblTextProtectionPower_Umin1_StartPower_2"));
        lblTextProtectionPower_Umin1_StartPower_2->setScaledContents(true);

        verticalLayout_29->addWidget(lblTextProtectionPower_Umin1_StartPower_2);

        lblTextProtectionPower_Umin1_StartPower_3 = new QLabel(gboxProtectionPropertiesPower_Umin1);
        lblTextProtectionPower_Umin1_StartPower_3->setObjectName(QStringLiteral("lblTextProtectionPower_Umin1_StartPower_3"));
        lblTextProtectionPower_Umin1_StartPower_3->setScaledContents(true);

        verticalLayout_29->addWidget(lblTextProtectionPower_Umin1_StartPower_3);


        horizontalLayout_27->addLayout(verticalLayout_29);

        vltProtectionMTZ31_13 = new QVBoxLayout();
        vltProtectionMTZ31_13->setSpacing(6);
        vltProtectionMTZ31_13->setObjectName(QStringLiteral("vltProtectionMTZ31_13"));
        cboxM38 = new QComboBox(gboxProtectionPropertiesPower_Umin1);
        cboxM38->addItem(QString());
        cboxM38->addItem(QString());
        cboxM38->addItem(QString());
        cboxM38->addItem(QString());
        cboxM38->setObjectName(QStringLiteral("cboxM38"));
        cboxM38->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_13->addWidget(cboxM38);

        cboxM39 = new QComboBox(gboxProtectionPropertiesPower_Umin1);
        cboxM39->addItem(QString());
        cboxM39->addItem(QString());
        cboxM39->addItem(QString());
        cboxM39->setObjectName(QStringLiteral("cboxM39"));
        cboxM39->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_13->addWidget(cboxM39);

        cboxM40 = new QComboBox(gboxProtectionPropertiesPower_Umin1);
        cboxM40->addItem(QString());
        cboxM40->addItem(QString());
        cboxM40->addItem(QString());
        cboxM40->setObjectName(QStringLiteral("cboxM40"));
        cboxM40->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_13->addWidget(cboxM40);

        leM41 = new QLineEdit(gboxProtectionPropertiesPower_Umin1);
        leM41->setObjectName(QStringLiteral("leM41"));
        sizePolicy5.setHeightForWidth(leM41->sizePolicy().hasHeightForWidth());
        leM41->setSizePolicy(sizePolicy5);
        leM41->setMaximumSize(QSize(100, 100));
        leM41->setMaxLength(10);
        leM41->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_13->addWidget(leM41);

        leM42 = new QLineEdit(gboxProtectionPropertiesPower_Umin1);
        leM42->setObjectName(QStringLiteral("leM42"));
        sizePolicy5.setHeightForWidth(leM42->sizePolicy().hasHeightForWidth());
        leM42->setSizePolicy(sizePolicy5);
        leM42->setMaximumSize(QSize(100, 100));
        leM42->setMaxLength(10);
        leM42->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_13->addWidget(leM42);

        leX13 = new QLineEdit(gboxProtectionPropertiesPower_Umin1);
        leX13->setObjectName(QStringLiteral("leX13"));
        sizePolicy5.setHeightForWidth(leX13->sizePolicy().hasHeightForWidth());
        leX13->setSizePolicy(sizePolicy5);
        leX13->setMaximumSize(QSize(100, 100));
        leX13->setMaxLength(10);
        leX13->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_13->addWidget(leX13);


        horizontalLayout_27->addLayout(vltProtectionMTZ31_13);


        horizontalLayout_84->addWidget(gboxProtectionPropertiesPower_Umin1);

        horizontalSpacer_44 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_84->addItem(horizontalSpacer_44);


        verticalLayout_97->addLayout(horizontalLayout_84);

        verticalSpacer_33 = new QSpacerItem(20, 102, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_97->addItem(verticalSpacer_33);

        stwgtProtectionPropertiesPower->addWidget(pageProtectionPropertiesUmin1);
        pageProtectionPropertiesUmin2 = new QWidget();
        pageProtectionPropertiesUmin2->setObjectName(QStringLiteral("pageProtectionPropertiesUmin2"));
        verticalLayout_98 = new QVBoxLayout(pageProtectionPropertiesUmin2);
        verticalLayout_98->setSpacing(6);
        verticalLayout_98->setContentsMargins(11, 11, 11, 11);
        verticalLayout_98->setObjectName(QStringLiteral("verticalLayout_98"));
        horizontalLayout_85 = new QHBoxLayout();
        horizontalLayout_85->setSpacing(6);
        horizontalLayout_85->setObjectName(QStringLiteral("horizontalLayout_85"));
        gboxProtectionPropertiesPower_Umin2 = new QGroupBox(pageProtectionPropertiesUmin2);
        gboxProtectionPropertiesPower_Umin2->setObjectName(QStringLiteral("gboxProtectionPropertiesPower_Umin2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesPower_Umin2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesPower_Umin2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesPower_Umin2->setAlignment(Qt::AlignCenter);
        horizontalLayout_28 = new QHBoxLayout(gboxProtectionPropertiesPower_Umin2);
        horizontalLayout_28->setSpacing(6);
        horizontalLayout_28->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_28->setObjectName(QStringLiteral("horizontalLayout_28"));
        verticalLayout_30 = new QVBoxLayout();
        verticalLayout_30->setSpacing(6);
        verticalLayout_30->setObjectName(QStringLiteral("verticalLayout_30"));
        lblTextProtectionPower_Umin2_Ctrl = new QLabel(gboxProtectionPropertiesPower_Umin2);
        lblTextProtectionPower_Umin2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionPower_Umin2_Ctrl"));
        lblTextProtectionPower_Umin2_Ctrl->setScaledContents(true);

        verticalLayout_30->addWidget(lblTextProtectionPower_Umin2_Ctrl);

        lblTextProtectionPower_Umin2_Logic = new QLabel(gboxProtectionPropertiesPower_Umin2);
        lblTextProtectionPower_Umin2_Logic->setObjectName(QStringLiteral("lblTextProtectionPower_Umin2_Logic"));
        lblTextProtectionPower_Umin2_Logic->setScaledContents(true);

        verticalLayout_30->addWidget(lblTextProtectionPower_Umin2_Logic);

        lblTextProtectionPower_Umin2_StartCtrl = new QLabel(gboxProtectionPropertiesPower_Umin2);
        lblTextProtectionPower_Umin2_StartCtrl->setObjectName(QStringLiteral("lblTextProtectionPower_Umin2_StartCtrl"));
        lblTextProtectionPower_Umin2_StartCtrl->setScaledContents(true);

        verticalLayout_30->addWidget(lblTextProtectionPower_Umin2_StartCtrl);

        lblTextProtectionPower_Umin2_StartPower = new QLabel(gboxProtectionPropertiesPower_Umin2);
        lblTextProtectionPower_Umin2_StartPower->setObjectName(QStringLiteral("lblTextProtectionPower_Umin2_StartPower"));
        lblTextProtectionPower_Umin2_StartPower->setScaledContents(true);

        verticalLayout_30->addWidget(lblTextProtectionPower_Umin2_StartPower);

        lblTextProtectionPower_Umin2_StartPower_2 = new QLabel(gboxProtectionPropertiesPower_Umin2);
        lblTextProtectionPower_Umin2_StartPower_2->setObjectName(QStringLiteral("lblTextProtectionPower_Umin2_StartPower_2"));
        lblTextProtectionPower_Umin2_StartPower_2->setScaledContents(true);

        verticalLayout_30->addWidget(lblTextProtectionPower_Umin2_StartPower_2);

        lblTextProtectionPower_Umin2_StartPower_3 = new QLabel(gboxProtectionPropertiesPower_Umin2);
        lblTextProtectionPower_Umin2_StartPower_3->setObjectName(QStringLiteral("lblTextProtectionPower_Umin2_StartPower_3"));
        lblTextProtectionPower_Umin2_StartPower_3->setScaledContents(true);

        verticalLayout_30->addWidget(lblTextProtectionPower_Umin2_StartPower_3);


        horizontalLayout_28->addLayout(verticalLayout_30);

        vltProtectionMTZ31_14 = new QVBoxLayout();
        vltProtectionMTZ31_14->setSpacing(6);
        vltProtectionMTZ31_14->setObjectName(QStringLiteral("vltProtectionMTZ31_14"));
        cboxM43 = new QComboBox(gboxProtectionPropertiesPower_Umin2);
        cboxM43->addItem(QString());
        cboxM43->addItem(QString());
        cboxM43->addItem(QString());
        cboxM43->addItem(QString());
        cboxM43->setObjectName(QStringLiteral("cboxM43"));
        cboxM43->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_14->addWidget(cboxM43);

        cboxM44 = new QComboBox(gboxProtectionPropertiesPower_Umin2);
        cboxM44->addItem(QString());
        cboxM44->addItem(QString());
        cboxM44->addItem(QString());
        cboxM44->setObjectName(QStringLiteral("cboxM44"));
        cboxM44->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_14->addWidget(cboxM44);

        cboxM45 = new QComboBox(gboxProtectionPropertiesPower_Umin2);
        cboxM45->addItem(QString());
        cboxM45->addItem(QString());
        cboxM45->addItem(QString());
        cboxM45->setObjectName(QStringLiteral("cboxM45"));
        cboxM45->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_14->addWidget(cboxM45);

        leM46 = new QLineEdit(gboxProtectionPropertiesPower_Umin2);
        leM46->setObjectName(QStringLiteral("leM46"));
        sizePolicy5.setHeightForWidth(leM46->sizePolicy().hasHeightForWidth());
        leM46->setSizePolicy(sizePolicy5);
        leM46->setMaximumSize(QSize(100, 100));
        leM46->setMaxLength(10);
        leM46->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_14->addWidget(leM46);

        leM47 = new QLineEdit(gboxProtectionPropertiesPower_Umin2);
        leM47->setObjectName(QStringLiteral("leM47"));
        sizePolicy5.setHeightForWidth(leM47->sizePolicy().hasHeightForWidth());
        leM47->setSizePolicy(sizePolicy5);
        leM47->setMaximumSize(QSize(100, 100));
        leM47->setMaxLength(10);
        leM47->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_14->addWidget(leM47);

        leX14 = new QLineEdit(gboxProtectionPropertiesPower_Umin2);
        leX14->setObjectName(QStringLiteral("leX14"));
        sizePolicy5.setHeightForWidth(leX14->sizePolicy().hasHeightForWidth());
        leX14->setSizePolicy(sizePolicy5);
        leX14->setMaximumSize(QSize(100, 100));
        leX14->setMaxLength(10);
        leX14->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_14->addWidget(leX14);


        horizontalLayout_28->addLayout(vltProtectionMTZ31_14);


        horizontalLayout_85->addWidget(gboxProtectionPropertiesPower_Umin2);

        horizontalSpacer_45 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_85->addItem(horizontalSpacer_45);


        verticalLayout_98->addLayout(horizontalLayout_85);

        verticalSpacer_34 = new QSpacerItem(20, 102, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_98->addItem(verticalSpacer_34);

        stwgtProtectionPropertiesPower->addWidget(pageProtectionPropertiesUmin2);
        pageProtectionProperties3UO = new QWidget();
        pageProtectionProperties3UO->setObjectName(QStringLiteral("pageProtectionProperties3UO"));
        verticalLayout_99 = new QVBoxLayout(pageProtectionProperties3UO);
        verticalLayout_99->setSpacing(6);
        verticalLayout_99->setContentsMargins(11, 11, 11, 11);
        verticalLayout_99->setObjectName(QStringLiteral("verticalLayout_99"));
        horizontalLayout_86 = new QHBoxLayout();
        horizontalLayout_86->setSpacing(6);
        horizontalLayout_86->setObjectName(QStringLiteral("horizontalLayout_86"));
        gboxProtectionPropertiesPower_3UO = new QGroupBox(pageProtectionProperties3UO);
        gboxProtectionPropertiesPower_3UO->setObjectName(QStringLiteral("gboxProtectionPropertiesPower_3UO"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesPower_3UO->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesPower_3UO->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesPower_3UO->setAlignment(Qt::AlignCenter);
        horizontalLayout_29 = new QHBoxLayout(gboxProtectionPropertiesPower_3UO);
        horizontalLayout_29->setSpacing(6);
        horizontalLayout_29->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_29->setObjectName(QStringLiteral("horizontalLayout_29"));
        verticalLayout_31 = new QVBoxLayout();
        verticalLayout_31->setSpacing(6);
        verticalLayout_31->setObjectName(QStringLiteral("verticalLayout_31"));
        lblTextProtectionPower_3UO_Ctrl = new QLabel(gboxProtectionPropertiesPower_3UO);
        lblTextProtectionPower_3UO_Ctrl->setObjectName(QStringLiteral("lblTextProtectionPower_3UO_Ctrl"));
        lblTextProtectionPower_3UO_Ctrl->setScaledContents(true);

        verticalLayout_31->addWidget(lblTextProtectionPower_3UO_Ctrl);

        lblTextProtectionPower_3UO_StartCurrent = new QLabel(gboxProtectionPropertiesPower_3UO);
        lblTextProtectionPower_3UO_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionPower_3UO_StartCurrent"));
        lblTextProtectionPower_3UO_StartCurrent->setScaledContents(true);

        verticalLayout_31->addWidget(lblTextProtectionPower_3UO_StartCurrent);

        lblTextProtectionPower_3UO_Pause = new QLabel(gboxProtectionPropertiesPower_3UO);
        lblTextProtectionPower_3UO_Pause->setObjectName(QStringLiteral("lblTextProtectionPower_3UO_Pause"));
        lblTextProtectionPower_3UO_Pause->setScaledContents(true);

        verticalLayout_31->addWidget(lblTextProtectionPower_3UO_Pause);

        lblTextProtectionPower_3UO_KvzPower = new QLabel(gboxProtectionPropertiesPower_3UO);
        lblTextProtectionPower_3UO_KvzPower->setObjectName(QStringLiteral("lblTextProtectionPower_3UO_KvzPower"));
        lblTextProtectionPower_3UO_KvzPower->setScaledContents(true);

        verticalLayout_31->addWidget(lblTextProtectionPower_3UO_KvzPower);


        horizontalLayout_29->addLayout(verticalLayout_31);

        vltProtectionMTZ31_15 = new QVBoxLayout();
        vltProtectionMTZ31_15->setSpacing(6);
        vltProtectionMTZ31_15->setObjectName(QStringLiteral("vltProtectionMTZ31_15"));
        cboxM48 = new QComboBox(gboxProtectionPropertiesPower_3UO);
        cboxM48->addItem(QString());
        cboxM48->addItem(QString());
        cboxM48->addItem(QString());
        cboxM48->setObjectName(QStringLiteral("cboxM48"));
        cboxM48->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_15->addWidget(cboxM48);

        leM49 = new QLineEdit(gboxProtectionPropertiesPower_3UO);
        leM49->setObjectName(QStringLiteral("leM49"));
        sizePolicy5.setHeightForWidth(leM49->sizePolicy().hasHeightForWidth());
        leM49->setSizePolicy(sizePolicy5);
        leM49->setMaximumSize(QSize(100, 100));
        leM49->setMaxLength(10);
        leM49->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_15->addWidget(leM49);

        leM50 = new QLineEdit(gboxProtectionPropertiesPower_3UO);
        leM50->setObjectName(QStringLiteral("leM50"));
        sizePolicy5.setHeightForWidth(leM50->sizePolicy().hasHeightForWidth());
        leM50->setSizePolicy(sizePolicy5);
        leM50->setMaximumSize(QSize(100, 100));
        leM50->setMaxLength(10);
        leM50->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_15->addWidget(leM50);

        leX15 = new QLineEdit(gboxProtectionPropertiesPower_3UO);
        leX15->setObjectName(QStringLiteral("leX15"));
        sizePolicy5.setHeightForWidth(leX15->sizePolicy().hasHeightForWidth());
        leX15->setSizePolicy(sizePolicy5);
        leX15->setMaximumSize(QSize(100, 100));
        leX15->setMaxLength(10);
        leX15->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_15->addWidget(leX15);


        horizontalLayout_29->addLayout(vltProtectionMTZ31_15);


        horizontalLayout_86->addWidget(gboxProtectionPropertiesPower_3UO);

        horizontalSpacer_46 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_86->addItem(horizontalSpacer_46);


        verticalLayout_99->addLayout(horizontalLayout_86);

        verticalSpacer_35 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_99->addItem(verticalSpacer_35);

        stwgtProtectionPropertiesPower->addWidget(pageProtectionProperties3UO);

        horizontalLayout_24->addWidget(stwgtProtectionPropertiesPower);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_24->addItem(horizontalSpacer_17);


        verticalLayout_95->addLayout(horizontalLayout_24);

        verticalSpacer_63 = new QSpacerItem(20, 78, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_95->addItem(verticalSpacer_63);

        stwgtMain->addWidget(pageProtectionPower);
        pageProtectionMotor = new QWidget();
        pageProtectionMotor->setObjectName(QStringLiteral("pageProtectionMotor"));
        verticalLayout_100 = new QVBoxLayout(pageProtectionMotor);
        verticalLayout_100->setSpacing(6);
        verticalLayout_100->setContentsMargins(11, 11, 11, 11);
        verticalLayout_100->setObjectName(QStringLiteral("verticalLayout_100"));
        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setSpacing(6);
        horizontalLayout_30->setObjectName(QStringLiteral("horizontalLayout_30"));
        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(2);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        pbtnProtectionMotorStarting = new QPushButton(pageProtectionMotor);
        pbtnProtectionMotorStarting->setObjectName(QStringLiteral("pbtnProtectionMotorStarting"));
        sizePolicy5.setHeightForWidth(pbtnProtectionMotorStarting->sizePolicy().hasHeightForWidth());
        pbtnProtectionMotorStarting->setSizePolicy(sizePolicy5);
        pbtnProtectionMotorStarting->setCheckable(false);

        verticalLayout_18->addWidget(pbtnProtectionMotorStarting);

        pbtnProtectionMotorImin = new QPushButton(pageProtectionMotor);
        pbtnProtectionMotorImin->setObjectName(QStringLiteral("pbtnProtectionMotorImin"));
        sizePolicy5.setHeightForWidth(pbtnProtectionMotorImin->sizePolicy().hasHeightForWidth());
        pbtnProtectionMotorImin->setSizePolicy(sizePolicy5);
        pbtnProtectionMotorImin->setCheckable(false);

        verticalLayout_18->addWidget(pbtnProtectionMotorImin);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_18->addItem(verticalSpacer_6);


        horizontalLayout_30->addLayout(verticalLayout_18);

        stwgtProtectionPropertiesMotor = new QStackedWidget(pageProtectionMotor);
        stwgtProtectionPropertiesMotor->setObjectName(QStringLiteral("stwgtProtectionPropertiesMotor"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesMotor->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesMotor->setSizePolicy(sizePolicy);
        pagePotectionPropertiesStarting = new QWidget();
        pagePotectionPropertiesStarting->setObjectName(QStringLiteral("pagePotectionPropertiesStarting"));
        verticalLayout_101 = new QVBoxLayout(pagePotectionPropertiesStarting);
        verticalLayout_101->setSpacing(6);
        verticalLayout_101->setContentsMargins(11, 11, 11, 11);
        verticalLayout_101->setObjectName(QStringLiteral("verticalLayout_101"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        gboxProtectionPropertiesMotor_StartingCurrent = new QGroupBox(pagePotectionPropertiesStarting);
        gboxProtectionPropertiesMotor_StartingCurrent->setObjectName(QStringLiteral("gboxProtectionPropertiesMotor_StartingCurrent"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesMotor_StartingCurrent->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesMotor_StartingCurrent->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesMotor_StartingCurrent->setAlignment(Qt::AlignCenter);
        horizontalLayout_13 = new QHBoxLayout(gboxProtectionPropertiesMotor_StartingCurrent);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(-1, 9, -1, 9);
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        lblTextProtectionMotor_Ctrl = new QLabel(gboxProtectionPropertiesMotor_StartingCurrent);
        lblTextProtectionMotor_Ctrl->setObjectName(QStringLiteral("lblTextProtectionMotor_Ctrl"));
        lblTextProtectionMotor_Ctrl->setScaledContents(true);

        verticalLayout_16->addWidget(lblTextProtectionMotor_Ctrl);

        lblTextProtectionMotor_StartCurrent = new QLabel(gboxProtectionPropertiesMotor_StartingCurrent);
        lblTextProtectionMotor_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionMotor_StartCurrent"));
        lblTextProtectionMotor_StartCurrent->setScaledContents(true);

        verticalLayout_16->addWidget(lblTextProtectionMotor_StartCurrent);

        lblTextProtectionMotor_Pause = new QLabel(gboxProtectionPropertiesMotor_StartingCurrent);
        lblTextProtectionMotor_Pause->setObjectName(QStringLiteral("lblTextProtectionMotor_Pause"));
        lblTextProtectionMotor_Pause->setScaledContents(true);

        verticalLayout_16->addWidget(lblTextProtectionMotor_Pause);

        lblTextProtectionMotor_Kvz_Current = new QLabel(gboxProtectionPropertiesMotor_StartingCurrent);
        lblTextProtectionMotor_Kvz_Current->setObjectName(QStringLiteral("lblTextProtectionMotor_Kvz_Current"));
        lblTextProtectionMotor_Kvz_Current->setScaledContents(true);

        verticalLayout_16->addWidget(lblTextProtectionMotor_Kvz_Current);


        horizontalLayout_13->addLayout(verticalLayout_16);

        vltProtectionMTZ31_5 = new QVBoxLayout();
        vltProtectionMTZ31_5->setSpacing(6);
        vltProtectionMTZ31_5->setObjectName(QStringLiteral("vltProtectionMTZ31_5"));
        cboxM19 = new QComboBox(gboxProtectionPropertiesMotor_StartingCurrent);
        cboxM19->addItem(QString());
        cboxM19->addItem(QString());
        cboxM19->addItem(QString());
        cboxM19->setObjectName(QStringLiteral("cboxM19"));
        cboxM19->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_5->addWidget(cboxM19);

        leM20 = new QLineEdit(gboxProtectionPropertiesMotor_StartingCurrent);
        leM20->setObjectName(QStringLiteral("leM20"));
        sizePolicy5.setHeightForWidth(leM20->sizePolicy().hasHeightForWidth());
        leM20->setSizePolicy(sizePolicy5);
        leM20->setMaximumSize(QSize(100, 100));
        leM20->setMaxLength(10);
        leM20->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_5->addWidget(leM20);

        leM21 = new QLineEdit(gboxProtectionPropertiesMotor_StartingCurrent);
        leM21->setObjectName(QStringLiteral("leM21"));
        sizePolicy5.setHeightForWidth(leM21->sizePolicy().hasHeightForWidth());
        leM21->setSizePolicy(sizePolicy5);
        leM21->setMaximumSize(QSize(100, 100));
        leM21->setMaxLength(10);
        leM21->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_5->addWidget(leM21);

        leX06 = new QLineEdit(gboxProtectionPropertiesMotor_StartingCurrent);
        leX06->setObjectName(QStringLiteral("leX06"));
        sizePolicy5.setHeightForWidth(leX06->sizePolicy().hasHeightForWidth());
        leX06->setSizePolicy(sizePolicy5);
        leX06->setMaximumSize(QSize(100, 100));
        leX06->setMaxLength(10);
        leX06->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_5->addWidget(leX06);


        horizontalLayout_13->addLayout(vltProtectionMTZ31_5);


        horizontalLayout_16->addWidget(gboxProtectionPropertiesMotor_StartingCurrent);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_15);


        verticalLayout_101->addLayout(horizontalLayout_16);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_101->addItem(verticalSpacer_7);

        stwgtProtectionPropertiesMotor->addWidget(pagePotectionPropertiesStarting);
        pageProtectionPropertiesImin = new QWidget();
        pageProtectionPropertiesImin->setObjectName(QStringLiteral("pageProtectionPropertiesImin"));
        verticalLayout_19 = new QVBoxLayout(pageProtectionPropertiesImin);
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setContentsMargins(11, 11, 11, 11);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        gboxProtectionPropertiesMotor_Imin = new QGroupBox(pageProtectionPropertiesImin);
        gboxProtectionPropertiesMotor_Imin->setObjectName(QStringLiteral("gboxProtectionPropertiesMotor_Imin"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesMotor_Imin->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesMotor_Imin->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesMotor_Imin->setAlignment(Qt::AlignCenter);
        horizontalLayout_14 = new QHBoxLayout(gboxProtectionPropertiesMotor_Imin);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(-1, 9, -1, 9);
        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        lblTextProtectionMotor_Imin_Ctrl = new QLabel(gboxProtectionPropertiesMotor_Imin);
        lblTextProtectionMotor_Imin_Ctrl->setObjectName(QStringLiteral("lblTextProtectionMotor_Imin_Ctrl"));
        lblTextProtectionMotor_Imin_Ctrl->setScaledContents(true);

        verticalLayout_17->addWidget(lblTextProtectionMotor_Imin_Ctrl);

        lblTextProtectionMotor_StartPower = new QLabel(gboxProtectionPropertiesMotor_Imin);
        lblTextProtectionMotor_StartPower->setObjectName(QStringLiteral("lblTextProtectionMotor_StartPower"));
        lblTextProtectionMotor_StartPower->setScaledContents(true);

        verticalLayout_17->addWidget(lblTextProtectionMotor_StartPower);

        lblTextProtectionMotor_Pause_2 = new QLabel(gboxProtectionPropertiesMotor_Imin);
        lblTextProtectionMotor_Pause_2->setObjectName(QStringLiteral("lblTextProtectionMotor_Pause_2"));
        lblTextProtectionMotor_Pause_2->setScaledContents(true);

        verticalLayout_17->addWidget(lblTextProtectionMotor_Pause_2);

        lblTextProtectionMotor_Kvz_Power = new QLabel(gboxProtectionPropertiesMotor_Imin);
        lblTextProtectionMotor_Kvz_Power->setObjectName(QStringLiteral("lblTextProtectionMotor_Kvz_Power"));
        lblTextProtectionMotor_Kvz_Power->setScaledContents(true);

        verticalLayout_17->addWidget(lblTextProtectionMotor_Kvz_Power);


        horizontalLayout_14->addLayout(verticalLayout_17);

        vltProtectionMTZ31_6 = new QVBoxLayout();
        vltProtectionMTZ31_6->setSpacing(6);
        vltProtectionMTZ31_6->setObjectName(QStringLiteral("vltProtectionMTZ31_6"));
        cboxM29 = new QComboBox(gboxProtectionPropertiesMotor_Imin);
        cboxM29->addItem(QString());
        cboxM29->addItem(QString());
        cboxM29->addItem(QString());
        cboxM29->setObjectName(QStringLiteral("cboxM29"));
        cboxM29->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_6->addWidget(cboxM29);

        leM30 = new QLineEdit(gboxProtectionPropertiesMotor_Imin);
        leM30->setObjectName(QStringLiteral("leM30"));
        sizePolicy5.setHeightForWidth(leM30->sizePolicy().hasHeightForWidth());
        leM30->setSizePolicy(sizePolicy5);
        leM30->setMaximumSize(QSize(100, 100));
        leM30->setMaxLength(10);
        leM30->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_6->addWidget(leM30);

        leM31 = new QLineEdit(gboxProtectionPropertiesMotor_Imin);
        leM31->setObjectName(QStringLiteral("leM31"));
        sizePolicy5.setHeightForWidth(leM31->sizePolicy().hasHeightForWidth());
        leM31->setSizePolicy(sizePolicy5);
        leM31->setMaximumSize(QSize(100, 100));
        leM31->setMaxLength(10);
        leM31->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_6->addWidget(leM31);

        leX10 = new QLineEdit(gboxProtectionPropertiesMotor_Imin);
        leX10->setObjectName(QStringLiteral("leX10"));
        sizePolicy5.setHeightForWidth(leX10->sizePolicy().hasHeightForWidth());
        leX10->setSizePolicy(sizePolicy5);
        leX10->setMaximumSize(QSize(100, 100));
        leX10->setMaxLength(10);
        leX10->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_6->addWidget(leX10);


        horizontalLayout_14->addLayout(vltProtectionMTZ31_6);


        horizontalLayout_15->addWidget(gboxProtectionPropertiesMotor_Imin);

        horizontalSpacer_47 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_47);


        verticalLayout_19->addLayout(horizontalLayout_15);

        verticalSpacer_36 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_19->addItem(verticalSpacer_36);

        stwgtProtectionPropertiesMotor->addWidget(pageProtectionPropertiesImin);

        horizontalLayout_30->addWidget(stwgtProtectionPropertiesMotor);

        horizontalSpacer_25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_30->addItem(horizontalSpacer_25);


        verticalLayout_100->addLayout(horizontalLayout_30);

        verticalSpacer_64 = new QSpacerItem(20, 140, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_100->addItem(verticalSpacer_64);

        stwgtMain->addWidget(pageProtectionMotor);
        pageProtectionFrequency = new QWidget();
        pageProtectionFrequency->setObjectName(QStringLiteral("pageProtectionFrequency"));
        verticalLayout_121 = new QVBoxLayout(pageProtectionFrequency);
        verticalLayout_121->setSpacing(6);
        verticalLayout_121->setContentsMargins(11, 11, 11, 11);
        verticalLayout_121->setObjectName(QStringLiteral("verticalLayout_121"));
        horizontalLayout_88 = new QHBoxLayout();
        horizontalLayout_88->setSpacing(6);
        horizontalLayout_88->setObjectName(QStringLiteral("horizontalLayout_88"));
        verticalLayout_33 = new QVBoxLayout();
        verticalLayout_33->setSpacing(0);
        verticalLayout_33->setObjectName(QStringLiteral("verticalLayout_33"));
        pbtnProtectionFrequency_ACR1 = new QPushButton(pageProtectionFrequency);
        pbtnProtectionFrequency_ACR1->setObjectName(QStringLiteral("pbtnProtectionFrequency_ACR1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionFrequency_ACR1->sizePolicy().hasHeightForWidth());
        pbtnProtectionFrequency_ACR1->setSizePolicy(sizePolicy5);
        pbtnProtectionFrequency_ACR1->setCheckable(false);

        verticalLayout_33->addWidget(pbtnProtectionFrequency_ACR1);

        pbtnProtectionFrequency_ACR2 = new QPushButton(pageProtectionFrequency);
        pbtnProtectionFrequency_ACR2->setObjectName(QStringLiteral("pbtnProtectionFrequency_ACR2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionFrequency_ACR2->sizePolicy().hasHeightForWidth());
        pbtnProtectionFrequency_ACR2->setSizePolicy(sizePolicy5);
        pbtnProtectionFrequency_ACR2->setCheckable(false);

        verticalLayout_33->addWidget(pbtnProtectionFrequency_ACR2);

        pbtnProtectionFrequency_ACR3 = new QPushButton(pageProtectionFrequency);
        pbtnProtectionFrequency_ACR3->setObjectName(QStringLiteral("pbtnProtectionFrequency_ACR3"));
        sizePolicy5.setHeightForWidth(pbtnProtectionFrequency_ACR3->sizePolicy().hasHeightForWidth());
        pbtnProtectionFrequency_ACR3->setSizePolicy(sizePolicy5);
        pbtnProtectionFrequency_ACR3->setCheckable(false);

        verticalLayout_33->addWidget(pbtnProtectionFrequency_ACR3);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_33->addItem(verticalSpacer_10);


        horizontalLayout_88->addLayout(verticalLayout_33);

        stwgtProtectionPropertiesFrequency = new QStackedWidget(pageProtectionFrequency);
        stwgtProtectionPropertiesFrequency->setObjectName(QStringLiteral("stwgtProtectionPropertiesFrequency"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesFrequency->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesFrequency->setSizePolicy(sizePolicy);
        pageProtectionPropertiesACHR1 = new QWidget();
        pageProtectionPropertiesACHR1->setObjectName(QStringLiteral("pageProtectionPropertiesACHR1"));
        verticalLayout_102 = new QVBoxLayout(pageProtectionPropertiesACHR1);
        verticalLayout_102->setSpacing(6);
        verticalLayout_102->setContentsMargins(11, 11, 11, 11);
        verticalLayout_102->setObjectName(QStringLiteral("verticalLayout_102"));
        horizontalLayout_35 = new QHBoxLayout();
        horizontalLayout_35->setSpacing(6);
        horizontalLayout_35->setObjectName(QStringLiteral("horizontalLayout_35"));
        gboxProtectionPropertiesFrequency_ACR1 = new QGroupBox(pageProtectionPropertiesACHR1);
        gboxProtectionPropertiesFrequency_ACR1->setObjectName(QStringLiteral("gboxProtectionPropertiesFrequency_ACR1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesFrequency_ACR1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesFrequency_ACR1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesFrequency_ACR1->setAlignment(Qt::AlignCenter);
        horizontalLayout_32 = new QHBoxLayout(gboxProtectionPropertiesFrequency_ACR1);
        horizontalLayout_32->setSpacing(6);
        horizontalLayout_32->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_32->setObjectName(QStringLiteral("horizontalLayout_32"));
        verticalLayout_34 = new QVBoxLayout();
        verticalLayout_34->setSpacing(6);
        verticalLayout_34->setObjectName(QStringLiteral("verticalLayout_34"));
        lblTextProtectionFrequency_ACR1_Ctrl = new QLabel(gboxProtectionPropertiesFrequency_ACR1);
        lblTextProtectionFrequency_ACR1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR1_Ctrl"));
        lblTextProtectionFrequency_ACR1_Ctrl->setScaledContents(true);

        verticalLayout_34->addWidget(lblTextProtectionFrequency_ACR1_Ctrl);

        lblTextProtectionFrequency_ACR1_StartFreq = new QLabel(gboxProtectionPropertiesFrequency_ACR1);
        lblTextProtectionFrequency_ACR1_StartFreq->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR1_StartFreq"));
        lblTextProtectionFrequency_ACR1_StartFreq->setScaledContents(true);

        verticalLayout_34->addWidget(lblTextProtectionFrequency_ACR1_StartFreq);

        lblTextProtectionFrequency_ACR1_Pause = new QLabel(gboxProtectionPropertiesFrequency_ACR1);
        lblTextProtectionFrequency_ACR1_Pause->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR1_Pause"));
        lblTextProtectionFrequency_ACR1_Pause->setScaledContents(true);

        verticalLayout_34->addWidget(lblTextProtectionFrequency_ACR1_Pause);

        lblTextProtectionFrequency_ACR1_Umin = new QLabel(gboxProtectionPropertiesFrequency_ACR1);
        lblTextProtectionFrequency_ACR1_Umin->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR1_Umin"));
        lblTextProtectionFrequency_ACR1_Umin->setScaledContents(true);

        verticalLayout_34->addWidget(lblTextProtectionFrequency_ACR1_Umin);

        lblTextProtectionFrequency_ACR1_KvzPower = new QLabel(gboxProtectionPropertiesFrequency_ACR1);
        lblTextProtectionFrequency_ACR1_KvzPower->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR1_KvzPower"));
        lblTextProtectionFrequency_ACR1_KvzPower->setScaledContents(true);

        verticalLayout_34->addWidget(lblTextProtectionFrequency_ACR1_KvzPower);


        horizontalLayout_32->addLayout(verticalLayout_34);

        vltProtectionMTZ31_16 = new QVBoxLayout();
        vltProtectionMTZ31_16->setSpacing(6);
        vltProtectionMTZ31_16->setObjectName(QStringLiteral("vltProtectionMTZ31_16"));
        cboxM51 = new QComboBox(gboxProtectionPropertiesFrequency_ACR1);
        cboxM51->addItem(QString());
        cboxM51->addItem(QString());
        cboxM51->addItem(QString());
        cboxM51->setObjectName(QStringLiteral("cboxM51"));
        cboxM51->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_16->addWidget(cboxM51);

        leM52 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR1);
        leM52->setObjectName(QStringLiteral("leM52"));
        sizePolicy5.setHeightForWidth(leM52->sizePolicy().hasHeightForWidth());
        leM52->setSizePolicy(sizePolicy5);
        leM52->setMaximumSize(QSize(100, 100));
        leM52->setMaxLength(10);
        leM52->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_16->addWidget(leM52);

        leM53 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR1);
        leM53->setObjectName(QStringLiteral("leM53"));
        sizePolicy5.setHeightForWidth(leM53->sizePolicy().hasHeightForWidth());
        leM53->setSizePolicy(sizePolicy5);
        leM53->setMaximumSize(QSize(100, 100));
        leM53->setMaxLength(10);
        leM53->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_16->addWidget(leM53);

        leM54 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR1);
        leM54->setObjectName(QStringLiteral("leM54"));
        sizePolicy5.setHeightForWidth(leM54->sizePolicy().hasHeightForWidth());
        leM54->setSizePolicy(sizePolicy5);
        leM54->setMaximumSize(QSize(100, 100));
        leM54->setMaxLength(10);
        leM54->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_16->addWidget(leM54);

        leX16 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR1);
        leX16->setObjectName(QStringLiteral("leX16"));
        sizePolicy5.setHeightForWidth(leX16->sizePolicy().hasHeightForWidth());
        leX16->setSizePolicy(sizePolicy5);
        leX16->setMaximumSize(QSize(100, 100));
        leX16->setMaxLength(10);
        leX16->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_16->addWidget(leX16);


        horizontalLayout_32->addLayout(vltProtectionMTZ31_16);


        horizontalLayout_35->addWidget(gboxProtectionPropertiesFrequency_ACR1);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_35->addItem(horizontalSpacer_18);


        verticalLayout_102->addLayout(horizontalLayout_35);

        verticalSpacer_37 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_102->addItem(verticalSpacer_37);

        stwgtProtectionPropertiesFrequency->addWidget(pageProtectionPropertiesACHR1);
        pageProtectionPropertiesACHR2 = new QWidget();
        pageProtectionPropertiesACHR2->setObjectName(QStringLiteral("pageProtectionPropertiesACHR2"));
        verticalLayout_103 = new QVBoxLayout(pageProtectionPropertiesACHR2);
        verticalLayout_103->setSpacing(6);
        verticalLayout_103->setContentsMargins(11, 11, 11, 11);
        verticalLayout_103->setObjectName(QStringLiteral("verticalLayout_103"));
        horizontalLayout_87 = new QHBoxLayout();
        horizontalLayout_87->setSpacing(6);
        horizontalLayout_87->setObjectName(QStringLiteral("horizontalLayout_87"));
        gboxProtectionPropertiesFrequency_ACR2 = new QGroupBox(pageProtectionPropertiesACHR2);
        gboxProtectionPropertiesFrequency_ACR2->setObjectName(QStringLiteral("gboxProtectionPropertiesFrequency_ACR2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesFrequency_ACR2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesFrequency_ACR2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesFrequency_ACR2->setAlignment(Qt::AlignCenter);
        horizontalLayout_33 = new QHBoxLayout(gboxProtectionPropertiesFrequency_ACR2);
        horizontalLayout_33->setSpacing(6);
        horizontalLayout_33->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_33->setObjectName(QStringLiteral("horizontalLayout_33"));
        verticalLayout_35 = new QVBoxLayout();
        verticalLayout_35->setSpacing(6);
        verticalLayout_35->setObjectName(QStringLiteral("verticalLayout_35"));
        lblTextProtectionFrequency_ACR2_Ctrl = new QLabel(gboxProtectionPropertiesFrequency_ACR2);
        lblTextProtectionFrequency_ACR2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR2_Ctrl"));
        lblTextProtectionFrequency_ACR2_Ctrl->setScaledContents(true);

        verticalLayout_35->addWidget(lblTextProtectionFrequency_ACR2_Ctrl);

        lblTextProtectionFrequency_ACR2_StartFreq = new QLabel(gboxProtectionPropertiesFrequency_ACR2);
        lblTextProtectionFrequency_ACR2_StartFreq->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR2_StartFreq"));
        lblTextProtectionFrequency_ACR2_StartFreq->setScaledContents(true);

        verticalLayout_35->addWidget(lblTextProtectionFrequency_ACR2_StartFreq);

        lblTextProtectionFrequency_ACR2_Pause = new QLabel(gboxProtectionPropertiesFrequency_ACR2);
        lblTextProtectionFrequency_ACR2_Pause->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR2_Pause"));
        lblTextProtectionFrequency_ACR2_Pause->setScaledContents(true);

        verticalLayout_35->addWidget(lblTextProtectionFrequency_ACR2_Pause);

        lblTextProtectionFrequency_ACR2_Umin = new QLabel(gboxProtectionPropertiesFrequency_ACR2);
        lblTextProtectionFrequency_ACR2_Umin->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR2_Umin"));
        lblTextProtectionFrequency_ACR2_Umin->setScaledContents(true);

        verticalLayout_35->addWidget(lblTextProtectionFrequency_ACR2_Umin);

        lblTextProtectionFrequency_ACR2_KvzPower = new QLabel(gboxProtectionPropertiesFrequency_ACR2);
        lblTextProtectionFrequency_ACR2_KvzPower->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR2_KvzPower"));
        lblTextProtectionFrequency_ACR2_KvzPower->setScaledContents(true);

        verticalLayout_35->addWidget(lblTextProtectionFrequency_ACR2_KvzPower);


        horizontalLayout_33->addLayout(verticalLayout_35);

        vltProtectionMTZ31_17 = new QVBoxLayout();
        vltProtectionMTZ31_17->setSpacing(6);
        vltProtectionMTZ31_17->setObjectName(QStringLiteral("vltProtectionMTZ31_17"));
        cboxM55 = new QComboBox(gboxProtectionPropertiesFrequency_ACR2);
        cboxM55->addItem(QString());
        cboxM55->addItem(QString());
        cboxM55->addItem(QString());
        cboxM55->setObjectName(QStringLiteral("cboxM55"));
        cboxM55->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_17->addWidget(cboxM55);

        leM56 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR2);
        leM56->setObjectName(QStringLiteral("leM56"));
        sizePolicy5.setHeightForWidth(leM56->sizePolicy().hasHeightForWidth());
        leM56->setSizePolicy(sizePolicy5);
        leM56->setMaximumSize(QSize(100, 100));
        leM56->setMaxLength(10);
        leM56->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_17->addWidget(leM56);

        leM57 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR2);
        leM57->setObjectName(QStringLiteral("leM57"));
        sizePolicy5.setHeightForWidth(leM57->sizePolicy().hasHeightForWidth());
        leM57->setSizePolicy(sizePolicy5);
        leM57->setMaximumSize(QSize(100, 100));
        leM57->setMaxLength(10);
        leM57->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_17->addWidget(leM57);

        leM58 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR2);
        leM58->setObjectName(QStringLiteral("leM58"));
        sizePolicy5.setHeightForWidth(leM58->sizePolicy().hasHeightForWidth());
        leM58->setSizePolicy(sizePolicy5);
        leM58->setMaximumSize(QSize(100, 100));
        leM58->setMaxLength(10);
        leM58->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_17->addWidget(leM58);

        leX17 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR2);
        leX17->setObjectName(QStringLiteral("leX17"));
        sizePolicy5.setHeightForWidth(leX17->sizePolicy().hasHeightForWidth());
        leX17->setSizePolicy(sizePolicy5);
        leX17->setMaximumSize(QSize(100, 100));
        leX17->setMaxLength(10);
        leX17->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_17->addWidget(leX17);


        horizontalLayout_33->addLayout(vltProtectionMTZ31_17);


        horizontalLayout_87->addWidget(gboxProtectionPropertiesFrequency_ACR2);

        horizontalSpacer_49 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_87->addItem(horizontalSpacer_49);


        verticalLayout_103->addLayout(horizontalLayout_87);

        verticalSpacer_38 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_103->addItem(verticalSpacer_38);

        stwgtProtectionPropertiesFrequency->addWidget(pageProtectionPropertiesACHR2);
        pageProtectionPropertiesACHR3 = new QWidget();
        pageProtectionPropertiesACHR3->setObjectName(QStringLiteral("pageProtectionPropertiesACHR3"));
        verticalLayout_37 = new QVBoxLayout(pageProtectionPropertiesACHR3);
        verticalLayout_37->setSpacing(6);
        verticalLayout_37->setContentsMargins(11, 11, 11, 11);
        verticalLayout_37->setObjectName(QStringLiteral("verticalLayout_37"));
        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setSpacing(6);
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        gboxProtectionPropertiesFrequency_ACR3 = new QGroupBox(pageProtectionPropertiesACHR3);
        gboxProtectionPropertiesFrequency_ACR3->setObjectName(QStringLiteral("gboxProtectionPropertiesFrequency_ACR3"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesFrequency_ACR3->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesFrequency_ACR3->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesFrequency_ACR3->setAlignment(Qt::AlignCenter);
        horizontalLayout_34 = new QHBoxLayout(gboxProtectionPropertiesFrequency_ACR3);
        horizontalLayout_34->setSpacing(6);
        horizontalLayout_34->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_34->setObjectName(QStringLiteral("horizontalLayout_34"));
        verticalLayout_36 = new QVBoxLayout();
        verticalLayout_36->setSpacing(6);
        verticalLayout_36->setObjectName(QStringLiteral("verticalLayout_36"));
        lblTextProtectionFrequency_ACR3_Ctrl = new QLabel(gboxProtectionPropertiesFrequency_ACR3);
        lblTextProtectionFrequency_ACR3_Ctrl->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR3_Ctrl"));
        lblTextProtectionFrequency_ACR3_Ctrl->setScaledContents(true);

        verticalLayout_36->addWidget(lblTextProtectionFrequency_ACR3_Ctrl);

        lblTextProtectionFrequency_ACR3_StartFreq = new QLabel(gboxProtectionPropertiesFrequency_ACR3);
        lblTextProtectionFrequency_ACR3_StartFreq->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR3_StartFreq"));
        lblTextProtectionFrequency_ACR3_StartFreq->setScaledContents(true);

        verticalLayout_36->addWidget(lblTextProtectionFrequency_ACR3_StartFreq);

        lblTextProtectionFrequency_ACR3_Pause = new QLabel(gboxProtectionPropertiesFrequency_ACR3);
        lblTextProtectionFrequency_ACR3_Pause->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR3_Pause"));
        lblTextProtectionFrequency_ACR3_Pause->setScaledContents(true);

        verticalLayout_36->addWidget(lblTextProtectionFrequency_ACR3_Pause);

        lblTextProtectionFrequency_ACR3_Umin = new QLabel(gboxProtectionPropertiesFrequency_ACR3);
        lblTextProtectionFrequency_ACR3_Umin->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR3_Umin"));
        lblTextProtectionFrequency_ACR3_Umin->setScaledContents(true);

        verticalLayout_36->addWidget(lblTextProtectionFrequency_ACR3_Umin);

        lblTextProtectionFrequency_ACR3_KvzPower = new QLabel(gboxProtectionPropertiesFrequency_ACR3);
        lblTextProtectionFrequency_ACR3_KvzPower->setObjectName(QStringLiteral("lblTextProtectionFrequency_ACR3_KvzPower"));
        lblTextProtectionFrequency_ACR3_KvzPower->setScaledContents(true);

        verticalLayout_36->addWidget(lblTextProtectionFrequency_ACR3_KvzPower);


        horizontalLayout_34->addLayout(verticalLayout_36);

        vltProtectionMTZ31_18 = new QVBoxLayout();
        vltProtectionMTZ31_18->setSpacing(6);
        vltProtectionMTZ31_18->setObjectName(QStringLiteral("vltProtectionMTZ31_18"));
        cboxM59 = new QComboBox(gboxProtectionPropertiesFrequency_ACR3);
        cboxM59->addItem(QString());
        cboxM59->addItem(QString());
        cboxM59->addItem(QString());
        cboxM59->setObjectName(QStringLiteral("cboxM59"));
        cboxM59->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_18->addWidget(cboxM59);

        leM60 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR3);
        leM60->setObjectName(QStringLiteral("leM60"));
        sizePolicy5.setHeightForWidth(leM60->sizePolicy().hasHeightForWidth());
        leM60->setSizePolicy(sizePolicy5);
        leM60->setMaximumSize(QSize(100, 100));
        leM60->setMaxLength(10);
        leM60->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_18->addWidget(leM60);

        leM61 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR3);
        leM61->setObjectName(QStringLiteral("leM61"));
        sizePolicy5.setHeightForWidth(leM61->sizePolicy().hasHeightForWidth());
        leM61->setSizePolicy(sizePolicy5);
        leM61->setMaximumSize(QSize(100, 100));
        leM61->setMaxLength(10);
        leM61->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_18->addWidget(leM61);

        leM62 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR3);
        leM62->setObjectName(QStringLiteral("leM62"));
        sizePolicy5.setHeightForWidth(leM62->sizePolicy().hasHeightForWidth());
        leM62->setSizePolicy(sizePolicy5);
        leM62->setMaximumSize(QSize(100, 100));
        leM62->setMaxLength(10);
        leM62->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_18->addWidget(leM62);

        leX18 = new QLineEdit(gboxProtectionPropertiesFrequency_ACR3);
        leX18->setObjectName(QStringLiteral("leX18"));
        sizePolicy5.setHeightForWidth(leX18->sizePolicy().hasHeightForWidth());
        leX18->setSizePolicy(sizePolicy5);
        leX18->setMaximumSize(QSize(100, 100));
        leX18->setMaxLength(10);
        leX18->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_18->addWidget(leX18);


        horizontalLayout_34->addLayout(vltProtectionMTZ31_18);


        horizontalLayout_31->addWidget(gboxProtectionPropertiesFrequency_ACR3);

        horizontalSpacer_52 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_31->addItem(horizontalSpacer_52);


        verticalLayout_37->addLayout(horizontalLayout_31);

        verticalSpacer_39 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_37->addItem(verticalSpacer_39);

        stwgtProtectionPropertiesFrequency->addWidget(pageProtectionPropertiesACHR3);

        horizontalLayout_88->addWidget(stwgtProtectionPropertiesFrequency);

        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_88->addItem(horizontalSpacer_26);


        verticalLayout_121->addLayout(horizontalLayout_88);

        verticalSpacer_65 = new QSpacerItem(20, 114, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_121->addItem(verticalSpacer_65);

        stwgtMain->addWidget(pageProtectionFrequency);
        pageProtectionExternal = new QWidget();
        pageProtectionExternal->setObjectName(QStringLiteral("pageProtectionExternal"));
        verticalLayout_122 = new QVBoxLayout(pageProtectionExternal);
        verticalLayout_122->setSpacing(6);
        verticalLayout_122->setContentsMargins(11, 11, 11, 11);
        verticalLayout_122->setObjectName(QStringLiteral("verticalLayout_122"));
        horizontalLayout_95 = new QHBoxLayout();
        horizontalLayout_95->setSpacing(6);
        horizontalLayout_95->setObjectName(QStringLiteral("horizontalLayout_95"));
        verticalLayout_38 = new QVBoxLayout();
        verticalLayout_38->setSpacing(0);
        verticalLayout_38->setObjectName(QStringLiteral("verticalLayout_38"));
        pbtnProtectionExternal_Arc = new QPushButton(pageProtectionExternal);
        pbtnProtectionExternal_Arc->setObjectName(QStringLiteral("pbtnProtectionExternal_Arc"));
        sizePolicy5.setHeightForWidth(pbtnProtectionExternal_Arc->sizePolicy().hasHeightForWidth());
        pbtnProtectionExternal_Arc->setSizePolicy(sizePolicy5);
        pbtnProtectionExternal_Arc->setCheckable(false);

        verticalLayout_38->addWidget(pbtnProtectionExternal_Arc);

        pbtnProtectionExternal1 = new QPushButton(pageProtectionExternal);
        pbtnProtectionExternal1->setObjectName(QStringLiteral("pbtnProtectionExternal1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionExternal1->sizePolicy().hasHeightForWidth());
        pbtnProtectionExternal1->setSizePolicy(sizePolicy5);
        pbtnProtectionExternal1->setCheckable(false);

        verticalLayout_38->addWidget(pbtnProtectionExternal1);

        pbtnProtectionExternal2 = new QPushButton(pageProtectionExternal);
        pbtnProtectionExternal2->setObjectName(QStringLiteral("pbtnProtectionExternal2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionExternal2->sizePolicy().hasHeightForWidth());
        pbtnProtectionExternal2->setSizePolicy(sizePolicy5);
        pbtnProtectionExternal2->setCheckable(false);

        verticalLayout_38->addWidget(pbtnProtectionExternal2);

        pbtnProtectionExternal3 = new QPushButton(pageProtectionExternal);
        pbtnProtectionExternal3->setObjectName(QStringLiteral("pbtnProtectionExternal3"));
        sizePolicy5.setHeightForWidth(pbtnProtectionExternal3->sizePolicy().hasHeightForWidth());
        pbtnProtectionExternal3->setSizePolicy(sizePolicy5);
        pbtnProtectionExternal3->setCheckable(false);

        verticalLayout_38->addWidget(pbtnProtectionExternal3);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_38->addItem(verticalSpacer_13);


        horizontalLayout_95->addLayout(verticalLayout_38);

        stwgtProtectionPropertiesExternal = new QStackedWidget(pageProtectionExternal);
        stwgtProtectionPropertiesExternal->setObjectName(QStringLiteral("stwgtProtectionPropertiesExternal"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesExternal->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesExternal->setSizePolicy(sizePolicy);
        pageProtectionPropertiesArc = new QWidget();
        pageProtectionPropertiesArc->setObjectName(QStringLiteral("pageProtectionPropertiesArc"));
        verticalLayout_47 = new QVBoxLayout(pageProtectionPropertiesArc);
        verticalLayout_47->setSpacing(6);
        verticalLayout_47->setContentsMargins(11, 11, 11, 11);
        verticalLayout_47->setObjectName(QStringLiteral("verticalLayout_47"));
        verticalLayout_45 = new QVBoxLayout();
        verticalLayout_45->setSpacing(6);
        verticalLayout_45->setObjectName(QStringLiteral("verticalLayout_45"));
        horizontalLayout_94 = new QHBoxLayout();
        horizontalLayout_94->setSpacing(6);
        horizontalLayout_94->setObjectName(QStringLiteral("horizontalLayout_94"));
        gboxProtectionPropertiesExternalArc = new QGroupBox(pageProtectionPropertiesArc);
        gboxProtectionPropertiesExternalArc->setObjectName(QStringLiteral("gboxProtectionPropertiesExternalArc"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesExternalArc->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesExternalArc->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesExternalArc->setAlignment(Qt::AlignCenter);
        horizontalLayout_40 = new QHBoxLayout(gboxProtectionPropertiesExternalArc);
        horizontalLayout_40->setSpacing(6);
        horizontalLayout_40->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_40->setObjectName(QStringLiteral("horizontalLayout_40"));
        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(6);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        verticalLayout_39 = new QVBoxLayout();
        verticalLayout_39->setSpacing(6);
        verticalLayout_39->setObjectName(QStringLiteral("verticalLayout_39"));
        lblTextProtectionExternal_Arc_Ctrl = new QLabel(gboxProtectionPropertiesExternalArc);
        lblTextProtectionExternal_Arc_Ctrl->setObjectName(QStringLiteral("lblTextProtectionExternal_Arc_Ctrl"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Arc_Ctrl->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Arc_Ctrl->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Arc_Ctrl->setScaledContents(true);

        verticalLayout_39->addWidget(lblTextProtectionExternal_Arc_Ctrl);

        lblTextProtectionExternal_Arc_StartCurrent = new QLabel(gboxProtectionPropertiesExternalArc);
        lblTextProtectionExternal_Arc_StartCurrent->setObjectName(QStringLiteral("lblTextProtectionExternal_Arc_StartCurrent"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Arc_StartCurrent->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Arc_StartCurrent->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Arc_StartCurrent->setScaledContents(true);

        verticalLayout_39->addWidget(lblTextProtectionExternal_Arc_StartCurrent);

        lblTextProtectionExtermal_Arc_KvzCurrent = new QLabel(gboxProtectionPropertiesExternalArc);
        lblTextProtectionExtermal_Arc_KvzCurrent->setObjectName(QStringLiteral("lblTextProtectionExtermal_Arc_KvzCurrent"));
        sizePolicy.setHeightForWidth(lblTextProtectionExtermal_Arc_KvzCurrent->sizePolicy().hasHeightForWidth());
        lblTextProtectionExtermal_Arc_KvzCurrent->setSizePolicy(sizePolicy);
        lblTextProtectionExtermal_Arc_KvzCurrent->setScaledContents(true);

        verticalLayout_39->addWidget(lblTextProtectionExtermal_Arc_KvzCurrent);


        horizontalLayout_36->addLayout(verticalLayout_39);

        vltProtectionMTZ31_19 = new QVBoxLayout();
        vltProtectionMTZ31_19->setSpacing(6);
        vltProtectionMTZ31_19->setObjectName(QStringLiteral("vltProtectionMTZ31_19"));
        cboxM63 = new QComboBox(gboxProtectionPropertiesExternalArc);
        cboxM63->addItem(QString());
        cboxM63->addItem(QString());
        cboxM63->addItem(QString());
        cboxM63->addItem(QString());
        cboxM63->setObjectName(QStringLiteral("cboxM63"));
        sizePolicy.setHeightForWidth(cboxM63->sizePolicy().hasHeightForWidth());
        cboxM63->setSizePolicy(sizePolicy);
        cboxM63->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_19->addWidget(cboxM63);

        leM64 = new QLineEdit(gboxProtectionPropertiesExternalArc);
        leM64->setObjectName(QStringLiteral("leM64"));
        sizePolicy.setHeightForWidth(leM64->sizePolicy().hasHeightForWidth());
        leM64->setSizePolicy(sizePolicy);
        leM64->setMaximumSize(QSize(100, 100));
        leM64->setMaxLength(10);
        leM64->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_19->addWidget(leM64);

        leX19 = new QLineEdit(gboxProtectionPropertiesExternalArc);
        leX19->setObjectName(QStringLiteral("leX19"));
        sizePolicy.setHeightForWidth(leX19->sizePolicy().hasHeightForWidth());
        leX19->setSizePolicy(sizePolicy);
        leX19->setMaximumSize(QSize(100, 100));
        leX19->setMaxLength(10);
        leX19->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_19->addWidget(leX19);


        horizontalLayout_36->addLayout(vltProtectionMTZ31_19);


        horizontalLayout_40->addLayout(horizontalLayout_36);


        horizontalLayout_94->addWidget(gboxProtectionPropertiesExternalArc);

        horizontalSpacer_56 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_94->addItem(horizontalSpacer_56);


        verticalLayout_45->addLayout(horizontalLayout_94);

        verticalSpacer_40 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_45->addItem(verticalSpacer_40);


        verticalLayout_47->addLayout(verticalLayout_45);

        verticalSpacer_16 = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_47->addItem(verticalSpacer_16);

        stwgtProtectionPropertiesExternal->addWidget(pageProtectionPropertiesArc);
        pageProtectionPropertiesExternal1 = new QWidget();
        pageProtectionPropertiesExternal1->setObjectName(QStringLiteral("pageProtectionPropertiesExternal1"));
        verticalLayout_48 = new QVBoxLayout(pageProtectionPropertiesExternal1);
        verticalLayout_48->setSpacing(6);
        verticalLayout_48->setContentsMargins(11, 11, 11, 11);
        verticalLayout_48->setObjectName(QStringLiteral("verticalLayout_48"));
        verticalLayout_40 = new QVBoxLayout();
        verticalLayout_40->setSpacing(6);
        verticalLayout_40->setObjectName(QStringLiteral("verticalLayout_40"));
        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setSpacing(6);
        horizontalLayout_41->setObjectName(QStringLiteral("horizontalLayout_41"));
        gboxProtectionPropertiesExternal1 = new QGroupBox(pageProtectionPropertiesExternal1);
        gboxProtectionPropertiesExternal1->setObjectName(QStringLiteral("gboxProtectionPropertiesExternal1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesExternal1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesExternal1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesExternal1->setAlignment(Qt::AlignCenter);
        horizontalLayout_89 = new QHBoxLayout(gboxProtectionPropertiesExternal1);
        horizontalLayout_89->setSpacing(6);
        horizontalLayout_89->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_89->setObjectName(QStringLiteral("horizontalLayout_89"));
        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setSpacing(6);
        horizontalLayout_37->setObjectName(QStringLiteral("horizontalLayout_37"));
        verticalLayout_42 = new QVBoxLayout();
        verticalLayout_42->setSpacing(6);
        verticalLayout_42->setObjectName(QStringLiteral("verticalLayout_42"));
        lblTextProtectionExternal_Ext1_Ctrl = new QLabel(gboxProtectionPropertiesExternal1);
        lblTextProtectionExternal_Ext1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionExternal_Ext1_Ctrl"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Ext1_Ctrl->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Ext1_Ctrl->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Ext1_Ctrl->setScaledContents(true);

        verticalLayout_42->addWidget(lblTextProtectionExternal_Ext1_Ctrl);

        lblTextProtectionExternal_Ext1_Pause = new QLabel(gboxProtectionPropertiesExternal1);
        lblTextProtectionExternal_Ext1_Pause->setObjectName(QStringLiteral("lblTextProtectionExternal_Ext1_Pause"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Ext1_Pause->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Ext1_Pause->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Ext1_Pause->setScaledContents(true);

        verticalLayout_42->addWidget(lblTextProtectionExternal_Ext1_Pause);


        horizontalLayout_37->addLayout(verticalLayout_42);

        vltProtectionMTZ31_20 = new QVBoxLayout();
        vltProtectionMTZ31_20->setSpacing(6);
        vltProtectionMTZ31_20->setObjectName(QStringLiteral("vltProtectionMTZ31_20"));
        cboxM71 = new QComboBox(gboxProtectionPropertiesExternal1);
        cboxM71->addItem(QString());
        cboxM71->addItem(QString());
        cboxM71->addItem(QString());
        cboxM71->setObjectName(QStringLiteral("cboxM71"));
        sizePolicy.setHeightForWidth(cboxM71->sizePolicy().hasHeightForWidth());
        cboxM71->setSizePolicy(sizePolicy);
        cboxM71->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_20->addWidget(cboxM71);

        leM72 = new QLineEdit(gboxProtectionPropertiesExternal1);
        leM72->setObjectName(QStringLiteral("leM72"));
        sizePolicy.setHeightForWidth(leM72->sizePolicy().hasHeightForWidth());
        leM72->setSizePolicy(sizePolicy);
        leM72->setMaximumSize(QSize(100, 100));
        leM72->setMaxLength(10);
        leM72->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_20->addWidget(leM72);


        horizontalLayout_37->addLayout(vltProtectionMTZ31_20);


        horizontalLayout_89->addLayout(horizontalLayout_37);


        horizontalLayout_41->addWidget(gboxProtectionPropertiesExternal1);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_41->addItem(horizontalSpacer_19);


        verticalLayout_40->addLayout(horizontalLayout_41);

        verticalSpacer_41 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_40->addItem(verticalSpacer_41);


        verticalLayout_48->addLayout(verticalLayout_40);

        verticalSpacer_11 = new QSpacerItem(20, 110, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_48->addItem(verticalSpacer_11);

        stwgtProtectionPropertiesExternal->addWidget(pageProtectionPropertiesExternal1);
        pageProtectionPropertiesExternal2 = new QWidget();
        pageProtectionPropertiesExternal2->setObjectName(QStringLiteral("pageProtectionPropertiesExternal2"));
        verticalLayout_49 = new QVBoxLayout(pageProtectionPropertiesExternal2);
        verticalLayout_49->setSpacing(6);
        verticalLayout_49->setContentsMargins(11, 11, 11, 11);
        verticalLayout_49->setObjectName(QStringLiteral("verticalLayout_49"));
        verticalLayout_41 = new QVBoxLayout();
        verticalLayout_41->setSpacing(6);
        verticalLayout_41->setObjectName(QStringLiteral("verticalLayout_41"));
        horizontalLayout_92 = new QHBoxLayout();
        horizontalLayout_92->setSpacing(6);
        horizontalLayout_92->setObjectName(QStringLiteral("horizontalLayout_92"));
        gboxProtectionPropertiesExternal2 = new QGroupBox(pageProtectionPropertiesExternal2);
        gboxProtectionPropertiesExternal2->setObjectName(QStringLiteral("gboxProtectionPropertiesExternal2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesExternal2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesExternal2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesExternal2->setAlignment(Qt::AlignCenter);
        horizontalLayout_90 = new QHBoxLayout(gboxProtectionPropertiesExternal2);
        horizontalLayout_90->setSpacing(6);
        horizontalLayout_90->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_90->setObjectName(QStringLiteral("horizontalLayout_90"));
        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setSpacing(6);
        horizontalLayout_38->setObjectName(QStringLiteral("horizontalLayout_38"));
        verticalLayout_44 = new QVBoxLayout();
        verticalLayout_44->setSpacing(6);
        verticalLayout_44->setObjectName(QStringLiteral("verticalLayout_44"));
        lblTextProtectionExternal_Ext2_Ctrl = new QLabel(gboxProtectionPropertiesExternal2);
        lblTextProtectionExternal_Ext2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionExternal_Ext2_Ctrl"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Ext2_Ctrl->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Ext2_Ctrl->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Ext2_Ctrl->setScaledContents(true);

        verticalLayout_44->addWidget(lblTextProtectionExternal_Ext2_Ctrl);

        lblTextProtectionExternal_Ext2_Pause = new QLabel(gboxProtectionPropertiesExternal2);
        lblTextProtectionExternal_Ext2_Pause->setObjectName(QStringLiteral("lblTextProtectionExternal_Ext2_Pause"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Ext2_Pause->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Ext2_Pause->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Ext2_Pause->setScaledContents(true);

        verticalLayout_44->addWidget(lblTextProtectionExternal_Ext2_Pause);


        horizontalLayout_38->addLayout(verticalLayout_44);

        vltProtectionMTZ31_21 = new QVBoxLayout();
        vltProtectionMTZ31_21->setSpacing(6);
        vltProtectionMTZ31_21->setObjectName(QStringLiteral("vltProtectionMTZ31_21"));
        cboxM73 = new QComboBox(gboxProtectionPropertiesExternal2);
        cboxM73->addItem(QString());
        cboxM73->addItem(QString());
        cboxM73->addItem(QString());
        cboxM73->setObjectName(QStringLiteral("cboxM73"));
        sizePolicy.setHeightForWidth(cboxM73->sizePolicy().hasHeightForWidth());
        cboxM73->setSizePolicy(sizePolicy);
        cboxM73->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_21->addWidget(cboxM73);

        leM74 = new QLineEdit(gboxProtectionPropertiesExternal2);
        leM74->setObjectName(QStringLiteral("leM74"));
        sizePolicy.setHeightForWidth(leM74->sizePolicy().hasHeightForWidth());
        leM74->setSizePolicy(sizePolicy);
        leM74->setMaximumSize(QSize(100, 100));
        leM74->setMaxLength(10);
        leM74->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_21->addWidget(leM74);


        horizontalLayout_38->addLayout(vltProtectionMTZ31_21);


        horizontalLayout_90->addLayout(horizontalLayout_38);


        horizontalLayout_92->addWidget(gboxProtectionPropertiesExternal2);

        horizontalSpacer_54 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_92->addItem(horizontalSpacer_54);


        verticalLayout_41->addLayout(horizontalLayout_92);

        verticalSpacer_42 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_41->addItem(verticalSpacer_42);


        verticalLayout_49->addLayout(verticalLayout_41);

        verticalSpacer_12 = new QSpacerItem(20, 110, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_49->addItem(verticalSpacer_12);

        stwgtProtectionPropertiesExternal->addWidget(pageProtectionPropertiesExternal2);
        pageProtectionPropertiesExternal3 = new QWidget();
        pageProtectionPropertiesExternal3->setObjectName(QStringLiteral("pageProtectionPropertiesExternal3"));
        verticalLayout_50 = new QVBoxLayout(pageProtectionPropertiesExternal3);
        verticalLayout_50->setSpacing(6);
        verticalLayout_50->setContentsMargins(11, 11, 11, 11);
        verticalLayout_50->setObjectName(QStringLiteral("verticalLayout_50"));
        verticalLayout_43 = new QVBoxLayout();
        verticalLayout_43->setSpacing(6);
        verticalLayout_43->setObjectName(QStringLiteral("verticalLayout_43"));
        horizontalLayout_93 = new QHBoxLayout();
        horizontalLayout_93->setSpacing(6);
        horizontalLayout_93->setObjectName(QStringLiteral("horizontalLayout_93"));
        gboxProtectionPropertiesExternal3 = new QGroupBox(pageProtectionPropertiesExternal3);
        gboxProtectionPropertiesExternal3->setObjectName(QStringLiteral("gboxProtectionPropertiesExternal3"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesExternal3->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesExternal3->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesExternal3->setAlignment(Qt::AlignCenter);
        horizontalLayout_91 = new QHBoxLayout(gboxProtectionPropertiesExternal3);
        horizontalLayout_91->setSpacing(6);
        horizontalLayout_91->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_91->setObjectName(QStringLiteral("horizontalLayout_91"));
        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(6);
        horizontalLayout_39->setObjectName(QStringLiteral("horizontalLayout_39"));
        verticalLayout_46 = new QVBoxLayout();
        verticalLayout_46->setSpacing(6);
        verticalLayout_46->setObjectName(QStringLiteral("verticalLayout_46"));
        lblTextProtectionExternal_Ext3_Ctrl = new QLabel(gboxProtectionPropertiesExternal3);
        lblTextProtectionExternal_Ext3_Ctrl->setObjectName(QStringLiteral("lblTextProtectionExternal_Ext3_Ctrl"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Ext3_Ctrl->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Ext3_Ctrl->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Ext3_Ctrl->setScaledContents(true);

        verticalLayout_46->addWidget(lblTextProtectionExternal_Ext3_Ctrl);

        lblTextProtectionExternal_Ext3_Pause = new QLabel(gboxProtectionPropertiesExternal3);
        lblTextProtectionExternal_Ext3_Pause->setObjectName(QStringLiteral("lblTextProtectionExternal_Ext3_Pause"));
        sizePolicy.setHeightForWidth(lblTextProtectionExternal_Ext3_Pause->sizePolicy().hasHeightForWidth());
        lblTextProtectionExternal_Ext3_Pause->setSizePolicy(sizePolicy);
        lblTextProtectionExternal_Ext3_Pause->setScaledContents(true);

        verticalLayout_46->addWidget(lblTextProtectionExternal_Ext3_Pause);


        horizontalLayout_39->addLayout(verticalLayout_46);

        vltProtectionMTZ31_22 = new QVBoxLayout();
        vltProtectionMTZ31_22->setSpacing(6);
        vltProtectionMTZ31_22->setObjectName(QStringLiteral("vltProtectionMTZ31_22"));
        cboxM75 = new QComboBox(gboxProtectionPropertiesExternal3);
        cboxM75->addItem(QString());
        cboxM75->addItem(QString());
        cboxM75->addItem(QString());
        cboxM75->setObjectName(QStringLiteral("cboxM75"));
        sizePolicy.setHeightForWidth(cboxM75->sizePolicy().hasHeightForWidth());
        cboxM75->setSizePolicy(sizePolicy);
        cboxM75->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_22->addWidget(cboxM75);

        leM76 = new QLineEdit(gboxProtectionPropertiesExternal3);
        leM76->setObjectName(QStringLiteral("leM76"));
        sizePolicy.setHeightForWidth(leM76->sizePolicy().hasHeightForWidth());
        leM76->setSizePolicy(sizePolicy);
        leM76->setMaximumSize(QSize(100, 100));
        leM76->setMaxLength(10);
        leM76->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_22->addWidget(leM76);


        horizontalLayout_39->addLayout(vltProtectionMTZ31_22);


        horizontalLayout_91->addLayout(horizontalLayout_39);


        horizontalLayout_93->addWidget(gboxProtectionPropertiesExternal3);

        horizontalSpacer_55 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_93->addItem(horizontalSpacer_55);


        verticalLayout_43->addLayout(horizontalLayout_93);

        verticalSpacer_49 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_43->addItem(verticalSpacer_49);


        verticalLayout_50->addLayout(verticalLayout_43);

        verticalSpacer_15 = new QSpacerItem(20, 110, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_50->addItem(verticalSpacer_15);

        stwgtProtectionPropertiesExternal->addWidget(pageProtectionPropertiesExternal3);

        horizontalLayout_95->addWidget(stwgtProtectionPropertiesExternal);

        horizontalSpacer_37 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_95->addItem(horizontalSpacer_37);


        verticalLayout_122->addLayout(horizontalLayout_95);

        verticalSpacer_66 = new QSpacerItem(20, 72, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_122->addItem(verticalSpacer_66);

        stwgtMain->addWidget(pageProtectionExternal);
        pageProtectionTemperature = new QWidget();
        pageProtectionTemperature->setObjectName(QStringLiteral("pageProtectionTemperature"));
        verticalLayout_123 = new QVBoxLayout(pageProtectionTemperature);
        verticalLayout_123->setSpacing(6);
        verticalLayout_123->setContentsMargins(11, 11, 11, 11);
        verticalLayout_123->setObjectName(QStringLiteral("verticalLayout_123"));
        horizontalLayout_96 = new QHBoxLayout();
        horizontalLayout_96->setSpacing(6);
        horizontalLayout_96->setObjectName(QStringLiteral("horizontalLayout_96"));
        verticalLayout_53 = new QVBoxLayout();
        verticalLayout_53->setSpacing(6);
        verticalLayout_53->setObjectName(QStringLiteral("verticalLayout_53"));
        verticalLayout_51 = new QVBoxLayout();
        verticalLayout_51->setSpacing(0);
        verticalLayout_51->setObjectName(QStringLiteral("verticalLayout_51"));
        pbtnProtectionTemp1 = new QPushButton(pageProtectionTemperature);
        pbtnProtectionTemp1->setObjectName(QStringLiteral("pbtnProtectionTemp1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionTemp1->sizePolicy().hasHeightForWidth());
        pbtnProtectionTemp1->setSizePolicy(sizePolicy5);
        pbtnProtectionTemp1->setCheckable(false);

        verticalLayout_51->addWidget(pbtnProtectionTemp1);

        pbtnProtectionTemp2 = new QPushButton(pageProtectionTemperature);
        pbtnProtectionTemp2->setObjectName(QStringLiteral("pbtnProtectionTemp2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionTemp2->sizePolicy().hasHeightForWidth());
        pbtnProtectionTemp2->setSizePolicy(sizePolicy5);
        pbtnProtectionTemp2->setCheckable(false);

        verticalLayout_51->addWidget(pbtnProtectionTemp2);


        verticalLayout_53->addLayout(verticalLayout_51);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_53->addItem(verticalSpacer_14);


        horizontalLayout_96->addLayout(verticalLayout_53);

        stwgtProtectionPropertiesTemperature = new QStackedWidget(pageProtectionTemperature);
        stwgtProtectionPropertiesTemperature->setObjectName(QStringLiteral("stwgtProtectionPropertiesTemperature"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesTemperature->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesTemperature->setSizePolicy(sizePolicy);
        pageProtectionPropertiesTemp1 = new QWidget();
        pageProtectionPropertiesTemp1->setObjectName(QStringLiteral("pageProtectionPropertiesTemp1"));
        verticalLayout_55 = new QVBoxLayout(pageProtectionPropertiesTemp1);
        verticalLayout_55->setSpacing(6);
        verticalLayout_55->setContentsMargins(11, 11, 11, 11);
        verticalLayout_55->setObjectName(QStringLiteral("verticalLayout_55"));
        horizontalLayout_45 = new QHBoxLayout();
        horizontalLayout_45->setSpacing(6);
        horizontalLayout_45->setObjectName(QStringLiteral("horizontalLayout_45"));
        gboxProtectionPropertiesTemperature1 = new QGroupBox(pageProtectionPropertiesTemp1);
        gboxProtectionPropertiesTemperature1->setObjectName(QStringLiteral("gboxProtectionPropertiesTemperature1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesTemperature1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesTemperature1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesTemperature1->setAlignment(Qt::AlignCenter);
        horizontalLayout_43 = new QHBoxLayout(gboxProtectionPropertiesTemperature1);
        horizontalLayout_43->setSpacing(6);
        horizontalLayout_43->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_43->setObjectName(QStringLiteral("horizontalLayout_43"));
        verticalLayout_52 = new QVBoxLayout();
        verticalLayout_52->setSpacing(6);
        verticalLayout_52->setObjectName(QStringLiteral("verticalLayout_52"));
        lblTextProtectionTemp1_Sensor1 = new QLabel(gboxProtectionPropertiesTemperature1);
        lblTextProtectionTemp1_Sensor1->setObjectName(QStringLiteral("lblTextProtectionTemp1_Sensor1"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp1_Sensor1->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp1_Sensor1->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp1_Sensor1->setScaledContents(true);

        verticalLayout_52->addWidget(lblTextProtectionTemp1_Sensor1);

        lblTextProtectionTemp1_Sensor2 = new QLabel(gboxProtectionPropertiesTemperature1);
        lblTextProtectionTemp1_Sensor2->setObjectName(QStringLiteral("lblTextProtectionTemp1_Sensor2"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp1_Sensor2->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp1_Sensor2->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp1_Sensor2->setScaledContents(true);

        verticalLayout_52->addWidget(lblTextProtectionTemp1_Sensor2);

        lblTextProtectionTemp1_StartTemp = new QLabel(gboxProtectionPropertiesTemperature1);
        lblTextProtectionTemp1_StartTemp->setObjectName(QStringLiteral("lblTextProtectionTemp1_StartTemp"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp1_StartTemp->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp1_StartTemp->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp1_StartTemp->setScaledContents(true);

        verticalLayout_52->addWidget(lblTextProtectionTemp1_StartTemp);

        lblTextProtectionTemp1_Pause = new QLabel(gboxProtectionPropertiesTemperature1);
        lblTextProtectionTemp1_Pause->setObjectName(QStringLiteral("lblTextProtectionTemp1_Pause"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp1_Pause->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp1_Pause->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp1_Pause->setScaledContents(true);

        verticalLayout_52->addWidget(lblTextProtectionTemp1_Pause);

        lblTextProtectionTemp1_KvzTemp = new QLabel(gboxProtectionPropertiesTemperature1);
        lblTextProtectionTemp1_KvzTemp->setObjectName(QStringLiteral("lblTextProtectionTemp1_KvzTemp"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp1_KvzTemp->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp1_KvzTemp->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp1_KvzTemp->setScaledContents(true);

        verticalLayout_52->addWidget(lblTextProtectionTemp1_KvzTemp);


        horizontalLayout_43->addLayout(verticalLayout_52);

        vltProtectionMTZ31_23 = new QVBoxLayout();
        vltProtectionMTZ31_23->setSpacing(6);
        vltProtectionMTZ31_23->setObjectName(QStringLiteral("vltProtectionMTZ31_23"));
        cboxM65 = new QComboBox(gboxProtectionPropertiesTemperature1);
        cboxM65->addItem(QString());
        cboxM65->addItem(QString());
        cboxM65->addItem(QString());
        cboxM65->setObjectName(QStringLiteral("cboxM65"));
        cboxM65->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_23->addWidget(cboxM65);

        cboxM66 = new QComboBox(gboxProtectionPropertiesTemperature1);
        cboxM66->addItem(QString());
        cboxM66->addItem(QString());
        cboxM66->addItem(QString());
        cboxM66->setObjectName(QStringLiteral("cboxM66"));
        cboxM66->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_23->addWidget(cboxM66);

        leM67 = new QLineEdit(gboxProtectionPropertiesTemperature1);
        leM67->setObjectName(QStringLiteral("leM67"));
        sizePolicy5.setHeightForWidth(leM67->sizePolicy().hasHeightForWidth());
        leM67->setSizePolicy(sizePolicy5);
        leM67->setMaximumSize(QSize(100, 100));
        leM67->setMaxLength(10);
        leM67->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_23->addWidget(leM67);

        leM69 = new QLineEdit(gboxProtectionPropertiesTemperature1);
        leM69->setObjectName(QStringLiteral("leM69"));
        sizePolicy5.setHeightForWidth(leM69->sizePolicy().hasHeightForWidth());
        leM69->setSizePolicy(sizePolicy5);
        leM69->setMaximumSize(QSize(100, 100));
        leM69->setMaxLength(10);
        leM69->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_23->addWidget(leM69);

        leX20 = new QLineEdit(gboxProtectionPropertiesTemperature1);
        leX20->setObjectName(QStringLiteral("leX20"));
        sizePolicy5.setHeightForWidth(leX20->sizePolicy().hasHeightForWidth());
        leX20->setSizePolicy(sizePolicy5);
        leX20->setMaximumSize(QSize(100, 100));
        leX20->setMaxLength(10);
        leX20->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_23->addWidget(leX20);


        horizontalLayout_43->addLayout(vltProtectionMTZ31_23);


        horizontalLayout_45->addWidget(gboxProtectionPropertiesTemperature1);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_45->addItem(horizontalSpacer_20);


        verticalLayout_55->addLayout(horizontalLayout_45);

        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_55->addItem(verticalSpacer_17);

        stwgtProtectionPropertiesTemperature->addWidget(pageProtectionPropertiesTemp1);
        pageProtectionPropertiesTemp2 = new QWidget();
        pageProtectionPropertiesTemp2->setObjectName(QStringLiteral("pageProtectionPropertiesTemp2"));
        verticalLayout_104 = new QVBoxLayout(pageProtectionPropertiesTemp2);
        verticalLayout_104->setSpacing(6);
        verticalLayout_104->setContentsMargins(11, 11, 11, 11);
        verticalLayout_104->setObjectName(QStringLiteral("verticalLayout_104"));
        horizontalLayout_42 = new QHBoxLayout();
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setObjectName(QStringLiteral("horizontalLayout_42"));
        gboxProtectionPropertiesTemperature2 = new QGroupBox(pageProtectionPropertiesTemp2);
        gboxProtectionPropertiesTemperature2->setObjectName(QStringLiteral("gboxProtectionPropertiesTemperature2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesTemperature2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesTemperature2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesTemperature2->setAlignment(Qt::AlignCenter);
        horizontalLayout_44 = new QHBoxLayout(gboxProtectionPropertiesTemperature2);
        horizontalLayout_44->setSpacing(6);
        horizontalLayout_44->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_44->setObjectName(QStringLiteral("horizontalLayout_44"));
        verticalLayout_54 = new QVBoxLayout();
        verticalLayout_54->setSpacing(6);
        verticalLayout_54->setObjectName(QStringLiteral("verticalLayout_54"));
        lblTextProtectionTemp2_Sensor1 = new QLabel(gboxProtectionPropertiesTemperature2);
        lblTextProtectionTemp2_Sensor1->setObjectName(QStringLiteral("lblTextProtectionTemp2_Sensor1"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp2_Sensor1->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp2_Sensor1->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp2_Sensor1->setScaledContents(true);

        verticalLayout_54->addWidget(lblTextProtectionTemp2_Sensor1);

        lblTextProtectionTemp2_Sensor2 = new QLabel(gboxProtectionPropertiesTemperature2);
        lblTextProtectionTemp2_Sensor2->setObjectName(QStringLiteral("lblTextProtectionTemp2_Sensor2"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp2_Sensor2->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp2_Sensor2->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp2_Sensor2->setScaledContents(true);

        verticalLayout_54->addWidget(lblTextProtectionTemp2_Sensor2);

        lblTextProtectionTemp2_StartTemp = new QLabel(gboxProtectionPropertiesTemperature2);
        lblTextProtectionTemp2_StartTemp->setObjectName(QStringLiteral("lblTextProtectionTemp2_StartTemp"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp2_StartTemp->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp2_StartTemp->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp2_StartTemp->setScaledContents(true);

        verticalLayout_54->addWidget(lblTextProtectionTemp2_StartTemp);

        lblTextProtectionTemp2_Pause = new QLabel(gboxProtectionPropertiesTemperature2);
        lblTextProtectionTemp2_Pause->setObjectName(QStringLiteral("lblTextProtectionTemp2_Pause"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp2_Pause->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp2_Pause->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp2_Pause->setScaledContents(true);

        verticalLayout_54->addWidget(lblTextProtectionTemp2_Pause);

        lblTextProtectionTemp2_KvzTemp = new QLabel(gboxProtectionPropertiesTemperature2);
        lblTextProtectionTemp2_KvzTemp->setObjectName(QStringLiteral("lblTextProtectionTemp2_KvzTemp"));
        sizePolicy5.setHeightForWidth(lblTextProtectionTemp2_KvzTemp->sizePolicy().hasHeightForWidth());
        lblTextProtectionTemp2_KvzTemp->setSizePolicy(sizePolicy5);
        lblTextProtectionTemp2_KvzTemp->setScaledContents(true);

        verticalLayout_54->addWidget(lblTextProtectionTemp2_KvzTemp);


        horizontalLayout_44->addLayout(verticalLayout_54);

        vltProtectionMTZ31_24 = new QVBoxLayout();
        vltProtectionMTZ31_24->setSpacing(6);
        vltProtectionMTZ31_24->setObjectName(QStringLiteral("vltProtectionMTZ31_24"));
        cboxProtectionTemp2_Sensor1 = new QComboBox(gboxProtectionPropertiesTemperature2);
        cboxProtectionTemp2_Sensor1->addItem(QString());
        cboxProtectionTemp2_Sensor1->addItem(QString());
        cboxProtectionTemp2_Sensor1->addItem(QString());
        cboxProtectionTemp2_Sensor1->setObjectName(QStringLiteral("cboxProtectionTemp2_Sensor1"));
        cboxProtectionTemp2_Sensor1->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_24->addWidget(cboxProtectionTemp2_Sensor1);

        cboxProtectionTemp2_Sensor2 = new QComboBox(gboxProtectionPropertiesTemperature2);
        cboxProtectionTemp2_Sensor2->addItem(QString());
        cboxProtectionTemp2_Sensor2->addItem(QString());
        cboxProtectionTemp2_Sensor2->addItem(QString());
        cboxProtectionTemp2_Sensor2->setObjectName(QStringLiteral("cboxProtectionTemp2_Sensor2"));
        cboxProtectionTemp2_Sensor2->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_24->addWidget(cboxProtectionTemp2_Sensor2);

        leM68 = new QLineEdit(gboxProtectionPropertiesTemperature2);
        leM68->setObjectName(QStringLiteral("leM68"));
        sizePolicy5.setHeightForWidth(leM68->sizePolicy().hasHeightForWidth());
        leM68->setSizePolicy(sizePolicy5);
        leM68->setMaximumSize(QSize(100, 100));
        leM68->setMaxLength(10);
        leM68->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_24->addWidget(leM68);

        leM70 = new QLineEdit(gboxProtectionPropertiesTemperature2);
        leM70->setObjectName(QStringLiteral("leM70"));
        sizePolicy5.setHeightForWidth(leM70->sizePolicy().hasHeightForWidth());
        leM70->setSizePolicy(sizePolicy5);
        leM70->setMaximumSize(QSize(100, 100));
        leM70->setMaxLength(10);
        leM70->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_24->addWidget(leM70);

        leX21 = new QLineEdit(gboxProtectionPropertiesTemperature2);
        leX21->setObjectName(QStringLiteral("leX21"));
        sizePolicy5.setHeightForWidth(leX21->sizePolicy().hasHeightForWidth());
        leX21->setSizePolicy(sizePolicy5);
        leX21->setMaximumSize(QSize(100, 100));
        leX21->setMaxLength(10);
        leX21->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_24->addWidget(leX21);


        horizontalLayout_44->addLayout(vltProtectionMTZ31_24);


        horizontalLayout_42->addWidget(gboxProtectionPropertiesTemperature2);

        horizontalSpacer_58 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_42->addItem(horizontalSpacer_58);


        verticalLayout_104->addLayout(horizontalLayout_42);

        verticalSpacer_50 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_104->addItem(verticalSpacer_50);

        stwgtProtectionPropertiesTemperature->addWidget(pageProtectionPropertiesTemp2);

        horizontalLayout_96->addWidget(stwgtProtectionPropertiesTemperature);

        horizontalSpacer_41 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_96->addItem(horizontalSpacer_41);


        verticalLayout_123->addLayout(horizontalLayout_96);

        verticalSpacer_67 = new QSpacerItem(20, 114, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_123->addItem(verticalSpacer_67);

        stwgtMain->addWidget(pageProtectionTemperature);
        pageProtectionLevel = new QWidget();
        pageProtectionLevel->setObjectName(QStringLiteral("pageProtectionLevel"));
        verticalLayout_114 = new QVBoxLayout(pageProtectionLevel);
        verticalLayout_114->setSpacing(6);
        verticalLayout_114->setContentsMargins(11, 11, 11, 11);
        verticalLayout_114->setObjectName(QStringLiteral("verticalLayout_114"));
        horizontalLayout_61 = new QHBoxLayout();
        horizontalLayout_61->setSpacing(6);
        horizontalLayout_61->setObjectName(QStringLiteral("horizontalLayout_61"));
        verticalLayout_57 = new QVBoxLayout();
        verticalLayout_57->setSpacing(0);
        verticalLayout_57->setObjectName(QStringLiteral("verticalLayout_57"));
        pbtnProtectionLevel1 = new QPushButton(pageProtectionLevel);
        pbtnProtectionLevel1->setObjectName(QStringLiteral("pbtnProtectionLevel1"));
        sizePolicy5.setHeightForWidth(pbtnProtectionLevel1->sizePolicy().hasHeightForWidth());
        pbtnProtectionLevel1->setSizePolicy(sizePolicy5);
        pbtnProtectionLevel1->setCheckable(false);

        verticalLayout_57->addWidget(pbtnProtectionLevel1);

        pbtnProtectionLevel2 = new QPushButton(pageProtectionLevel);
        pbtnProtectionLevel2->setObjectName(QStringLiteral("pbtnProtectionLevel2"));
        sizePolicy5.setHeightForWidth(pbtnProtectionLevel2->sizePolicy().hasHeightForWidth());
        pbtnProtectionLevel2->setSizePolicy(sizePolicy5);
        pbtnProtectionLevel2->setCheckable(false);

        verticalLayout_57->addWidget(pbtnProtectionLevel2);

        pbtnProtectionLevelSignStart = new QPushButton(pageProtectionLevel);
        pbtnProtectionLevelSignStart->setObjectName(QStringLiteral("pbtnProtectionLevelSignStart"));
        sizePolicy5.setHeightForWidth(pbtnProtectionLevelSignStart->sizePolicy().hasHeightForWidth());
        pbtnProtectionLevelSignStart->setSizePolicy(sizePolicy5);
        pbtnProtectionLevelSignStart->setCheckable(false);

        verticalLayout_57->addWidget(pbtnProtectionLevelSignStart);

        verticalSpacer_18 = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_57->addItem(verticalSpacer_18);


        horizontalLayout_61->addLayout(verticalLayout_57);

        stwgtProtectionPropertiesLevel = new QStackedWidget(pageProtectionLevel);
        stwgtProtectionPropertiesLevel->setObjectName(QStringLiteral("stwgtProtectionPropertiesLevel"));
        sizePolicy.setHeightForWidth(stwgtProtectionPropertiesLevel->sizePolicy().hasHeightForWidth());
        stwgtProtectionPropertiesLevel->setSizePolicy(sizePolicy);
        pageProtectionPropertiesLevel1 = new QWidget();
        pageProtectionPropertiesLevel1->setObjectName(QStringLiteral("pageProtectionPropertiesLevel1"));
        verticalLayout_106 = new QVBoxLayout(pageProtectionPropertiesLevel1);
        verticalLayout_106->setSpacing(6);
        verticalLayout_106->setContentsMargins(11, 11, 11, 11);
        verticalLayout_106->setObjectName(QStringLiteral("verticalLayout_106"));
        verticalLayout_61 = new QVBoxLayout();
        verticalLayout_61->setSpacing(6);
        verticalLayout_61->setObjectName(QStringLiteral("verticalLayout_61"));
        horizontalLayout_49 = new QHBoxLayout();
        horizontalLayout_49->setSpacing(6);
        horizontalLayout_49->setObjectName(QStringLiteral("horizontalLayout_49"));
        gboxProtectionPropertiesLevel1 = new QGroupBox(pageProtectionPropertiesLevel1);
        gboxProtectionPropertiesLevel1->setObjectName(QStringLiteral("gboxProtectionPropertiesLevel1"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesLevel1->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesLevel1->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesLevel1->setAlignment(Qt::AlignCenter);
        horizontalLayout_47 = new QHBoxLayout(gboxProtectionPropertiesLevel1);
        horizontalLayout_47->setSpacing(6);
        horizontalLayout_47->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_47->setObjectName(QStringLiteral("horizontalLayout_47"));
        verticalLayout_58 = new QVBoxLayout();
        verticalLayout_58->setSpacing(6);
        verticalLayout_58->setObjectName(QStringLiteral("verticalLayout_58"));
        lblTextProtectionLevel1_Ctrl = new QLabel(gboxProtectionPropertiesLevel1);
        lblTextProtectionLevel1_Ctrl->setObjectName(QStringLiteral("lblTextProtectionLevel1_Ctrl"));
        lblTextProtectionLevel1_Ctrl->setScaledContents(true);

        verticalLayout_58->addWidget(lblTextProtectionLevel1_Ctrl);

        lblTextProtectionLevel1_Pause = new QLabel(gboxProtectionPropertiesLevel1);
        lblTextProtectionLevel1_Pause->setObjectName(QStringLiteral("lblTextProtectionLevel1_Pause"));
        lblTextProtectionLevel1_Pause->setScaledContents(true);

        verticalLayout_58->addWidget(lblTextProtectionLevel1_Pause);


        horizontalLayout_47->addLayout(verticalLayout_58);

        vltProtectionMTZ31_25 = new QVBoxLayout();
        vltProtectionMTZ31_25->setSpacing(6);
        vltProtectionMTZ31_25->setObjectName(QStringLiteral("vltProtectionMTZ31_25"));
        cboxM77 = new QComboBox(gboxProtectionPropertiesLevel1);
        cboxM77->addItem(QString());
        cboxM77->addItem(QString());
        cboxM77->addItem(QString());
        cboxM77->addItem(QString());
        cboxM77->addItem(QString());
        cboxM77->addItem(QString());
        cboxM77->setObjectName(QStringLiteral("cboxM77"));
        cboxM77->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_25->addWidget(cboxM77);

        leM78 = new QLineEdit(gboxProtectionPropertiesLevel1);
        leM78->setObjectName(QStringLiteral("leM78"));
        sizePolicy5.setHeightForWidth(leM78->sizePolicy().hasHeightForWidth());
        leM78->setSizePolicy(sizePolicy5);
        leM78->setMaximumSize(QSize(100, 100));
        leM78->setMaxLength(10);
        leM78->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_25->addWidget(leM78);


        horizontalLayout_47->addLayout(vltProtectionMTZ31_25);


        horizontalLayout_49->addWidget(gboxProtectionPropertiesLevel1);

        horizontalSpacer_60 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_49->addItem(horizontalSpacer_60);


        verticalLayout_61->addLayout(horizontalLayout_49);

        verticalSpacer_51 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_61->addItem(verticalSpacer_51);


        verticalLayout_106->addLayout(verticalLayout_61);

        verticalSpacer_53 = new QSpacerItem(20, 65, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_106->addItem(verticalSpacer_53);

        stwgtProtectionPropertiesLevel->addWidget(pageProtectionPropertiesLevel1);
        pageProtectionPropertiesLevel2 = new QWidget();
        pageProtectionPropertiesLevel2->setObjectName(QStringLiteral("pageProtectionPropertiesLevel2"));
        verticalLayout_107 = new QVBoxLayout(pageProtectionPropertiesLevel2);
        verticalLayout_107->setSpacing(6);
        verticalLayout_107->setContentsMargins(11, 11, 11, 11);
        verticalLayout_107->setObjectName(QStringLiteral("verticalLayout_107"));
        verticalLayout_105 = new QVBoxLayout();
        verticalLayout_105->setSpacing(6);
        verticalLayout_105->setObjectName(QStringLiteral("verticalLayout_105"));
        horizontalLayout_46 = new QHBoxLayout();
        horizontalLayout_46->setSpacing(6);
        horizontalLayout_46->setObjectName(QStringLiteral("horizontalLayout_46"));
        gboxProtectionPropertiesLevel2 = new QGroupBox(pageProtectionPropertiesLevel2);
        gboxProtectionPropertiesLevel2->setObjectName(QStringLiteral("gboxProtectionPropertiesLevel2"));
        sizePolicy.setHeightForWidth(gboxProtectionPropertiesLevel2->sizePolicy().hasHeightForWidth());
        gboxProtectionPropertiesLevel2->setSizePolicy(sizePolicy);
        gboxProtectionPropertiesLevel2->setAlignment(Qt::AlignCenter);
        horizontalLayout_48 = new QHBoxLayout(gboxProtectionPropertiesLevel2);
        horizontalLayout_48->setSpacing(6);
        horizontalLayout_48->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_48->setObjectName(QStringLiteral("horizontalLayout_48"));
        verticalLayout_62 = new QVBoxLayout();
        verticalLayout_62->setSpacing(6);
        verticalLayout_62->setObjectName(QStringLiteral("verticalLayout_62"));
        lblTextProtectionLevel2_Ctrl = new QLabel(gboxProtectionPropertiesLevel2);
        lblTextProtectionLevel2_Ctrl->setObjectName(QStringLiteral("lblTextProtectionLevel2_Ctrl"));
        lblTextProtectionLevel2_Ctrl->setScaledContents(true);

        verticalLayout_62->addWidget(lblTextProtectionLevel2_Ctrl);

        lblTextProtectionLevel2_Pause = new QLabel(gboxProtectionPropertiesLevel2);
        lblTextProtectionLevel2_Pause->setObjectName(QStringLiteral("lblTextProtectionLevel2_Pause"));
        lblTextProtectionLevel2_Pause->setScaledContents(true);

        verticalLayout_62->addWidget(lblTextProtectionLevel2_Pause);


        horizontalLayout_48->addLayout(verticalLayout_62);

        vltProtectionMTZ31_26 = new QVBoxLayout();
        vltProtectionMTZ31_26->setSpacing(6);
        vltProtectionMTZ31_26->setObjectName(QStringLiteral("vltProtectionMTZ31_26"));
        cboxProtectionLeve2_Ctrl = new QComboBox(gboxProtectionPropertiesLevel2);
        cboxProtectionLeve2_Ctrl->addItem(QString());
        cboxProtectionLeve2_Ctrl->addItem(QString());
        cboxProtectionLeve2_Ctrl->addItem(QString());
        cboxProtectionLeve2_Ctrl->addItem(QString());
        cboxProtectionLeve2_Ctrl->addItem(QString());
        cboxProtectionLeve2_Ctrl->addItem(QString());
        cboxProtectionLeve2_Ctrl->setObjectName(QStringLiteral("cboxProtectionLeve2_Ctrl"));
        cboxProtectionLeve2_Ctrl->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_26->addWidget(cboxProtectionLeve2_Ctrl);

        leM79 = new QLineEdit(gboxProtectionPropertiesLevel2);
        leM79->setObjectName(QStringLiteral("leM79"));
        sizePolicy5.setHeightForWidth(leM79->sizePolicy().hasHeightForWidth());
        leM79->setSizePolicy(sizePolicy5);
        leM79->setMaximumSize(QSize(100, 100));
        leM79->setMaxLength(10);
        leM79->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_26->addWidget(leM79);


        horizontalLayout_48->addLayout(vltProtectionMTZ31_26);


        horizontalLayout_46->addWidget(gboxProtectionPropertiesLevel2);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_46->addItem(horizontalSpacer_21);


        verticalLayout_105->addLayout(horizontalLayout_46);

        verticalSpacer_52 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_105->addItem(verticalSpacer_52);


        verticalLayout_107->addLayout(verticalLayout_105);

        verticalSpacer_54 = new QSpacerItem(20, 65, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_107->addItem(verticalSpacer_54);

        stwgtProtectionPropertiesLevel->addWidget(pageProtectionPropertiesLevel2);
        pageProtectionLevelSignStart = new QWidget();
        pageProtectionLevelSignStart->setObjectName(QStringLiteral("pageProtectionLevelSignStart"));
        verticalLayout_113 = new QVBoxLayout(pageProtectionLevelSignStart);
        verticalLayout_113->setSpacing(6);
        verticalLayout_113->setContentsMargins(11, 11, 11, 11);
        verticalLayout_113->setObjectName(QStringLiteral("verticalLayout_113"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_56 = new QHBoxLayout();
        horizontalLayout_56->setSpacing(6);
        horizontalLayout_56->setObjectName(QStringLiteral("horizontalLayout_56"));
        verticalLayout_66 = new QVBoxLayout();
        verticalLayout_66->setSpacing(6);
        verticalLayout_66->setObjectName(QStringLiteral("verticalLayout_66"));
        lblTextStartSignal_1 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_1->setObjectName(QStringLiteral("lblTextStartSignal_1"));
        lblTextStartSignal_1->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_1);

        lblTextStartSignal_2 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_2->setObjectName(QStringLiteral("lblTextStartSignal_2"));
        lblTextStartSignal_2->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_2);

        lblTextStartSignal_3 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_3->setObjectName(QStringLiteral("lblTextStartSignal_3"));
        lblTextStartSignal_3->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_3);

        lblTextStartSignal_4 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_4->setObjectName(QStringLiteral("lblTextStartSignal_4"));
        lblTextStartSignal_4->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_4);

        lblTextStartSignal_5 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_5->setObjectName(QStringLiteral("lblTextStartSignal_5"));
        lblTextStartSignal_5->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_5);

        lblTextStartSignal_6 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_6->setObjectName(QStringLiteral("lblTextStartSignal_6"));
        lblTextStartSignal_6->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_6);

        lblTextStartSignal_7 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_7->setObjectName(QStringLiteral("lblTextStartSignal_7"));
        lblTextStartSignal_7->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_7);

        lblTextStartSignal_8 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_8->setObjectName(QStringLiteral("lblTextStartSignal_8"));
        lblTextStartSignal_8->setScaledContents(true);

        verticalLayout_66->addWidget(lblTextStartSignal_8);


        horizontalLayout_56->addLayout(verticalLayout_66);

        verticalLayout_63 = new QVBoxLayout();
        verticalLayout_63->setSpacing(6);
        verticalLayout_63->setObjectName(QStringLiteral("verticalLayout_63"));
        cboxMeasBRU = new QComboBox(pageProtectionLevelSignStart);
        cboxMeasBRU->addItem(QString());
        cboxMeasBRU->addItem(QString());
        cboxMeasBRU->setObjectName(QStringLiteral("cboxMeasBRU"));

        verticalLayout_63->addWidget(cboxMeasBRU);

        cboxfA = new QComboBox(pageProtectionLevelSignStart);
        cboxfA->addItem(QString());
        cboxfA->addItem(QString());
        cboxfA->setObjectName(QStringLiteral("cboxfA"));

        verticalLayout_63->addWidget(cboxfA);

        cboxfB = new QComboBox(pageProtectionLevelSignStart);
        cboxfB->addItem(QString());
        cboxfB->addItem(QString());
        cboxfB->setObjectName(QStringLiteral("cboxfB"));

        verticalLayout_63->addWidget(cboxfB);

        cboxfC = new QComboBox(pageProtectionLevelSignStart);
        cboxfC->addItem(QString());
        cboxfC->addItem(QString());
        cboxfC->setObjectName(QStringLiteral("cboxfC"));

        verticalLayout_63->addWidget(cboxfC);

        cboxWorkVak = new QComboBox(pageProtectionLevelSignStart);
        cboxWorkVak->addItem(QString());
        cboxWorkVak->addItem(QString());
        cboxWorkVak->setObjectName(QStringLiteral("cboxWorkVak"));

        verticalLayout_63->addWidget(cboxWorkVak);

        cboxWorkBRU = new QComboBox(pageProtectionLevelSignStart);
        cboxWorkBRU->addItem(QString());
        cboxWorkBRU->addItem(QString());
        cboxWorkBRU->setObjectName(QStringLiteral("cboxWorkBRU"));

        verticalLayout_63->addWidget(cboxWorkBRU);

        cboxOnBRU = new QComboBox(pageProtectionLevelSignStart);
        cboxOnBRU->addItem(QString());
        cboxOnBRU->addItem(QString());
        cboxOnBRU->setObjectName(QStringLiteral("cboxOnBRU"));

        verticalLayout_63->addWidget(cboxOnBRU);

        cboxRmeasOK = new QComboBox(pageProtectionLevelSignStart);
        cboxRmeasOK->addItem(QString());
        cboxRmeasOK->addItem(QString());
        cboxRmeasOK->setObjectName(QStringLiteral("cboxRmeasOK"));

        verticalLayout_63->addWidget(cboxRmeasOK);


        horizontalLayout_56->addLayout(verticalLayout_63);


        horizontalLayout_4->addLayout(horizontalLayout_56);

        line = new QFrame(pageProtectionLevelSignStart);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line);

        horizontalLayout_57 = new QHBoxLayout();
        horizontalLayout_57->setSpacing(6);
        horizontalLayout_57->setObjectName(QStringLiteral("horizontalLayout_57"));
        verticalLayout_68 = new QVBoxLayout();
        verticalLayout_68->setSpacing(6);
        verticalLayout_68->setObjectName(QStringLiteral("verticalLayout_68"));
        lblTextStartSignal_9 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_9->setObjectName(QStringLiteral("lblTextStartSignal_9"));
        lblTextStartSignal_9->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_9);

        lblTextStartSignal_10 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_10->setObjectName(QStringLiteral("lblTextStartSignal_10"));
        lblTextStartSignal_10->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_10);

        lblTextStartSignal_11 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_11->setObjectName(QStringLiteral("lblTextStartSignal_11"));
        lblTextStartSignal_11->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_11);

        lblTextStartSignal_12 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_12->setObjectName(QStringLiteral("lblTextStartSignal_12"));
        lblTextStartSignal_12->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_12);

        lblTextStartSignal_13 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_13->setObjectName(QStringLiteral("lblTextStartSignal_13"));
        lblTextStartSignal_13->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_13);

        lblTextStartSignal_14 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_14->setObjectName(QStringLiteral("lblTextStartSignal_14"));
        lblTextStartSignal_14->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_14);

        lblTextStartSignal_15 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_15->setObjectName(QStringLiteral("lblTextStartSignal_15"));
        lblTextStartSignal_15->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_15);

        lblTextStartSignal_16 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_16->setObjectName(QStringLiteral("lblTextStartSignal_16"));
        lblTextStartSignal_16->setScaledContents(true);

        verticalLayout_68->addWidget(lblTextStartSignal_16);


        horizontalLayout_57->addLayout(verticalLayout_68);

        verticalLayout_67 = new QVBoxLayout();
        verticalLayout_67->setSpacing(6);
        verticalLayout_67->setObjectName(QStringLiteral("verticalLayout_67"));
        cboxOnBRU_2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOnBRU_2->addItem(QString());
        cboxOnBRU_2->addItem(QString());
        cboxOnBRU_2->setObjectName(QStringLiteral("cboxOnBRU_2"));

        verticalLayout_67->addWidget(cboxOnBRU_2);

        cboxOffMTZ1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffMTZ1->addItem(QString());
        cboxOffMTZ1->addItem(QString());
        cboxOffMTZ1->setObjectName(QStringLiteral("cboxOffMTZ1"));

        verticalLayout_67->addWidget(cboxOffMTZ1);

        cboxOffMTZ2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffMTZ2->addItem(QString());
        cboxOffMTZ2->addItem(QString());
        cboxOffMTZ2->setObjectName(QStringLiteral("cboxOffMTZ2"));

        verticalLayout_67->addWidget(cboxOffMTZ2);

        cboxOffMTZ3 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffMTZ3->addItem(QString());
        cboxOffMTZ3->addItem(QString());
        cboxOffMTZ3->setObjectName(QStringLiteral("cboxOffMTZ3"));

        verticalLayout_67->addWidget(cboxOffMTZ3);

        cboxOffMTZ4 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffMTZ4->addItem(QString());
        cboxOffMTZ4->addItem(QString());
        cboxOffMTZ4->setObjectName(QStringLiteral("cboxOffMTZ4"));

        verticalLayout_67->addWidget(cboxOffMTZ4);

        cboxOffStart = new QComboBox(pageProtectionLevelSignStart);
        cboxOffStart->addItem(QString());
        cboxOffStart->addItem(QString());
        cboxOffStart->setObjectName(QStringLiteral("cboxOffStart"));

        verticalLayout_67->addWidget(cboxOffStart);

        cboxOffOZZ1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffOZZ1->addItem(QString());
        cboxOffOZZ1->addItem(QString());
        cboxOffOZZ1->setObjectName(QStringLiteral("cboxOffOZZ1"));

        verticalLayout_67->addWidget(cboxOffOZZ1);

        cboxOffNZZ1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffNZZ1->addItem(QString());
        cboxOffNZZ1->addItem(QString());
        cboxOffNZZ1->setObjectName(QStringLiteral("cboxOffNZZ1"));

        verticalLayout_67->addWidget(cboxOffNZZ1);


        horizontalLayout_57->addLayout(verticalLayout_67);


        horizontalLayout_4->addLayout(horizontalLayout_57);

        line_2 = new QFrame(pageProtectionLevelSignStart);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_2);

        horizontalLayout_58 = new QHBoxLayout();
        horizontalLayout_58->setSpacing(6);
        horizontalLayout_58->setObjectName(QStringLiteral("horizontalLayout_58"));
        verticalLayout_70 = new QVBoxLayout();
        verticalLayout_70->setSpacing(6);
        verticalLayout_70->setObjectName(QStringLiteral("verticalLayout_70"));
        lblTextStartSignal_17 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_17->setObjectName(QStringLiteral("lblTextStartSignal_17"));
        lblTextStartSignal_17->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_17);

        lblTextStartSignal_18 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_18->setObjectName(QStringLiteral("lblTextStartSignal_18"));
        lblTextStartSignal_18->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_18);

        lblTextStartSignal_19 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_19->setObjectName(QStringLiteral("lblTextStartSignal_19"));
        lblTextStartSignal_19->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_19);

        lblTextStartSignal_20 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_20->setObjectName(QStringLiteral("lblTextStartSignal_20"));
        lblTextStartSignal_20->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_20);

        lblTextStartSignal_21 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_21->setObjectName(QStringLiteral("lblTextStartSignal_21"));
        lblTextStartSignal_21->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_21);

        lblTextStartSignal_22 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_22->setObjectName(QStringLiteral("lblTextStartSignal_22"));
        lblTextStartSignal_22->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_22);

        lblTextStartSignal_23 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_23->setObjectName(QStringLiteral("lblTextStartSignal_23"));
        lblTextStartSignal_23->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_23);

        lblTextStartSignal_24 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_24->setObjectName(QStringLiteral("lblTextStartSignal_24"));
        lblTextStartSignal_24->setScaledContents(true);

        verticalLayout_70->addWidget(lblTextStartSignal_24);


        horizontalLayout_58->addLayout(verticalLayout_70);

        verticalLayout_69 = new QVBoxLayout();
        verticalLayout_69->setSpacing(6);
        verticalLayout_69->setObjectName(QStringLiteral("verticalLayout_69"));
        cboxOffImin = new QComboBox(pageProtectionLevelSignStart);
        cboxOffImin->addItem(QString());
        cboxOffImin->addItem(QString());
        cboxOffImin->setObjectName(QStringLiteral("cboxOffImin"));

        verticalLayout_69->addWidget(cboxOffImin);

        cboxOffImax1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffImax1->addItem(QString());
        cboxOffImax1->addItem(QString());
        cboxOffImax1->setObjectName(QStringLiteral("cboxOffImax1"));

        verticalLayout_69->addWidget(cboxOffImax1);

        cboxOffImax2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffImax2->addItem(QString());
        cboxOffImax2->addItem(QString());
        cboxOffImax2->setObjectName(QStringLiteral("cboxOffImax2"));

        verticalLayout_69->addWidget(cboxOffImax2);

        cboxOffImin1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffImin1->addItem(QString());
        cboxOffImin1->addItem(QString());
        cboxOffImin1->setObjectName(QStringLiteral("cboxOffImin1"));

        verticalLayout_69->addWidget(cboxOffImin1);

        cboxOffImin2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffImin2->addItem(QString());
        cboxOffImin2->addItem(QString());
        cboxOffImin2->setObjectName(QStringLiteral("cboxOffImin2"));

        verticalLayout_69->addWidget(cboxOffImin2);

        cboxOffI3UO = new QComboBox(pageProtectionLevelSignStart);
        cboxOffI3UO->addItem(QString());
        cboxOffI3UO->addItem(QString());
        cboxOffI3UO->setObjectName(QStringLiteral("cboxOffI3UO"));

        verticalLayout_69->addWidget(cboxOffI3UO);

        cboxOffAVR = new QComboBox(pageProtectionLevelSignStart);
        cboxOffAVR->addItem(QString());
        cboxOffAVR->addItem(QString());
        cboxOffAVR->setObjectName(QStringLiteral("cboxOffAVR"));

        verticalLayout_69->addWidget(cboxOffAVR);

        cboxOffACR1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffACR1->addItem(QString());
        cboxOffACR1->addItem(QString());
        cboxOffACR1->setObjectName(QStringLiteral("cboxOffACR1"));

        verticalLayout_69->addWidget(cboxOffACR1);


        horizontalLayout_58->addLayout(verticalLayout_69);


        horizontalLayout_4->addLayout(horizontalLayout_58);

        line_3 = new QFrame(pageProtectionLevelSignStart);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_3);

        horizontalLayout_59 = new QHBoxLayout();
        horizontalLayout_59->setSpacing(6);
        horizontalLayout_59->setObjectName(QStringLiteral("horizontalLayout_59"));
        verticalLayout_71 = new QVBoxLayout();
        verticalLayout_71->setSpacing(6);
        verticalLayout_71->setObjectName(QStringLiteral("verticalLayout_71"));
        lblTextStartSignal_25 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_25->setObjectName(QStringLiteral("lblTextStartSignal_25"));
        lblTextStartSignal_25->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_25);

        lblTextStartSignal_26 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_26->setObjectName(QStringLiteral("lblTextStartSignal_26"));
        lblTextStartSignal_26->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_26);

        lblTextStartSignal_27 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_27->setObjectName(QStringLiteral("lblTextStartSignal_27"));
        lblTextStartSignal_27->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_27);

        lblTextStartSignal_28 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_28->setObjectName(QStringLiteral("lblTextStartSignal_28"));
        lblTextStartSignal_28->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_28);

        lblTextStartSignal_29 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_29->setObjectName(QStringLiteral("lblTextStartSignal_29"));
        lblTextStartSignal_29->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_29);

        lblTextStartSignal_30 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_30->setObjectName(QStringLiteral("lblTextStartSignal_30"));
        lblTextStartSignal_30->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_30);

        lblTextStartSignal_31 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_31->setObjectName(QStringLiteral("lblTextStartSignal_31"));
        lblTextStartSignal_31->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_31);

        lblTextStartSignal_32 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_32->setObjectName(QStringLiteral("lblTextStartSignal_32"));
        lblTextStartSignal_32->setScaledContents(true);

        verticalLayout_71->addWidget(lblTextStartSignal_32);


        horizontalLayout_59->addLayout(verticalLayout_71);

        verticalLayout_72 = new QVBoxLayout();
        verticalLayout_72->setSpacing(6);
        verticalLayout_72->setObjectName(QStringLiteral("verticalLayout_72"));
        cboxOffACR2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffACR2->addItem(QString());
        cboxOffACR2->addItem(QString());
        cboxOffACR2->setObjectName(QStringLiteral("cboxOffACR2"));

        verticalLayout_72->addWidget(cboxOffACR2);

        cboxOffACR3 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffACR3->addItem(QString());
        cboxOffACR3->addItem(QString());
        cboxOffACR3->setObjectName(QStringLiteral("cboxOffACR3"));

        verticalLayout_72->addWidget(cboxOffACR3);

        cboxOffTemp1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffTemp1->addItem(QString());
        cboxOffTemp1->addItem(QString());
        cboxOffTemp1->setObjectName(QStringLiteral("cboxOffTemp1"));

        verticalLayout_72->addWidget(cboxOffTemp1);

        cboxOffTemp2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffTemp2->addItem(QString());
        cboxOffTemp2->addItem(QString());
        cboxOffTemp2->setObjectName(QStringLiteral("cboxOffTemp2"));

        verticalLayout_72->addWidget(cboxOffTemp2);

        cboxOffExt1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffExt1->addItem(QString());
        cboxOffExt1->addItem(QString());
        cboxOffExt1->setObjectName(QStringLiteral("cboxOffExt1"));

        verticalLayout_72->addWidget(cboxOffExt1);

        cboxOffExt2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffExt2->addItem(QString());
        cboxOffExt2->addItem(QString());
        cboxOffExt2->setObjectName(QStringLiteral("cboxOffExt2"));

        verticalLayout_72->addWidget(cboxOffExt2);

        cboxOffExt3 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffExt3->addItem(QString());
        cboxOffExt3->addItem(QString());
        cboxOffExt3->setObjectName(QStringLiteral("cboxOffExt3"));

        verticalLayout_72->addWidget(cboxOffExt3);

        cboxOffLevel1 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffLevel1->addItem(QString());
        cboxOffLevel1->addItem(QString());
        cboxOffLevel1->setObjectName(QStringLiteral("cboxOffLevel1"));

        verticalLayout_72->addWidget(cboxOffLevel1);


        horizontalLayout_59->addLayout(verticalLayout_72);


        horizontalLayout_4->addLayout(horizontalLayout_59);

        line_4 = new QFrame(pageProtectionLevelSignStart);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        horizontalLayout_4->addWidget(line_4);

        verticalLayout_73 = new QVBoxLayout();
        verticalLayout_73->setSpacing(6);
        verticalLayout_73->setObjectName(QStringLiteral("verticalLayout_73"));
        horizontalLayout_60 = new QHBoxLayout();
        horizontalLayout_60->setSpacing(6);
        horizontalLayout_60->setObjectName(QStringLiteral("horizontalLayout_60"));
        lblTextStartSignal_33 = new QLabel(pageProtectionLevelSignStart);
        lblTextStartSignal_33->setObjectName(QStringLiteral("lblTextStartSignal_33"));
        lblTextStartSignal_33->setScaledContents(true);

        horizontalLayout_60->addWidget(lblTextStartSignal_33);

        cboxOffLevel2 = new QComboBox(pageProtectionLevelSignStart);
        cboxOffLevel2->addItem(QString());
        cboxOffLevel2->addItem(QString());
        cboxOffLevel2->setObjectName(QStringLiteral("cboxOffLevel2"));

        horizontalLayout_60->addWidget(cboxOffLevel2);


        verticalLayout_73->addLayout(horizontalLayout_60);

        verticalSpacer_19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_73->addItem(verticalSpacer_19);


        horizontalLayout_4->addLayout(verticalLayout_73);


        verticalLayout_113->addLayout(horizontalLayout_4);

        verticalSpacer_68 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_113->addItem(verticalSpacer_68);

        stwgtProtectionPropertiesLevel->addWidget(pageProtectionLevelSignStart);

        horizontalLayout_61->addWidget(stwgtProtectionPropertiesLevel);

        horizontalSpacer_48 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_61->addItem(horizontalSpacer_48);


        verticalLayout_114->addLayout(horizontalLayout_61);

        verticalSpacer_69 = new QSpacerItem(20, 95, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_114->addItem(verticalSpacer_69);

        stwgtMain->addWidget(pageProtectionLevel);
        pageProtectBRU = new QWidget();
        pageProtectBRU->setObjectName(QStringLiteral("pageProtectBRU"));
        verticalLayout_112 = new QVBoxLayout(pageProtectBRU);
        verticalLayout_112->setSpacing(6);
        verticalLayout_112->setContentsMargins(11, 11, 11, 11);
        verticalLayout_112->setObjectName(QStringLiteral("verticalLayout_112"));
        horizontalLayout_63 = new QHBoxLayout();
        horizontalLayout_63->setSpacing(6);
        horizontalLayout_63->setObjectName(QStringLiteral("horizontalLayout_63"));
        groupBox_11 = new QGroupBox(pageProtectBRU);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(groupBox_11->sizePolicy().hasHeightForWidth());
        groupBox_11->setSizePolicy(sizePolicy7);
        horizontalLayout_102 = new QHBoxLayout(groupBox_11);
        horizontalLayout_102->setSpacing(6);
        horizontalLayout_102->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_102->setObjectName(QStringLiteral("horizontalLayout_102"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setSpacing(6);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        lblTextProtectionMTZ_Ctrl_5 = new QLabel(groupBox_11);
        lblTextProtectionMTZ_Ctrl_5->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_5"));
        sizePolicy5.setHeightForWidth(lblTextProtectionMTZ_Ctrl_5->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Ctrl_5->setSizePolicy(sizePolicy5);
        lblTextProtectionMTZ_Ctrl_5->setScaledContents(true);

        gridLayout_10->addWidget(lblTextProtectionMTZ_Ctrl_5, 0, 0, 1, 1);

        lblTextProtectionMTZ_Timeout_4 = new QLabel(groupBox_11);
        lblTextProtectionMTZ_Timeout_4->setObjectName(QStringLiteral("lblTextProtectionMTZ_Timeout_4"));
        sizePolicy5.setHeightForWidth(lblTextProtectionMTZ_Timeout_4->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Timeout_4->setSizePolicy(sizePolicy5);
        lblTextProtectionMTZ_Timeout_4->setScaledContents(true);

        gridLayout_10->addWidget(lblTextProtectionMTZ_Timeout_4, 2, 0, 1, 1);

        cboxM93 = new QComboBox(groupBox_11);
        cboxM93->addItem(QString());
        cboxM93->addItem(QString());
        cboxM93->addItem(QString());
        cboxM93->setObjectName(QStringLiteral("cboxM93"));
        cboxM93->setMaximumSize(QSize(100, 100));

        gridLayout_10->addWidget(cboxM93, 0, 1, 1, 1);

        leM96 = new QLineEdit(groupBox_11);
        leM96->setObjectName(QStringLiteral("leM96"));
        sizePolicy5.setHeightForWidth(leM96->sizePolicy().hasHeightForWidth());
        leM96->setSizePolicy(sizePolicy5);
        leM96->setMaximumSize(QSize(100, 100));
        leM96->setMaxLength(10);
        leM96->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(leM96, 2, 1, 1, 1);

        lblTextProtectionMTZ_StartCurrent_5 = new QLabel(groupBox_11);
        lblTextProtectionMTZ_StartCurrent_5->setObjectName(QStringLiteral("lblTextProtectionMTZ_StartCurrent_5"));
        sizePolicy5.setHeightForWidth(lblTextProtectionMTZ_StartCurrent_5->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_StartCurrent_5->setSizePolicy(sizePolicy5);
        lblTextProtectionMTZ_StartCurrent_5->setScaledContents(true);

        gridLayout_10->addWidget(lblTextProtectionMTZ_StartCurrent_5, 1, 0, 1, 1);

        leM97 = new QLineEdit(groupBox_11);
        leM97->setObjectName(QStringLiteral("leM97"));
        sizePolicy5.setHeightForWidth(leM97->sizePolicy().hasHeightForWidth());
        leM97->setSizePolicy(sizePolicy5);
        leM97->setMaximumSize(QSize(100, 100));
        leM97->setMaxLength(10);
        leM97->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(leM97, 3, 1, 1, 1);

        lblTextProtectionMTZ_EnterBlocking_3 = new QLabel(groupBox_11);
        lblTextProtectionMTZ_EnterBlocking_3->setObjectName(QStringLiteral("lblTextProtectionMTZ_EnterBlocking_3"));
        sizePolicy5.setHeightForWidth(lblTextProtectionMTZ_EnterBlocking_3->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_EnterBlocking_3->setSizePolicy(sizePolicy5);
        lblTextProtectionMTZ_EnterBlocking_3->setScaledContents(true);

        gridLayout_10->addWidget(lblTextProtectionMTZ_EnterBlocking_3, 4, 0, 1, 1);

        lblTextProtectionMTZ_EnterBlocking_2 = new QLabel(groupBox_11);
        lblTextProtectionMTZ_EnterBlocking_2->setObjectName(QStringLiteral("lblTextProtectionMTZ_EnterBlocking_2"));
        sizePolicy5.setHeightForWidth(lblTextProtectionMTZ_EnterBlocking_2->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_EnterBlocking_2->setSizePolicy(sizePolicy5);
        lblTextProtectionMTZ_EnterBlocking_2->setScaledContents(true);

        gridLayout_10->addWidget(lblTextProtectionMTZ_EnterBlocking_2, 3, 0, 1, 1);

        cboxM95 = new QComboBox(groupBox_11);
        cboxM95->addItem(QString());
        cboxM95->addItem(QString());
        cboxM95->addItem(QString());
        cboxM95->setObjectName(QStringLiteral("cboxM95"));
        cboxM95->setMaximumSize(QSize(100, 100));

        gridLayout_10->addWidget(cboxM95, 1, 1, 1, 1);

        leM98 = new QLineEdit(groupBox_11);
        leM98->setObjectName(QStringLiteral("leM98"));
        sizePolicy5.setHeightForWidth(leM98->sizePolicy().hasHeightForWidth());
        leM98->setSizePolicy(sizePolicy5);
        leM98->setMaximumSize(QSize(100, 100));
        leM98->setMaxLength(10);
        leM98->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(leM98, 4, 1, 1, 1);

        lblTextProtectionMTZ_EnterBlocking_4 = new QLabel(groupBox_11);
        lblTextProtectionMTZ_EnterBlocking_4->setObjectName(QStringLiteral("lblTextProtectionMTZ_EnterBlocking_4"));
        sizePolicy5.setHeightForWidth(lblTextProtectionMTZ_EnterBlocking_4->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_EnterBlocking_4->setSizePolicy(sizePolicy5);
        lblTextProtectionMTZ_EnterBlocking_4->setScaledContents(true);

        gridLayout_10->addWidget(lblTextProtectionMTZ_EnterBlocking_4, 5, 0, 1, 1);

        leM99 = new QLineEdit(groupBox_11);
        leM99->setObjectName(QStringLiteral("leM99"));
        sizePolicy5.setHeightForWidth(leM99->sizePolicy().hasHeightForWidth());
        leM99->setSizePolicy(sizePolicy5);
        leM99->setMaximumSize(QSize(100, 100));
        leM99->setMaxLength(10);
        leM99->setAlignment(Qt::AlignCenter);

        gridLayout_10->addWidget(leM99, 5, 1, 1, 1);


        horizontalLayout_102->addLayout(gridLayout_10);


        horizontalLayout_63->addWidget(groupBox_11);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_63->addItem(horizontalSpacer_10);


        verticalLayout_112->addLayout(horizontalLayout_63);

        verticalSpacer_59 = new QSpacerItem(20, 67, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_112->addItem(verticalSpacer_59);

        stwgtMain->addWidget(pageProtectBRU);
        pageProtectVacuum = new QWidget();
        pageProtectVacuum->setObjectName(QStringLiteral("pageProtectVacuum"));
        verticalLayout_115 = new QVBoxLayout(pageProtectVacuum);
        verticalLayout_115->setSpacing(6);
        verticalLayout_115->setContentsMargins(11, 11, 11, 11);
        verticalLayout_115->setObjectName(QStringLiteral("verticalLayout_115"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_12 = new QGroupBox(pageProtectVacuum);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        QSizePolicy sizePolicy8(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(groupBox_12->sizePolicy().hasHeightForWidth());
        groupBox_12->setSizePolicy(sizePolicy8);
        horizontalLayout_103 = new QHBoxLayout(groupBox_12);
        horizontalLayout_103->setSpacing(6);
        horizontalLayout_103->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_103->setObjectName(QStringLiteral("horizontalLayout_103"));
        gridLayout_11 = new QGridLayout();
        gridLayout_11->setSpacing(6);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        leM91 = new QLineEdit(groupBox_12);
        leM91->setObjectName(QStringLiteral("leM91"));
        sizePolicy5.setHeightForWidth(leM91->sizePolicy().hasHeightForWidth());
        leM91->setSizePolicy(sizePolicy5);
        leM91->setMaximumSize(QSize(75, 75));
        leM91->setMaxLength(10);
        leM91->setAlignment(Qt::AlignCenter);

        gridLayout_11->addWidget(leM91, 1, 1, 1, 1);

        lblTextProtectionMTZ_Ctrl_8 = new QLabel(groupBox_12);
        lblTextProtectionMTZ_Ctrl_8->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_8"));
        sizePolicy1.setHeightForWidth(lblTextProtectionMTZ_Ctrl_8->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Ctrl_8->setSizePolicy(sizePolicy1);
        lblTextProtectionMTZ_Ctrl_8->setMinimumSize(QSize(50, 0));
        lblTextProtectionMTZ_Ctrl_8->setScaledContents(true);

        gridLayout_11->addWidget(lblTextProtectionMTZ_Ctrl_8, 2, 0, 1, 1);

        cboxM90 = new QComboBox(groupBox_12);
        cboxM90->addItem(QString());
        cboxM90->addItem(QString());
        cboxM90->addItem(QString());
        cboxM90->setObjectName(QStringLiteral("cboxM90"));
        sizePolicy1.setHeightForWidth(cboxM90->sizePolicy().hasHeightForWidth());
        cboxM90->setSizePolicy(sizePolicy1);
        cboxM90->setMinimumSize(QSize(50, 0));
        cboxM90->setMaximumSize(QSize(100, 100));

        gridLayout_11->addWidget(cboxM90, 0, 1, 1, 1);

        lblTextProtectionMTZ_Ctrl_6 = new QLabel(groupBox_12);
        lblTextProtectionMTZ_Ctrl_6->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_6"));
        sizePolicy1.setHeightForWidth(lblTextProtectionMTZ_Ctrl_6->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Ctrl_6->setSizePolicy(sizePolicy1);
        lblTextProtectionMTZ_Ctrl_6->setMinimumSize(QSize(50, 0));
        lblTextProtectionMTZ_Ctrl_6->setScaledContents(true);

        gridLayout_11->addWidget(lblTextProtectionMTZ_Ctrl_6, 0, 0, 1, 1);

        lblTextProtectionMTZ_Ctrl_7 = new QLabel(groupBox_12);
        lblTextProtectionMTZ_Ctrl_7->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_7"));
        sizePolicy1.setHeightForWidth(lblTextProtectionMTZ_Ctrl_7->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Ctrl_7->setSizePolicy(sizePolicy1);
        lblTextProtectionMTZ_Ctrl_7->setMinimumSize(QSize(50, 0));
        lblTextProtectionMTZ_Ctrl_7->setScaledContents(true);

        gridLayout_11->addWidget(lblTextProtectionMTZ_Ctrl_7, 1, 0, 1, 1);

        leM92 = new QLineEdit(groupBox_12);
        leM92->setObjectName(QStringLiteral("leM92"));
        sizePolicy5.setHeightForWidth(leM92->sizePolicy().hasHeightForWidth());
        leM92->setSizePolicy(sizePolicy5);
        leM92->setMaximumSize(QSize(75, 75));
        leM92->setMaxLength(10);
        leM92->setAlignment(Qt::AlignCenter);

        gridLayout_11->addWidget(leM92, 2, 1, 1, 1);

        lblTextProtectionMTZ_Ctrl_9 = new QLabel(groupBox_12);
        lblTextProtectionMTZ_Ctrl_9->setObjectName(QStringLiteral("lblTextProtectionMTZ_Ctrl_9"));
        sizePolicy1.setHeightForWidth(lblTextProtectionMTZ_Ctrl_9->sizePolicy().hasHeightForWidth());
        lblTextProtectionMTZ_Ctrl_9->setSizePolicy(sizePolicy1);
        lblTextProtectionMTZ_Ctrl_9->setMinimumSize(QSize(50, 0));
        lblTextProtectionMTZ_Ctrl_9->setScaledContents(true);

        gridLayout_11->addWidget(lblTextProtectionMTZ_Ctrl_9, 3, 0, 1, 1);

        leX23 = new QLineEdit(groupBox_12);
        leX23->setObjectName(QStringLiteral("leX23"));
        sizePolicy5.setHeightForWidth(leX23->sizePolicy().hasHeightForWidth());
        leX23->setSizePolicy(sizePolicy5);
        leX23->setMaximumSize(QSize(75, 75));
        leX23->setMaxLength(10);
        leX23->setAlignment(Qt::AlignCenter);

        gridLayout_11->addWidget(leX23, 3, 1, 1, 1);


        horizontalLayout_103->addLayout(gridLayout_11);


        verticalLayout_5->addWidget(groupBox_12);

        verticalSpacer_70 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_70);


        horizontalLayout_9->addLayout(verticalLayout_5);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);


        verticalLayout_115->addLayout(horizontalLayout_9);

        verticalSpacer_60 = new QSpacerItem(20, 95, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_115->addItem(verticalSpacer_60);

        stwgtMain->addWidget(pageProtectVacuum);
        pageSwitchDevice = new QWidget();
        pageSwitchDevice->setObjectName(QStringLiteral("pageSwitchDevice"));
        horizontalLayout_55 = new QHBoxLayout(pageSwitchDevice);
        horizontalLayout_55->setSpacing(6);
        horizontalLayout_55->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_55->setObjectName(QStringLiteral("horizontalLayout_55"));
        verticalLayout_75 = new QVBoxLayout();
        verticalLayout_75->setSpacing(0);
        verticalLayout_75->setObjectName(QStringLiteral("verticalLayout_75"));
        pbtnSwDevBreaker = new QPushButton(pageSwitchDevice);
        pbtnSwDevBreaker->setObjectName(QStringLiteral("pbtnSwDevBreaker"));
        sizePolicy5.setHeightForWidth(pbtnSwDevBreaker->sizePolicy().hasHeightForWidth());
        pbtnSwDevBreaker->setSizePolicy(sizePolicy5);
        pbtnSwDevBreaker->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevBreaker);

        pbtnSwDevBlock = new QPushButton(pageSwitchDevice);
        pbtnSwDevBlock->setObjectName(QStringLiteral("pbtnSwDevBlock"));
        sizePolicy5.setHeightForWidth(pbtnSwDevBlock->sizePolicy().hasHeightForWidth());
        pbtnSwDevBlock->setSizePolicy(sizePolicy5);
        pbtnSwDevBlock->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevBlock);

        pbtnSwDevSCHR = new QPushButton(pageSwitchDevice);
        pbtnSwDevSCHR->setObjectName(QStringLiteral("pbtnSwDevSCHR"));
        sizePolicy5.setHeightForWidth(pbtnSwDevSCHR->sizePolicy().hasHeightForWidth());
        pbtnSwDevSCHR->setSizePolicy(sizePolicy5);
        pbtnSwDevSCHR->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevSCHR);

        pbtnSwDevLR = new QPushButton(pageSwitchDevice);
        pbtnSwDevLR->setObjectName(QStringLiteral("pbtnSwDevLR"));
        sizePolicy5.setHeightForWidth(pbtnSwDevLR->sizePolicy().hasHeightForWidth());
        pbtnSwDevLR->setSizePolicy(sizePolicy5);
        pbtnSwDevLR->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevLR);

        pbtnSwDevZR = new QPushButton(pageSwitchDevice);
        pbtnSwDevZR->setObjectName(QStringLiteral("pbtnSwDevZR"));
        sizePolicy5.setHeightForWidth(pbtnSwDevZR->sizePolicy().hasHeightForWidth());
        pbtnSwDevZR->setSizePolicy(sizePolicy5);
        pbtnSwDevZR->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevZR);

        pbtnSwDevTruck = new QPushButton(pageSwitchDevice);
        pbtnSwDevTruck->setObjectName(QStringLiteral("pbtnSwDevTruck"));
        sizePolicy5.setHeightForWidth(pbtnSwDevTruck->sizePolicy().hasHeightForWidth());
        pbtnSwDevTruck->setSizePolicy(sizePolicy5);
        pbtnSwDevTruck->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevTruck);

        pbtnSwDevCtrl = new QPushButton(pageSwitchDevice);
        pbtnSwDevCtrl->setObjectName(QStringLiteral("pbtnSwDevCtrl"));
        sizePolicy5.setHeightForWidth(pbtnSwDevCtrl->sizePolicy().hasHeightForWidth());
        pbtnSwDevCtrl->setSizePolicy(sizePolicy5);
        pbtnSwDevCtrl->setCheckable(false);

        verticalLayout_75->addWidget(pbtnSwDevCtrl);

        verticalSpacer_22 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_75->addItem(verticalSpacer_22);


        horizontalLayout_55->addLayout(verticalLayout_75);

        stwgtSwitchDevice = new QStackedWidget(pageSwitchDevice);
        stwgtSwitchDevice->setObjectName(QStringLiteral("stwgtSwitchDevice"));
        sizePolicy.setHeightForWidth(stwgtSwitchDevice->sizePolicy().hasHeightForWidth());
        stwgtSwitchDevice->setSizePolicy(sizePolicy);
        pageSwDevBreaker = new QWidget();
        pageSwDevBreaker->setObjectName(QStringLiteral("pageSwDevBreaker"));
        verticalLayout_125 = new QVBoxLayout(pageSwDevBreaker);
        verticalLayout_125->setSpacing(6);
        verticalLayout_125->setContentsMargins(11, 11, 11, 11);
        verticalLayout_125->setObjectName(QStringLiteral("verticalLayout_125"));
        groupBox = new QGroupBox(pageSwDevBreaker);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout_76 = new QVBoxLayout(groupBox);
        verticalLayout_76->setSpacing(6);
        verticalLayout_76->setContentsMargins(11, 11, 11, 11);
        verticalLayout_76->setObjectName(QStringLiteral("verticalLayout_76"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        lblTextSwitchON = new QLabel(groupBox);
        lblTextSwitchON->setObjectName(QStringLiteral("lblTextSwitchON"));
        lblTextSwitchON->setScaledContents(true);

        gridLayout_6->addWidget(lblTextSwitchON, 1, 0, 1, 1);

        lblCtrlOT = new QLabel(groupBox);
        lblCtrlOT->setObjectName(QStringLiteral("lblCtrlOT"));
        lblCtrlOT->setScaledContents(true);

        gridLayout_6->addWidget(lblCtrlOT, 1, 3, 1, 1);

        lblT_KCU = new QLabel(groupBox);
        lblT_KCU->setObjectName(QStringLiteral("lblT_KCU"));
        lblT_KCU->setScaledContents(true);

        gridLayout_6->addWidget(lblT_KCU, 3, 3, 1, 1);

        leK09 = new QLineEdit(groupBox);
        leK09->setObjectName(QStringLiteral("leK09"));
        sizePolicy5.setHeightForWidth(leK09->sizePolicy().hasHeightForWidth());
        leK09->setSizePolicy(sizePolicy5);
        leK09->setMaximumSize(QSize(100, 100));
        leK09->setMaxLength(10);
        leK09->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(leK09, 3, 4, 1, 1);

        cboxK06 = new QComboBox(groupBox);
        cboxK06->addItem(QString());
        cboxK06->addItem(QString());
        cboxK06->addItem(QString());
        cboxK06->setObjectName(QStringLiteral("cboxK06"));
        cboxK06->setMaximumSize(QSize(100, 100));

        gridLayout_6->addWidget(cboxK06, 0, 4, 1, 1);

        lblKcu = new QLabel(groupBox);
        lblKcu->setObjectName(QStringLiteral("lblKcu"));
        lblKcu->setScaledContents(true);

        gridLayout_6->addWidget(lblKcu, 0, 3, 1, 1);

        lblPosVV = new QLabel(groupBox);
        lblPosVV->setObjectName(QStringLiteral("lblPosVV"));
        lblPosVV->setScaledContents(true);

        gridLayout_6->addWidget(lblPosVV, 2, 3, 1, 1);

        cboxK32 = new QComboBox(groupBox);
        cboxK32->addItem(QString());
        cboxK32->addItem(QString());
        cboxK32->addItem(QString());
        cboxK32->setObjectName(QStringLiteral("cboxK32"));
        cboxK32->setMaximumSize(QSize(100, 100));

        gridLayout_6->addWidget(cboxK32, 0, 1, 1, 1);

        lblTextSwBreakerModeCtrl = new QLabel(groupBox);
        lblTextSwBreakerModeCtrl->setObjectName(QStringLiteral("lblTextSwBreakerModeCtrl"));
        lblTextSwBreakerModeCtrl->setScaledContents(true);

        gridLayout_6->addWidget(lblTextSwBreakerModeCtrl, 0, 0, 1, 1);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_11, 0, 2, 1, 1);

        cboxK17 = new QComboBox(groupBox);
        cboxK17->addItem(QString());
        cboxK17->addItem(QString());
        cboxK17->addItem(QString());
        cboxK17->setObjectName(QStringLiteral("cboxK17"));
        cboxK17->setMaximumSize(QSize(100, 100));

        gridLayout_6->addWidget(cboxK17, 1, 4, 1, 1);

        lblTextSwitchOFF = new QLabel(groupBox);
        lblTextSwitchOFF->setObjectName(QStringLiteral("lblTextSwitchOFF"));
        lblTextSwitchOFF->setScaledContents(true);

        gridLayout_6->addWidget(lblTextSwitchOFF, 2, 0, 1, 1);

        cboxK07 = new QComboBox(groupBox);
        cboxK07->addItem(QString());
        cboxK07->addItem(QString());
        cboxK07->addItem(QString());
        cboxK07->setObjectName(QStringLiteral("cboxK07"));
        cboxK07->setMaximumSize(QSize(100, 100));

        gridLayout_6->addWidget(cboxK07, 2, 4, 1, 1);

        cboxK01 = new QComboBox(groupBox);
        cboxK01->addItem(QString());
        cboxK01->addItem(QString());
        cboxK01->addItem(QString());
        cboxK01->setObjectName(QStringLiteral("cboxK01"));
        cboxK01->setMaximumSize(QSize(100, 100));

        gridLayout_6->addWidget(cboxK01, 1, 1, 1, 1);

        leK02 = new QLineEdit(groupBox);
        leK02->setObjectName(QStringLiteral("leK02"));
        sizePolicy5.setHeightForWidth(leK02->sizePolicy().hasHeightForWidth());
        leK02->setSizePolicy(sizePolicy5);
        leK02->setMaximumSize(QSize(100, 100));
        leK02->setMaxLength(10);
        leK02->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(leK02, 3, 1, 1, 1);

        leK04 = new QLineEdit(groupBox);
        leK04->setObjectName(QStringLiteral("leK04"));
        sizePolicy5.setHeightForWidth(leK04->sizePolicy().hasHeightForWidth());
        leK04->setSizePolicy(sizePolicy5);
        leK04->setMaximumSize(QSize(100, 100));
        leK04->setMaxLength(10);
        leK04->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(leK04, 4, 1, 1, 1);

        cboxK03 = new QComboBox(groupBox);
        cboxK03->addItem(QString());
        cboxK03->addItem(QString());
        cboxK03->addItem(QString());
        cboxK03->setObjectName(QStringLiteral("cboxK03"));
        cboxK03->setMaximumSize(QSize(100, 100));

        gridLayout_6->addWidget(cboxK03, 2, 1, 1, 1);

        lblTextSwitchON_T = new QLabel(groupBox);
        lblTextSwitchON_T->setObjectName(QStringLiteral("lblTextSwitchON_T"));
        lblTextSwitchON_T->setScaledContents(true);

        gridLayout_6->addWidget(lblTextSwitchON_T, 3, 0, 1, 1);

        lblTextSwitchOFF_T = new QLabel(groupBox);
        lblTextSwitchOFF_T->setObjectName(QStringLiteral("lblTextSwitchOFF_T"));
        lblTextSwitchOFF_T->setScaledContents(true);

        gridLayout_6->addWidget(lblTextSwitchOFF_T, 4, 0, 1, 1);

        lblTotv = new QLabel(groupBox);
        lblTotv->setObjectName(QStringLiteral("lblTotv"));
        lblTotv->setScaledContents(true);

        gridLayout_6->addWidget(lblTotv, 5, 0, 1, 1);

        leK05 = new QLineEdit(groupBox);
        leK05->setObjectName(QStringLiteral("leK05"));
        sizePolicy5.setHeightForWidth(leK05->sizePolicy().hasHeightForWidth());
        leK05->setSizePolicy(sizePolicy5);
        leK05->setMaximumSize(QSize(100, 100));
        leK05->setMaxLength(10);
        leK05->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(leK05, 5, 1, 1, 1);

        lblI_VV_on = new QLabel(groupBox);
        lblI_VV_on->setObjectName(QStringLiteral("lblI_VV_on"));
        lblI_VV_on->setScaledContents(true);

        gridLayout_6->addWidget(lblI_VV_on, 4, 3, 1, 1);

        lblKv_KCU_I = new QLabel(groupBox);
        lblKv_KCU_I->setObjectName(QStringLiteral("lblKv_KCU_I"));
        lblKv_KCU_I->setScaledContents(true);

        gridLayout_6->addWidget(lblKv_KCU_I, 5, 3, 1, 1);

        leX22 = new QLineEdit(groupBox);
        leX22->setObjectName(QStringLiteral("leX22"));
        sizePolicy5.setHeightForWidth(leX22->sizePolicy().hasHeightForWidth());
        leX22->setSizePolicy(sizePolicy5);
        leX22->setMaximumSize(QSize(100, 100));
        leX22->setMaxLength(10);
        leX22->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(leX22, 5, 4, 1, 1);

        leK08 = new QLineEdit(groupBox);
        leK08->setObjectName(QStringLiteral("leK08"));
        sizePolicy5.setHeightForWidth(leK08->sizePolicy().hasHeightForWidth());
        leK08->setSizePolicy(sizePolicy5);
        leK08->setMaximumSize(QSize(100, 100));
        leK08->setMaxLength(10);
        leK08->setAlignment(Qt::AlignCenter);

        gridLayout_6->addWidget(leK08, 4, 4, 1, 1);


        verticalLayout_76->addLayout(gridLayout_6);


        verticalLayout_125->addWidget(groupBox);

        verticalSpacer_4 = new QSpacerItem(20, 51, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_125->addItem(verticalSpacer_4);

        stwgtSwitchDevice->addWidget(pageSwDevBreaker);
        pageSwDevDisconnect = new QWidget();
        pageSwDevDisconnect->setObjectName(QStringLiteral("pageSwDevDisconnect"));
        verticalLayout_8 = new QVBoxLayout(pageSwDevDisconnect);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout_70 = new QHBoxLayout();
        horizontalLayout_70->setSpacing(6);
        horizontalLayout_70->setObjectName(QStringLiteral("horizontalLayout_70"));
        gboxSwDevDisconnect = new QGroupBox(pageSwDevDisconnect);
        gboxSwDevDisconnect->setObjectName(QStringLiteral("gboxSwDevDisconnect"));
        sizePolicy1.setHeightForWidth(gboxSwDevDisconnect->sizePolicy().hasHeightForWidth());
        gboxSwDevDisconnect->setSizePolicy(sizePolicy1);
        gboxSwDevDisconnect->setAlignment(Qt::AlignCenter);
        horizontalLayout_54 = new QHBoxLayout(gboxSwDevDisconnect);
        horizontalLayout_54->setSpacing(6);
        horizontalLayout_54->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_54->setObjectName(QStringLiteral("horizontalLayout_54"));
        verticalLayout_77 = new QVBoxLayout();
        verticalLayout_77->setSpacing(6);
        verticalLayout_77->setObjectName(QStringLiteral("verticalLayout_77"));
        lblTextVk_VV_tel = new QLabel(gboxSwDevDisconnect);
        lblTextVk_VV_tel->setObjectName(QStringLiteral("lblTextVk_VV_tel"));
        sizePolicy6.setHeightForWidth(lblTextVk_VV_tel->sizePolicy().hasHeightForWidth());
        lblTextVk_VV_tel->setSizePolicy(sizePolicy6);
        lblTextVk_VV_tel->setScaledContents(true);

        verticalLayout_77->addWidget(lblTextVk_VV_tel);

        lblTextVk_VV_tel_2 = new QLabel(gboxSwDevDisconnect);
        lblTextVk_VV_tel_2->setObjectName(QStringLiteral("lblTextVk_VV_tel_2"));
        sizePolicy6.setHeightForWidth(lblTextVk_VV_tel_2->sizePolicy().hasHeightForWidth());
        lblTextVk_VV_tel_2->setSizePolicy(sizePolicy6);
        lblTextVk_VV_tel_2->setScaledContents(true);

        verticalLayout_77->addWidget(lblTextVk_VV_tel_2);

        lblTextVk_VV_tel_3 = new QLabel(gboxSwDevDisconnect);
        lblTextVk_VV_tel_3->setObjectName(QStringLiteral("lblTextVk_VV_tel_3"));
        sizePolicy6.setHeightForWidth(lblTextVk_VV_tel_3->sizePolicy().hasHeightForWidth());
        lblTextVk_VV_tel_3->setSizePolicy(sizePolicy6);
        lblTextVk_VV_tel_3->setScaledContents(true);

        verticalLayout_77->addWidget(lblTextVk_VV_tel_3);


        horizontalLayout_54->addLayout(verticalLayout_77);

        vltProtectionMTZ31_28 = new QVBoxLayout();
        vltProtectionMTZ31_28->setSpacing(6);
        vltProtectionMTZ31_28->setObjectName(QStringLiteral("vltProtectionMTZ31_28"));
        vltProtectionMTZ31_28->setSizeConstraint(QLayout::SetFixedSize);
        cboxK13 = new QComboBox(gboxSwDevDisconnect);
        cboxK13->addItem(QString());
        cboxK13->addItem(QString());
        cboxK13->addItem(QString());
        cboxK13->setObjectName(QStringLiteral("cboxK13"));
        sizePolicy5.setHeightForWidth(cboxK13->sizePolicy().hasHeightForWidth());
        cboxK13->setSizePolicy(sizePolicy5);
        cboxK13->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_28->addWidget(cboxK13);

        cboxK14 = new QComboBox(gboxSwDevDisconnect);
        cboxK14->addItem(QString());
        cboxK14->addItem(QString());
        cboxK14->addItem(QString());
        cboxK14->setObjectName(QStringLiteral("cboxK14"));
        sizePolicy5.setHeightForWidth(cboxK14->sizePolicy().hasHeightForWidth());
        cboxK14->setSizePolicy(sizePolicy5);
        cboxK14->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_28->addWidget(cboxK14);

        cboxK15 = new QComboBox(gboxSwDevDisconnect);
        cboxK15->addItem(QString());
        cboxK15->addItem(QString());
        cboxK15->addItem(QString());
        cboxK15->setObjectName(QStringLiteral("cboxK15"));
        sizePolicy5.setHeightForWidth(cboxK15->sizePolicy().hasHeightForWidth());
        cboxK15->setSizePolicy(sizePolicy5);
        cboxK15->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_28->addWidget(cboxK15);


        horizontalLayout_54->addLayout(vltProtectionMTZ31_28);


        horizontalLayout_70->addWidget(gboxSwDevDisconnect);

        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_70->addItem(horizontalSpacer_27);


        verticalLayout_8->addLayout(horizontalLayout_70);

        verticalSpacer_43 = new QSpacerItem(20, 230, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer_43);

        stwgtSwitchDevice->addWidget(pageSwDevDisconnect);
        pageSwDevSCHR = new QWidget();
        pageSwDevSCHR->setObjectName(QStringLiteral("pageSwDevSCHR"));
        verticalLayout_84 = new QVBoxLayout(pageSwDevSCHR);
        verticalLayout_84->setSpacing(6);
        verticalLayout_84->setContentsMargins(11, 11, 11, 11);
        verticalLayout_84->setObjectName(QStringLiteral("verticalLayout_84"));
        horizontalLayout_71 = new QHBoxLayout();
        horizontalLayout_71->setSpacing(6);
        horizontalLayout_71->setObjectName(QStringLiteral("horizontalLayout_71"));
        gboxSwDevSCHR = new QGroupBox(pageSwDevSCHR);
        gboxSwDevSCHR->setObjectName(QStringLiteral("gboxSwDevSCHR"));
        sizePolicy1.setHeightForWidth(gboxSwDevSCHR->sizePolicy().hasHeightForWidth());
        gboxSwDevSCHR->setSizePolicy(sizePolicy1);
        gboxSwDevSCHR->setAlignment(Qt::AlignCenter);
        horizontalLayout_64 = new QHBoxLayout(gboxSwDevSCHR);
        horizontalLayout_64->setSpacing(6);
        horizontalLayout_64->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_64->setObjectName(QStringLiteral("horizontalLayout_64"));
        verticalLayout_78 = new QVBoxLayout();
        verticalLayout_78->setSpacing(6);
        verticalLayout_78->setObjectName(QStringLiteral("verticalLayout_78"));
        lblTextSCHR_Ctrl = new QLabel(gboxSwDevSCHR);
        lblTextSCHR_Ctrl->setObjectName(QStringLiteral("lblTextSCHR_Ctrl"));
        lblTextSCHR_Ctrl->setScaledContents(true);

        verticalLayout_78->addWidget(lblTextSCHR_Ctrl);

        lblTextSCHR_Cmd = new QLabel(gboxSwDevSCHR);
        lblTextSCHR_Cmd->setObjectName(QStringLiteral("lblTextSCHR_Cmd"));
        lblTextSCHR_Cmd->setScaledContents(true);

        verticalLayout_78->addWidget(lblTextSCHR_Cmd);

        lblTextSCHR_Timp = new QLabel(gboxSwDevSCHR);
        lblTextSCHR_Timp->setObjectName(QStringLiteral("lblTextSCHR_Timp"));
        lblTextSCHR_Timp->setScaledContents(true);

        verticalLayout_78->addWidget(lblTextSCHR_Timp);

        lblTextSCHR_Toff = new QLabel(gboxSwDevSCHR);
        lblTextSCHR_Toff->setObjectName(QStringLiteral("lblTextSCHR_Toff"));
        lblTextSCHR_Toff->setScaledContents(true);

        verticalLayout_78->addWidget(lblTextSCHR_Toff);


        horizontalLayout_64->addLayout(verticalLayout_78);

        vltProtectionMTZ31_29 = new QVBoxLayout();
        vltProtectionMTZ31_29->setSpacing(6);
        vltProtectionMTZ31_29->setObjectName(QStringLiteral("vltProtectionMTZ31_29"));
        cboxK34 = new QComboBox(gboxSwDevSCHR);
        cboxK34->addItem(QString());
        cboxK34->addItem(QString());
        cboxK34->addItem(QString());
        cboxK34->setObjectName(QStringLiteral("cboxK34"));
        cboxK34->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_29->addWidget(cboxK34);

        cboxK38 = new QComboBox(gboxSwDevSCHR);
        cboxK38->addItem(QString());
        cboxK38->addItem(QString());
        cboxK38->addItem(QString());
        cboxK38->setObjectName(QStringLiteral("cboxK38"));
        cboxK38->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_29->addWidget(cboxK38);

        leK42 = new QLineEdit(gboxSwDevSCHR);
        leK42->setObjectName(QStringLiteral("leK42"));
        sizePolicy5.setHeightForWidth(leK42->sizePolicy().hasHeightForWidth());
        leK42->setSizePolicy(sizePolicy5);
        leK42->setMaximumSize(QSize(100, 100));
        leK42->setMaxLength(10);
        leK42->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_29->addWidget(leK42);

        leK46 = new QLineEdit(gboxSwDevSCHR);
        leK46->setObjectName(QStringLiteral("leK46"));
        sizePolicy5.setHeightForWidth(leK46->sizePolicy().hasHeightForWidth());
        leK46->setSizePolicy(sizePolicy5);
        leK46->setMaximumSize(QSize(100, 100));
        leK46->setMaxLength(10);
        leK46->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_29->addWidget(leK46);


        horizontalLayout_64->addLayout(vltProtectionMTZ31_29);


        horizontalLayout_71->addWidget(gboxSwDevSCHR);

        horizontalSpacer_28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_71->addItem(horizontalSpacer_28);


        verticalLayout_84->addLayout(horizontalLayout_71);

        verticalSpacer_44 = new QSpacerItem(20, 204, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_84->addItem(verticalSpacer_44);

        stwgtSwitchDevice->addWidget(pageSwDevSCHR);
        pageSwDevLR = new QWidget();
        pageSwDevLR->setObjectName(QStringLiteral("pageSwDevLR"));
        verticalLayout_85 = new QVBoxLayout(pageSwDevLR);
        verticalLayout_85->setSpacing(6);
        verticalLayout_85->setContentsMargins(11, 11, 11, 11);
        verticalLayout_85->setObjectName(QStringLiteral("verticalLayout_85"));
        horizontalLayout_72 = new QHBoxLayout();
        horizontalLayout_72->setSpacing(6);
        horizontalLayout_72->setObjectName(QStringLiteral("horizontalLayout_72"));
        gboxSwDevLR = new QGroupBox(pageSwDevLR);
        gboxSwDevLR->setObjectName(QStringLiteral("gboxSwDevLR"));
        sizePolicy1.setHeightForWidth(gboxSwDevLR->sizePolicy().hasHeightForWidth());
        gboxSwDevLR->setSizePolicy(sizePolicy1);
        gboxSwDevLR->setAlignment(Qt::AlignCenter);
        horizontalLayout_65 = new QHBoxLayout(gboxSwDevLR);
        horizontalLayout_65->setSpacing(6);
        horizontalLayout_65->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_65->setObjectName(QStringLiteral("horizontalLayout_65"));
        verticalLayout_79 = new QVBoxLayout();
        verticalLayout_79->setSpacing(6);
        verticalLayout_79->setObjectName(QStringLiteral("verticalLayout_79"));
        lblTextLR_Ctrl = new QLabel(gboxSwDevLR);
        lblTextLR_Ctrl->setObjectName(QStringLiteral("lblTextLR_Ctrl"));
        lblTextLR_Ctrl->setScaledContents(true);

        verticalLayout_79->addWidget(lblTextLR_Ctrl);

        lblTextLR_Cmd = new QLabel(gboxSwDevLR);
        lblTextLR_Cmd->setObjectName(QStringLiteral("lblTextLR_Cmd"));
        lblTextLR_Cmd->setScaledContents(true);

        verticalLayout_79->addWidget(lblTextLR_Cmd);

        lblTextLR_Timp = new QLabel(gboxSwDevLR);
        lblTextLR_Timp->setObjectName(QStringLiteral("lblTextLR_Timp"));
        lblTextLR_Timp->setScaledContents(true);

        verticalLayout_79->addWidget(lblTextLR_Timp);

        lblTextLR_Toff = new QLabel(gboxSwDevLR);
        lblTextLR_Toff->setObjectName(QStringLiteral("lblTextLR_Toff"));
        lblTextLR_Toff->setScaledContents(true);

        verticalLayout_79->addWidget(lblTextLR_Toff);


        horizontalLayout_65->addLayout(verticalLayout_79);

        vltProtectionMTZ31_30 = new QVBoxLayout();
        vltProtectionMTZ31_30->setSpacing(6);
        vltProtectionMTZ31_30->setObjectName(QStringLiteral("vltProtectionMTZ31_30"));
        cboxK35 = new QComboBox(gboxSwDevLR);
        cboxK35->addItem(QString());
        cboxK35->addItem(QString());
        cboxK35->addItem(QString());
        cboxK35->setObjectName(QStringLiteral("cboxK35"));
        cboxK35->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_30->addWidget(cboxK35);

        cboxK39 = new QComboBox(gboxSwDevLR);
        cboxK39->addItem(QString());
        cboxK39->addItem(QString());
        cboxK39->addItem(QString());
        cboxK39->setObjectName(QStringLiteral("cboxK39"));
        cboxK39->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_30->addWidget(cboxK39);

        leK43 = new QLineEdit(gboxSwDevLR);
        leK43->setObjectName(QStringLiteral("leK43"));
        sizePolicy5.setHeightForWidth(leK43->sizePolicy().hasHeightForWidth());
        leK43->setSizePolicy(sizePolicy5);
        leK43->setMaximumSize(QSize(100, 100));
        leK43->setMaxLength(10);
        leK43->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_30->addWidget(leK43);

        leK47 = new QLineEdit(gboxSwDevLR);
        leK47->setObjectName(QStringLiteral("leK47"));
        sizePolicy5.setHeightForWidth(leK47->sizePolicy().hasHeightForWidth());
        leK47->setSizePolicy(sizePolicy5);
        leK47->setMaximumSize(QSize(100, 100));
        leK47->setMaxLength(10);
        leK47->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_30->addWidget(leK47);


        horizontalLayout_65->addLayout(vltProtectionMTZ31_30);


        horizontalLayout_72->addWidget(gboxSwDevLR);

        horizontalSpacer_30 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_72->addItem(horizontalSpacer_30);


        verticalLayout_85->addLayout(horizontalLayout_72);

        verticalSpacer_45 = new QSpacerItem(20, 204, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_85->addItem(verticalSpacer_45);

        stwgtSwitchDevice->addWidget(pageSwDevLR);
        pageSwDevZR = new QWidget();
        pageSwDevZR->setObjectName(QStringLiteral("pageSwDevZR"));
        verticalLayout_9 = new QVBoxLayout(pageSwDevZR);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_73 = new QHBoxLayout();
        horizontalLayout_73->setSpacing(6);
        horizontalLayout_73->setObjectName(QStringLiteral("horizontalLayout_73"));
        gboxSwDevZR = new QGroupBox(pageSwDevZR);
        gboxSwDevZR->setObjectName(QStringLiteral("gboxSwDevZR"));
        sizePolicy1.setHeightForWidth(gboxSwDevZR->sizePolicy().hasHeightForWidth());
        gboxSwDevZR->setSizePolicy(sizePolicy1);
        gboxSwDevZR->setAlignment(Qt::AlignCenter);
        horizontalLayout_66 = new QHBoxLayout(gboxSwDevZR);
        horizontalLayout_66->setSpacing(6);
        horizontalLayout_66->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_66->setObjectName(QStringLiteral("horizontalLayout_66"));
        verticalLayout_80 = new QVBoxLayout();
        verticalLayout_80->setSpacing(6);
        verticalLayout_80->setObjectName(QStringLiteral("verticalLayout_80"));
        lblTextZR_Ctrl = new QLabel(gboxSwDevZR);
        lblTextZR_Ctrl->setObjectName(QStringLiteral("lblTextZR_Ctrl"));
        lblTextZR_Ctrl->setScaledContents(true);

        verticalLayout_80->addWidget(lblTextZR_Ctrl);

        lblTextZR_Cmd = new QLabel(gboxSwDevZR);
        lblTextZR_Cmd->setObjectName(QStringLiteral("lblTextZR_Cmd"));
        lblTextZR_Cmd->setScaledContents(true);

        verticalLayout_80->addWidget(lblTextZR_Cmd);

        lblTextZR_Timp = new QLabel(gboxSwDevZR);
        lblTextZR_Timp->setObjectName(QStringLiteral("lblTextZR_Timp"));
        lblTextZR_Timp->setScaledContents(true);

        verticalLayout_80->addWidget(lblTextZR_Timp);

        lblTextZR_Toff = new QLabel(gboxSwDevZR);
        lblTextZR_Toff->setObjectName(QStringLiteral("lblTextZR_Toff"));
        lblTextZR_Toff->setScaledContents(true);

        verticalLayout_80->addWidget(lblTextZR_Toff);


        horizontalLayout_66->addLayout(verticalLayout_80);

        vltProtectionMTZ31_31 = new QVBoxLayout();
        vltProtectionMTZ31_31->setSpacing(6);
        vltProtectionMTZ31_31->setObjectName(QStringLiteral("vltProtectionMTZ31_31"));
        cboxK36 = new QComboBox(gboxSwDevZR);
        cboxK36->addItem(QString());
        cboxK36->addItem(QString());
        cboxK36->addItem(QString());
        cboxK36->setObjectName(QStringLiteral("cboxK36"));
        cboxK36->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_31->addWidget(cboxK36);

        cboxK40 = new QComboBox(gboxSwDevZR);
        cboxK40->addItem(QString());
        cboxK40->addItem(QString());
        cboxK40->addItem(QString());
        cboxK40->setObjectName(QStringLiteral("cboxK40"));
        cboxK40->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_31->addWidget(cboxK40);

        leK44 = new QLineEdit(gboxSwDevZR);
        leK44->setObjectName(QStringLiteral("leK44"));
        sizePolicy5.setHeightForWidth(leK44->sizePolicy().hasHeightForWidth());
        leK44->setSizePolicy(sizePolicy5);
        leK44->setMaximumSize(QSize(100, 100));
        leK44->setMaxLength(10);
        leK44->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_31->addWidget(leK44);

        leK48 = new QLineEdit(gboxSwDevZR);
        leK48->setObjectName(QStringLiteral("leK48"));
        sizePolicy5.setHeightForWidth(leK48->sizePolicy().hasHeightForWidth());
        leK48->setSizePolicy(sizePolicy5);
        leK48->setMaximumSize(QSize(100, 100));
        leK48->setMaxLength(10);
        leK48->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_31->addWidget(leK48);


        horizontalLayout_66->addLayout(vltProtectionMTZ31_31);


        horizontalLayout_73->addWidget(gboxSwDevZR);

        horizontalSpacer_31 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_73->addItem(horizontalSpacer_31);


        verticalLayout_9->addLayout(horizontalLayout_73);

        verticalSpacer_46 = new QSpacerItem(20, 204, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_46);

        stwgtSwitchDevice->addWidget(pageSwDevZR);
        pageSwDevTruck = new QWidget();
        pageSwDevTruck->setObjectName(QStringLiteral("pageSwDevTruck"));
        verticalLayout_86 = new QVBoxLayout(pageSwDevTruck);
        verticalLayout_86->setSpacing(6);
        verticalLayout_86->setContentsMargins(11, 11, 11, 11);
        verticalLayout_86->setObjectName(QStringLiteral("verticalLayout_86"));
        horizontalLayout_74 = new QHBoxLayout();
        horizontalLayout_74->setSpacing(6);
        horizontalLayout_74->setObjectName(QStringLiteral("horizontalLayout_74"));
        gboxSwDevTruck = new QGroupBox(pageSwDevTruck);
        gboxSwDevTruck->setObjectName(QStringLiteral("gboxSwDevTruck"));
        sizePolicy1.setHeightForWidth(gboxSwDevTruck->sizePolicy().hasHeightForWidth());
        gboxSwDevTruck->setSizePolicy(sizePolicy1);
        gboxSwDevTruck->setAlignment(Qt::AlignCenter);
        horizontalLayout_67 = new QHBoxLayout(gboxSwDevTruck);
        horizontalLayout_67->setSpacing(6);
        horizontalLayout_67->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_67->setObjectName(QStringLiteral("horizontalLayout_67"));
        verticalLayout_81 = new QVBoxLayout();
        verticalLayout_81->setSpacing(6);
        verticalLayout_81->setObjectName(QStringLiteral("verticalLayout_81"));
        lblTextTruck_Ctrl = new QLabel(gboxSwDevTruck);
        lblTextTruck_Ctrl->setObjectName(QStringLiteral("lblTextTruck_Ctrl"));
        lblTextTruck_Ctrl->setScaledContents(true);

        verticalLayout_81->addWidget(lblTextTruck_Ctrl);

        lblTextTruck_Cmd = new QLabel(gboxSwDevTruck);
        lblTextTruck_Cmd->setObjectName(QStringLiteral("lblTextTruck_Cmd"));
        lblTextTruck_Cmd->setScaledContents(true);

        verticalLayout_81->addWidget(lblTextTruck_Cmd);

        lblTextTruck_Timp = new QLabel(gboxSwDevTruck);
        lblTextTruck_Timp->setObjectName(QStringLiteral("lblTextTruck_Timp"));
        lblTextTruck_Timp->setScaledContents(true);

        verticalLayout_81->addWidget(lblTextTruck_Timp);

        lblTextTruck_Toff = new QLabel(gboxSwDevTruck);
        lblTextTruck_Toff->setObjectName(QStringLiteral("lblTextTruck_Toff"));
        lblTextTruck_Toff->setScaledContents(true);

        verticalLayout_81->addWidget(lblTextTruck_Toff);


        horizontalLayout_67->addLayout(verticalLayout_81);

        vltProtectionMTZ31_32 = new QVBoxLayout();
        vltProtectionMTZ31_32->setSpacing(6);
        vltProtectionMTZ31_32->setObjectName(QStringLiteral("vltProtectionMTZ31_32"));
        cboxK37 = new QComboBox(gboxSwDevTruck);
        cboxK37->addItem(QString());
        cboxK37->addItem(QString());
        cboxK37->addItem(QString());
        cboxK37->setObjectName(QStringLiteral("cboxK37"));
        cboxK37->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_32->addWidget(cboxK37);

        cboxK41 = new QComboBox(gboxSwDevTruck);
        cboxK41->addItem(QString());
        cboxK41->addItem(QString());
        cboxK41->addItem(QString());
        cboxK41->setObjectName(QStringLiteral("cboxK41"));
        cboxK41->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_32->addWidget(cboxK41);

        leK45 = new QLineEdit(gboxSwDevTruck);
        leK45->setObjectName(QStringLiteral("leK45"));
        sizePolicy5.setHeightForWidth(leK45->sizePolicy().hasHeightForWidth());
        leK45->setSizePolicy(sizePolicy5);
        leK45->setMaximumSize(QSize(100, 100));
        leK45->setMaxLength(10);
        leK45->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_32->addWidget(leK45);

        leK49 = new QLineEdit(gboxSwDevTruck);
        leK49->setObjectName(QStringLiteral("leK49"));
        sizePolicy5.setHeightForWidth(leK49->sizePolicy().hasHeightForWidth());
        leK49->setSizePolicy(sizePolicy5);
        leK49->setMaximumSize(QSize(100, 100));
        leK49->setMaxLength(10);
        leK49->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_32->addWidget(leK49);


        horizontalLayout_67->addLayout(vltProtectionMTZ31_32);


        horizontalLayout_74->addWidget(gboxSwDevTruck);

        horizontalSpacer_32 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_74->addItem(horizontalSpacer_32);


        verticalLayout_86->addLayout(horizontalLayout_74);

        verticalSpacer_47 = new QSpacerItem(20, 204, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_86->addItem(verticalSpacer_47);

        stwgtSwitchDevice->addWidget(pageSwDevTruck);
        pageSwDev_TN_Ctrl = new QWidget();
        pageSwDev_TN_Ctrl->setObjectName(QStringLiteral("pageSwDev_TN_Ctrl"));
        verticalLayout_87 = new QVBoxLayout(pageSwDev_TN_Ctrl);
        verticalLayout_87->setSpacing(6);
        verticalLayout_87->setContentsMargins(11, 11, 11, 11);
        verticalLayout_87->setObjectName(QStringLiteral("verticalLayout_87"));
        horizontalLayout_50 = new QHBoxLayout();
        horizontalLayout_50->setSpacing(6);
        horizontalLayout_50->setObjectName(QStringLiteral("horizontalLayout_50"));
        verticalLayout_83 = new QVBoxLayout();
        verticalLayout_83->setSpacing(6);
        verticalLayout_83->setObjectName(QStringLiteral("verticalLayout_83"));
        gboxSwDevCtrl = new QGroupBox(pageSwDev_TN_Ctrl);
        gboxSwDevCtrl->setObjectName(QStringLiteral("gboxSwDevCtrl"));
        sizePolicy1.setHeightForWidth(gboxSwDevCtrl->sizePolicy().hasHeightForWidth());
        gboxSwDevCtrl->setSizePolicy(sizePolicy1);
        gboxSwDevCtrl->setAlignment(Qt::AlignCenter);
        horizontalLayout_68 = new QHBoxLayout(gboxSwDevCtrl);
        horizontalLayout_68->setSpacing(6);
        horizontalLayout_68->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_68->setObjectName(QStringLiteral("horizontalLayout_68"));
        verticalLayout_82 = new QVBoxLayout();
        verticalLayout_82->setSpacing(6);
        verticalLayout_82->setObjectName(QStringLiteral("verticalLayout_82"));
        lblTextCtrl_Ctrl = new QLabel(gboxSwDevCtrl);
        lblTextCtrl_Ctrl->setObjectName(QStringLiteral("lblTextCtrl_Ctrl"));
        sizePolicy5.setHeightForWidth(lblTextCtrl_Ctrl->sizePolicy().hasHeightForWidth());
        lblTextCtrl_Ctrl->setSizePolicy(sizePolicy5);
        lblTextCtrl_Ctrl->setScaledContents(true);

        verticalLayout_82->addWidget(lblTextCtrl_Ctrl);

        lblTextTruck_Toff_2 = new QLabel(gboxSwDevCtrl);
        lblTextTruck_Toff_2->setObjectName(QStringLiteral("lblTextTruck_Toff_2"));
        lblTextTruck_Toff_2->setScaledContents(true);

        verticalLayout_82->addWidget(lblTextTruck_Toff_2);


        horizontalLayout_68->addLayout(verticalLayout_82);

        vltProtectionMTZ31_33 = new QVBoxLayout();
        vltProtectionMTZ31_33->setSpacing(6);
        vltProtectionMTZ31_33->setObjectName(QStringLiteral("vltProtectionMTZ31_33"));
        vltProtectionMTZ31_33->setSizeConstraint(QLayout::SetMinimumSize);
        cboxK18 = new QComboBox(gboxSwDevCtrl);
        cboxK18->addItem(QString());
        cboxK18->addItem(QString());
        cboxK18->addItem(QString());
        cboxK18->setObjectName(QStringLiteral("cboxK18"));
        cboxK18->setMaximumSize(QSize(100, 100));

        vltProtectionMTZ31_33->addWidget(cboxK18);

        leT02 = new QLineEdit(gboxSwDevCtrl);
        leT02->setObjectName(QStringLiteral("leT02"));
        sizePolicy5.setHeightForWidth(leT02->sizePolicy().hasHeightForWidth());
        leT02->setSizePolicy(sizePolicy5);
        leT02->setMaximumSize(QSize(100, 100));
        leT02->setMaxLength(10);
        leT02->setAlignment(Qt::AlignCenter);

        vltProtectionMTZ31_33->addWidget(leT02);


        horizontalLayout_68->addLayout(vltProtectionMTZ31_33);


        verticalLayout_83->addWidget(gboxSwDevCtrl);

        verticalSpacer_72 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_83->addItem(verticalSpacer_72);


        horizontalLayout_50->addLayout(verticalLayout_83);

        horizontalSpacer_33 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_50->addItem(horizontalSpacer_33);


        verticalLayout_87->addLayout(horizontalLayout_50);

        verticalSpacer_48 = new QSpacerItem(20, 184, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_87->addItem(verticalSpacer_48);

        stwgtSwitchDevice->addWidget(pageSwDev_TN_Ctrl);

        horizontalLayout_55->addWidget(stwgtSwitchDevice);

        horizontalSpacer_3 = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_55->addItem(horizontalSpacer_3);

        stwgtMain->addWidget(pageSwitchDevice);
        pageAutomation = new QWidget();
        pageAutomation->setObjectName(QStringLiteral("pageAutomation"));
        horizontalLayout_75 = new QHBoxLayout(pageAutomation);
        horizontalLayout_75->setSpacing(6);
        horizontalLayout_75->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_75->setObjectName(QStringLiteral("horizontalLayout_75"));
        verticalLayout_108 = new QVBoxLayout();
        verticalLayout_108->setSpacing(0);
        verticalLayout_108->setObjectName(QStringLiteral("verticalLayout_108"));
        pbtnAddAVR = new QPushButton(pageAutomation);
        pbtnAddAVR->setObjectName(QStringLiteral("pbtnAddAVR"));
        sizePolicy5.setHeightForWidth(pbtnAddAVR->sizePolicy().hasHeightForWidth());
        pbtnAddAVR->setSizePolicy(sizePolicy5);
        pbtnAddAVR->setCheckable(false);

        verticalLayout_108->addWidget(pbtnAddAVR);

        pbtnAddAVRSection = new QPushButton(pageAutomation);
        pbtnAddAVRSection->setObjectName(QStringLiteral("pbtnAddAVRSection"));
        sizePolicy5.setHeightForWidth(pbtnAddAVRSection->sizePolicy().hasHeightForWidth());
        pbtnAddAVRSection->setSizePolicy(sizePolicy5);
        pbtnAddAVRSection->setCheckable(false);

        verticalLayout_108->addWidget(pbtnAddAVRSection);

        pbtnAddAPV = new QPushButton(pageAutomation);
        pbtnAddAPV->setObjectName(QStringLiteral("pbtnAddAPV"));
        sizePolicy5.setHeightForWidth(pbtnAddAPV->sizePolicy().hasHeightForWidth());
        pbtnAddAPV->setSizePolicy(sizePolicy5);
        pbtnAddAPV->setCheckable(false);

        verticalLayout_108->addWidget(pbtnAddAPV);

        pbtnAddAPV_Start = new QPushButton(pageAutomation);
        pbtnAddAPV_Start->setObjectName(QStringLiteral("pbtnAddAPV_Start"));
        sizePolicy5.setHeightForWidth(pbtnAddAPV_Start->sizePolicy().hasHeightForWidth());
        pbtnAddAPV_Start->setSizePolicy(sizePolicy5);
        pbtnAddAPV_Start->setCheckable(false);

        verticalLayout_108->addWidget(pbtnAddAPV_Start);

        verticalSpacer_55 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_108->addItem(verticalSpacer_55);


        horizontalLayout_75->addLayout(verticalLayout_108);

        stwgtAdditional = new QStackedWidget(pageAutomation);
        stwgtAdditional->setObjectName(QStringLiteral("stwgtAdditional"));
        sizePolicy.setHeightForWidth(stwgtAdditional->sizePolicy().hasHeightForWidth());
        stwgtAdditional->setSizePolicy(sizePolicy);
        pageAdditionalPropertiesAVRInput = new QWidget();
        pageAdditionalPropertiesAVRInput->setObjectName(QStringLiteral("pageAdditionalPropertiesAVRInput"));
        verticalLayout_109 = new QVBoxLayout(pageAdditionalPropertiesAVRInput);
        verticalLayout_109->setSpacing(6);
        verticalLayout_109->setContentsMargins(11, 11, 11, 11);
        verticalLayout_109->setObjectName(QStringLiteral("verticalLayout_109"));
        horizontalLayout_104 = new QHBoxLayout();
        horizontalLayout_104->setSpacing(6);
        horizontalLayout_104->setObjectName(QStringLiteral("horizontalLayout_104"));
        horizontalLayout_97 = new QHBoxLayout();
        horizontalLayout_97->setSpacing(6);
        horizontalLayout_97->setObjectName(QStringLiteral("horizontalLayout_97"));
        groupBox_2 = new QGroupBox(pageAdditionalPropertiesAVRInput);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        verticalLayout_111 = new QVBoxLayout(groupBox_2);
        verticalLayout_111->setSpacing(6);
        verticalLayout_111->setContentsMargins(11, 11, 11, 11);
        verticalLayout_111->setObjectName(QStringLiteral("verticalLayout_111"));
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setSpacing(6);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy5.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy5);
        label_4->setScaledContents(true);

        gridLayout_12->addWidget(label_4, 3, 0, 1, 1);

        leM84 = new QLineEdit(groupBox_2);
        leM84->setObjectName(QStringLiteral("leM84"));
        sizePolicy5.setHeightForWidth(leM84->sizePolicy().hasHeightForWidth());
        leM84->setSizePolicy(sizePolicy5);
        leM84->setMaxLength(10);
        leM84->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(leM84, 3, 1, 1, 1);

        leM82 = new QLineEdit(groupBox_2);
        leM82->setObjectName(QStringLiteral("leM82"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(leM82->sizePolicy().hasHeightForWidth());
        leM82->setSizePolicy(sizePolicy9);
        leM82->setMaxLength(10);
        leM82->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(leM82, 1, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy5.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy5);
        label->setScaledContents(true);

        gridLayout_12->addWidget(label, 0, 0, 1, 1);

        cboxM81 = new QComboBox(groupBox_2);
        cboxM81->addItem(QString());
        cboxM81->addItem(QString());
        cboxM81->addItem(QString());
        cboxM81->setObjectName(QStringLiteral("cboxM81"));
        sizePolicy9.setHeightForWidth(cboxM81->sizePolicy().hasHeightForWidth());
        cboxM81->setSizePolicy(sizePolicy9);

        gridLayout_12->addWidget(cboxM81, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy5.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy5);
        label_2->setScaledContents(true);

        gridLayout_12->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy5.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy5);
        label_3->setScaledContents(true);

        gridLayout_12->addWidget(label_3, 2, 0, 1, 1);

        leM83 = new QLineEdit(groupBox_2);
        leM83->setObjectName(QStringLiteral("leM83"));
        sizePolicy5.setHeightForWidth(leM83->sizePolicy().hasHeightForWidth());
        leM83->setSizePolicy(sizePolicy5);
        leM83->setMaxLength(10);
        leM83->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(leM83, 2, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy5.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy5);
        label_5->setScaledContents(true);

        gridLayout_12->addWidget(label_5, 4, 0, 1, 1);

        leM85 = new QLineEdit(groupBox_2);
        leM85->setObjectName(QStringLiteral("leM85"));
        sizePolicy5.setHeightForWidth(leM85->sizePolicy().hasHeightForWidth());
        leM85->setSizePolicy(sizePolicy5);
        leM85->setMaxLength(10);
        leM85->setAlignment(Qt::AlignCenter);

        gridLayout_12->addWidget(leM85, 4, 1, 1, 1);


        verticalLayout_111->addLayout(gridLayout_12);


        horizontalLayout_97->addWidget(groupBox_2);

        horizontalSpacer_62 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_97->addItem(horizontalSpacer_62);


        horizontalLayout_104->addLayout(horizontalLayout_97);

        horizontalSpacer_71 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_104->addItem(horizontalSpacer_71);


        verticalLayout_109->addLayout(horizontalLayout_104);

        verticalSpacer_56 = new QSpacerItem(20, 151, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_109->addItem(verticalSpacer_56);

        stwgtAdditional->addWidget(pageAdditionalPropertiesAVRInput);
        pageAdditionalAVRSection = new QWidget();
        pageAdditionalAVRSection->setObjectName(QStringLiteral("pageAdditionalAVRSection"));
        verticalLayout_117 = new QVBoxLayout(pageAdditionalAVRSection);
        verticalLayout_117->setSpacing(6);
        verticalLayout_117->setContentsMargins(11, 11, 11, 11);
        verticalLayout_117->setObjectName(QStringLiteral("verticalLayout_117"));
        horizontalLayout_98 = new QHBoxLayout();
        horizontalLayout_98->setSpacing(6);
        horizontalLayout_98->setObjectName(QStringLiteral("horizontalLayout_98"));
        gridLayout_14 = new QGridLayout();
        gridLayout_14->setSpacing(6);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        label_12 = new QLabel(pageAdditionalAVRSection);
        label_12->setObjectName(QStringLiteral("label_12"));
        sizePolicy5.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy5);
        label_12->setScaledContents(true);

        gridLayout_14->addWidget(label_12, 0, 0, 1, 1);

        cboxK33 = new QComboBox(pageAdditionalAVRSection);
        cboxK33->addItem(QString());
        cboxK33->addItem(QString());
        cboxK33->addItem(QString());
        cboxK33->setObjectName(QStringLiteral("cboxK33"));
        sizePolicy9.setHeightForWidth(cboxK33->sizePolicy().hasHeightForWidth());
        cboxK33->setSizePolicy(sizePolicy9);

        gridLayout_14->addWidget(cboxK33, 0, 1, 1, 1);


        horizontalLayout_98->addLayout(gridLayout_14);

        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_98->addItem(horizontalSpacer_23);


        verticalLayout_117->addLayout(horizontalLayout_98);

        verticalSpacer_71 = new QSpacerItem(20, 218, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_117->addItem(verticalSpacer_71);

        stwgtAdditional->addWidget(pageAdditionalAVRSection);
        pageAdditionalPropertiesAPV = new QWidget();
        pageAdditionalPropertiesAPV->setObjectName(QStringLiteral("pageAdditionalPropertiesAPV"));
        verticalLayout_4 = new QVBoxLayout(pageAdditionalPropertiesAPV);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_105 = new QHBoxLayout();
        horizontalLayout_105->setSpacing(6);
        horizontalLayout_105->setObjectName(QStringLiteral("horizontalLayout_105"));
        horizontalLayout_100 = new QHBoxLayout();
        horizontalLayout_100->setSpacing(6);
        horizontalLayout_100->setObjectName(QStringLiteral("horizontalLayout_100"));
        verticalLayout_110 = new QVBoxLayout();
        verticalLayout_110->setSpacing(6);
        verticalLayout_110->setObjectName(QStringLiteral("verticalLayout_110"));
        groupBox_4 = new QGroupBox(pageAdditionalPropertiesAPV);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        sizePolicy1.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy1);
        horizontalLayout_99 = new QHBoxLayout(groupBox_4);
        horizontalLayout_99->setSpacing(6);
        horizontalLayout_99->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_99->setObjectName(QStringLiteral("horizontalLayout_99"));
        gridLayout_13 = new QGridLayout();
        gridLayout_13->setSpacing(6);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy5.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy5);
        label_7->setScaledContents(true);

        gridLayout_13->addWidget(label_7, 1, 0, 1, 1);

        cboxM87 = new QComboBox(groupBox_4);
        cboxM87->addItem(QString());
        cboxM87->addItem(QString());
        cboxM87->addItem(QString());
        cboxM87->setObjectName(QStringLiteral("cboxM87"));
        sizePolicy1.setHeightForWidth(cboxM87->sizePolicy().hasHeightForWidth());
        cboxM87->setSizePolicy(sizePolicy1);

        gridLayout_13->addWidget(cboxM87, 0, 1, 1, 1);

        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy5.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy5);
        label_6->setScaledContents(true);

        gridLayout_13->addWidget(label_6, 0, 0, 1, 1);

        leM88 = new QLineEdit(groupBox_4);
        leM88->setObjectName(QStringLiteral("leM88"));
        sizePolicy1.setHeightForWidth(leM88->sizePolicy().hasHeightForWidth());
        leM88->setSizePolicy(sizePolicy1);
        leM88->setMaxLength(10);
        leM88->setAlignment(Qt::AlignCenter);

        gridLayout_13->addWidget(leM88, 1, 1, 1, 1);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy5.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy5);
        label_8->setScaledContents(true);

        gridLayout_13->addWidget(label_8, 2, 0, 1, 1);

        leM89 = new QLineEdit(groupBox_4);
        leM89->setObjectName(QStringLiteral("leM89"));
        sizePolicy1.setHeightForWidth(leM89->sizePolicy().hasHeightForWidth());
        leM89->setSizePolicy(sizePolicy1);
        leM89->setMaxLength(10);
        leM89->setAlignment(Qt::AlignCenter);

        gridLayout_13->addWidget(leM89, 2, 1, 1, 1);


        horizontalLayout_99->addLayout(gridLayout_13);


        verticalLayout_110->addWidget(groupBox_4);

        verticalSpacer_58 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_110->addItem(verticalSpacer_58);


        horizontalLayout_100->addLayout(verticalLayout_110);

        horizontalSpacer_63 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_100->addItem(horizontalSpacer_63);


        horizontalLayout_105->addLayout(horizontalLayout_100);

        horizontalSpacer_72 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_105->addItem(horizontalSpacer_72);


        verticalLayout_4->addLayout(horizontalLayout_105);

        verticalSpacer_57 = new QSpacerItem(20, 152, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_57);

        stwgtAdditional->addWidget(pageAdditionalPropertiesAPV);
        pageAdditionalPropertiesAPVStart = new QWidget();
        pageAdditionalPropertiesAPVStart->setObjectName(QStringLiteral("pageAdditionalPropertiesAPVStart"));
        verticalLayout_127 = new QVBoxLayout(pageAdditionalPropertiesAPVStart);
        verticalLayout_127->setSpacing(6);
        verticalLayout_127->setContentsMargins(11, 11, 11, 11);
        verticalLayout_127->setObjectName(QStringLiteral("verticalLayout_127"));
        groupBox_10 = new QGroupBox(pageAdditionalPropertiesAPVStart);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        sizePolicy1.setHeightForWidth(groupBox_10->sizePolicy().hasHeightForWidth());
        groupBox_10->setSizePolicy(sizePolicy1);
        verticalLayout_126 = new QVBoxLayout(groupBox_10);
        verticalLayout_126->setSpacing(6);
        verticalLayout_126->setContentsMargins(11, 11, 11, 11);
        verticalLayout_126->setObjectName(QStringLiteral("verticalLayout_126"));
        gridLayout_25 = new QGridLayout();
        gridLayout_25->setSpacing(6);
        gridLayout_25->setObjectName(QStringLiteral("gridLayout_25"));
        horizontalSpacer_184 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_25->addItem(horizontalSpacer_184, 0, 11, 1, 1);

        cboxAdditionalAPV_StartRmeasOK = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartRmeasOK->addItem(QString());
        cboxAdditionalAPV_StartRmeasOK->addItem(QString());
        cboxAdditionalAPV_StartRmeasOK->setObjectName(QStringLiteral("cboxAdditionalAPV_StartRmeasOK"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartRmeasOK, 7, 1, 1, 1);

        cboxAdditionalAPV_StartBlockOnBRU = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartBlockOnBRU->addItem(QString());
        cboxAdditionalAPV_StartBlockOnBRU->addItem(QString());
        cboxAdditionalAPV_StartBlockOnBRU->setObjectName(QStringLiteral("cboxAdditionalAPV_StartBlockOnBRU"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartBlockOnBRU, 6, 1, 1, 1);

        label_39 = new QLabel(groupBox_10);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setScaledContents(true);

        gridLayout_25->addWidget(label_39, 6, 9, 1, 1);

        cboxAdditionalAPV_StartOffExt1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffExt1->addItem(QString());
        cboxAdditionalAPV_StartOffExt1->addItem(QString());
        cboxAdditionalAPV_StartOffExt1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffExt1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffExt1, 4, 10, 1, 1);

        label_38 = new QLabel(groupBox_10);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setScaledContents(true);

        gridLayout_25->addWidget(label_38, 5, 9, 1, 1);

        cboxAdditionalAPV_StartOffExt3 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffExt3->addItem(QString());
        cboxAdditionalAPV_StartOffExt3->addItem(QString());
        cboxAdditionalAPV_StartOffExt3->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffExt3"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffExt3, 6, 10, 1, 1);

        cboxAdditionalAPV_StartOffExt2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffExt2->addItem(QString());
        cboxAdditionalAPV_StartOffExt2->addItem(QString());
        cboxAdditionalAPV_StartOffExt2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffExt2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffExt2, 5, 10, 1, 1);

        label_40 = new QLabel(groupBox_10);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setScaledContents(true);

        gridLayout_25->addWidget(label_40, 7, 9, 1, 1);

        cboxAdditionalAPV_StartOffAVR = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffAVR->addItem(QString());
        cboxAdditionalAPV_StartOffAVR->addItem(QString());
        cboxAdditionalAPV_StartOffAVR->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffAVR"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffAVR, 6, 7, 1, 1);

        cboxAdditionalAPV_StartOffACHR3 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffACHR3->addItem(QString());
        cboxAdditionalAPV_StartOffACHR3->addItem(QString());
        cboxAdditionalAPV_StartOffACHR3->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffACHR3"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffACHR3, 1, 10, 1, 1);

        cboxAdditionalAPV_StartOffTemp1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffTemp1->addItem(QString());
        cboxAdditionalAPV_StartOffTemp1->addItem(QString());
        cboxAdditionalAPV_StartOffTemp1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffTemp1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffTemp1, 2, 10, 1, 1);

        horizontalSpacer_185 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_25->addItem(horizontalSpacer_185, 0, 5, 1, 1);

        label_25 = new QLabel(groupBox_10);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setScaledContents(true);

        gridLayout_25->addWidget(label_25, 4, 3, 1, 1);

        label_26 = new QLabel(groupBox_10);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setScaledContents(true);

        gridLayout_25->addWidget(label_26, 0, 6, 1, 1);

        label_27 = new QLabel(groupBox_10);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setScaledContents(true);

        gridLayout_25->addWidget(label_27, 5, 3, 1, 1);

        label_28 = new QLabel(groupBox_10);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setScaledContents(true);

        gridLayout_25->addWidget(label_28, 6, 3, 1, 1);

        cboxAdditionalAPV_StartOffUmax1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffUmax1->addItem(QString());
        cboxAdditionalAPV_StartOffUmax1->addItem(QString());
        cboxAdditionalAPV_StartOffUmax1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffUmax1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffUmax1, 1, 7, 1, 1);

        cboxAdditionalAPV_StartOffUmax2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffUmax2->addItem(QString());
        cboxAdditionalAPV_StartOffUmax2->addItem(QString());
        cboxAdditionalAPV_StartOffUmax2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffUmax2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffUmax2, 2, 7, 1, 1);

        cboxAdditionalAPV_StartOffUmin1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffUmin1->addItem(QString());
        cboxAdditionalAPV_StartOffUmin1->addItem(QString());
        cboxAdditionalAPV_StartOffUmin1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffUmin1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffUmin1, 3, 7, 1, 1);

        label_29 = new QLabel(groupBox_10);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setScaledContents(true);

        gridLayout_25->addWidget(label_29, 5, 0, 1, 1);

        label_30 = new QLabel(groupBox_10);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setScaledContents(true);

        gridLayout_25->addWidget(label_30, 4, 0, 1, 1);

        label_31 = new QLabel(groupBox_10);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setScaledContents(true);

        gridLayout_25->addWidget(label_31, 7, 0, 1, 1);

        cboxAdditionalAPV_StartBRUWork = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartBRUWork->addItem(QString());
        cboxAdditionalAPV_StartBRUWork->addItem(QString());
        cboxAdditionalAPV_StartBRUWork->setObjectName(QStringLiteral("cboxAdditionalAPV_StartBRUWork"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartBRUWork, 5, 1, 1, 1);

        cboxAdditionalAPV_StartVacWork = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartVacWork->addItem(QString());
        cboxAdditionalAPV_StartVacWork->addItem(QString());
        cboxAdditionalAPV_StartVacWork->setObjectName(QStringLiteral("cboxAdditionalAPV_StartVacWork"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartVacWork, 4, 1, 1, 1);

        cboxAdditionalAPV_StartOffLev1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffLev1->addItem(QString());
        cboxAdditionalAPV_StartOffLev1->addItem(QString());
        cboxAdditionalAPV_StartOffLev1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffLev1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffLev1, 7, 10, 1, 1);

        cboxAdditionalAPV_StartOffACHR1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffACHR1->addItem(QString());
        cboxAdditionalAPV_StartOffACHR1->addItem(QString());
        cboxAdditionalAPV_StartOffACHR1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffACHR1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffACHR1, 7, 7, 1, 1);

        cboxAdditionalAPV_StartOff3UO = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOff3UO->addItem(QString());
        cboxAdditionalAPV_StartOff3UO->addItem(QString());
        cboxAdditionalAPV_StartOff3UO->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOff3UO"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOff3UO, 5, 7, 1, 1);

        horizontalSpacer_186 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_25->addItem(horizontalSpacer_186, 0, 8, 1, 1);

        horizontalSpacer_187 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_25->addItem(horizontalSpacer_187, 0, 2, 1, 1);

        label_32 = new QLabel(groupBox_10);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setScaledContents(true);

        gridLayout_25->addWidget(label_32, 3, 3, 1, 1);

        label_33 = new QLabel(groupBox_10);
        label_33->setObjectName(QStringLiteral("label_33"));
        label_33->setScaledContents(true);

        gridLayout_25->addWidget(label_33, 0, 3, 1, 1);

        cboxAdditionalAPV_StartOffMTZ1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffMTZ1->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ1->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffMTZ1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffMTZ1, 1, 4, 1, 1);

        cboxAdditionalAPV_StartOffMTZ2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffMTZ2->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ2->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffMTZ2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffMTZ2, 2, 4, 1, 1);

        cboxAdditionalAPV_StartOffStart = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffStart->addItem(QString());
        cboxAdditionalAPV_StartOffStart->addItem(QString());
        cboxAdditionalAPV_StartOffStart->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffStart"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffStart, 5, 4, 1, 1);

        label_34 = new QLabel(groupBox_10);
        label_34->setObjectName(QStringLiteral("label_34"));
        label_34->setScaledContents(true);

        gridLayout_25->addWidget(label_34, 7, 3, 1, 1);

        label_35 = new QLabel(groupBox_10);
        label_35->setObjectName(QStringLiteral("label_35"));
        label_35->setScaledContents(true);

        gridLayout_25->addWidget(label_35, 5, 6, 1, 1);

        label_36 = new QLabel(groupBox_10);
        label_36->setObjectName(QStringLiteral("label_36"));
        label_36->setScaledContents(true);

        gridLayout_25->addWidget(label_36, 3, 6, 1, 1);

        cboxAdditionalAPV_StartOffUmin2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffUmin2->addItem(QString());
        cboxAdditionalAPV_StartOffUmin2->addItem(QString());
        cboxAdditionalAPV_StartOffUmin2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffUmin2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffUmin2, 4, 7, 1, 1);

        label_37 = new QLabel(groupBox_10);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setScaledContents(true);

        gridLayout_25->addWidget(label_37, 7, 6, 1, 1);

        label_41 = new QLabel(groupBox_10);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setScaledContents(true);

        gridLayout_25->addWidget(label_41, 6, 6, 1, 1);

        cboxAdditionalAPV_StartOffOZZ1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffOZZ1->addItem(QString());
        cboxAdditionalAPV_StartOffOZZ1->addItem(QString());
        cboxAdditionalAPV_StartOffOZZ1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffOZZ1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffOZZ1, 6, 4, 1, 1);

        cboxAdditionalAPV_StartOffNZZ1 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffNZZ1->addItem(QString());
        cboxAdditionalAPV_StartOffNZZ1->addItem(QString());
        cboxAdditionalAPV_StartOffNZZ1->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffNZZ1"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffNZZ1, 7, 4, 1, 1);

        label_42 = new QLabel(groupBox_10);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setScaledContents(true);

        gridLayout_25->addWidget(label_42, 0, 12, 1, 1);

        label_43 = new QLabel(groupBox_10);
        label_43->setObjectName(QStringLiteral("label_43"));
        label_43->setScaledContents(true);

        gridLayout_25->addWidget(label_43, 4, 6, 1, 1);

        cboxAdditionalAPV_StartOffMTZ4 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffMTZ4->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ4->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ4->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffMTZ4"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffMTZ4, 4, 4, 1, 1);

        label_44 = new QLabel(groupBox_10);
        label_44->setObjectName(QStringLiteral("label_44"));
        label_44->setScaledContents(true);

        gridLayout_25->addWidget(label_44, 4, 9, 1, 1);

        cboxAdditionalAPV_StartOnFromBRU = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOnFromBRU->addItem(QString());
        cboxAdditionalAPV_StartOnFromBRU->addItem(QString());
        cboxAdditionalAPV_StartOnFromBRU->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOnFromBRU"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOnFromBRU, 0, 4, 1, 1);

        label_45 = new QLabel(groupBox_10);
        label_45->setObjectName(QStringLiteral("label_45"));
        label_45->setScaledContents(true);

        gridLayout_25->addWidget(label_45, 2, 3, 1, 1);

        label_46 = new QLabel(groupBox_10);
        label_46->setObjectName(QStringLiteral("label_46"));
        label_46->setScaledContents(true);

        gridLayout_25->addWidget(label_46, 1, 3, 1, 1);

        label_47 = new QLabel(groupBox_10);
        label_47->setObjectName(QStringLiteral("label_47"));
        label_47->setScaledContents(true);

        gridLayout_25->addWidget(label_47, 0, 0, 1, 1);

        label_48 = new QLabel(groupBox_10);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setScaledContents(true);

        gridLayout_25->addWidget(label_48, 2, 0, 1, 1);

        label_49 = new QLabel(groupBox_10);
        label_49->setObjectName(QStringLiteral("label_49"));
        label_49->setScaledContents(true);

        gridLayout_25->addWidget(label_49, 1, 0, 1, 1);

        cboxAdditionalAPV_StartPskVacPhaseC = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartPskVacPhaseC->addItem(QString());
        cboxAdditionalAPV_StartPskVacPhaseC->addItem(QString());
        cboxAdditionalAPV_StartPskVacPhaseC->setObjectName(QStringLiteral("cboxAdditionalAPV_StartPskVacPhaseC"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartPskVacPhaseC, 3, 1, 1, 1);

        cboxAdditionalAPV_StartMeasBRU = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartMeasBRU->addItem(QString());
        cboxAdditionalAPV_StartMeasBRU->addItem(QString());
        cboxAdditionalAPV_StartMeasBRU->setObjectName(QStringLiteral("cboxAdditionalAPV_StartMeasBRU"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartMeasBRU, 0, 1, 1, 1);

        label_50 = new QLabel(groupBox_10);
        label_50->setObjectName(QStringLiteral("label_50"));
        label_50->setScaledContents(true);

        gridLayout_25->addWidget(label_50, 0, 9, 1, 1);

        cboxAdditionalAPV_StartPskVacPhaseA = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartPskVacPhaseA->addItem(QString());
        cboxAdditionalAPV_StartPskVacPhaseA->addItem(QString());
        cboxAdditionalAPV_StartPskVacPhaseA->setObjectName(QStringLiteral("cboxAdditionalAPV_StartPskVacPhaseA"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartPskVacPhaseA, 1, 1, 1, 1);

        cboxAdditionalAPV_StartPskVacPhaseB = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartPskVacPhaseB->addItem(QString());
        cboxAdditionalAPV_StartPskVacPhaseB->addItem(QString());
        cboxAdditionalAPV_StartPskVacPhaseB->setObjectName(QStringLiteral("cboxAdditionalAPV_StartPskVacPhaseB"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartPskVacPhaseB, 2, 1, 1, 1);

        cboxAdditionalAPV_StartOffACHR2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffACHR2->addItem(QString());
        cboxAdditionalAPV_StartOffACHR2->addItem(QString());
        cboxAdditionalAPV_StartOffACHR2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffACHR2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffACHR2, 0, 10, 1, 1);

        label_51 = new QLabel(groupBox_10);
        label_51->setObjectName(QStringLiteral("label_51"));
        label_51->setScaledContents(true);

        gridLayout_25->addWidget(label_51, 2, 9, 1, 1);

        label_52 = new QLabel(groupBox_10);
        label_52->setObjectName(QStringLiteral("label_52"));
        label_52->setScaledContents(true);

        gridLayout_25->addWidget(label_52, 1, 9, 1, 1);

        label_53 = new QLabel(groupBox_10);
        label_53->setObjectName(QStringLiteral("label_53"));
        label_53->setScaledContents(true);

        gridLayout_25->addWidget(label_53, 3, 9, 1, 1);

        cboxAdditionalAPV_StartOffMTZ3 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffMTZ3->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ3->addItem(QString());
        cboxAdditionalAPV_StartOffMTZ3->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffMTZ3"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffMTZ3, 3, 4, 1, 1);

        label_54 = new QLabel(groupBox_10);
        label_54->setObjectName(QStringLiteral("label_54"));
        label_54->setScaledContents(true);

        gridLayout_25->addWidget(label_54, 6, 0, 1, 1);

        cboxAdditionalAPV_StartOffImin = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffImin->addItem(QString());
        cboxAdditionalAPV_StartOffImin->addItem(QString());
        cboxAdditionalAPV_StartOffImin->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffImin"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffImin, 0, 7, 1, 1);

        label_55 = new QLabel(groupBox_10);
        label_55->setObjectName(QStringLiteral("label_55"));
        label_55->setScaledContents(true);

        gridLayout_25->addWidget(label_55, 1, 6, 1, 1);

        label_56 = new QLabel(groupBox_10);
        label_56->setObjectName(QStringLiteral("label_56"));
        label_56->setScaledContents(true);

        gridLayout_25->addWidget(label_56, 2, 6, 1, 1);

        label_57 = new QLabel(groupBox_10);
        label_57->setObjectName(QStringLiteral("label_57"));
        label_57->setScaledContents(true);

        gridLayout_25->addWidget(label_57, 3, 0, 1, 1);

        cboxAdditionalAPV_StartOffTemp2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffTemp2->addItem(QString());
        cboxAdditionalAPV_StartOffTemp2->addItem(QString());
        cboxAdditionalAPV_StartOffTemp2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffTemp2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffTemp2, 3, 10, 1, 1);

        cboxAdditionalAPV_StartOffLev2 = new QComboBox(groupBox_10);
        cboxAdditionalAPV_StartOffLev2->addItem(QString());
        cboxAdditionalAPV_StartOffLev2->addItem(QString());
        cboxAdditionalAPV_StartOffLev2->setObjectName(QStringLiteral("cboxAdditionalAPV_StartOffLev2"));

        gridLayout_25->addWidget(cboxAdditionalAPV_StartOffLev2, 0, 13, 1, 1);


        verticalLayout_126->addLayout(gridLayout_25);


        verticalLayout_127->addWidget(groupBox_10);

        verticalSpacer_2 = new QSpacerItem(20, 65, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_127->addItem(verticalSpacer_2);

        stwgtAdditional->addWidget(pageAdditionalPropertiesAPVStart);

        horizontalLayout_75->addWidget(stwgtAdditional);

        horizontalSpacer_29 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_75->addItem(horizontalSpacer_29);

        stwgtMain->addWidget(pageAutomation);
        pageJournalCrash = new QWidget();
        pageJournalCrash->setObjectName(QStringLiteral("pageJournalCrash"));
        stwgtMain->addWidget(pageJournalCrash);
        pageJournalEvent = new QWidget();
        pageJournalEvent->setObjectName(QStringLiteral("pageJournalEvent"));
        verticalLayout_132 = new QVBoxLayout(pageJournalEvent);
        verticalLayout_132->setSpacing(6);
        verticalLayout_132->setContentsMargins(11, 11, 11, 11);
        verticalLayout_132->setObjectName(QStringLiteral("verticalLayout_132"));
        horizontalLayout_122 = new QHBoxLayout();
        horizontalLayout_122->setSpacing(5);
        horizontalLayout_122->setObjectName(QStringLiteral("horizontalLayout_122"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setSizeConstraint(QLayout::SetFixedSize);
        pbtnEventJournalReadToTable = new QPushButton(pageJournalEvent);
        pbtnEventJournalReadToTable->setObjectName(QStringLiteral("pbtnEventJournalReadToTable"));

        verticalLayout_7->addWidget(pbtnEventJournalReadToTable);

        pbtnEventJournalTableClear = new QPushButton(pageJournalEvent);
        pbtnEventJournalTableClear->setObjectName(QStringLiteral("pbtnEventJournalTableClear"));

        verticalLayout_7->addWidget(pbtnEventJournalTableClear);

        checkboxEventJournalScrollTable = new QCheckBox(pageJournalEvent);
        checkboxEventJournalScrollTable->setObjectName(QStringLiteral("checkboxEventJournalScrollTable"));

        verticalLayout_7->addWidget(checkboxEventJournalScrollTable);


        horizontalLayout_122->addLayout(verticalLayout_7);

        groupboxEventJournalReadInterval = new QGroupBox(pageJournalEvent);
        groupboxEventJournalReadInterval->setObjectName(QStringLiteral("groupboxEventJournalReadInterval"));
        sizePolicy5.setHeightForWidth(groupboxEventJournalReadInterval->sizePolicy().hasHeightForWidth());
        groupboxEventJournalReadInterval->setSizePolicy(sizePolicy5);
        groupboxEventJournalReadInterval->setAlignment(Qt::AlignCenter);
        groupboxEventJournalReadInterval->setCheckable(true);
        groupboxEventJournalReadInterval->setChecked(false);
        verticalLayout = new QVBoxLayout(groupboxEventJournalReadInterval);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_117 = new QHBoxLayout();
        horizontalLayout_117->setSpacing(6);
        horizontalLayout_117->setObjectName(QStringLiteral("horizontalLayout_117"));
        radiobtnEventJournalInterval = new QRadioButton(groupboxEventJournalReadInterval);
        radiobtnEventJournalInterval->setObjectName(QStringLiteral("radiobtnEventJournalInterval"));
        radiobtnEventJournalInterval->setChecked(true);

        horizontalLayout_117->addWidget(radiobtnEventJournalInterval);

        radiobtnEventJournalDate = new QRadioButton(groupboxEventJournalReadInterval);
        radiobtnEventJournalDate->setObjectName(QStringLiteral("radiobtnEventJournalDate"));

        horizontalLayout_117->addWidget(radiobtnEventJournalDate);

        horizontalSpacer_64 = new QSpacerItem(1, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_117->addItem(horizontalSpacer_64);


        verticalLayout->addLayout(horizontalLayout_117);

        horizontalLayout_121 = new QHBoxLayout();
        horizontalLayout_121->setSpacing(6);
        horizontalLayout_121->setObjectName(QStringLiteral("horizontalLayout_121"));
        wgtEventJournalRange = new QWidget(groupboxEventJournalReadInterval);
        wgtEventJournalRange->setObjectName(QStringLiteral("wgtEventJournalRange"));
        sizePolicy6.setHeightForWidth(wgtEventJournalRange->sizePolicy().hasHeightForWidth());
        wgtEventJournalRange->setSizePolicy(sizePolicy6);
        horizontalLayout_119 = new QHBoxLayout(wgtEventJournalRange);
        horizontalLayout_119->setSpacing(0);
        horizontalLayout_119->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_119->setObjectName(QStringLiteral("horizontalLayout_119"));
        horizontalLayout_119->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_101 = new QHBoxLayout();
        horizontalLayout_101->setSpacing(6);
        horizontalLayout_101->setObjectName(QStringLiteral("horizontalLayout_101"));
        label_13 = new QLabel(wgtEventJournalRange);
        label_13->setObjectName(QStringLiteral("label_13"));
        sizePolicy6.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy6);
        label_13->setScaledContents(true);

        horizontalLayout_101->addWidget(label_13);

        spinBoxEventJournalReadBegin = new QSpinBox(wgtEventJournalRange);
        spinBoxEventJournalReadBegin->setObjectName(QStringLiteral("spinBoxEventJournalReadBegin"));
        sizePolicy5.setHeightForWidth(spinBoxEventJournalReadBegin->sizePolicy().hasHeightForWidth());
        spinBoxEventJournalReadBegin->setSizePolicy(sizePolicy5);
        spinBoxEventJournalReadBegin->setMaximum(256000);

        horizontalLayout_101->addWidget(spinBoxEventJournalReadBegin);

        label_14 = new QLabel(wgtEventJournalRange);
        label_14->setObjectName(QStringLiteral("label_14"));
        sizePolicy6.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy6);
        label_14->setScaledContents(true);

        horizontalLayout_101->addWidget(label_14);

        spinBoxEvenJournalReadCount = new QSpinBox(wgtEventJournalRange);
        spinBoxEvenJournalReadCount->setObjectName(QStringLiteral("spinBoxEvenJournalReadCount"));
        sizePolicy5.setHeightForWidth(spinBoxEvenJournalReadCount->sizePolicy().hasHeightForWidth());
        spinBoxEvenJournalReadCount->setSizePolicy(sizePolicy5);
        spinBoxEvenJournalReadCount->setMaximum(256000);

        horizontalLayout_101->addWidget(spinBoxEvenJournalReadCount);


        horizontalLayout_119->addLayout(horizontalLayout_101);


        horizontalLayout_121->addWidget(wgtEventJournalRange);

        wgtEventJournalCalendar = new QWidget(groupboxEventJournalReadInterval);
        wgtEventJournalCalendar->setObjectName(QStringLiteral("wgtEventJournalCalendar"));
        QSizePolicy sizePolicy10(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(wgtEventJournalCalendar->sizePolicy().hasHeightForWidth());
        wgtEventJournalCalendar->setSizePolicy(sizePolicy10);
        horizontalLayout_120 = new QHBoxLayout(wgtEventJournalCalendar);
        horizontalLayout_120->setSpacing(0);
        horizontalLayout_120->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_120->setObjectName(QStringLiteral("horizontalLayout_120"));
        horizontalLayout_120->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_118 = new QHBoxLayout();
        horizontalLayout_118->setSpacing(6);
        horizontalLayout_118->setObjectName(QStringLiteral("horizontalLayout_118"));
        leJournalEventDate = new QLineEdit(wgtEventJournalCalendar);
        leJournalEventDate->setObjectName(QStringLiteral("leJournalEventDate"));
        leJournalEventDate->setMinimumSize(QSize(150, 0));
        leJournalEventDate->setReadOnly(true);

        horizontalLayout_118->addWidget(leJournalEventDate);

        toolbtnEventJournalCalendarOpen = new QToolButton(wgtEventJournalCalendar);
        toolbtnEventJournalCalendarOpen->setObjectName(QStringLiteral("toolbtnEventJournalCalendarOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/resource/images/calendar.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolbtnEventJournalCalendarOpen->setIcon(icon1);

        horizontalLayout_118->addWidget(toolbtnEventJournalCalendarOpen);


        horizontalLayout_120->addLayout(horizontalLayout_118);


        horizontalLayout_121->addWidget(wgtEventJournalCalendar);


        verticalLayout->addLayout(horizontalLayout_121);


        horizontalLayout_122->addWidget(groupboxEventJournalReadInterval);

        horizontalSpacer_9 = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_122->addItem(horizontalSpacer_9);


        verticalLayout_132->addLayout(horizontalLayout_122);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_11 = new QLabel(pageJournalEvent);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setScaledContents(true);

        horizontalLayout_3->addWidget(label_11);

        leEventCount = new QLineEdit(pageJournalEvent);
        leEventCount->setObjectName(QStringLiteral("leEventCount"));
        sizePolicy7.setHeightForWidth(leEventCount->sizePolicy().hasHeightForWidth());
        leEventCount->setSizePolicy(sizePolicy7);
        leEventCount->setAlignment(Qt::AlignCenter);
        leEventCount->setReadOnly(true);

        horizontalLayout_3->addWidget(leEventCount);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        label_15 = new QLabel(pageJournalEvent);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setScaledContents(true);

        horizontalLayout_3->addWidget(label_15);

        leEventProcessTime = new QLineEdit(pageJournalEvent);
        leEventProcessTime->setObjectName(QStringLiteral("leEventProcessTime"));
        sizePolicy7.setHeightForWidth(leEventProcessTime->sizePolicy().hasHeightForWidth());
        leEventProcessTime->setSizePolicy(sizePolicy7);
        leEventProcessTime->setAlignment(Qt::AlignCenter);
        leEventProcessTime->setReadOnly(true);

        horizontalLayout_3->addWidget(leEventProcessTime);


        verticalLayout_132->addLayout(horizontalLayout_3);

        tablewgtEventJournal = new QTableWidget(pageJournalEvent);
        tablewgtEventJournal->setObjectName(QStringLiteral("tablewgtEventJournal"));

        verticalLayout_132->addWidget(tablewgtEventJournal);

        stwgtMain->addWidget(pageJournalEvent);
        pageJournalOscilloscope = new QWidget();
        pageJournalOscilloscope->setObjectName(QStringLiteral("pageJournalOscilloscope"));
        stwgtMain->addWidget(pageJournalOscilloscope);
        pageMeasurePrimaryValues = new QWidget();
        pageMeasurePrimaryValues->setObjectName(QStringLiteral("pageMeasurePrimaryValues"));
        stwgtMain->addWidget(pageMeasurePrimaryValues);
        pageMeasureSecondaryValues = new QWidget();
        pageMeasureSecondaryValues->setObjectName(QStringLiteral("pageMeasureSecondaryValues"));
        stwgtMain->addWidget(pageMeasureSecondaryValues);
        pageMeasureElectricPower = new QWidget();
        pageMeasureElectricPower->setObjectName(QStringLiteral("pageMeasureElectricPower"));
        stwgtMain->addWidget(pageMeasureElectricPower);
        pageMonitorInputDiscret = new QWidget();
        pageMonitorInputDiscret->setObjectName(QStringLiteral("pageMonitorInputDiscret"));
        stwgtMain->addWidget(pageMonitorInputDiscret);
        pageMonitorOutputDiscret = new QWidget();
        pageMonitorOutputDiscret->setObjectName(QStringLiteral("pageMonitorOutputDiscret"));
        stwgtMain->addWidget(pageMonitorOutputDiscret);
        pageSetLedPurpose = new QWidget();
        pageSetLedPurpose->setObjectName(QStringLiteral("pageSetLedPurpose"));
        verticalLayout_2 = new QVBoxLayout(pageSetLedPurpose);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_106 = new QHBoxLayout();
        horizontalLayout_106->setSpacing(6);
        horizontalLayout_106->setObjectName(QStringLiteral("horizontalLayout_106"));
        pbtnClearLedOutput = new QPushButton(pageSetLedPurpose);
        pbtnClearLedOutput->setObjectName(QStringLiteral("pbtnClearLedOutput"));

        horizontalLayout_106->addWidget(pbtnClearLedOutput);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_106->addItem(horizontalSpacer_12);


        verticalLayout_2->addLayout(horizontalLayout_106);

        tablewgtLedPurpose = new QTableView(pageSetLedPurpose);
        tablewgtLedPurpose->setObjectName(QStringLiteral("tablewgtLedPurpose"));

        verticalLayout_2->addWidget(tablewgtLedPurpose);

        stwgtMain->addWidget(pageSetLedPurpose);
        pageSetDiscretInputPurpose = new QWidget();
        pageSetDiscretInputPurpose->setObjectName(QStringLiteral("pageSetDiscretInputPurpose"));
        verticalLayout_56 = new QVBoxLayout(pageSetDiscretInputPurpose);
        verticalLayout_56->setSpacing(6);
        verticalLayout_56->setContentsMargins(11, 11, 11, 11);
        verticalLayout_56->setObjectName(QStringLiteral("verticalLayout_56"));
        horizontalLayout_107 = new QHBoxLayout();
        horizontalLayout_107->setSpacing(6);
        horizontalLayout_107->setObjectName(QStringLiteral("horizontalLayout_107"));
        pbtnClearDiscreteInput = new QPushButton(pageSetDiscretInputPurpose);
        pbtnClearDiscreteInput->setObjectName(QStringLiteral("pbtnClearDiscreteInput"));

        horizontalLayout_107->addWidget(pbtnClearDiscreteInput);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_107->addItem(horizontalSpacer_14);


        verticalLayout_56->addLayout(horizontalLayout_107);

        tablewgtDiscreteInputPurpose = new QTableView(pageSetDiscretInputPurpose);
        tablewgtDiscreteInputPurpose->setObjectName(QStringLiteral("tablewgtDiscreteInputPurpose"));

        verticalLayout_56->addWidget(tablewgtDiscreteInputPurpose);

        stwgtMain->addWidget(pageSetDiscretInputPurpose);
        pageSetRelayPurpose = new QWidget();
        pageSetRelayPurpose->setObjectName(QStringLiteral("pageSetRelayPurpose"));
        verticalLayout_118 = new QVBoxLayout(pageSetRelayPurpose);
        verticalLayout_118->setSpacing(6);
        verticalLayout_118->setContentsMargins(11, 11, 11, 11);
        verticalLayout_118->setObjectName(QStringLiteral("verticalLayout_118"));
        horizontalLayout_108 = new QHBoxLayout();
        horizontalLayout_108->setSpacing(6);
        horizontalLayout_108->setObjectName(QStringLiteral("horizontalLayout_108"));
        pbtnClearRelayOutput = new QPushButton(pageSetRelayPurpose);
        pbtnClearRelayOutput->setObjectName(QStringLiteral("pbtnClearRelayOutput"));

        horizontalLayout_108->addWidget(pbtnClearRelayOutput);

        horizontalSpacer_50 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_108->addItem(horizontalSpacer_50);


        verticalLayout_118->addLayout(horizontalLayout_108);

        tablewgtRelayPurpose = new QTableView(pageSetRelayPurpose);
        tablewgtRelayPurpose->setObjectName(QStringLiteral("tablewgtRelayPurpose"));

        verticalLayout_118->addWidget(tablewgtRelayPurpose);

        stwgtMain->addWidget(pageSetRelayPurpose);
        pageSetKeyboardPurpose = new QWidget();
        pageSetKeyboardPurpose->setObjectName(QStringLiteral("pageSetKeyboardPurpose"));
        verticalLayout_119 = new QVBoxLayout(pageSetKeyboardPurpose);
        verticalLayout_119->setSpacing(6);
        verticalLayout_119->setContentsMargins(11, 11, 11, 11);
        verticalLayout_119->setObjectName(QStringLiteral("verticalLayout_119"));
        horizontalLayout_109 = new QHBoxLayout();
        horizontalLayout_109->setSpacing(6);
        horizontalLayout_109->setObjectName(QStringLiteral("horizontalLayout_109"));
        pbtnClearKeyboardPurpose = new QPushButton(pageSetKeyboardPurpose);
        pbtnClearKeyboardPurpose->setObjectName(QStringLiteral("pbtnClearKeyboardPurpose"));

        horizontalLayout_109->addWidget(pbtnClearKeyboardPurpose);

        horizontalSpacer_51 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_109->addItem(horizontalSpacer_51);


        verticalLayout_119->addLayout(horizontalLayout_109);

        tablewgtKeyboardPurpose = new QTableView(pageSetKeyboardPurpose);
        tablewgtKeyboardPurpose->setObjectName(QStringLiteral("tablewgtKeyboardPurpose"));

        verticalLayout_119->addWidget(tablewgtKeyboardPurpose);

        stwgtMain->addWidget(pageSetKeyboardPurpose);

        verticalLayout_133->addWidget(stwgtMain);

        horizontalLayout_62 = new QHBoxLayout();
        horizontalLayout_62->setSpacing(6);
        horizontalLayout_62->setObjectName(QStringLiteral("horizontalLayout_62"));
        label_9 = new QLabel(centralWgt);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_62->addWidget(label_9);

        pbtnReadCurrentBlock = new QPushButton(centralWgt);
        pbtnReadCurrentBlock->setObjectName(QStringLiteral("pbtnReadCurrentBlock"));

        horizontalLayout_62->addWidget(pbtnReadCurrentBlock);

        pbtnWriteCurrentBlock = new QPushButton(centralWgt);
        pbtnWriteCurrentBlock->setObjectName(QStringLiteral("pbtnWriteCurrentBlock"));

        horizontalLayout_62->addWidget(pbtnWriteCurrentBlock);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_62->addItem(horizontalSpacer_4);

        label_10 = new QLabel(centralWgt);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_62->addWidget(label_10);

        pbtnReadAllBlock = new QPushButton(centralWgt);
        pbtnReadAllBlock->setObjectName(QStringLiteral("pbtnReadAllBlock"));

        horizontalLayout_62->addWidget(pbtnReadAllBlock);

        pbtnWriteAllBlock = new QPushButton(centralWgt);
        pbtnWriteAllBlock->setObjectName(QStringLiteral("pbtnWriteAllBlock"));

        horizontalLayout_62->addWidget(pbtnWriteAllBlock);


        verticalLayout_133->addLayout(horizontalLayout_62);


        verticalLayout_131->addWidget(centralWgt);

        ConfiguratorWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ConfiguratorWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ConfiguratorWindow->setStatusBar(statusBar);
        dockwgtDeviceMenu = new QDockWidget(ConfiguratorWindow);
        dockwgtDeviceMenu->setObjectName(QStringLiteral("dockwgtDeviceMenu"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_116 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_116->setSpacing(6);
        verticalLayout_116->setContentsMargins(11, 11, 11, 11);
        verticalLayout_116->setObjectName(QStringLiteral("verticalLayout_116"));
        horizontalLayout_69 = new QHBoxLayout();
        horizontalLayout_69->setSpacing(6);
        horizontalLayout_69->setObjectName(QStringLiteral("horizontalLayout_69"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_69->addItem(horizontalSpacer_6);

        tbntExpandItems = new QToolButton(dockWidgetContents);
        tbntExpandItems->setObjectName(QStringLiteral("tbntExpandItems"));
        tbntExpandItems->setMinimumSize(QSize(24, 24));
        tbntExpandItems->setMaximumSize(QSize(24, 24));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/resource/images/branch_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        tbntExpandItems->setIcon(icon2);
        tbntExpandItems->setIconSize(QSize(24, 24));
        tbntExpandItems->setCheckable(true);

        horizontalLayout_69->addWidget(tbntExpandItems);


        verticalLayout_116->addLayout(horizontalLayout_69);

        treewgtDeviceMenu = new QTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treewgtDeviceMenu->setHeaderItem(__qtreewidgetitem);
        treewgtDeviceMenu->setObjectName(QStringLiteral("treewgtDeviceMenu"));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        treewgtDeviceMenu->setFont(font);
        treewgtDeviceMenu->setStyleSheet(QLatin1String("QTreeView::branch:has-children:!has-siblings:closed,\n"
"QTreeView::branch:closed:has-children:has-siblings\n"
"{\n"
"        border-image: none;\n"
"        image: url(:/images/resource/images/branch_close.png);\n"
"}\n"
"\n"
"QTreeView::branch:open:has-children:!has-siblings,\n"
"QTreeView::branch:open:has-children:has-siblings  \n"
"{\n"
"        border-image: none;\n"
"        image: url(:/images/resource/images/branch_open.png);\n"
"}"));
        treewgtDeviceMenu->setHeaderHidden(true);

        verticalLayout_116->addWidget(treewgtDeviceMenu);

        dockwgtDeviceMenu->setWidget(dockWidgetContents);
        ConfiguratorWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockwgtDeviceMenu);

        retranslateUi(ConfiguratorWindow);

        tabwgtMenu->setCurrentIndex(1);
        cboxBaudrate->setCurrentIndex(9);
        stwgtMain->setCurrentIndex(15);
        stwgtProtectionPropertiesMTZ->setCurrentIndex(1);
        stwgtProtectionPropertiesEarthly->setCurrentIndex(0);
        stwgtProtectionPropertiesPower->setCurrentIndex(4);
        stwgtProtectionPropertiesMotor->setCurrentIndex(0);
        stwgtProtectionPropertiesFrequency->setCurrentIndex(2);
        stwgtProtectionPropertiesExternal->setCurrentIndex(0);
        stwgtProtectionPropertiesTemperature->setCurrentIndex(1);
        stwgtProtectionPropertiesLevel->setCurrentIndex(2);
        cboxMeasBRU->setCurrentIndex(0);
        cboxfA->setCurrentIndex(0);
        cboxfB->setCurrentIndex(0);
        cboxfC->setCurrentIndex(0);
        cboxWorkVak->setCurrentIndex(0);
        cboxWorkBRU->setCurrentIndex(0);
        cboxOnBRU->setCurrentIndex(0);
        cboxRmeasOK->setCurrentIndex(0);
        cboxOnBRU_2->setCurrentIndex(0);
        cboxOffMTZ1->setCurrentIndex(0);
        cboxOffMTZ2->setCurrentIndex(0);
        cboxOffMTZ3->setCurrentIndex(0);
        cboxOffMTZ4->setCurrentIndex(0);
        cboxOffStart->setCurrentIndex(0);
        cboxOffOZZ1->setCurrentIndex(0);
        cboxOffNZZ1->setCurrentIndex(0);
        cboxOffImin->setCurrentIndex(0);
        cboxOffImax1->setCurrentIndex(0);
        cboxOffImax2->setCurrentIndex(0);
        cboxOffImin1->setCurrentIndex(0);
        cboxOffImin2->setCurrentIndex(0);
        cboxOffI3UO->setCurrentIndex(0);
        cboxOffAVR->setCurrentIndex(0);
        cboxOffACR1->setCurrentIndex(0);
        cboxOffACR2->setCurrentIndex(0);
        cboxOffACR3->setCurrentIndex(0);
        cboxOffTemp1->setCurrentIndex(0);
        cboxOffTemp2->setCurrentIndex(0);
        cboxOffExt1->setCurrentIndex(0);
        cboxOffExt2->setCurrentIndex(0);
        cboxOffExt3->setCurrentIndex(0);
        cboxOffLevel1->setCurrentIndex(0);
        cboxOffLevel2->setCurrentIndex(0);
        stwgtSwitchDevice->setCurrentIndex(0);
        stwgtAdditional->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(ConfiguratorWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ConfiguratorWindow)
    {
        ConfiguratorWindow->setWindowTitle(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\204\320\270\320\263\321\203\321\200\320\260\321\202\320\276\321\200 \320\221\320\227\320\243-16", nullptr));
        groupboxMenuFile->setTitle(QString());
        pbtnMenuNewCreate->setText(QApplication::translate("ConfiguratorWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        pbtnMenuSave->setText(QApplication::translate("ConfiguratorWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        pbtnMenuOpen->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        pbtnMenuSaveAs->setText(QApplication::translate("ConfiguratorWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272...", nullptr));
        pbtnMenuExit->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        tabwgtMenu->setTabText(tabwgtMenu->indexOf(tabFile), QApplication::translate("ConfiguratorWindow", "\320\244\320\260\320\271\320\273", nullptr));
        pbtnMenuExportToPDF->setText(QApplication::translate("ConfiguratorWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\262 PDF", nullptr));
        pbtnExportPurpose->setText(QApplication::translate("ConfiguratorWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\277\321\200\320\270\320\262\321\217\320\267\320\276\320\272", nullptr));
        pushbtnExportEventJournalDb->setText(QApplication::translate("ConfiguratorWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202 \320\266\321\203\321\200\320\275\320\260\320\273\320\260 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", nullptr));
        pbtnImportPurpose->setText(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\277\321\200\320\270\320\262\321\217\320\267\320\276\320\272", nullptr));
        pushbtnImportEventDb->setText(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\320\276\321\200\321\202 \320\266\321\203\321\200\320\275\320\260\320\273\320\260 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", nullptr));
        tabwgtMenu->setTabText(tabwgtMenu->indexOf(tabExport), QApplication::translate("ConfiguratorWindow", "\320\255\320\272\321\201\320\277\320\276\321\200\321\202", nullptr));
        pbtnMenuPanelMenuCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273/\320\262\321\213\320\272\320\273 \320\277\320\260\320\275\320\265\320\273\321\214 \320\274\320\265\320\275\321\216", nullptr));
        pbtnMenuPanelVariableCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273/\320\262\321\213\320\272\320\273 \320\277\320\260\320\275\320\265\320\273\321\214 \320\277\320\265\321\200\320\265\320\274\320\265\320\275\320\275\321\213\321\205", nullptr));
        tabwgtMenu->setTabText(tabwgtMenu->indexOf(tabView), QApplication::translate("ConfiguratorWindow", "\320\222\320\270\320\264", nullptr));
        gboxMenuSerialPort->setTitle(QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\320\273\320\265\320\264\320\276\320\262\320\260\321\202\320\265\320\273\321\214\320\275\321\213\320\271 \320\277\320\276\321\200\321\202", nullptr));
#ifndef QT_NO_TOOLTIP
        tbtnPortRefresh->setToolTip(QApplication::translate("ConfiguratorWindow", "<html><head/><body><p>\320\236\320\261\320\275\320\276\320\262\320\270\321\202\321\214 \321\201\320\277\320\270\321\201\320\276\320\272 \320\264\320\276\321\201\321\202\321\203\320\277\320\275\321\213\321\205 \320\277\320\276\321\200\321\202\320\276\320\262</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        tbtnPortRefresh->setText(QApplication::translate("ConfiguratorWindow", "...", nullptr));
#ifndef QT_NO_TOOLTIP
        cboxPortName->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxBaudrate->setItemText(0, QApplication::translate("ConfiguratorWindow", "1200", nullptr));
        cboxBaudrate->setItemText(1, QApplication::translate("ConfiguratorWindow", "2400", nullptr));
        cboxBaudrate->setItemText(2, QApplication::translate("ConfiguratorWindow", "4800", nullptr));
        cboxBaudrate->setItemText(3, QApplication::translate("ConfiguratorWindow", "9600", nullptr));
        cboxBaudrate->setItemText(4, QApplication::translate("ConfiguratorWindow", "14400", nullptr));
        cboxBaudrate->setItemText(5, QApplication::translate("ConfiguratorWindow", "19200", nullptr));
        cboxBaudrate->setItemText(6, QApplication::translate("ConfiguratorWindow", "28800", nullptr));
        cboxBaudrate->setItemText(7, QApplication::translate("ConfiguratorWindow", "38400", nullptr));
        cboxBaudrate->setItemText(8, QApplication::translate("ConfiguratorWindow", "57600", nullptr));
        cboxBaudrate->setItemText(9, QApplication::translate("ConfiguratorWindow", "115200", nullptr));
        cboxBaudrate->setItemText(10, QApplication::translate("ConfiguratorWindow", "230400", nullptr));
        cboxBaudrate->setItemText(11, QApplication::translate("ConfiguratorWindow", "460800", nullptr));
        cboxBaudrate->setItemText(12, QApplication::translate("ConfiguratorWindow", "921600", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxBaudrate->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextPortName->setText(QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\200\321\202", nullptr));
        lblTextPortSpeed->setText(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
        pbtnPortCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        pbtnSerialPortSettings->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        groupboxMenuCommunication->setTitle(QApplication::translate("ConfiguratorWindow", "\320\241\320\262\321\217\320\267\321\214", nullptr));
        lblTextSlaveID->setText(QApplication::translate("ConfiguratorWindow", "ID \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
#ifndef QT_NO_TOOLTIP
        sboxSlaveID->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\220\320\264\321\200\320\265\321\201 \320\262\320\265\320\264\320\276\320\274\320\276\320\263\320\276 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        chboxTerminal->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\265\321\200\320\274\320\270\320\275\320\260\320\273", nullptr));
        groupboxSyncDevice->setTitle(QApplication::translate("ConfiguratorWindow", "\320\241\320\270\320\275\321\205\321\200\320\276\320\275\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        label_16->setText(QApplication::translate("ConfiguratorWindow", "\320\276\320\277\321\200\320\276\321\201, \320\274\321\201", nullptr));
        tabwgtMenu->setTabText(tabwgtMenu->indexOf(tabSettings), QApplication::translate("ConfiguratorWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        pbtnMenuReference->setText(QApplication::translate("ConfiguratorWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", nullptr));
        pbtnVersionSoftware->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\265\321\200\321\201\320\270\321\217", nullptr));
        tabwgtMenu->setTabText(tabwgtMenu->indexOf(tabHelp), QApplication::translate("ConfiguratorWindow", "\320\237\320\276\320\274\320\276\321\211\321\214", nullptr));
        checkboxCalibTimeout->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\277\321\200\320\276\321\201\320\260 \321\200\320\260\321\201\321\207\320\265\321\202\320\275\321\213\321\205 \320\262\320\265\320\273\320\270\321\207\320\270\320\275, \320\274\321\201", nullptr));
#ifndef QT_NO_TOOLTIP
        sboxTimeoutCalc->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\277\321\200\320\276\321\201\320\260 \321\200\320\260\321\201\321\207\320\265\321\202\320\275\321\213\321\205 \320\262\320\265\320\273\320\270\321\207\320\270\320\275 \320\277\320\276 \321\202\320\260\320\271\320\274\320\265\321\200\321\203", nullptr));
#endif // QT_NO_TOOLTIP
        gboxMain->setTitle(QString());
        lblTextMainKtt->setText(QApplication::translate("ConfiguratorWindow", "\320\232\321\202\321\202", nullptr));
        lblTextMainKt0->setText(QApplication::translate("ConfiguratorWindow", "\320\232\321\2020", nullptr));
        lblTextMainKtn->setText(QApplication::translate("ConfiguratorWindow", "\320\232\321\202\320\275", nullptr));
        lblMainOnTN->setText(QApplication::translate("ConfiguratorWindow", "\320\237\320\276\320\264\320\272\320\273 \320\242\320\235", nullptr));
        leM01->setInputMask(QString());
        leM01->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM02->setInputMask(QString());
        leM02->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM03->setInputMask(QString());
        leM03->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        cboxM04->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM04->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\244\320\260\320\267\320\275\320\276\320\265", nullptr));
        cboxM04->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\236\320\264\320\275\320\276\321\204\320\260\320\267\320\275\320\276\320\265", nullptr));
        cboxM04->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\320\276\320\265", nullptr));

        lblTextCalibFactorPowerPhase_B_C->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203\321\204\320\260\320\267\320\275\320\276\320\265 \320\222-\320\241", nullptr));
        lblTextCalibFactorPowerUAx->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 UAx", nullptr));
        lblTextCalibFactorPowerUBx->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 UBx", nullptr));
        lblTextCalibFactorChannel3U0->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\272\320\260\320\275\320\260\320\273\320\260 3U0", nullptr));
        lblTextCalibFactorPowerUCx->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 UCx", nullptr));
        lblTextCalibFactorPowerPhase_A_B->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203\321\204\320\260\320\267\320\275\320\276\320\265 \320\220-\320\222", nullptr));
        lblTextCalibFactorPower3U0x->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 3U0X", nullptr));
        leKUBC->setInputMask(QString());
        leKUBC->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUCA->setInputMask(QString());
        leKUCA->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUAB->setInputMask(QString());
        leKUAB->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUCX->setInputMask(QString());
        leKUCX->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK3U0X->setInputMask(QString());
        leK3U0X->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKU0X_->setInputMask(QString());
        leKU0X_->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKU0X->setInputMask(QString());
        leKU0X->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUAX->setInputMask(QString());
        leKUAX->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUBX->setInputMask(QString());
        leKUBX->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextCalibFactorCurrent3I0->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 3I0", nullptr));
        leKUB->setInputMask(QString());
        leKUB->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextCalibFactorPowerPhase_C->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\267\320\260 \320\241", nullptr));
        leK3U0->setInputMask(QString());
        leK3U0->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextCalibFactorPower3U0->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 3U0", nullptr));
        lblTextCalibFactorPowerPhase_B->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\267\320\260 \320\222", nullptr));
        lblTextCalibFactorPowerPhase_A->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\267\320\260 \320\220", nullptr));
        leKUA->setInputMask(QString());
        leKUA->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUC->setInputMask(QString());
        leKUC->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKUS->setInputMask(QString());
        leKUS->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK3I0->setInputMask(QString());
        leK3I0->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextCalibFactorPowerTotal->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \321\201\321\203\320\274\320\274\320\260\321\200\320\275\320\276\320\265", nullptr));
        lblTextCalibFactorCurrentPhase_A->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 \321\204\320\260\320\267\320\260 \320\220", nullptr));
        lblTextCalibFactorCurrentPhase_B->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 \321\204\320\260\320\267\320\260 \320\222", nullptr));
        leKIA->setInputMask(QString());
        leKIA->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leKIB->setInputMask(QString());
        leKIB->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextCalibFactorCurrentPhase_C->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \321\202\320\276\320\272 \321\204\320\260\320\267\320\260 \320\241", nullptr));
        leKIC->setInputMask(QString());
        leKIC->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextCalibFactorPowerPhase_C_A->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265 \320\274\320\265\320\266\320\264\321\203\321\204\320\260\320\267\320\275\320\276\320\265 \320\241-\320\220", nullptr));
        lblTextCalibFactorChannel3Us->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\260\320\273\320\270\320\261\321\200. \320\272\320\276\321\215\321\204\321\204. \320\272\320\260\320\275\320\260\320\273\320\260 Us", nullptr));
        pbtnProtectionMTZ1->setText(QApplication::translate("ConfiguratorWindow", "\320\234\320\242\320\2271", nullptr));
        pbtnProtectionMTZ2->setText(QApplication::translate("ConfiguratorWindow", "\320\234\320\242\320\2272", nullptr));
        pbtnProtectionMTZ3->setText(QApplication::translate("ConfiguratorWindow", "\320\234\320\242\320\2273", nullptr));
        pbtnProtectionMTZ4->setText(QApplication::translate("ConfiguratorWindow", "\320\234\320\242\320\2274", nullptr));
        gboxProtectionPropertiesMTZ1->setTitle(QString());
        lblTextProtectionMTZ_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMTZ_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Timeout->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionMTZ_EnterBlocking->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\262\320\276\320\264 \320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\272\320\270", nullptr));
        lblTextProtectionMTZ_EnterBlocking_5->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxM05->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM05->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM05->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));
        cboxM05->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\241 \320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\272\320\276\320\271", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM05->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM06->setInputMask(QString());
        leM06->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM08->setInputMask(QString());
        leM08->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK31->setInputMask(QString());
        leK31->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX01->setInputMask(QString());
        leX01->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesMTZ2->setTitle(QString());
        lblTextProtectionMTZ_Ctrl_2->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMTZ_StartCurrent_2->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Timeout_2->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Acceleration->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\262\320\276\320\264 \321\203\321\201\320\272\320\276\321\200", nullptr));
        lblTextProtectionMTZ_Acceleration_2->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxM09->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM09->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM09->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));
        cboxM09->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\241 \321\203\321\201\320\272\320\276\321\200\320\265\320\275\320\270\320\265\320\274", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM09->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM10->setInputMask(QString());
        leM10->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM11->setInputMask(QString());
        leM11->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM12->setInputMask(QString());
        leM12->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX03->setInputMask(QString());
        leX03->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesMTZ3->setTitle(QString());
        lblTextProtectionMTZ_Ctrl_3->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMTZ_StartCurrent_3->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Timeout_3->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Factor->setText(QApplication::translate("ConfiguratorWindow", "\320\234\320\275\320\276\320\266\320\270\321\202\320\265\320\273\321\214", nullptr));
        lblTextProtectionMTZ_Factor_2->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxM13->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM13->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM13->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\245-\320\272\320\260 A", nullptr));
        cboxM13->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\245-\320\272\320\260 B", nullptr));
        cboxM13->setItemText(4, QApplication::translate("ConfiguratorWindow", "\320\245-\320\272\320\260 \320\241", nullptr));
        cboxM13->setItemText(5, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM13->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM14->setInputMask(QString());
        leM14->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK22->setInputMask(QString());
        leK22->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM15->setInputMask(QString());
        leM15->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX04->setInputMask(QString());
        leX04->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesMTZ4->setTitle(QString());
        lblTextProtectionMTZ_Ctrl_4->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMTZ_StartCurrent_4->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMTZ_DirStart->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200. \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Angle->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\263\320\276\320\273", nullptr));
        lblTextProtectionMTZ_Angle_2->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionMTZ_Angle_3->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        lblTextProtectionMTZ_Angle_4->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM16->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM16->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM16->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\241 \320\277\321\203\321\201\320\272\320\276\320\274", nullptr));
        cboxM16->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM16->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM17->setInputMask(QString());
        leM17->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM07->setInputMask(QString());
        leM07->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK20->setInputMask(QString());
        leK20->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM18->setInputMask(QString());
        leM18->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX05->setInputMask(QString());
        leX05->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX05a->setInputMask(QString());
        leX05a->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionEarthly_OZZ1->setText(QApplication::translate("ConfiguratorWindow", "\320\236\320\227\320\2271", nullptr));
        pbtnProtectionEarthly_OZZ2->setText(QApplication::translate("ConfiguratorWindow", "\320\236\320\227\320\2272", nullptr));
        pbtnProtectionEarthly_NZZ1->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\227\320\2271", nullptr));
        pbtnProtectionEarthly_NZZ2->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\227\320\2272", nullptr));
        gboxProtectionPropertiesEarthly_OZZ1->setTitle(QString());
        lblTextProtectionEarthly_OZZ1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionEarthly_OZZ1_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionEarthly_OZZ1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionEarthly_OZZ1_KvzCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxM22->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM22->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM22->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM22->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM23->setInputMask(QString());
        leM23->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM24->setInputMask(QString());
        leM24->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX07->setInputMask(QString());
        leX07->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesEarthly_OZZ2->setTitle(QString());
        lblTextProtectionEarthly_OZZ2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionEarthly_OZZ2_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionEarthly_OZZ2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionEarthly_OZZ2_KvzCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxK23->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK23->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK23->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK23->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK24->setInputMask(QString());
        leK24->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK25->setInputMask(QString());
        leK25->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX07a->setInputMask(QString());
        leX07a->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesEarthly_NZZ1->setTitle(QString());
        lblTextProtectionEarthly_NZZ1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionEarthly_NZZ1_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionEarthly_NZZ1_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionEarthly_NZZ1_Angle->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\263\320\276\320\273", nullptr));
        lblTextProtectionEarthly_NZZ1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionEarthly_NZZ1_KvzCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        lblTextProtectionEarthly_NZZ1_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265", nullptr));
        cboxM25->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM25->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM25->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM25->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM26->setInputMask(QString());
        leM26->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM27->setInputMask(QString());
        leM27->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK21->setInputMask(QString());
        leK21->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM28->setInputMask(QString());
        leM28->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX08->setInputMask(QString());
        leX08->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX09->setInputMask(QString());
        leX09->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesEarthly_NZZ2->setTitle(QString());
        lblTextProtectionEarthly_NZZ2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionEarthly_NZZ2_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionEarthly_NZZ2_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionEarthly_NZZ2_Angle->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\263\320\276\320\273", nullptr));
        lblTextProtectionEarthly_NZZ2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionEarthly_NZZ2_KvzCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        lblTextProtectionEarthly_NZZ2_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200\321\217\320\266\320\265\320\275\320\270\320\265", nullptr));
        cboxK26->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK26->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK26->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK26->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK27->setInputMask(QString());
        leK27->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK28->setInputMask(QString());
        leK28->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK29->setInputMask(QString());
        leK29->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK30->setInputMask(QString());
        leK30->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX08a->setInputMask(QString());
        leX08a->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX09a->setInputMask(QString());
        leX09a->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionPower_Umax1->setText(QApplication::translate("ConfiguratorWindow", "Umax1", nullptr));
        pbtnProtectionPower_Umax2->setText(QApplication::translate("ConfiguratorWindow", "Umax2", nullptr));
        pbtnProtectionPower_Umin1->setText(QApplication::translate("ConfiguratorWindow", "Umin1", nullptr));
        pbtnProtectionPower_Umin2->setText(QApplication::translate("ConfiguratorWindow", "Umin2", nullptr));
        pbtnProtectionPower_3UO->setText(QApplication::translate("ConfiguratorWindow", "3UO", nullptr));
        gboxProtectionPropertiesPower_Umax1->setTitle(QString());
        lblTextProtectionPower_Umax1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionPower_Umax1_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_Umax1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionPower_Umax1_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM32->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM32->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM32->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\321\213\320\265", nullptr));
        cboxM32->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\244\320\260\320\267\320\275\321\213\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM32->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM33->setInputMask(QString());
        leM33->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM34->setInputMask(QString());
        leM34->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX11->setInputMask(QString());
        leX11->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesPower_Umax2->setTitle(QString());
        lblTextProtectionPower_Umax2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionPower_Umax2_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_Umax2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionPower_Umax2_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM35->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM35->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM35->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\321\213\320\265", nullptr));
        cboxM35->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\244\320\260\320\267\320\275\321\213\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM35->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM36->setInputMask(QString());
        leM36->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM37->setInputMask(QString());
        leM37->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX12->setInputMask(QString());
        leX12->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesPower_Umin1->setTitle(QString());
        lblTextProtectionPower_Umin1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionPower_Umin1_Logic->setText(QApplication::translate("ConfiguratorWindow", "\320\233\320\276\320\263\320\270\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin1_StartCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin1_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin1_StartPower_2->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin1_StartPower_3->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM38->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM38->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM38->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\321\213\320\265", nullptr));
        cboxM38->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\244\320\260\320\267\320\275\321\213\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM38->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxM39->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM39->setItemText(1, QApplication::translate("ConfiguratorWindow", "-\320\230-", nullptr));
        cboxM39->setItemText(2, QApplication::translate("ConfiguratorWindow", "-\320\230\320\233\320\230-", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM39->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxM40->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM40->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM40->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM40->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM41->setInputMask(QString());
        leM41->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM42->setInputMask(QString());
        leM42->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX13->setInputMask(QString());
        leX13->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesPower_Umin2->setTitle(QString());
        lblTextProtectionPower_Umin2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionPower_Umin2_Logic->setText(QApplication::translate("ConfiguratorWindow", "\320\233\320\276\320\263\320\270\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin2_StartCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin2_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin2_StartPower_2->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionPower_Umin2_StartPower_3->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM43->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM43->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM43->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\233\320\270\320\275\320\265\320\271\320\275\321\213\320\265", nullptr));
        cboxM43->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\244\320\260\320\267\320\275\321\213\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM43->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxM44->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM44->setItemText(1, QApplication::translate("ConfiguratorWindow", "-\320\230-", nullptr));
        cboxM44->setItemText(2, QApplication::translate("ConfiguratorWindow", "-\320\230\320\233\320\230-", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM44->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxM45->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM45->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM45->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM45->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM46->setInputMask(QString());
        leM46->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM47->setInputMask(QString());
        leM47->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX14->setInputMask(QString());
        leX14->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesPower_3UO->setTitle(QString());
        lblTextProtectionPower_3UO_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionPower_3UO_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionPower_3UO_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionPower_3UO_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM48->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM48->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM48->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM48->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM49->setInputMask(QString());
        leM49->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM50->setInputMask(QString());
        leM50->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX15->setInputMask(QString());
        leX15->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionMotorStarting->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\203\321\201\320\272\320\276\320\262\320\260\321\217", nullptr));
        pbtnProtectionMotorImin->setText(QApplication::translate("ConfiguratorWindow", "Imin", nullptr));
        gboxProtectionPropertiesMotor_StartingCurrent->setTitle(QString());
        lblTextProtectionMotor_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMotor_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMotor_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionMotor_Kvz_Current->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxM19->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM19->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM19->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM19->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM20->setInputMask(QString());
        leM20->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM21->setInputMask(QString());
        leM21->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX06->setInputMask(QString());
        leX06->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesMotor_Imin->setTitle(QString());
        lblTextProtectionMotor_Imin_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMotor_StartPower->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionMotor_Pause_2->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionMotor_Kvz_Power->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM29->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM29->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM29->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM29->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM30->setInputMask(QString());
        leM30->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM31->setInputMask(QString());
        leM31->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX10->setInputMask(QString());
        leX10->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionFrequency_ACR1->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\247\320\2401", nullptr));
        pbtnProtectionFrequency_ACR2->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\247\320\2402", nullptr));
        pbtnProtectionFrequency_ACR3->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\247\320\2403", nullptr));
        gboxProtectionPropertiesFrequency_ACR1->setTitle(QString());
        lblTextProtectionFrequency_ACR1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionFrequency_ACR1_StartFreq->setText(QApplication::translate("ConfiguratorWindow", "\320\247\320\260\321\201\321\202 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionFrequency_ACR1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionFrequency_ACR1_Umin->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\274\320\270\320\275", nullptr));
        lblTextProtectionFrequency_ACR1_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM51->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM51->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM51->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM51->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM52->setInputMask(QString());
        leM52->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM53->setInputMask(QString());
        leM53->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM54->setInputMask(QString());
        leM54->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX16->setInputMask(QString());
        leX16->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesFrequency_ACR2->setTitle(QString());
        lblTextProtectionFrequency_ACR2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionFrequency_ACR2_StartFreq->setText(QApplication::translate("ConfiguratorWindow", "\320\247\320\260\321\201\321\202 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionFrequency_ACR2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionFrequency_ACR2_Umin->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\274\320\270\320\275", nullptr));
        lblTextProtectionFrequency_ACR2_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM55->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM55->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM55->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM55->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM56->setInputMask(QString());
        leM56->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM57->setInputMask(QString());
        leM57->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM58->setInputMask(QString());
        leM58->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX17->setInputMask(QString());
        leX17->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesFrequency_ACR3->setTitle(QString());
        lblTextProtectionFrequency_ACR3_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionFrequency_ACR3_StartFreq->setText(QApplication::translate("ConfiguratorWindow", "\320\247\320\260\321\201\321\202 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionFrequency_ACR3_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionFrequency_ACR3_Umin->setText(QApplication::translate("ConfiguratorWindow", "\320\235\320\260\320\277\321\200 \320\274\320\270\320\275", nullptr));
        lblTextProtectionFrequency_ACR3_KvzPower->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        cboxM59->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM59->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM59->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM59->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM60->setInputMask(QString());
        leM60->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM61->setInputMask(QString());
        leM61->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM62->setInputMask(QString());
        leM62->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX18->setInputMask(QString());
        leX18->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionExternal_Arc->setText(QApplication::translate("ConfiguratorWindow", "\320\224\321\203\320\263\320\276\320\262\320\260\321\217", nullptr));
        pbtnProtectionExternal1->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\275\320\265\321\210\320\275\321\217\321\2171", nullptr));
        pbtnProtectionExternal2->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\275\320\265\321\210\320\275\321\217\321\2172", nullptr));
        pbtnProtectionExternal3->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\275\320\265\321\210\320\275\321\217\321\2173", nullptr));
        gboxProtectionPropertiesExternalArc->setTitle(QString());
        lblTextProtectionExternal_Arc_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionExternal_Arc_StartCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionExtermal_Arc_KvzCurrent->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\276\320\272", nullptr));
        cboxM63->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM63->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM63->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));
        cboxM63->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\241 \320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\272\320\276\320\271 \320\277\320\276 \321\202\320\276\320\272\321\203", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM63->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM64->setInputMask(QString());
        leM64->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX19->setInputMask(QString());
        leX19->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesExternal1->setTitle(QString());
        lblTextProtectionExternal_Ext1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionExternal_Ext1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        cboxM71->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM71->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM71->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM71->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM72->setInputMask(QString());
        leM72->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesExternal2->setTitle(QString());
        lblTextProtectionExternal_Ext2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionExternal_Ext2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        cboxM73->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM73->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM73->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM73->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM74->setInputMask(QString());
        leM74->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesExternal3->setTitle(QString());
        lblTextProtectionExternal_Ext3_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionExternal_Ext3_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        cboxM75->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM75->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM75->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM75->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM76->setInputMask(QString());
        leM76->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionTemp1->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\265\320\274\320\277 1", nullptr));
        pbtnProtectionTemp2->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\265\320\274\320\277 2", nullptr));
        gboxProtectionPropertiesTemperature1->setTitle(QString());
        lblTextProtectionTemp1_Sensor1->setText(QApplication::translate("ConfiguratorWindow", "\320\224\320\260\321\202\321\207\320\270\320\272 1", nullptr));
        lblTextProtectionTemp1_Sensor2->setText(QApplication::translate("ConfiguratorWindow", "\320\224\320\260\321\202\321\207\320\270\320\272 2", nullptr));
        lblTextProtectionTemp1_StartTemp->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\274\320\277 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionTemp1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionTemp1_KvzTemp->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\274\320\277", nullptr));
        cboxM65->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM65->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM65->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM65->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxM66->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM66->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM66->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM66->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM67->setInputMask(QString());
        leM67->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM69->setInputMask(QString());
        leM69->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX20->setInputMask(QString());
        leX20->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesTemperature2->setTitle(QString());
        lblTextProtectionTemp2_Sensor1->setText(QApplication::translate("ConfiguratorWindow", "\320\224\320\260\321\202\321\207\320\270\320\272 1", nullptr));
        lblTextProtectionTemp2_Sensor2->setText(QApplication::translate("ConfiguratorWindow", "\320\224\320\260\321\202\321\207\320\270\320\272 2", nullptr));
        lblTextProtectionTemp2_StartTemp->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\274\320\277 \320\277\321\203\321\201\320\272\320\260", nullptr));
        lblTextProtectionTemp2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        lblTextProtectionTemp2_KvzTemp->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \321\202\320\274\320\277", nullptr));
        cboxProtectionTemp2_Sensor1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxProtectionTemp2_Sensor1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxProtectionTemp2_Sensor1->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxProtectionTemp2_Sensor1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxProtectionTemp2_Sensor2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxProtectionTemp2_Sensor2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxProtectionTemp2_Sensor2->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxProtectionTemp2_Sensor2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM68->setInputMask(QString());
        leM68->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM70->setInputMask(QString());
        leM70->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leX21->setInputMask(QString());
        leX21->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnProtectionLevel1->setText(QApplication::translate("ConfiguratorWindow", "\320\243\321\200\320\276\320\262 1", nullptr));
        pbtnProtectionLevel2->setText(QApplication::translate("ConfiguratorWindow", "\320\243\321\200\320\276\320\262 2", nullptr));
        pbtnProtectionLevelSignStart->setText(QApplication::translate("ConfiguratorWindow", "\320\241\320\270\320\263\320\275. \320\277\321\203\321\201\320\272\320\260", nullptr));
        gboxProtectionPropertiesLevel1->setTitle(QString());
        lblTextProtectionLevel1_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionLevel1_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        cboxM77->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM77->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM77->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\320\276\320\273\321\214 \320\222\320\222", nullptr));
        cboxM77->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272", nullptr));
        cboxM77->setItemText(4, QApplication::translate("ConfiguratorWindow", "\320\232.\320\230.\320\242", nullptr));
        cboxM77->setItemText(5, QApplication::translate("ConfiguratorWindow", "\320\232.\320\230\320\233\320\230.\320\242", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM77->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM78->setInputMask(QString());
        leM78->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxProtectionPropertiesLevel2->setTitle(QString());
        lblTextProtectionLevel2_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionLevel2_Pause->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\277\321\203\321\201\320\272\320\260", nullptr));
        cboxProtectionLeve2_Ctrl->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxProtectionLeve2_Ctrl->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxProtectionLeve2_Ctrl->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214 \320\222\320\222", nullptr));
        cboxProtectionLeve2_Ctrl->setItemText(3, QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272", nullptr));
        cboxProtectionLeve2_Ctrl->setItemText(4, QApplication::translate("ConfiguratorWindow", "\320\232.\320\230.\320\242", nullptr));
        cboxProtectionLeve2_Ctrl->setItemText(5, QApplication::translate("ConfiguratorWindow", "\320\232.\320\230\320\233\320\230.\320\242", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxProtectionLeve2_Ctrl->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM79->setInputMask(QString());
        leM79->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextStartSignal_1->setText(QApplication::translate("ConfiguratorWindow", "\320\230\320\267\320\274\320\265\321\200 \320\221\320\240\320\243", nullptr));
        lblTextStartSignal_2->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\201\320\272 \320\222\320\260\320\272 \321\204\320\220", nullptr));
        lblTextStartSignal_3->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\201\320\272 \320\222\320\260\320\272 \321\204\320\222", nullptr));
        lblTextStartSignal_4->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\201\320\272 \320\222\320\260\320\272 \321\204\320\241", nullptr));
        lblTextStartSignal_5->setText(QApplication::translate("ConfiguratorWindow", "\320\240\320\260\320\261\320\276\321\202\320\260 \320\222\320\260\320\272", nullptr));
        lblTextStartSignal_6->setText(QApplication::translate("ConfiguratorWindow", "\320\240\320\260\320\261\320\276\321\202\320\260 \320\221\320\240\320\243", nullptr));
        lblTextStartSignal_7->setText(QApplication::translate("ConfiguratorWindow", "\320\221\320\273. \320\222\320\232\320\233 \320\221\320\240\320\243", nullptr));
        lblTextStartSignal_8->setText(QApplication::translate("ConfiguratorWindow", "R\320\270\320\267 \320\275\320\276\321\200\320\274\320\260", nullptr));
        cboxMeasBRU->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxMeasBRU->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxMeasBRU->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxfA->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxfA->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxfA->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxfB->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxfB->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxfB->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxfC->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxfC->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxfC->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxWorkVak->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxWorkVak->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxWorkVak->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxWorkBRU->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxWorkBRU->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxWorkBRU->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOnBRU->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOnBRU->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOnBRU->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxRmeasOK->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxRmeasOK->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxRmeasOK->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextStartSignal_9->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\232\320\233 \320\276\321\202 \320\221\320\240\320\243", nullptr));
        lblTextStartSignal_10->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2271", nullptr));
        lblTextStartSignal_11->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2272", nullptr));
        lblTextStartSignal_12->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2273", nullptr));
        lblTextStartSignal_13->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2274", nullptr));
        lblTextStartSignal_14->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\237\321\203\321\201\320\272\320\276\320\262", nullptr));
        lblTextStartSignal_15->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\236\320\227\320\2271", nullptr));
        lblTextStartSignal_16->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\235\320\227\320\2271", nullptr));
        cboxOnBRU_2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOnBRU_2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOnBRU_2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffMTZ1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffMTZ1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffMTZ1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffMTZ2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffMTZ2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffMTZ2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffMTZ3->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffMTZ3->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffMTZ3->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffMTZ4->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffMTZ4->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffMTZ4->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffStart->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffStart->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffStart->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffOZZ1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffOZZ1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffOZZ1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffNZZ1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffNZZ1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffNZZ1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextStartSignal_17->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Imin", nullptr));
        lblTextStartSignal_18->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umax1", nullptr));
        lblTextStartSignal_19->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umax2", nullptr));
        lblTextStartSignal_20->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umin1", nullptr));
        lblTextStartSignal_21->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umin2", nullptr));
        lblTextStartSignal_22->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 3UO", nullptr));
        lblTextStartSignal_23->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\222\320\240", nullptr));
        lblTextStartSignal_24->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\247\320\2401", nullptr));
        cboxOffImin->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffImin->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffImin->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffImax1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffImax1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffImax1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffImax2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffImax2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffImax2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffImin1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffImin1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffImin1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffImin2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffImin2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffImin2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffI3UO->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffI3UO->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffI3UO->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffAVR->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffAVR->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffAVR->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffACR1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffACR1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffACR1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextStartSignal_25->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\247\320\2402", nullptr));
        lblTextStartSignal_26->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\247\320\2403", nullptr));
        lblTextStartSignal_27->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\242\320\274\320\2771", nullptr));
        lblTextStartSignal_28->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\242\320\274\320\2772", nullptr));
        lblTextStartSignal_29->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\222\320\2751", nullptr));
        lblTextStartSignal_30->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\222\320\2752", nullptr));
        lblTextStartSignal_31->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\222\320\2753", nullptr));
        lblTextStartSignal_32->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\243\320\2401", nullptr));
        cboxOffACR2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffACR2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffACR2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffACR3->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffACR3->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffACR3->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffTemp1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffTemp1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffTemp1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffTemp2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffTemp2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffTemp2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffExt1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffExt1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffExt1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffExt2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffExt2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffExt2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffExt3->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffExt3->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffExt3->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        cboxOffLevel1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffLevel1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffLevel1->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextStartSignal_33->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\243\320\2402", nullptr));
        cboxOffLevel2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxOffLevel2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxOffLevel2->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214", nullptr));
#endif // QT_NO_TOOLTIP
        groupBox_11->setTitle(QString());
        lblTextProtectionMTZ_Ctrl_5->setText(QApplication::translate("ConfiguratorWindow", "\320\237\320\265\321\200\320\265\320\264 \320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265\320\274", nullptr));
        lblTextProtectionMTZ_Timeout_4->setText(QApplication::translate("ConfiguratorWindow", "T \320\270\320\274\320\265\321\200\320\265\320\275", nullptr));
        cboxM93->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM93->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM93->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM93->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM96->setInputMask(QString());
        leM96->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextProtectionMTZ_StartCurrent_5->setText(QApplication::translate("ConfiguratorWindow", "\320\237\320\265\321\200\320\270\320\276\320\264\320\270\321\207\320\265\321\201\320\272\320\270", nullptr));
        leM97->setInputMask(QString());
        leM97->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextProtectionMTZ_EnterBlocking_3->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260 \320\276\321\202\320\272\320\273", nullptr));
        lblTextProtectionMTZ_EnterBlocking_2->setText(QApplication::translate("ConfiguratorWindow", "\320\243\321\201\321\202\320\260\320\262\320\272\320\260", nullptr));
        cboxM95->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM95->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM95->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM95->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leM98->setInputMask(QString());
        leM98->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextProtectionMTZ_EnterBlocking_4->setText(QApplication::translate("ConfiguratorWindow", "\320\237\320\265\321\200\320\270\320\276\320\264\320\270\321\207\320\275\320\276\321\201\321\202\321\214", nullptr));
        leM99->setInputMask(QString());
        leM99->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        groupBox_12->setTitle(QString());
        leM91->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextProtectionMTZ_Ctrl_8->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        cboxM90->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM90->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM90->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxM90->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextProtectionMTZ_Ctrl_6->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextProtectionMTZ_Ctrl_7->setText(QApplication::translate("ConfiguratorWindow", "\320\243\321\201\321\202\320\260\320\262\320\272\320\260", nullptr));
        leM92->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblTextProtectionMTZ_Ctrl_9->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\262\320\267 \320\275\320\260\320\277\321\200", nullptr));
        leX23->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnSwDevBreaker->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\213\320\272\320\273\321\216\321\207\320\260\321\202.", nullptr));
        pbtnSwDevBlock->setText(QApplication::translate("ConfiguratorWindow", "\320\221\320\273\320\276\320\272\320\270\321\200\320\276\320\262.", nullptr));
        pbtnSwDevSCHR->setText(QApplication::translate("ConfiguratorWindow", "\320\250\320\240", nullptr));
        pbtnSwDevLR->setText(QApplication::translate("ConfiguratorWindow", "\320\233\320\240", nullptr));
        pbtnSwDevZR->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\240", nullptr));
        pbtnSwDevTruck->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\265\320\273\320\265\320\266\320\272\320\260", nullptr));
        pbtnSwDevCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\321\200.\320\242\320\235", nullptr));
        groupBox->setTitle(QString());
        lblTextSwitchON->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\260 \320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217", nullptr));
        lblCtrlOT->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214 \320\236\320\242", nullptr));
        lblT_KCU->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\276\320\272 \320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\275\320\276\320\263\320\276 \321\201\320\276\321\201\321\202\320\276\321\217\320\275\320\270\321\217", nullptr));
        leK09->setInputMask(QString());
        leK09->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        cboxK06->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK06->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK06->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK06->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        lblKcu->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\246\320\243", nullptr));
        lblPosVV->setText(QApplication::translate("ConfiguratorWindow", "\320\236\320\277\321\200\320\265\320\264\320\265\320\273\320\265\320\275\320\270\320\265 \320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\321\217 \320\222\320\222", nullptr));
        cboxK32->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK32->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\234\320\265\321\201\321\202", nullptr));
        cboxK32->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\224\320\270\321\201\321\202", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK32->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextSwBreakerModeCtrl->setText(QApplication::translate("ConfiguratorWindow", "\320\240\320\265\320\266\320\270\320\274 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        cboxK17->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK17->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK17->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK17->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextSwitchOFF->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\260 \320\276\321\202\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217", nullptr));
        cboxK07->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK07->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276 \320\221\320\232 \320\222\320\222", nullptr));
        cboxK07->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\237\320\276 \320\272\320\260\321\202\321\203\321\210\320\272\320\260\320\274 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK07->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK01->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK01->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\260\321\217", nullptr));
        cboxK01->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201\320\275\320\260\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK01->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK02->setInputMask(QString());
        leK02->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK04->setInputMask(QString());
        leK04->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        cboxK03->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK03->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\260\321\217", nullptr));
        cboxK03->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201\320\275\320\260\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK03->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        lblTextSwitchON_T->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\272\320\276\320\274\320\260\320\274\320\275\320\264\321\213 \320\262\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217", nullptr));
        lblTextSwitchOFF_T->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \320\276\321\202\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217", nullptr));
        lblTotv->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\261\321\200\320\260\321\202\320\275\320\276\320\271 \321\201\320\262\321\217\320\267\320\270", nullptr));
        leK05->setInputMask(QString());
        leK05->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        lblI_VV_on->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\275\320\265\320\270\321\201\320\277\321\200\320\260\320\262\320\275\320\276\321\201\321\202\320\270", nullptr));
        lblKv_KCU_I->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202 \320\262\320\276\320\267\320\262\321\200\320\260\321\202. \321\202\320\276\320\272\320\260", nullptr));
        leX22->setInputMask(QString());
        leX22->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK08->setInputMask(QString());
        leK08->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxSwDevDisconnect->setTitle(QString());
        lblTextVk_VV_tel->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265 \320\222\320\222 \320\277\320\276 \320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\321\216 \321\202\320\265\320\273\320\265\320\266\320\272\320\270", nullptr));
        lblTextVk_VV_tel_2->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\265 \320\222\320\222 \320\277\320\276 \320\227\320\240", nullptr));
        lblTextVk_VV_tel_3->setText(QApplication::translate("ConfiguratorWindow", "\320\221\320\273\320\276\320\272 \321\200\320\260\320\267\321\212\320\265\320\264\320\270\320\275\320\270\321\202\320\265\320\273\320\265\320\271", nullptr));
        cboxK13->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK13->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK13->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK13->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK14->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK14->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK14->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK14->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK15->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK15->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK15->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK15->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        gboxSwDevSCHR->setTitle(QString());
        lblTextSCHR_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextSCHR_Cmd->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\260", nullptr));
        lblTextSCHR_Timp->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        lblTextSCHR_Toff->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217 \320\276\321\202\320\262\320\265\321\202\320\260", nullptr));
        cboxK34->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK34->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\234\320\265\321\201\321\202\320\275\320\276\320\265", nullptr));
        cboxK34->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\224\320\270\321\201\321\202\320\260\320\275\321\206\320\270\320\276\320\275\320\275\320\276\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK34->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK38->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK38->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\260\321\217", nullptr));
        cboxK38->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201\320\275\320\260\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK38->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK42->setInputMask(QString());
        leK42->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK46->setInputMask(QString());
        leK46->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxSwDevLR->setTitle(QString());
        lblTextLR_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextLR_Cmd->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\260", nullptr));
        lblTextLR_Timp->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        lblTextLR_Toff->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217 \320\276\321\202\320\262\320\265\321\202\320\260", nullptr));
        cboxK35->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK35->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\234\320\265\321\201\321\202\320\275\320\276\320\265", nullptr));
        cboxK35->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\224\320\270\321\201\321\202\320\260\320\275\321\206\320\270\320\276\320\275\320\275\320\276\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK35->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK39->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK39->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\260\321\217", nullptr));
        cboxK39->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201\320\275\320\260\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK39->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK43->setInputMask(QString());
        leK43->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK47->setInputMask(QString());
        leK47->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxSwDevZR->setTitle(QString());
        lblTextZR_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextZR_Cmd->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\260", nullptr));
        lblTextZR_Timp->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        lblTextZR_Toff->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217 \320\276\321\202\320\262\320\265\321\202\320\260", nullptr));
        cboxK36->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK36->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\234\320\265\321\201\321\202\320\275\320\276\320\265", nullptr));
        cboxK36->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\224\320\270\321\201\321\202\320\260\320\275\321\206\320\270\320\276\320\275\320\275\320\276\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK36->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK40->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK40->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\260\321\217", nullptr));
        cboxK40->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201\320\275\320\260\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK40->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK44->setInputMask(QString());
        leK44->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK48->setInputMask(QString());
        leK48->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxSwDevTruck->setTitle(QString());
        lblTextTruck_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        lblTextTruck_Cmd->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\274\320\260\320\275\320\264\320\260", nullptr));
        lblTextTruck_Timp->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\272\320\276\320\274\320\260\320\275\320\264\321\213 \321\203\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\321\217", nullptr));
        lblTextTruck_Toff->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217 \320\276\321\202\320\262\320\265\321\202\320\260", nullptr));
        cboxK37->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK37->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\234\320\265\321\201\321\202\320\275\320\276\320\265", nullptr));
        cboxK37->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\224\320\270\321\201\321\202\320\260\320\275\321\206\320\270\320\276\320\275\320\275\320\276\320\265", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK37->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        cboxK41->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK41->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\237\320\276\321\201\321\202\320\276\321\217\320\275\320\275\320\260\321\217", nullptr));
        cboxK41->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201\320\275\320\260\321\217", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK41->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leK45->setInputMask(QString());
        leK45->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leK49->setInputMask(QString());
        leK49->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        gboxSwDevCtrl->setTitle(QString());
        lblTextCtrl_Ctrl->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\321\200\320\276\320\273\321\214 \320\242\320\235 \320\277\320\276 \320\224\320\222", nullptr));
        lblTextTruck_Toff_2->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        cboxK18->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK18->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK18->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

#ifndef QT_NO_TOOLTIP
        cboxK18->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\230\320\274\321\217 \320\277\320\276\321\200\321\202\320\260", nullptr));
#endif // QT_NO_TOOLTIP
        leT02->setInputMask(QString());
        leT02->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        pbtnAddAVR->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\222\320\240 \320\262\320\262\320\276\320\264\320\260", nullptr));
        pbtnAddAVRSection->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\222\320\240 \321\201\320\265\320\272\321\206\320\270\321\217", nullptr));
        pbtnAddAPV->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\237\320\222", nullptr));
        pbtnAddAPV_Start->setText(QApplication::translate("ConfiguratorWindow", "\320\220\320\237\320\222:\320\277\321\203\321\201\320\272", nullptr));
        groupBox_2->setTitle(QString());
        label_4->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\264\320\265\321\200\320\266\320\272\320\260", nullptr));
        leM84->setInputMask(QString());
        leM84->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        leM82->setInputMask(QString());
        leM82->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        label->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        cboxM81->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM81->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM81->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

        label_2->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\321\200 U\321\200\320\260\320\261", nullptr));
        label_3->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\275\321\202\321\200 U\321\200\320\267\321\200", nullptr));
        leM83->setInputMask(QString());
        leM83->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        label_5->setText(QApplication::translate("ConfiguratorWindow", "T \320\276\320\264\320\275\320\276\320\272\321\200\320\260\321\202", nullptr));
        leM85->setInputMask(QString());
        leM85->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        label_12->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        cboxK33->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxK33->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxK33->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

        groupBox_4->setTitle(QString());
        label_7->setText(QApplication::translate("ConfiguratorWindow", "\320\242 \320\261\320\273\320\272 \320\262\320\272\320\273", nullptr));
        cboxM87->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202 \320\267\320\275.", nullptr));
        cboxM87->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273", nullptr));
        cboxM87->setItemText(2, QApplication::translate("ConfiguratorWindow", "\320\222\320\272\320\273", nullptr));

        label_6->setText(QApplication::translate("ConfiguratorWindow", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265", nullptr));
        leM88->setInputMask(QString());
        leM88->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        label_8->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\200\320\265\320\274\321\217 \320\220\320\237\320\222", nullptr));
        leM89->setInputMask(QString());
        leM89->setText(QApplication::translate("ConfiguratorWindow", "0", nullptr));
        groupBox_10->setTitle(QString());
        cboxAdditionalAPV_StartRmeasOK->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartRmeasOK->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartBlockOnBRU->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartBlockOnBRU->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_39->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\222\320\2753", nullptr));
        cboxAdditionalAPV_StartOffExt1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffExt1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_38->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\222\320\2752", nullptr));
        cboxAdditionalAPV_StartOffExt3->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffExt3->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffExt2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffExt2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_40->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\243\320\2401", nullptr));
        cboxAdditionalAPV_StartOffAVR->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffAVR->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffACHR3->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffACHR3->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffTemp1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffTemp1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_25->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2274", nullptr));
        label_26->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Imin", nullptr));
        label_27->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\237\321\203\321\201\320\272\320\276\320\262", nullptr));
        label_28->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\236\320\227\320\2271", nullptr));
        cboxAdditionalAPV_StartOffUmax1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffUmax1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffUmax2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffUmax2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffUmin1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffUmin1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_29->setText(QApplication::translate("ConfiguratorWindow", "\320\240\320\260\320\261\320\276\321\202\320\260 \320\221\320\240\320\243", nullptr));
        label_30->setText(QApplication::translate("ConfiguratorWindow", "\320\240\320\260\320\261\320\276\321\202\320\260 \320\222\320\260\320\272", nullptr));
        label_31->setText(QApplication::translate("ConfiguratorWindow", "R\320\270\320\267 \320\275\320\276\321\200\320\274\320\260", nullptr));
        cboxAdditionalAPV_StartBRUWork->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartBRUWork->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartVacWork->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartVacWork->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffLev1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffLev1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffACHR1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffACHR1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOff3UO->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOff3UO->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_32->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2273", nullptr));
        label_33->setText(QApplication::translate("ConfiguratorWindow", "\320\222\320\232\320\233 \320\276\321\202 \320\221\320\240\320\243", nullptr));
        cboxAdditionalAPV_StartOffMTZ1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffMTZ1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffMTZ2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffMTZ2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffStart->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffStart->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_34->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\235\320\227\320\2271", nullptr));
        label_35->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 3UO", nullptr));
        label_36->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umin1", nullptr));
        cboxAdditionalAPV_StartOffUmin2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffUmin2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_37->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\247\320\2401", nullptr));
        label_41->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\222\320\240", nullptr));
        cboxAdditionalAPV_StartOffOZZ1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffOZZ1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffNZZ1->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffNZZ1->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_42->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\243\320\2402", nullptr));
        label_43->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umin2", nullptr));
        cboxAdditionalAPV_StartOffMTZ4->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffMTZ4->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_44->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\222\320\2751", nullptr));
        cboxAdditionalAPV_StartOnFromBRU->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOnFromBRU->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_45->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2272", nullptr));
        label_46->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\234\320\242\320\2271", nullptr));
        label_47->setText(QApplication::translate("ConfiguratorWindow", "\320\230\320\267\320\274\320\265\321\200 \320\221\320\240\320\243", nullptr));
        label_48->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\201\320\272 \320\222\320\260\320\272 \321\204\320\222", nullptr));
        label_49->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\201\320\272 \320\222\320\260\320\272 \321\204\320\220", nullptr));
        cboxAdditionalAPV_StartPskVacPhaseC->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartPskVacPhaseC->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartMeasBRU->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartMeasBRU->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_50->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\247\320\2402", nullptr));
        cboxAdditionalAPV_StartPskVacPhaseA->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartPskVacPhaseA->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartPskVacPhaseB->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartPskVacPhaseB->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffACHR2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffACHR2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_51->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\242\320\274\320\2771", nullptr));
        label_52->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\220\320\247\320\2403", nullptr));
        label_53->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 \320\242\320\274\320\2772", nullptr));
        cboxAdditionalAPV_StartOffMTZ3->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffMTZ3->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_54->setText(QApplication::translate("ConfiguratorWindow", "\320\221\320\273.\320\222\320\232\320\233 \320\221\320\240\320\243", nullptr));
        cboxAdditionalAPV_StartOffImin->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffImin->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        label_55->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umax1", nullptr));
        label_56->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202\320\272\320\273 Umax2", nullptr));
        label_57->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\201\320\272 \320\222\320\260\320\272 \321\204\320\241", nullptr));
        cboxAdditionalAPV_StartOffTemp2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffTemp2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        cboxAdditionalAPV_StartOffLev2->setItemText(0, QApplication::translate("ConfiguratorWindow", "\320\235\320\265\321\202", nullptr));
        cboxAdditionalAPV_StartOffLev2->setItemText(1, QApplication::translate("ConfiguratorWindow", "\320\224\320\260", nullptr));

        pbtnEventJournalReadToTable->setText(QApplication::translate("ConfiguratorWindow", "\320\247\321\202\320\265\320\275\320\270\320\265 \320\266\321\203\321\200\320\275\320\260\320\273\320\260", nullptr));
        pbtnEventJournalTableClear->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\207\320\270\321\201\321\202\320\272\320\260", nullptr));
        checkboxEventJournalScrollTable->setText(QApplication::translate("ConfiguratorWindow", "\320\237\321\200\320\276\320\272\321\200\321\203\321\202\320\272\320\260 \321\202\320\260\320\261\320\273\320\270\321\206\321\213", nullptr));
        groupboxEventJournalReadInterval->setTitle(QApplication::translate("ConfiguratorWindow", "\320\247\321\202\320\265\320\275\320\270\320\265 \320\270\320\275\321\202\320\265\321\200\320\262\320\260\320\273\320\260 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", nullptr));
        radiobtnEventJournalInterval->setText(QApplication::translate("ConfiguratorWindow", "\320\230\320\275\321\202\320\265\321\200\320\262\320\260\320\273", nullptr));
        radiobtnEventJournalDate->setText(QApplication::translate("ConfiguratorWindow", "\320\224\320\260\321\202\320\260", nullptr));
        label_13->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\202", nullptr));
        label_14->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\267\320\260\320\277\320\270\321\201\320\265\320\271", nullptr));
        toolbtnEventJournalCalendarOpen->setText(QString());
        label_11->setText(QApplication::translate("ConfiguratorWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", nullptr));
        label_15->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\321\202\321\200\320\260\321\207\320\265\320\275\320\275\320\276\320\265 \320\262\321\200\320\265\320\274\321\217", nullptr));
        pbtnClearLedOutput->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        pbtnClearDiscreteInput->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        pbtnClearRelayOutput->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        pbtnClearKeyboardPurpose->setText(QApplication::translate("ConfiguratorWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", nullptr));
        label_9->setText(QApplication::translate("ConfiguratorWindow", "\320\242\320\265\320\272\321\203\321\211\320\270\320\271 \320\261\320\273\320\276\320\272:", nullptr));
        pbtnReadCurrentBlock->setText(QApplication::translate("ConfiguratorWindow", "\320\247\321\202\320\265\320\275\320\270\320\265", nullptr));
        pbtnWriteCurrentBlock->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\277\320\270\321\201\321\214", nullptr));
        label_10->setText(QApplication::translate("ConfiguratorWindow", "\320\222\321\201\320\265 \320\261\320\273\320\276\320\272\320\270:", nullptr));
        pbtnReadAllBlock->setText(QApplication::translate("ConfiguratorWindow", "\320\247\321\202\320\265\320\275\320\270\320\265", nullptr));
        pbtnWriteAllBlock->setText(QApplication::translate("ConfiguratorWindow", "\320\227\320\260\320\277\320\270\321\201\321\214", nullptr));
        dockwgtDeviceMenu->setWindowTitle(QApplication::translate("ConfiguratorWindow", "\320\234\320\265\320\275\321\216 \321\203\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\260", nullptr));
#ifndef QT_NO_TOOLTIP
        tbntExpandItems->setToolTip(QApplication::translate("ConfiguratorWindow", "\320\240\320\260\320\267\320\262\320\265\321\200\320\275\321\203\321\202\321\214 \320\274\320\265\320\275\321\216", nullptr));
#endif // QT_NO_TOOLTIP
        tbntExpandItems->setText(QApplication::translate("ConfiguratorWindow", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfiguratorWindow: public Ui_ConfiguratorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGURATORWINDOW_H
