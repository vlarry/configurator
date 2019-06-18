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
                calibration_data_t shiftUa;
                calibration_data_t shiftUb;
                calibration_data_t shiftUc;
                calibration_data_t inclineUa;
                calibration_data_t inclineUb;
                calibration_data_t inclineUc;
                calibration_data_t shiftMultiplier;
                calibration_data_t inclineMultiplier;
            };
            //--------------
            enum ChannelType
            {
                POWER_SHIFT_UA,
                POWER_SHIFT_UB,
                POWER_SHIFT_UC,
                POWER_INCLINE_UA,
                POWER_INCLINE_UB,
                POWER_INCLINE_UC,
                POWER_SHIFT_MULTIPLIER,
                POWER_INCLINE_MULTIPLIER
            };

        public:
            explicit CCalibrationWidgetBRUPowerDC(QWidget *parent = nullptr);
            ~CCalibrationWidgetBRUPowerDC();

            CModBusDataUnit calculateValue(ChannelType channel);
            QVector<CModBusDataUnit> calculateValueList();

            int dataCount() const;
            int pauseRequest() const;

            float standardPhaseShift() const;
            float standardPhaseIncline() const;
            float standardPhaseMultiplierShift() const;
            float standardPhaseMultiplierIncline() const;

            bool stateShiftUa() const;
            bool stateShiftUb() const;
            bool stateShiftUc() const;

            bool stateInclineUa() const;
            bool stateInclineUb() const;
            bool stateInclineUc() const;

            bool stateUMultiplierShift() const;
            bool stateUMultiplierIncline() const;

            float valueShiftUa() const;
            float valueShiftUb() const;
            float valueShiftUc() const;
            float valueInclineUa() const;
            float valueInclineUb() const;
            float valueInclineUc() const;
            float valueShiftUMultiplier() const;
            float valueInclineUMultiplier() const;

            float measureShiftUa() const;
            float measureShiftUb() const;
            float measureShiftUc() const;
            float measureInclineUa() const;
            float measureInclineUb() const;
            float measureInclineUc() const;
            float measureShiftUMultiplier() const;
            float measureInclineUMultiplier() const;

            bool stateCalculateUpdate() const;

            void setFactorShiftUa(float value);
            void setFactorShiftUb(float value);
            void setFactorShiftUc(float value);

            void setFactorInclineUa(float value);
            void setFactorInclineUb(float value);
            void setFactorInclineUc(float value);

            void setFactorShiftUMultiplier(float value);
            void setFactorInclineUMultiplier(float value);

            void setMeasureShiftUa(float average);
            void setMeasureShiftUb(float average);
            void setMeasureShiftUc(float average);

            void setMeasureInclineUa(float average);
            void setMeasureInclineUb(float average);
            void setMeasureInclineUc(float average);

            void setMeasureShiftUMultiplier(float average);
            void setMeasureInclineUMultiplier(float average);

            void setDeviationShiftUa(float value);
            void setDeviationShiftUb(float value);
            void setDeviationShiftUc(float value);

            void setDeviationInclineUa(float value);
            void setDeviationInclineUb(float value);
            void setDeviationInclineUc(float value);

            void setDeviationShiftUMultiplier(float value);
            void setDeviationInclineMultiplier(float value);

            void showMessageError(const QString &message);

            float newCalibrationFactor(float standard, float power_factor, const calibration_data_t &measure_list);
            QPointF standardDeviation(const calibration_data_t &data);

        public slots:
            void display(const calibration_t &data);
            void stateButton(bool state = false);
            void saveCalibrationToFlash();
            void valueCurrentStandardChanged(const QString&);
            void stateChoiceChannelChanged(bool);
            void calibrationParameterStart();
            void calibrationDataProcess(QVector<CModBusDataUnit> &data);
            void calibrationWriteProcess();
            void setCalibrartionFactorActual(const QString &key, float value);
            void setCalculateActualValue(CModBusDataUnit &unit);
            void progressBarIncrement();

        signals:
            void calibrationWriteStart(QVector<CModBusDataUnit>&);
            void saveToFlash();
            void calibrationStart(QVector<CModBusDataUnit>&, int);
            void calibrationFactorAllStart();
            void calibrationEnd(bool = false);

        protected:
            void paintEvent(QPaintEvent *event);

        private:
            Ui::CCalibrationWidgetBRUPowerDC *ui;
    };
#endif // CALIBRATIONWIDGETBRUPOWERDC_H
