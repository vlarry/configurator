#ifndef CALIBRATECONTROLLER_H
    #define CALIBRATECONTROLLER_H
    //----------------
    #include <QObject>
    #include <QTimer>
    #include "modbusdataunit.h"
    #include "calibrationwidgetofcurrent.h"
    #include "calibrationwidgetpower.h"
    //------------------------------------------
    class CCalibrationController: public QObject
    {
        Q_OBJECT

        struct calibration_t
        {
            int                      request_all; // всего запросов
            int                      request_count; // количество отправленных запросов
            int                      pause; // пауза между запросами в мс
            QVector<CModBusDataUnit> units; // массив запросов
            QTimer*                  timer; // таймер отправки запросов
        };

        enum CalibrationType
        {
            TYPE_NONE,
            TYPE_CURRENT,
            TYPE_POWER
        };

        struct calibration_data_t
        {
            int counter; // счетчик данных
            int limit; // количество данных, которые необходимо принять
            QVector<CModBusDataUnit> data; // данные
        };

        CCalibrationWidgetOfCurrent *m_widget_of_current;
        CCalibrationWidgetPower     *m_widget_power;
        calibration_t                m_calibration;
        CalibrationType              m_calibration_type;
        calibration_data_t           m_calibration_data;

        public:
            CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power, QObject *parent = nullptr);
            ~CCalibrationController();
            void setWidgetCalibrationOfCurrent(CCalibrationWidgetOfCurrent *widget);
            void setWidgetCalibrationPower(CCalibrationWidgetPower *widget);

        signals:
            void calibration(CModBusDataUnit&);
            void calibrationFactorAllRead();

        public slots:
            void dataIsReady(CModBusDataUnit &unit);
            void calibrationProcess();
            void calibrationProcessStart(QVector<CModBusDataUnit> &unit_list, int param_count);
    };
#endif // CALIBRATECONTROLLER_H
