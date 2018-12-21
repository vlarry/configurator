#ifndef CFILTERDIALOG_H
    #define CFILTERDIALOG_H
    //----------------
    #include <QDialog>
    #include <QButtonGroup>
    #include <QPushButton>
    #include <QDebug>
    #include "RangeSlider.h"
    #include "ctimefilterdialog.h"
    #include "cfilter.h"
    //----------
    namespace Ui
    {
        class CFilterDialog;
    }
    //---------------------------------
    class CFilterDialog: public QDialog
    {
        Q_OBJECT

        public:
            explicit CFilterDialog(const CFilter& filter, QWidget* parent = nullptr);

            const CFilter                     filter();
            const CFilter::FilterIntervalType interval();
            const CFilter::FilterDateType     date();
            const QTime                       time();

        private slots:
            void filterChanged(int index);
            void spinBoxIntervalFromChanged(int value);
            void spinBoxIntervalToChanged(int value);
            void sliderIntervalFromChanged();
            void sliderIntervalToChanged();

        protected:
            void showEvent(QShowEvent* event);

        private:
            Ui::CFilterDialog* ui;
            QButtonGroup*      m_btnGroup;
            int                m_intervalMax;
    };
#endif // CFILTERDIALOG_H
