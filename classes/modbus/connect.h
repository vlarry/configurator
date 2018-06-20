#ifndef CCONNECT_H
    #define CCONNECT_H
    //----------------
    #include <QObject>
    #include <QSerialPort>
    #include <QDebug>
    //----------------------------
    class CConnect: public QObject
    {
        Q_OBJECT

        typedef QVector<QSerialPort::BaudRate> baudrate_list_t;
        typedef QMap<QSerialPort::SerialPortError, QString> error_t;

        public:
            struct SerialPortType
            {
                QString               name;
                QSerialPort::BaudRate baudrate;
                QSerialPort::DataBits databits;
                QSerialPort::Parity   parity;
                QSerialPort::StopBits stopbits;
            };

        public:
            explicit CConnect(QObject* parent = nullptr);
            ~CConnect();

            bool                  autochoicespeed() const;
            QSerialPort::BaudRate baudrateNext() const;
            bool                  isOpen() const;
            void                  setAutochoicespeed(bool state);
            void                  setSettings(SerialPortType& settings);
            const SerialPortType& settings() const;

        signals:
            void portError(const QString&);
            void readyRead(QByteArray&);
            void stateChanged(bool = false);

        public slots:
            void close();
            void endWrite(qint64 size);
            void errorOccuredPort(QSerialPort::SerialPortError error);
            void open();
            void readyReadData();

        private:
            QSerialPort*           m_port;
            SerialPortType         m_settings;
            static baudrate_list_t m_baudrate_list;
            static error_t         m_error_list;
            bool                   m_autochoicespeed;
    };
#endif // CCONNECT_H
