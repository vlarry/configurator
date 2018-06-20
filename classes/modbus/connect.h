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

        public:
            explicit CConnect(QObject* parent = nullptr);

        signals:

        public slots:
    };
#endif // CCONNECT_H
