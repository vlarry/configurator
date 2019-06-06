#ifndef CALIBRATIONWIDGETPOWER_H
    #define CALIBRATIONWIDGETPOWER_H
    //----------------
    #include <QWidget>
    #include <QPaintEvent>
    #include <QPainter>
    #include "modbusdataunit.h"
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
            explicit CCalibrationWidgetPower(QWidget *parent = nullptr);
            ~CCalibrationWidgetPower();

        public slots:
            void calibrationDataProcess(QVector<CModBusDataUnit> &data);

        protected:
            void paintEvent(QPaintEvent *event);

        private:
            Ui::CCalibrationWidgetPower *ui;
    };
#endif // CALIBRATIONWIDGETPOWER_H
