#ifndef CMODBUS_H
    #define CMODBUS_H
    //----------------
    #include <QObject>
    #include <QTimer>
    #include <QTime>
    #include <QDebug>
    #include "modbusdataunit.h"
    #include "connect.h"
    //---------------------------
    class CModBus: public QObject
    {
        Q_OBJECT

        typedef QVector<CModBusDataUnit> queue_t;

        public:
            explicit CModBus(QObject* parent = nullptr);

            CConnect* channel();
            bool      isConnected() const;
            void      sendData(CModBusDataUnit& unit, int count = 1);
            void      setIntervalResponce(int interval);
            void      setIntervalSilence(int interval);
            void      setTryCount(int count);

        signals:
            void autochoicespeed();
            void baudrateChanged(QSerialPort::BaudRate);
            void close();
            void closeDevice(bool = false);
            void errorDevice(const QString&);
            void errorChannel(const QString&);
            void open();
            void rawData(QByteArray&, bool = true);
            void readyRead(CModBusDataUnit&);
            void stateChanged(bool = false);

        public slots:
            void autochoicespeedProcess();
            void disconnected();
            void readyReadData(QByteArray& bytes);
            void timeoutResponce();
            void timeoutSilencce();
            void userStateCtrl(bool state); // событие на нажатие кнопки пользователем (пока очистка флага m_is_autochoicespeed)

        private:
            void    block();
            quint16 crc16(QByteArray& data, size_t length);
            bool    isBlock() const;
            void    request(CModBusDataUnit& unit);
            void    unblock();

        private:
            CConnect*       m_channel;
            CModBusDataUnit m_request;
            queue_t         m_queue;
            QByteArray      m_buffer;
            bool            m_connect;
            bool            m_block;
            bool            m_is_autochoicespeed;
            int             m_interval_timeout_response;
            int             m_interval_timeout_silence;
            int             m_trycount;
            int             m_try_counter;
            QSerialPort::BaudRate m_baudrate_init;
            QTimer*         m_timer_timeout_response;
            QTimer*         m_timer_timeout_silence;
            QTime           m_time_process;
    };
#endif // CMODBUS_H
