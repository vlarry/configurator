#ifndef CJOURNALWIDGET_H
    #define CJOURNALWIDGET_H
    //----------------
    #include <QWidget>
    #include <QTableWidget>
    #include <QTextEdit>
    #include <QDateTime>
    #include <QKeyEvent>
    #include <QMessageBox>
    #include <QTableWidgetSelectionRange>
    #include <QClipboard>
    #include <QList>
    #include <QTableWidgetItem>
    #include <QMouseEvent>
    #include <QEvent>
    #include <QMetaType>
    #include <QDebug>
    #include <QVariant>
    #include "cheaderjournal.h"
    #include "ctablewidgetitem.h"
    #include "cjournaltable.h"
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
            QTextEdit*      propertyJournal() const;
            void            print(const QVector<quint16>& data) const;

            void setTableHeaders(const QStringList& headers);
            void setTableColumnWidth(const QVector<int>& list);
            void setTableColumnWidth(int column, int width);
            void setJournalDataType(QVariant data);
            void setVisibleProperty(bool state = false);

            void tableClear() const;

            static QDateTime unpackDateTime(QVector<quint8>& data);
            static void      convertHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest);

        protected:
            void printCrash(const QVector<quint8>& data) const;
            void printEvent(const QVector<quint8>& data) const;
            void keyPressEvent(QKeyEvent* event);
            void mousePressEvent(QMouseEvent* event);

        signals:
            void clickedButtonRead(bool = false);

        private:
            Ui::CJournalWidget* ui;
            QVariant            m_journal_data;
    };
#endif // CJOURNALWIDGET_H
