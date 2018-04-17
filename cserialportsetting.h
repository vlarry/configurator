#ifndef CSERIALPORTSETTING_H
    #define CSERIALPORTSETTING_H
    //----------------
    #include <QWidget>
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

            const QString dataBits() const;
            const QString parity() const;
            const QString stopBits() const;
            int           modbusTimeout() const;
            int           modbusTryCount() const;
            bool          autospeedState() const;

            void setAutospeed(bool state);
            void setDataBits(const QString& databits);
            void setStopBits(const QString& stopbits);
            void setParity(const QString& parity);
            void setModbusTimeout(int value);
            void setModbusTryCount(int value);

        public slots:
            void show();

        signals:
            void numberRepeat(int);
            void timeout(int);
            void autospeed(bool);

        private:
            Ui::CSerialPortSetting* ui;
    };

#endif // CSERIALPORTSETTING_H
