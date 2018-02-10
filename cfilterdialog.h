#ifndef CFILTERDIALOG_H
    #define CFILTERDIALOG_H
    //----------------
    #include <QDialog>
    #include <QButtonGroup>
    #include <QPushButton>
    #include <QDebug>
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

        private slots:
            void filterChanged(QAbstractButton* button);
            void intervalChanged(int value);

        private:
            Ui::CFilterDialog* ui;
            QButtonGroup*      m_btnGroup;
            int                m_intervalMax;
    };
#endif // CFILTERDIALOG_H
