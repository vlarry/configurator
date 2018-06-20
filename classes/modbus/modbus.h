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

        signals:

        public slots:
    };
#endif // CMODBUS_H
