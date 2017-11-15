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
    #include "qpanel.h"
    //----------
    namespace Ui 
    {
        class ConfiguratorWindow;
    }
    //---------------
    enum REQUEST_TYPE
    {
        EMPTY_TYPE,
        CALC_TYPE,
        CALIB_TYPE,
        PROTECTION_TYPE
    };
    //-------------------------------------------
    class ConfiguratorWindow: public QMainWindow
    {
        Q_OBJECT
      
        public:
            explicit ConfiguratorWindow(QWidget* parent = Q_NULLPTR);
            ~ConfiguratorWindow();
        
        public slots:
            void serialPortCtrl();
            void stateChanged(QModbusDevice::State state);
            void refreshSerialPort();
            void calcValue();
            void readCalibration();
            void writeCalibration();
            void readProtection();
            void writeProtection();
            void readReady();
            void timeout();
            void timeoutChanged(int newTimeout);
            void show();
            void request(QModbusDataUnit& unit, bool isRead = true);
            void checkboxCalcTimeoutStateChanged(bool state);
            void timeCalcChanged(int newTime);
            void protectMTZChangedID(int id);
      
        private:
            Ui::ConfiguratorWindow* ui;
            QModbusRtuSerialMaster* m_modbusDevice;
            REQUEST_TYPE            m_request_type;
            QPanel*                 m_panel;
            QTimer*                 m_calc_timer;
            QTimer*                 m_timeout_timer;
            QVector<QLineEdit*>     m_input_channel_cell;
            QVector<QLineEdit*>     m_calib_cell;
            QButtonGroup*           m_protect_mtz_group;
    };
#endif // CONFIGURATORWINDOW_H
