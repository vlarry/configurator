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
                calibration_data_t ra;
                calibration_data_t rb;
                calibration_data_t rc;
            };
            //------------------
            enum CalibrationType
            {
                CALIBRATION_NONE,
                CALIBRATION_MIN,
                CALIBRATION_MAX
            };
            //--------------
            enum ChannelType
            {
                RESISTANCE_RA,
                RESISTANCE_RB,
                RESISTANCE_RC
            };
            //--------------
            struct RangeType
            {
                float value;
                calibration_t data;
            };

        public:
            explicit CCalibrationWidgetBRUResistance(QWidget *parent = nullptr);
            ~CCalibrationWidgetBRUResistance();

            CModBusDataUnit calculateValue(ChannelType channel);
            QVector<CModBusDataUnit> calculateValueList();

            int dataCount() const;
            int pauseRequest() const;

            float standardPhaseMin() const;
            float standardPhaseMax() const;

            bool stateRa() const;
            bool stateRb() const;
            bool stateRc() const;

            float valueShiftRa() const;
            float valueShiftRb() const;
            float valueShiftRc() const;
            float valueInclineRa() const;
            float valueInclineRb() const;
            float valueInclineRc() const;

            float measureRa() const;
            float measureRb() const;
            float measureRc() const;

            bool stateCalculateUpdate() const;

            void setFactorShiftRa(float value);
            void setFactorShiftRb(float value);
            void setFactorShiftRc(float value);

            void setFactorInclineRa(float value);
            void setFactorInclineRb(float value);
            void setFactorInclineRc(float value);

            void setMeasureRa(float average);
            void setMeasureRb(float average);
            void setMeasureRc(float average);

            void setDeviationRa(float min, float max);
            void setDeviationRb(float min, float max);
            void setDeviationRc(float min, float max);

            void showMessageError(const QString &message);

            float newCalibrationFactor(float standard, float power_factor, const calibration_data_t &measure_list);
            QPointF standardDeviation(const calibration_data_t &data);

        public slots:
            void display();
            void stateButton(bool state = false);
            void saveCalibrationToFlash();
            void valueCurrentStandardChanged(const QString&);
            void stateChoiceChannelChanged(bool = false);
            void calibrationParameterStart();
            void calibrationDataProcess(QVector<CModBusDataUnit> &data);
            void calibrationWriteProcess();
            void setCalibrartionFactorActual(const QString &key, float value);
            void setCalculateActualValue(CModBusDataUnit &unit);
            void progressBarIncrement();
            void checkCalibrationReady();
            void processCheckCalibrationReady(CModBusDataUnit &unit);

        signals:
            void calibrationWriteStart(QVector<CModBusDataUnit>&);
            void saveToFlash();
            void calibrationStart(QVector<CModBusDataUnit>&, int);
            void calibrationFactorAllStart();
            void calibrationEnd(bool = false);
            void checkReady(CModBusDataUnit&);

        protected:
            void paintEvent(QPaintEvent *event);

        private:
            Ui::CCalibrationWidgetBRUResistance *ui;
            CalibrationType m_calibration_type;
            RangeType m_calibration_min;
            RangeType m_calibration_max;
            bool m_is_ready;
    };
#endif // CALIBRATIONWIDGETBRURESISTANCE_H
