#ifndef CSERIALPORTSETTING_H
    #define CSERIALPORTSETTING_H
    //----------------
    #include <QWidget>
    #include <QtSerialPort>
    #include <QButtonGroup>
    //----------
    namespace Ui
    {
        class CSerialPortSetting;
    }
    //--------------------------------------
    class CSerialPortSetting: public QWidget
    {
        Q_OBJECT

        public:
            explicit CSerialPortSetting(QWidget* parent = nullptr);
            ~CSerialPortSetting();

            QSerialPort::DataBits dataBits() const;
            QSerialPort::Parity   parity() const;
            QSerialPort::StopBits stopBits() const;
            QSerialPort::BaudRate baudrate() const;
            QString               serialPortName() const;
            const QVector<QSerialPort::BaudRate> baudrateList();

            int  modbusTimeout() const;
            int  modbusTryCount() const;
            int  modbusIntervalSilence() const;
            bool autospeedState() const;
            int  deviceID() const;
            int  deviceSync() const;

            void setAutospeed(bool state);
            void setSerialPortList(const QStringList& list);
            void setDataBits(QSerialPort::DataBits databits);
            void setStopBits(QSerialPort::StopBits stopbits);
            void setParity(QSerialPort::Parity parity);

            void setDeviceID(int id);
            void setDeviceSync(int time_sync);
            void setModbusTimeout(int value);
            void setModbusTryCount(int value);
            void setModbusIntervalSilence(int value);

        public slots:
            void show();
            void cancel();
            void setBaudrate(QSerialPort::BaudRate baudrate);

        signals:
            void numberRepeat(int);
            void timeout(int);
            void autospeed(bool);
            void refreshSerialPort();

        protected:
            void closeEvent(QCloseEvent* event);

        private:
            Ui::CSerialPortSetting* ui;
            QButtonGroup*           m_group_databits;
            QButtonGroup*           m_group_stopbits;
            QButtonGroup*           m_group_parity;
            QSerialPort::BaudRate   m_default_baudrate;
            QSerialPort::DataBits   m_default_databits;
            QSerialPort::StopBits   m_default_stopbits;
            QSerialPort::Parity     m_default_parity;
            bool                    m_default_autospeed;
            int                     m_default_id;
            int                     m_default_interval_silence;
            int                     m_default_timeout;
            int                     m_default_trycount;
            int                     m_default_sync;
    };

#endif // CSERIALPORTSETTING_H
