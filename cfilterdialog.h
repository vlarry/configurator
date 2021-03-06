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
            const CFilter &filter();

        private slots:
            void filterChanged(int index);
            void spinBoxIntervalFromChanged(int value);
            void spinBoxIntervalToChanged(int value);
            void sliderIntervalFromChanged(int value);
            void sliderIntervalToChanged(int value);

        protected:
            void showEvent(QShowEvent* event);

        private:
            Ui::CFilterDialog* ui;
            QButtonGroup*      m_btnGroup;
            CFilter            m_filter;
    };
#endif // CFILTERDIALOG_H
