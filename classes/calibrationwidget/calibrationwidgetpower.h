#ifndef CALIBRATIONWIDGETPOWER_H
    #define CALIBRATIONWIDGETPOWER_H
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
        class CCalibrationWidgetPower;
    }
    //-------------------------------------------
    class CCalibrationWidgetPower: public QWidget
    {
        Q_OBJECT

        public:
            typedef QVector<float> calibration_data_t;
            //------------------
            struct calibration_t
            {
                calibration_data_t Ua;
                calibration_data_t Ub;
                calibration_data_t Uc;
                calibration_data_t Uab;
                calibration_data_t Ubc;
                calibration_data_t Uca;
                calibration_data_t _3U0S;
                calibration_data_t _3US;
                calibration_data_t _3U0;
            };
            //--------------
            enum ChannelType
            {
                POWER_UA,
                POWER_UB,
                POWER_UC,
                POWER_UAB,
                POWER_UBC,
                POWER_UCA,
                POWER_3U0S,
                POWER_3US,
                POWER_3U0
            };

        public:
            explicit CCalibrationWidgetPower(QWidget *parent = nullptr);
            ~CCalibrationWidgetPower();

            CModBusDataUnit calculateValue(ChannelType channel);
            QVector<CModBusDataUnit> calculateValueList();

            int dataCount() const;
            int pauseRequest() const;

            float standardPhase() const;
            float standardPhaseLinear() const;
            float standard3U() const;

            bool stateUa() const;
            bool stateUb() const;
            bool stateUc() const;

            bool stateUab() const;
            bool stateUbc() const;
            bool stateUca() const;

            bool state3U0S() const;
            bool state3US() const;
            bool state3I0() const;

            float valueUa() const;
            float valueUb() const;
            float valueUc() const;
            float valueUab() const;
            float valueUbc() const;
            float valueUca() const;
            float value3U0S() const;
            float value3US() const;
            float value3U0() const;

            float measureUa() const;
            float measureUb() const;
            float measureUc() const;
            float measureUab() const;
            float measureUbc() const;
            float measureUca() const;
            float measure3U0S() const;
            float measure3US() const;
            float measure3U0() const;

            bool stateCalculateUpdate() const;

            void setFactorUa(float value);
            void setFactorUb(float value);
            void setFactorUc(float value);

            void setFactorUab(float value);
            void setFactorUbc(float value);
            void setFactorUca(float value);

            void setFactor3U0S(float value);
            void setFactor3US(float value);
            void setFactor3U0(float value);

            void setMeasureUa(float average);
            void setMeasureUb(float average);
            void setMeasureUc(float average);

            void setMeasureUab(float average);
            void setMeasureUbc(float average);
            void setMeasureUca(float average);

            void setMeasure3U0S(float average);
            void setMeasure3US(float average);
            void setMeasure3U0(float average);

            void setDeviationUa(float value);
            void setDeviationUb(float value);
            void setDeviationUc(float value);

            void setDeviationUab(float value);
            void setDeviationUbc(float value);
            void setDeviationUca(float value);

            void setDeviation3U0S(float value);
            void setDeviation3US(float value);
            void setDeviation3U0(float value);

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
            Ui::CCalibrationWidgetPower *ui;
    };
#endif // CALIBRATIONWIDGETPOWER_H
