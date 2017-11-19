#ifndef CONFIGURATORWINDOW_H
    #define CONFIGURATORWINDOW_H
    //--------------------
    #include <QMainWindow>
    #include <QModbusRtuSerialMaster>
    #include <QModbusDataUnit>
    #include <QModbusReply>
    #include <QSerialPort>
    #include <QSerialPortInfo>
    #include <QMessageBox>
    #include <QTextCodec>
    #include <QDebug>
    #include <QListWidget>
    #include <QTimer>
    #include <QSpinBox>
    #include <QButtonGroup>
    #include <QAbstractButton>
    #include "cmodbus.h"
    #include "qpanel.h"
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
                CalibrationAddress = 362
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
            void errorDevice(const QString& error);
            
        private:
            void displayCalculateValues(QVector<quint16> values);
            void displayCalibrationValues(QVector<quint16> values);
            
        private:
            Ui::ConfiguratorWindow* ui;
            QModbusRtuSerialMaster* m_modbusDevice;
            QPanel*                 m_panel;
            QTimer*                 m_tim_calculate;
            QVector<QLineEdit*>     m_calculate_cell;
            QVector<QLineEdit*>     m_calibration_cell;
            QButtonGroup*           m_protect_mtz_group;
            CModbus*                m_modbusDeviceNew;
    };
#endif // CONFIGURATORWINDOW_H
