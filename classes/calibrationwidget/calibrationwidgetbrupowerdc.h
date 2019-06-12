#ifndef CALIBRATIONWIDGETBRUPOWERDC_H
    #define CALIBRATIONWIDGETBRUPOWERDC_H
    //----------------
    #include <QWidget>
    #include <QPaintEvent>
    #include <QMessageBox>
    #include <QPainter>
    #include "modbusdataunit.h"
    #include "clineedit.h"
    #include "math.h"
    //----------
    namespace Ui
    {
        class CCalibrationWidgetBRUPowerDC;
    }
    //------------------------------------------------
    class CCalibrationWidgetBRUPowerDC: public QWidget
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
                POWER_UA,
                POWER_UB,
                POWER_UC,
                POWER_UAB,
                POWER_UBC,
                POWER_UCA,
                POWER_3U0S,
                POWER_3US,
                POWER_3U0
            };

        public:
            explicit CCalibrationWidgetBRUPowerDC(QWidget *parent = nullptr);
            ~CCalibrationWidgetBRUPowerDC();

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
            Ui::CCalibrationWidgetBRUPowerDC *ui;
    };
#endif // CALIBRATIONWIDGETBRUPOWERDC_H
