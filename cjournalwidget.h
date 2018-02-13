#ifndef CJOURNALWIDGET_H
    #define CJOURNALWIDGET_H
    //----------------
    #include <QWidget>
    #include <QTableWidget>
    #include <QDebug>
    #include "cheaderjournal.h"
    //----------
    namespace Ui
    {
        class CJournalWidget;
    }
    //----------------------------------
    class CJournalWidget: public QWidget
    {
        Q_OBJECT

        public:
            explicit CJournalWidget(QWidget* parent = nullptr);
            ~CJournalWidget();

            CHeaderJournal* header() const;
            void            headerClear() const;
            QTableWidget*   table() const;

            void setTableHeaders(const QStringList& headers);
            void setTableColumnWidth(const QVector<int>& list);
            void setTableColumnWidth(int column, int width);

            void tableClear() const;

        signals:
            void clickedButtonRead(bool = false);

        private:
            Ui::CJournalWidget* ui;
    };
#endif // CJOURNALWIDGET_H
