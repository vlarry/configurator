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
    #include "cmatrixpurposemodel.h"
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
                CALCULATE_ADDRESS = 64
            };

            enum RequestType
            {
                CALCULATE_TYPE,
                IN_ANALOG_TYPE,
                CONTROL_SET_TYPE,
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
                SWITCH_DEV_SET_TYPE,
                GENERAL_TYPE
            };
            //-------------
            enum WidgetType
            {
                INT,
                FLOAT, // QLineEdit (set validator: QDoubleValidator)
                LIST   // QComboBox
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
            void controlStateRead();
            void controlStateWrite();
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
            void sendReadRequest(const QString& first_key, const QString& second_key,
                                 CDataUnitType::FunctionType type, int size);
            void sendWriteRequest(const QString& first_key, const QString& second_key);
            
        private:
            void initConnect();
            void initMenuPanel();
            void initButtonGroup();
            void initCellBind();
            void displayCalculateValues(QVector<quint16> values);
            void displayResponse(CDataUnitType& unit);
            void versionParser();
            void addNewBind(const QString& key, QWidget* widget, int address, WidgetType wtype);
            int  sizeBindBlock(const QString& first_key, const QString& second_key);
            int  addressKey(const QString& key) const;
            QVector<QWidget*> listWidget(const QString& first_key, const QString& second_key);
            
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
            CVersionSoftware*       m_versionWidget;

            QVector<QPair<QString, QWidget*> > m_cell_list;

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
            QTreeWidgetItem* itemSetLedPurpose;
            QTreeWidgetItem* itemSetDiscretInPurpose;
            QTreeWidgetItem* itemSetRelayPurpose;
            QTreeWidgetItem* itemSetKeyboardPurpose;
    };
#endif // CONFIGURATORWINDOW_H
