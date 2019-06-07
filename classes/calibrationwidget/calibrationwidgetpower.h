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

            int dataSetCount() const;
            int timePauseRequest() const;

        public slots:
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
