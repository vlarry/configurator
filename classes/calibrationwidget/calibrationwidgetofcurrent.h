#ifndef CALIBRATIONWIDGET_H
    #define CALIBRATIONWIDGET_H
    //----------------
    #include <QWidget>
    #include <QPainter>
    #include <QLayoutItem>
    #include <QCheckBox>
    #include <QDebug>
    #include <QDoubleValidator>
    #include <QMessageBox>
    #include <QMessageBox>
    #include <math.h>
    #include "modbusdataunit.h"
    #include "clineedit.h"
    //----------
    namespace Ui
    {
        class CCalibrationWidgetOfCurrent;
    }
    //-----------------------------------------------
    class CCalibrationWidgetOfCurrent: public QWidget
    {
        Q_OBJECT

        public:
            typedef QVector<float> calibration_data_t;
            //------------------
            struct calibration_t
            {
                calibration_data_t Ia;
                calibration_data_t Ib;
                calibration_data_t Ic;
                calibration_data_t _3I0;
            };

            enum ChannelType
            {
                CURRENT_IA,
                CURRENT_IB,
                CURRENT_IC,
                CURRENT_3I0
            };

        public:
            explicit CCalibrationWidgetOfCurrent(QWidget* parent = nullptr);
            ~CCalibrationWidgetOfCurrent();

            CModBusDataUnit calculateValue(ChannelType channel);
            QVector<CModBusDataUnit> calculateValueList();

            bool stateIa() const;
            bool stateIb() const;
            bool stateIc() const;
            bool state3I0() const;

            int dataCount() const;
            int pauseRequest() const;

            float standardPhase() const;
            float standard3I0() const;
            float valueIa() const;
            float valueIb() const;
            float valueIc() const;
            float value3I0() const;

            bool stateCalculateUpdate() const;

            void setStandardPhase(float value);
            void setStandard3I0(float value);
            void setIaState(bool state);
            void setIbState(bool state);
            void setIcState(bool state);
            void set3I0State(bool state);

            void setDataCount(int count);
            void setPauseRequest(int pause);

            float newCalibrationFactor(float standard, float cur_factor, const calibration_data_t &measure_list);
            QPointF standardDeviation(const calibration_data_t &data);

            void setFactorIa(float value);
            void setFactorIb(float value);
            void setFactorIc(float value);
            void setFactor3I0(float value);

            void setMeasureIa(float average);
            void setMeasureIb(float average);
            void setMeasureIc(float average);
            void setMeasure3I0(float average);

            void setDeviationIa(float value);
            void setDeviationIb(float value);
            void setDeviationIc(float value);
            void setDeviation3I0(float value);

        signals:
            void calibrationWriteStart(QVector<CModBusDataUnit>&);
            void saveToFlash();
            void calibrationStart(QVector<CModBusDataUnit>&, int);
            void calibrationFactorAllStart();
            void calibrationEnd(bool = false);
            void dataIncrement();

        public slots:
            void display(const calibration_t &data);
            void stateButton(bool state = false);
            void valueStandardChanged(const QString&);
            void stateChoiceChannelChanged(bool);
            void saveCalibrationToFlash();
            void progressBarIncrement();
            void calibrationParameterStart();
            void calibrationDataProcess(QVector<CModBusDataUnit> &data);
            void calibrationWriteProcess();
            void setCalibrartionFactorActual(const QString &key, float value);
            void setCalculateActualValue(CModBusDataUnit &unit);

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            Ui::CCalibrationWidgetOfCurrent* ui;
    };
#endif // CALIBRATIONWIDGET_H
