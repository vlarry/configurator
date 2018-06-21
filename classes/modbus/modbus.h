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
            void      sendData(CModBusDataUnit& unit);
            void      setIntervalResponce(int interval);
            void      setIntervalSilence(int interval);
            void      setTryCount(int count);

        signals:
            void close();
            void errorDevice(const QString&);
            void open();
            void rawData(QByteArray&, bool = true);
            void readyRead(CModBusDataUnit&);
            void stateChanged(bool = false);

        public slots:
            void disconnected();
            void readyReadData(QByteArray& bytes);
            void timeoutResponce();
            void timeoutSilencce();

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
            QTimer*         m_timer_timeout_response;
            QTimer*         m_timer_timeout_silence;
            QTime           m_time_process;
    };
#endif // CMODBUS_H
