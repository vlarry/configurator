#ifndef CSERIALPORTSETTING_H
    #define CSERIALPORTSETTING_H
    //----------------
    #include <QWidget>
    #include <QtSerialPort>
    #include <QButtonGroup>
    #include <QDebug>
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

            QSerialPort::Parity   parity() const;
            QSerialPort::BaudRate baudrate() const;
            QString               serialPortName() const;
            const QVector<QSerialPort::BaudRate> baudrateList();

            bool autospeedState() const;
            int  deviceID() const;
            void setAutospeed(bool state);
            void setSerialPortList(const QStringList& list);
            void setSerialPortCurrentName(const QString &name);
            void setParity(QSerialPort::Parity parity);

            void setDeviceID(int id);
            void setDeviceSync(int time_sync);

        public slots:
            void show();
            void cancelProcess();
            void okProcess();
            void setBaudrate(QSerialPort::BaudRate baudrate);

        signals:
            void numberRepeat(int);
            void timeout(int);
            void autospeed(bool);
            void refreshSerialPort();
            void updateSettings();

        protected:
            void closeEvent(QCloseEvent* event);

        private:
            Ui::CSerialPortSetting* ui;
            QSerialPort::BaudRate   m_default_baudrate;
            QSerialPort::Parity     m_default_parity;
            bool                    m_default_autospeed;
            int                     m_default_id;
            int                     m_default_timeout;
    };

#endif // CSERIALPORTSETTING_H
