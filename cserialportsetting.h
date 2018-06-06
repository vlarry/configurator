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
            void setBaudrate(QSerialPort::BaudRate baudrate);

            void setDeviceID(int id);
            void setDeviceSync(int time_sync);
            void setModbusTimeout(int value);
            void setModbusTryCount(int value);
            void setModbusIntervalSilence(int value);

        public slots:
            void show();

        signals:
            void numberRepeat(int);
            void timeout(int);
            void autospeed(bool);
            void refreshSerialPort();

        private:
            Ui::CSerialPortSetting* ui;
            QButtonGroup*           m_group_databits;
            QButtonGroup*           m_group_stopbits;
            QButtonGroup*           m_group_parity;
    };

#endif // CSERIALPORTSETTING_H
