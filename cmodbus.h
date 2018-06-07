#ifndef CMODBUS_H
    #define CMODBUS_H
    //----------------
    #include <QObject>
    #include <QSerialPort>
    #include <QVector>
    #include <QTimer>
    #include <QTime>
    #include <QMetaMethod>
    #include <QDebug>
    #include <cdataunittype.h>
    //------------------
    #define DEBUG_MODBUS
    //---------------------------
    class CModbus: public QObject
    {
        Q_OBJECT

        public:
            /*!
             *  \brief The AutoConnectType struct
             *  Определяет необходимые переменные для автоподлкючения
             */
            struct AutoConnectType
            {
                bool                           is_connect;
                bool                           baud_reconnect;
                QSerialPort::BaudRate          baudrate_init;
                int                            index_current;
                int                            index_start;
                QVector<QSerialPort::BaudRate> baudrate_list;
            };
        
        public:
            explicit CModbus(QObject* parent = nullptr);
            ~CModbus();
        
            void setPortName(const QString& name);
            void setBaudrate(QSerialPort::BaudRate baudrate);
            void setDatabits(QSerialPort::DataBits databits);
            void setStopbits(QSerialPort::StopBits stopbits);
            void setParity(QSerialPort::Parity parity);
            void setBaudrateList(const QVector<QSerialPort::BaudRate>& baudrate_list);
            
            const QString&        portName() const;
            quint32               baudrate() const;
            QSerialPort::DataBits databits() const;
            QSerialPort::StopBits stopbits() const;
            QSerialPort::Parity   parity() const;
            
            bool     autospeed() const;
            void     clearQueueRequest();
            void     removeRequest(const QString& key, const QString& value);
            quint32  requestQueueCount() const;
            bool     is_open() const;
            quint16  timeoutRepeat() const;
            quint8   requestCountRepeat() const;
            void     setTimeoutRepeat(quint16 newTimeout);
            void     setRequestCountRepeat(quint8 newCount);
            void     setAutospeed(bool state);
            quint32  sizeQueue() const;
            
        signals:
            void errorDevice(const QString&);
            void error(const QString&);
            void connectDeviceState(bool);
            void dataReady(CDataUnitType&);
            void timeoutResponse();
            void rawData(QByteArray& data, bool = true);
            void noConnect(bool = false);
            void baudrateChanged(QSerialPort::BaudRate);
            void newBaudrate(int);
            void saveSettings();
            
        public slots:
            void connectDevice();
            void disconnectDevice(bool isClear = true);
            void sendRequest(CDataUnitType& unit);
            void readyRead();
            void errorPort(QSerialPort::SerialPortError error);
            void timeoutReadWait();
            void sendRequestWait();
            
        private:
            void    request(CDataUnitType& unit);
            void    process_request_queue();
            quint16 CRC16(QVector<quint8>& data, quint8 length);
            void    block();
            void    unblock();
            bool    is_blocking() const;
        
        private:
            QSerialPort*           m_device;
            QString                m_port_name;
            QSerialPort::BaudRate  m_baudrate;
            QSerialPort::DataBits  m_data_bits;
            QSerialPort::StopBits  m_stop_bits;
            QSerialPort::Parity    m_parity;
            QVector<CDataUnitType> m_request_queue; // очередь запросов (если передача заблокирована, то сохраняем в очередь)
            bool                   m_blocking_send; // блокировка отправки сообщения (передается предыдущий запрос)
            CDataUnitType          m_request_cur; // текущий запрос
            CDataUnitType          m_request_send_wait; // запрос ожидающий отправки
            QByteArray             m_receive_buffer; // буфер приема ответа на запрос
            QTimer*                m_timeout_timer; // таймер для отслеживания таймаута ответа
            QTimer*                m_send_wait_timer; // таймер ожидания отправки сообщения
            quint8                 m_counter_request_error; // счетчик неудачных отправок
            quint8                 m_request_count_repeat; // количество повторений отправки в случае ошибки
            quint16                m_timeout_repeat; // таймаут попытки отправить сообщениие устройству.
                                                     // В случае неуспешной отправки запроса в течении
                                                     // m_request_count_repeat раз порт отключается и очередь сообщений
                                                     // чистится
            quint32                m_sizeQuery;
            AutoConnectType        m_connect;
            bool                   m_autospeed; // режим включения автоподбора скорости
            QTime                  m_time;
    };
#endif // CMODBUS_H
