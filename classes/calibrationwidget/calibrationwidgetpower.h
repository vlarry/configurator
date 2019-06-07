#ifndef CALIBRATIONWIDGETPOWER_H
    #define CALIBRATIONWIDGETPOWER_H
    //----------------
    #include <QWidget>
    #include <QPaintEvent>
    #include <QMessageBox>
    #include <QPainter>
    #include "modbusdataunit.h"
    #include "clineedit.h"
    //----------
    namespace Ui
    {
        class CCalibrationWidgetPower;
    }
    //-------------------------------------------
    class CCalibrationWidgetPower: public QWidget
    {
        Q_OBJECT

        public:
            typedef QVector<float> calibration_data_t;
            //------------------
            struct calibration_t
            {
                calibration_data_t Ua;
                calibration_data_t Ub;
                calibration_data_t Uc;
                calibration_data_t Uab;
                calibration_data_t Ubc;
                calibration_data_t Uca;
                calibration_data_t _3U0S;
                calibration_data_t _3US;
                calibration_data_t _3U0;
            };
            //--------------
            enum ChannelType
            {
                CURRENT_UA,
                CURRENT_UB,
                CURRENT_UC,
                CURRENT_UAB,
                CURRENT_UBC,
                CURRENT_UCA,
                CURRENT_3U0S,
                CURRENT_3US,
                CURRENT_3U0
            };

        public:
            explicit CCalibrationWidgetPower(QWidget *parent = nullptr);
            ~CCalibrationWidgetPower();

            int dataCount() const;
            int pauseRequest() const;

            bool stateUa() const;
            bool stateUb() const;
            bool stateUc() const;

            bool stateUab() const;
            bool stateUbc() const;
            bool stateUca() const;

            bool state3U0S() const;
            bool state3US() const;
            bool state3I0() const;

        public slots:
            void calibrationParameterStart();
            void calibrationDataProcess(QVector<CModBusDataUnit> &data);

        signals:
            void calibrationWriteStart(QVector<CModBusDataUnit>&);
            void saveToFlash();
            void calibrationStart(QVector<CModBusDataUnit>&, int);
            void calibrationFactorAllStart();
            void calibrationEnd(bool = false);
            void dataIncrement();

        protected:
            void paintEvent(QPaintEvent *event);

        private:
            Ui::CCalibrationWidgetPower *ui;
    };
#endif // CALIBRATIONWIDGETPOWER_H
