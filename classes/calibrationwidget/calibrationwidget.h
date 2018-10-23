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
    //----------
    namespace Ui
    {
        class CCalibrationWidget;
    }
    //--------------------------------------
    class CCalibrationWidget: public QWidget
    {
        Q_OBJECT

        public:
            typedef QVector<float> calibration_data_t;
            //--------------------------
            struct calibration_current_t
            {
                calibration_data_t Ia;
                calibration_data_t Ib;
                calibration_data_t Ic;
                calibration_data_t _3I0;
            };

        public:
            explicit CCalibrationWidget(QWidget* parent = nullptr);
            ~CCalibrationWidget();

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
            const calibration_current_t& calibrationCurrent() const;
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

            void setCurrentStandardPhase(float value);
            void setCurrentStandard3I0(float value);

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
            void calibration();
            void calibrationEnd(bool = false);
            void dataIncrement();

        public slots:
            void stateButton(bool state = false);
            void valueCurrentStandardChanged(const QString&);
            void stateChoiceCurrentChannelChanged(bool);
            void saveCalibrationToFlash();
            void progressBarIncrement();

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            Ui::CCalibrationWidget* ui;
            calibration_current_t   m_calibration_current_data;
    };
#endif // CALIBRATIONWIDGET_H
