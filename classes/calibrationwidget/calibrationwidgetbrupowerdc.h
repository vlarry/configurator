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
                calibration_data_t ua;
                calibration_data_t ub;
                calibration_data_t uc;
                calibration_data_t multiplier;
            };
            //--------------
            enum ChannelType
            {
                POWER_UA,
                POWER_UB,
                POWER_UC,
                POWER_MULTIPLIER
            };
            //------------------
            enum CalibrationType
            {
                CALIBRATION_NONE,
                CALIBRATION_MIN,
                CALIBRATION_MAX
            };
            //--------------
            struct RangeType
            {
                float value;
                float multyplierValue;
                calibration_t data;
            };

        public:
            explicit CCalibrationWidgetBRUPowerDC(QWidget *parent = nullptr);
            ~CCalibrationWidgetBRUPowerDC();

            CModBusDataUnit calculateValue(ChannelType channel);
            QVector<CModBusDataUnit> calculateValueList();

            int dataCount() const;
            int pauseRequest() const;

            float standardPhaseMin() const;
            float standardPhaseMax() const;
            float standardPhaseMultiplierMin() const;
            float standardPhaseMultiplierMax() const;

            bool stateUa() const;
            bool stateUb() const;
            bool stateUc() const;

            bool stateUMultiplier() const;

            float valueShiftUa() const;
            float valueShiftUb() const;
            float valueShiftUc() const;
            float valueInclineUa() const;
            float valueInclineUb() const;
            float valueInclineUc() const;
            float valueShiftUMultiplier() const;
            float valueInclineUMultiplier() const;

            float measureUa() const;
            float measureUb() const;
            float measureUc() const;
            float measureUMultiplier() const;

            bool stateCalculateUpdate() const;

            void setFactorShiftUa(float value);
            void setFactorShiftUb(float value);
            void setFactorShiftUc(float value);

            void setFactorInclineUa(float value);
            void setFactorInclineUb(float value);
            void setFactorInclineUc(float value);

            void setFactorShiftUMultiplier(float value);
            void setFactorInclineUMultiplier(float value);

            void setMeasureUa(float average);
            void setMeasureUb(float average);
            void setMeasureUc(float average);
            void setMeasureUMultiplier(float average);

            void setDeviationUa(float min, float max);
            void setDeviationUb(float min, float max);
            void setDeviationUc(float min, float max);
            void setDeviationUMultiplier(float min, float max);

            void showMessageError(const QString &message);

            float newCalibrationFactor(float standard, float power_factor, const calibration_data_t &measure_list);
            QPointF standardDeviation(const calibration_data_t &data);

        public slots:
            void display();
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
            CalibrationType m_calibration_type;
            RangeType m_calibration_min;
            RangeType m_calibration_max;
    };
#endif // CALIBRATIONWIDGETBRUPOWERDC_H
