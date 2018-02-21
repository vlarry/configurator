#ifndef CJOURNALWIDGET_H
    #define CJOURNALWIDGET_H
    //----------------
    #include <QWidget>
    #include <QTableWidget>
    #include <QTextEdit>
    #include <QDateTime>
    #include <QDebug>
    #include "cheaderjournal.h"
    #include "ctablewidgetitem.h"
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
            /*!
             * \brief Стурктура event_t
             *
             * Хранение данных о событиях (код события, имя события и вектор подсобытий)
             */
            struct event_t
            {
                int              code;
                QString          name;
                QVector<event_t> sub_event;
            };

        public:
            explicit CJournalWidget(QWidget* parent = nullptr);
            ~CJournalWidget();

            CHeaderJournal* header() const;
            void            headerClear() const;
            QTableWidget*   table() const;
            QTextEdit*      propertyJournal() const;
            void            print(const QVector<quint16>& data) const;

            void setTableHeaders(const QStringList& headers);
            void setTableColumnWidth(const QVector<int>& list);
            void setTableColumnWidth(int column, int width);
            void setEventList(const QVector<event_t>& events);
            void setVisibleProperty(bool state = false);

            void tableClear() const;

            static QDateTime unpackDateTime(QVector<quint8>& data);
            static void      convertHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest);

        protected:
            void printCrash(const QVector<quint8>& data) const;
            void printEvent(const QVector<quint8>& data) const;

        signals:
            void clickedButtonRead(bool = false);

        private:
            Ui::CJournalWidget* ui;
            QVector<event_t>    m_event_list;
    };
#endif // CJOURNALWIDGET_H
