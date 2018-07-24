#ifndef CALIBRATIONWIDGET_H
    #define CALIBRATIONWIDGET_H
    //----------------
    #include <QWidget>
    #include <QPainter>
    #include <QLayoutItem>
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
            explicit CCalibrationWidget(QWidget* parent = nullptr);
            ~CCalibrationWidget();

        protected:
            void paintEvent(QPaintEvent* event);

        private:
            Ui::CCalibrationWidget* ui;
    };
#endif // CALIBRATIONWIDGET_H
