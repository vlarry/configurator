#ifndef CCALENDARWIDGET_H
    #define CCALENDARWIDGET_H
    //----------------
    #include <QWidget>
    #include <QDate>
    //----------
    namespace Ui
    {
        class CCalendarWidget;
    }
    //-----------------------------------
    class CCalendarWidget: public QWidget
    {
        Q_OBJECT

        public:
            explicit CCalendarWidget(QWidget* parent = nullptr);
            ~CCalendarWidget();

        public slots:
            QDate dateBegin() const;
            QDate dateEnd() const;
            void  setDateRange(const QDate& date_beg, const QDate& date_end);

        signals:
            void dateChanged();

        private:
            Ui::CCalendarWidget* ui;
    };
#endif // CCALENDARWIDGET_H
