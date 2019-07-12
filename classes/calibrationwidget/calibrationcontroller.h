#ifndef CALIBRATECONTROLLER_H
    #define CALIBRATECONTROLLER_H
    //----------------
    #include <QObject>
    #include <QTimer>
    #include "modbusdataunit.h"
    #include "calibrationwidgetofcurrent.h"
    #include "calibrationwidgetpower.h"
    #include "calibrationwidgetbrupowerdc.h"
    #include "calibrationwidgetbruresistance.h"
    //------------------------------------------
    class CCalibrationController: public QObject
    {
        Q_OBJECT

        public:
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
                TYPE_NONE = 0xFF,
                TYPE_CURRENT = 0, // калибровка тока
                TYPE_POWER_AC, // калибровка напряжения AC
                TYPE_BRU, // таб калибровок бру по напряжению и сопротивлению
                TYPE_BRU_POWER_DC, // калибровка напряжение DC, находится на отдельном виджете
                TYPE_BRU_RESISTANCE // калибровка сопротивления, находится на отдельном виджете
            };

            struct calibration_data_t
            {
                int counter; // счетчик данных
                int limit; // количество данных, которые необходимо принять
                QVector<CModBusDataUnit> data; // данные
            };

        private:
            CCalibrationWidgetOfCurrent     *m_widget_of_current;
            CCalibrationWidgetPower         *m_widget_power;
            CCalibrationWidgetBRUPowerDC    *m_widget_bru_power_dc;
            CCalibrationWidgetBRUResistance *m_widget_bru_resistance;
            calibration_t                    m_calibration;
            CalibrationType                  m_calibration_type;
            CalibrationType                  m_calculate_type;
            calibration_data_t               m_calibration_data;
            QTimer                          *m_timer_caluculate;

        public:
            CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power,
                                   CCalibrationWidgetBRUPowerDC *widget_bru_power_dc, CCalibrationWidgetBRUResistance *widget_bru_resistance,
                                   QObject *parent = nullptr);
            ~CCalibrationController();
            void setWidgetCalibrationOfCurrent(CCalibrationWidgetOfCurrent *widget);
            void setWidgetCalibrationPower(CCalibrationWidgetPower *widget);
            void setWidgetCalibrationBRUPowerDC(CCalibrationWidgetBRUPowerDC *widget);
            void setWidgetCalibrationBRUResistance(CCalibrationWidgetBRUResistance *widget);

        signals:
            void calibration(CModBusDataUnit&);
            void calibrationFactorAllRead(); // получить все текущие калибровки
            void calibrationWrite(QVector<CModBusDataUnit>&);
            void calibrationSaveToFlash();
            void calibrationFactorActual(const QString&, float); // получение значения теущего калибровочного коэффициента
            void calculate(QVector<CModBusDataUnit>&);
            void calculateResponse(CModBusDataUnit&);
            void dataIncrement();
            void bruResistanceRequest(CModBusDataUnit&);
            void bruRequestIsReady(CModBusDataUnit&);

        public slots:
            void dataIsReady(CModBusDataUnit &unit);
            void calibrationProcess();
            void calibrationProcessStart(QVector<CModBusDataUnit> &unit_list, int param_count);
            void calculateValueRead();
            void setCalculateState(bool state, CalibrationType type);
    };
#endif // CALIBRATECONTROLLER_H
