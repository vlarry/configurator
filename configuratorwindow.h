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
    #include "cmodbus.h"
    #include "qpanel.h"
    #include "cterminal.h"
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
      
        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(bool state);
            void refreshSerialPort();
            void calculate_value(); // запрос расчетных величин
            void calibration_read();
            void calibration_write();
            void protection_read();
            void protection_write();
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
            void errorDevice(const QString& error);
            void terminalVisiblity(int state);
            void saveLog(const QString& info);
            
        private:
            void initButtonGroup();
            void displayCalculateValues(QVector<quint16> values);
            void displayCalibrationValues(QVector<quint16> values);
            void displayProtectionValues(QVector<quint16> values);
            
            void initConnect();
            
        private:
            Ui::ConfiguratorWindow* ui;
            QPanel*                 m_panel;
            QTimer*                 m_tim_calculate;
            QVector<QLineEdit*>     m_calculate_cell;
            QVector<QLineEdit*>     m_calibration_cell;
            QButtonGroup*           m_protect_mtz_group;
            QButtonGroup*           m_protect_earthly_group;
            QButtonGroup*           m_protect_power_group;
            QButtonGroup*           m_protect_motor_group;
            QButtonGroup*           m_protect_frequency_group;
            QButtonGroup*           m_protect_external_group;
            QButtonGroup*           m_protect_temperature_group;
            QButtonGroup*           m_protect_level_group;
            CModbus*                m_modbusDevice;
            CTerminal*              m_terminal;
            QFile*                  m_logFile;
    };
#endif // CONFIGURATORWINDOW_H
