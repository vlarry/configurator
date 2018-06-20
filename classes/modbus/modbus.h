#ifndef CMODBUS_H
    #define CMODBUS_H
    //----------------
    #include <QObject>
    #include <QDebug>
    #include "modbusdataunit.h"
    #include "connect.h"
    //---------------------------
    class CModBus: public QObject
    {
        Q_OBJECT

        public:
            explicit CModBus(QObject* parent = nullptr);

            CConnect* channel();
            void      sendData(CModBusDataUnit& unit);

        signals:
            void close();
            void open();
            void rawData(QByteArray&, bool = true);
            void stateChanged(bool = false);

        public slots:
            void readyReadData(QByteArray& bytes);

        private:
            void    request(CModBusDataUnit& unit);
            quint16 crc16(QByteArray& data, size_t length);

        private:
            CConnect*       m_channel;
            CModBusDataUnit m_request;
            QByteArray      m_buffer;
    };
#endif // CMODBUS_H
