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
    #include "cmodbus.h"
    #include "qpanel.h"
    #include "cterminal.h"
    #include "qcustomplot.h"
    #include "ctreedevicemenu.h"
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
                CalculateAddress   = 64,
                CalibrationAddress = 362,
                ProtectionAddress  = 22
            };

            enum RequestType
            {
                CALCULATE_TYPE,
                CALIBRATION_TYPE,
                PROTECTION_TYPE
            };
      
        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(bool state);
            void refreshSerialPort();
            void calculateRead(); // запрос расчетных величин
            void calibrationRead();
            void calibrationWrite();
            void protectionRead();
            void protectionWrite();
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
            
        private:
            void initButtonGroup();
            void displayCalculateValues(QVector<quint16> values);
            void displayCalibrationValues(QVector<quint16> values);
            void displayProtectionValues(QVector<quint16> values);
            
            void initConnect();
            
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
            QVector<QLineEdit*>     m_calibration_cell;
    };
#endif // CONFIGURATORWINDOW_H
