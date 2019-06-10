#ifndef CALIBRATIONWIDGETPOWERDC_H
    #define CALIBRATIONWIDGETPOWERDC_H
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
        class CCalibrationWidgetPowerDC;
    }
    //---------------------------------------------
    class CCalibrationWidgetPowerDC: public QWidget
    {
        Q_OBJECT

        public:
            explicit CCalibrationWidgetPowerDC(QWidget *parent = nullptr);
            ~CCalibrationWidgetPowerDC();

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
            Ui::CCalibrationWidgetPowerDC *ui;
    };
#endif // CALIBRATIONWIDGETPOWERDC_H
