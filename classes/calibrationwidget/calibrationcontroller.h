#ifndef CALIBRATECONTROLLER_H
    #define CALIBRATECONTROLLER_H
    //----------------
    #include <QObject>
    #include "modbusdataunit.h"
    #include "calibrationwidgetofcurrent.h"
    #include "calibrationwidgetpower.h"
    //------------------------------------------
    class CCalibrationController: public QObject
    {
        Q_OBJECT

        private:
            CCalibrationWidgetOfCurrent *m_widget_of_current;
            CCalibrationWidgetPower *m_widget_power;

        public:
            CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power, QObject *parent = nullptr);
            ~CCalibrationController();
            void setWidgetCalibrationOfCurrent(CCalibrationWidgetOfCurrent *widget);
            void setWidgetCalibrationPower(CCalibrationWidgetPower *widget);

        signals:
            void readData(CModBusDataUnit&);

        public slots:
            void dataIsReady(CModBusDataUnit &unit);
    };
#endif // CALIBRATECONTROLLER_H
