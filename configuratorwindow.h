#ifndef CONFIGURATORWINDOW_H
    #define CONFIGURATORWINDOW_H
    //--------------------
    #include <QMainWindow>
    #include <QSerialPort>
    #include <QSerialPortInfo>
    #include <QMessageBox>
    #include <QDebug>
    #include <QListWidget>
    #include <QTimer>
    #include <QSpinBox>
    #include <QButtonGroup>
    #include <QAbstractButton>
    #include <QFile>
    #include <QTreeWidget>
    #include <QTextStream>
    #include "cmodbus.h"
    #include "qpanel.h"
    #include "cterminal.h"
    #include "qcustomplot.h"
    #include "cversionsoftware.h"
    //----------
    namespace Ui 
    {
        class ConfiguratorWindow;
    }
    //-------------------------------------------
    class ConfiguratorWindow: public QMainWindow
    {
        Q_OBJECT
        
        public:
            enum RegisterAddress
            {
                CALCULATE_ADDRESS                = 64,
                IN_ANALOG_ADDRESS                = 356,
                PROTECTION_MTZ_ADDRESS           = 22,
                PROTECTION_MTZ_SET_ADDRESS       = 128,
                PROTECTION_EARTHY_SET_ADDRESS    = 166,
                PROTECTION_POWER_SET_ADDRESS     = 202,
                PROTECTION_MOTOR_SET_ADDRESS     = 232,
                PROTECTION_FREQUENCY_SET_ADDRESS = 244,
                PROTECTION_EXTERNAL_SET_ADDRESS  = 268,
                PROTECTION_TEMP_SET_ADDRESS      = 278,
                PROTECTION_LEVEL_SET_ADDRESS     = 290,
                PROTECTION_BRU_SET_ADDRESS       = 294,
                PROTECTION_VACUUM_SET_ADDRESS    = 302,
                AUTOMATION_SET_ADDRESS           = 308,
                SWITCH_DEV_SET_ADDRESS           = 320
            };

            enum RequestType
            {
                CALCULATE_TYPE,
                IN_ANALOG_TYPE,
                PROTECTION_MTZ_TYPE,
                PROTECTION_MTZ_SET_TYPE,
                PROTECTION_EARTHY_SET_TYPE,
                PROTECTION_POWER_SET_TYPE,
                PROTECTION_MOTOR_SET_TYPE,
                PROTECTION_FREQUENCY_SET_TYPE,
                PROTECTION_EXTERNAL_SET_TYPE,
                PROTECTION_TEMP_SET_TYPE,
                PROTECTION_LEVEL_SET_TYPE,
                PROTECTION_BRU_SET_TYPE,
                PROTECTION_VACUUM_SET_TYPE,
                AUTOMATION_SET_TYPE,
                SWITCH_DEV_SET_TYPE
            };
      
        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(bool state);
            void refreshSerialPort();
            void calculateRead(); // запрос расчетных величин
            void inAnalogRead();
            void inAnalogWrite();
            void protectionMTZStateRead();
            void protectionMTZStateWrite();
            void protectionMTZSetRead();
            void protectionMTZSetWrite();
            void protectionEarthySetRead();
            void protectionEarthySetWrite();
            void protectionPowerSetRead();
            void protectionPowerSetWrite();
            void protectionMotorSetRead();
            void protectionMotorSetWrite();
            void protectionFrequencySetRead();
            void protectionFrequencySetWrite();
            void protectionExternalSetRead();
            void protectionExternalSetWrite();
            void protectionTemperatureSetRead();
            void protectionTemperatureSetWrite();
            void protectionLevelSetRead();
            void protectionLevelSetWrite();
            void protectionBruSetRead();
            void protectionBruSetWrite();
            void protectionVacuumSetRead();
            void protectionVacuumSetWrite();
            void automationSetRead();
            void automationSetWrite();
            void switchDeviceSetRead();
            void switchDeviceSetWrite();
            void responseRead(CDataUnitType& unit);
            void show();
            void chboxCalculateTimeoutStateChanged(bool state);
            void timeCalculateChanged(int newTime);
            void timeoutValueChanged(int newTime);
            void numberRepeatChanged(int number);
            void protectMTZChangedID(int id);
            void protectEarthlyChangedID(int id);
            void protectPowerChangedID(int id);
            void protectMotorChangedID(int id);
            void protectFrequencyChangedID(int id);
            void protectExternalChangedID(int id);
            void protectTemperatureChangedID(int id);
            void protectLevelChangedID(int id);
            void switchDeviceChangedID(int id);
            void additionalChangedID(int id);
            void errorDevice(const QString& error);
            void terminalVisiblity(int state);
            void saveLog(const QString& info);
            void itemClicked(QTreeWidgetItem* item, int col);
            void readSettings();
            void readSetCurrent();
            void writeSettings();
            void writeSetCurrent();
            void expandItemTree(bool state);
            void versionSowftware();
            
        private:
            void initConnect();
            void initMenuPanel();
            void initButtonGroup();
            void displayCalculateValues(QVector<quint16> values);
            void displayInAnalogValues(QVector<quint16> values);
            void displayProtectionMTZStateValues(QVector<quint16> values);
            void displayProtectionMTZSetValues(QVector<quint16> values);
            void displayProtectionEarthySetValues(QVector<quint16> values);
            void displayProtectionPowerSetValues(QVector<quint16> values);
            void displayProtectionMotorSetValues(QVector<quint16> values);
            void displayProtectionFrequencySetValues(QVector<quint16> values);
            void displayProtectionExternalSetValues(QVector<quint16> values);
            void displayProtectionTemperatureSetValues(QVector<quint16> values);
            void displayProtectionLevelSetValues(QVector<quint16> values);
            void displayProtectionBruSetValues(QVector<quint16> values);
            void displayProtectionVacuumSetValues(QVector<quint16> values);
            void displayAutomationValues(QVector<quint16> values);
            void displaySwitchDeviceValues(QVector<quint16> values);
            void versionParser();
            
        private:
            Ui::ConfiguratorWindow* ui;
            CModbus*                m_modbusDevice;
            QPanel*                 m_calculateWidget;
            CTerminal*              m_terminal;
            QFile*                  m_logFile;
            QTimer*                 m_tim_calculate;
            QButtonGroup*           m_protect_mtz_group;
            QButtonGroup*           m_protect_earthly_group;
            QButtonGroup*           m_protect_power_group;
            QButtonGroup*           m_protect_motor_group;
            QButtonGroup*           m_protect_frequency_group;
            QButtonGroup*           m_protect_external_group;
            QButtonGroup*           m_protect_temperature_group;
            QButtonGroup*           m_protect_level_group;
            QButtonGroup*           m_switch_device_group;
            QButtonGroup*           m_additional_group;
            QVector<QLineEdit*>     m_calculate_cell;
            QVector<QLineEdit*>     m_in_analog_cell;
            QVector<QLineEdit*>     m_protectionMTZ_cell;
            QVector<QLineEdit*>     m_protectionEarthy_cell;
            QVector<QLineEdit*>     m_protectionPower_cell;
            QVector<QLineEdit*>     m_protectionMotor_cell;
            QVector<QLineEdit*>     m_protectionFrequency_cell;
            QVector<QLineEdit*>     m_protectionExternal_cell;
            QVector<QLineEdit*>     m_protectionTemperature_cell;
            QVector<QLineEdit*>     m_protectionLevel_cell;
            QVector<QLineEdit*>     m_protectionVacuum_cell;
            QVector<QLineEdit*>     m_protectionBru_cell;
            QVector<QLineEdit*>     m_automation_cell;
            QVector<QLineEdit*>     m_switch_device_cell;
            CVersionSoftware*       m_versionWidget;

            QTreeWidgetItem* itemSettings;
            QTreeWidgetItem* itemJournals;
            QTreeWidgetItem* itemMeasures;
            QTreeWidgetItem* itemMonitoring;
            QTreeWidgetItem* itemSetInputAnalogs;
            QTreeWidgetItem* itemSetProtections;
            QTreeWidgetItem* itemSetDevConnections;
            QTreeWidgetItem* itemSetAutomation;
            QTreeWidgetItem* itemJournalCrashs;
            QTreeWidgetItem* itemJournalEvents;
            QTreeWidgetItem* itemJournalOscill;
            QTreeWidgetItem* itemMeasPrimaryValues;
            QTreeWidgetItem* itemMeasSecondaryValues;
            QTreeWidgetItem* itemMeasPowerElectric;
            QTreeWidgetItem* itemMonitorInputDiscrets;
            QTreeWidgetItem* itemMonitorOutputDiscrets;
            QTreeWidgetItem* itemInAnalogMain;
            QTreeWidgetItem* itemInAnalogCalibration;
            QTreeWidgetItem* itemProtectCurrentMax;
            QTreeWidgetItem* itemProtectEarthy;
            QTreeWidgetItem* itemProtectPower;
            QTreeWidgetItem* itemProtectMotor;
            QTreeWidgetItem* itemProtectFrequency;
            QTreeWidgetItem* itemProtectExternal;
            QTreeWidgetItem* itemProtectTemperature;
            QTreeWidgetItem* itemProtectLevel;
            QTreeWidgetItem* itemProtectBRU;
            QTreeWidgetItem* itemProtectVacuum;
    };
#endif // CONFIGURATORWINDOW_H
