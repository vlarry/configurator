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
    enum REQUEST_FUNC
    {
        EMPTY_FUNC,
        CALCULATE_FUNC,
        CALIBRATION_FUNC,
        PROTECTION_FUNC
    };
    //---------------
    enum REQUEST_TYPE
    {
        READ_TYPE,
        WRITE_TYPE
    };
    //------------------
    struct request_cmd_t
    {
        REQUEST_TYPE    type;
        REQUEST_FUNC    function;
        QModbusDataUnit unit;
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
            void request(request_cmd_t& cur_request);
            void checkboxCalcTimeoutStateChanged(bool state);
            void timeCalcChanged(int newTime);
            void protectMTZChangedID(int id);
            void errorProtocol(QModbusDevice::Error error);
            
        private:
            void block();
            void unblock();
            bool is_block() const;
      
        private:
            Ui::ConfiguratorWindow* ui;
            QModbusRtuSerialMaster* m_modbusDevice;
            request_cmd_t           m_request;
            QPanel*                 m_panel;
            QTimer*                 m_calc_timer;
            QTimer*                 m_timeout_timer;
            QVector<QLineEdit*>     m_input_channel_cell;
            QVector<QLineEdit*>     m_calib_cell;
            QButtonGroup*           m_protect_mtz_group;
            bool                    m_blocking;
            QVector<request_cmd_t>  m_request_queue;
    };
#endif // CONFIGURATORWINDOW_H
