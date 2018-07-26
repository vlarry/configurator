#ifndef CALIBRATIONWIDGET_H
    #define CALIBRATIONWIDGET_H
    //----------------
    #include <QWidget>
    #include <QPainter>
    #include <QLayoutItem>
    #include <QCheckBox>
    #include <QDebug>
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

            int timeSetData() const;
            int timePauseRequest() const;

            void                         addCalibrationIa(float value);
            void                         addCalibrationIb(float value);
            void                         addCalibrationIc(float value);
            void                         addCalibration3I0(float value);
            const calibration_current_t& calibrationCurrent() const;
            float                        calibrationCurrentStandard() const;
            float                        calibrationCurrentStandard3I0() const;
            float                        calibrationCurrentIa() const;
            float                        calibrationCurrentIb() const;
            float                        calibrationCurrentIc() const;
            float                        calibrationCurrent3I0() const;

            void setFactorIa(float value);
            void setFactorIb(float value);
            void setFactorIc(float value);
            void setFactor3I0(float value);

            void setMeasureIa(float average, float deviation);
            void setMeasureIb(float average, float deviation);
            void setMeasureIc(float average, float deviation);
            void setMeasure3I0(float average, float deviation);

            void setAmIa(float value);
            void setAmIb(float value);
            void setAmIc(float value);
            void setAm3I0(float value);

        signals:
            void apply();
            void calibration();
            void calibrationEnd(bool = false);

        public slots:
            void stateButton(bool state = false);

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            Ui::CCalibrationWidget* ui;
            calibration_current_t   m_calibration_current_data;
    };
#endif // CALIBRATIONWIDGET_H
