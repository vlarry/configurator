#ifndef CFILTERDIALOG_H
    #define CFILTERDIALOG_H
    //----------------
    #include <QDialog>
    #include <QButtonGroup>
    #include <QPushButton>
    #include <QShowEvent>
    #include <QDate>
    #include <QDebug>
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
            //-------------
            enum FilterType
            {
                INTERVAL_TYPE = 0,
                DATE_TYPE
            };
            //--------------------
            struct FilterValueType
            {
                FilterType type;
                int        intervalMax;
                int        intervalBegin;
                int        intervalCount;
                QDate      dateBegin;
                QDate      dateEnd;
            };

        public:
            explicit CFilterDialog(const FilterValueType& value, QWidget* parent = nullptr);
            explicit CFilterDialog(QWidget* parent = nullptr);
            ~CFilterDialog();

            FilterValueType value();
            void            setValue(const FilterValueType& value);

        private slots:
            void filterChanged(QAbstractButton* button);
            void intervalChanged(int value);

        private:
            void initFilter();
            void showEvent(QShowEvent* event);

        private:
            Ui::CFilterDialog* ui;
            QButtonGroup*      m_btnGroup;
            int                m_intervalMax;
    };
#endif // CFILTERDIALOG_H
