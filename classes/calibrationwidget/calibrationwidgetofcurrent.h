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
    //----------
    namespace Ui
    {
        class CCalibrationWidgetOfCurrent;
    }
    //--------------------------------------
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

            QCheckBox* ctrlIa() const;
            QCheckBox* ctrlIb() const;
            QCheckBox* ctrlIc() const;
            QCheckBox* ctrl3I0() const;

            int dataSetCount() const;
            int timePauseRequest() const;

            void                         addCalibrationIa(float value);
            void                         addCalibrationIb(float value);
            void                         addCalibrationIc(float value);
            void                         addCalibration3I0(float value);
            float                        calibrationCurrentStandardPhase() const;
            float                        calibrationCurrentStandard3I0() const;
            float                        calibrationCurrentIa() const;
            float                        calibrationCurrentIb() const;
            float                        calibrationCurrentIc() const;
            float                        calibrationCurrent3I0() const;
            void                         calibrationCurrentClear();
            bool                         calibrationCurrentIaState() const;
            bool                         calibrationCurrentIbState() const;
            bool                         calibrationCurrentIcState() const;
            bool                         calibrationCurrent3I0State() const;
            int                          calibrationCurrentDataCount() const;
            int                          calibrationCurrentPauseRequest() const;
            float newCalibrationFactor(float standard, float cur_factor, const calibration_data_t &measure_list);
            QPointF standardDeviation(const calibration_data_t &data);

            void setCurrentStandardPhase(float value);
            void setCurrentStandard3I0(float value);

            void setCurrentIaState(bool state);
            void setCurrentIbState(bool state);
            void setCurrentIcState(bool state);
            void setCurrent3I0State(bool state);

            void setCurrentDataCount(int count);
            void setCurrentPauseRequest(int pause);

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
            void apply();
            void saveToFlash(int);
            void calibrationStart(QVector<CModBusDataUnit> &unit_list);
            void calibrationEnd(bool = false);
            void dataIncrement();

        public slots:
            void display(const calibration_t &data);
            void stateButton(bool state = false);
            void valueCurrentStandardChanged(const QString&);
            void stateChoiceCurrentChannelChanged(bool);
            void saveCalibrationToFlash();
            void progressBarIncrement();
            void calibrationParameterStart();
            void calibrationDataProcess(QVector<CModBusDataUnit> &data);

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            Ui::CCalibrationWidgetOfCurrent* ui;
    };
#endif // CALIBRATIONWIDGET_H
