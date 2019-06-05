#ifndef CALIBRATECONTROLLER_H
    #define CALIBRATECONTROLLER_H
    //-------------------------
    #include "modbusdataunit.h"
    #include "calibrationwidgetofcurrent.h"
    #include "calibrationwidgetpower.h"
    //--------------------------
    class CCalibrationController
    {
        private:
            CCalibrationWidgetOfCurrent *m_widget_of_current;
            CCalibrationWidgetPower *m_widget_power;

        public:
            CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power);
            void setWidgetCalibrationOfCurrent(CCalibrationWidgetOfCurrent *widget);
            void setWidgetCalibrationPower(CCalibrationWidgetPower *widget);

        signals:
            void readData(CModBusDataUnit&);

        public slots:
            void dataIsReady(CModBusDataUnit &unit);
    };
#endif // CALIBRATECONTROLLER_H
