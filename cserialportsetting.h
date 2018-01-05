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

        public slots:
            void show();

        signals:
            void numberRepeat(int);
            void timeout(int);

        private:
            Ui::CSerialPortSetting* ui;
    };

#endif // CSERIALPORTSETTING_H
