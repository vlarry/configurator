#ifndef CALIBRATIONWIDGETBRURESISTANCE_H
    #define CALIBRATIONWIDGETBRURESISTANCE_H
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
        class CCalibrationWidgetBRUResistance;
    }
    //---------------------------------------------------
    class CCalibrationWidgetBRUResistance: public QWidget
    {
        Q_OBJECT

        public:
            typedef QVector<float> calibration_data_t;
            //------------------
            struct calibration_t
            {
                calibration_data_t shiftRa;
                calibration_data_t shiftRb;
                calibration_data_t shiftRc;
                calibration_data_t inclineRa;
                calibration_data_t inclineRb;
                calibration_data_t inclineRc;
            };
            //--------------
            enum ChannelType
            {
                RESISTANCE_SHIFT_RA,
                RESISTANCE_SHIFT_RB,
                RESISTANCE_SHIFT_RC,
                RESISTANCE_INCLINE_RA,
                RESISTANCE_INCLINE_RB,
                RESISTANCE_INCLINE_RC
            };

        public:
            explicit CCalibrationWidgetBRUResistance(QWidget *parent = nullptr);
            ~CCalibrationWidgetBRUResistance();

            CModBusDataUnit calculateValue(ChannelType channel);
            QVector<CModBusDataUnit> calculateValueList();

            int dataCount() const;
            int pauseRequest() const;

            float standardPhaseShift() const;
            float standardPhaseIncline() const;

            bool stateShiftRa() const;
            bool stateShiftRb() const;
            bool stateShiftRc() const;

            bool stateInclineRa() const;
            bool stateInclineRb() const;
            bool stateInclineRc() const;

            float valueShiftRa() const;
            float valueShiftRb() const;
            float valueShiftRc() const;
            float valueInclineRa() const;
            float valueInclineRb() const;
            float valueInclineRc() const;

            float measureShiftRa() const;
            float measureShiftRb() const;
            float measureShiftRc() const;
            float measureInclineRa() const;
            float measureInclineRb() const;
            float measureInclineRc() const;

            bool stateCalculateUpdate() const;

            void setFactorShiftRa(float value);
            void setFactorShiftRb(float value);
            void setFactorShiftRc(float value);

            void setFactorInclineRa(float value);
            void setFactorInclineRb(float value);
            void setFactorInclineRc(float value);

            void setMeasureShiftRa(float average);
            void setMeasureShiftRb(float average);
            void setMeasureShiftRc(float average);

            void setMeasureInclineRa(float average);
            void setMeasureInclineRb(float average);
            void setMeasureInclineRc(float average);

            void setDeviationShiftRa(float value);
            void setDeviationShiftRb(float value);
            void setDeviationShiftRc(float value);

            void setDeviationInclineRa(float value);
            void setDeviationInclineRb(float value);
            void setDeviationInclineRc(float value);

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
            Ui::CCalibrationWidgetBRUResistance *ui;
    };
#endif // CALIBRATIONWIDGETBRURESISTANCE_H
