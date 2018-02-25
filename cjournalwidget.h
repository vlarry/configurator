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
    /*!
     * \brief Стурктура event_t
     *
     * Хранение данных событий (код события, имя события и вектор подсобытий)
     */
    struct event_t
    {
        int              code;
        QString          name;
        QVector<event_t> sub_event;
    };
    /*!
     * \brief set_item_t
     *
     * Описание одного пункта типа LIST (список вариантов настроек переменной)
     */
    struct set_item_t
    {
        int     row;
        QString name;
    };
    /*!
     * \brief protection_list_set
     *
     * Описание всех наборов настроек переменной - ключ это индекс переменной типа LIST
     */
    typedef QMap<QString, QVector<QString> > protection_list_set; // ключ QString index пункта с типом LIST
    /*!
     * \brief The protection_item_t struct
     *
     * Структура описывающая одну переменную защиты
     */
    struct protection_item_t
    {
        QString name;   // имя переменной
        QString index;  // индекс переменной
        QString type;   // тип переменной (LIST - 1 байт, FLOAT - 4 байта, BIT - 4 байта)
        int     first;  // позиция первого байта
    };
    /*!
     * \brief protection_list_item
     *
     * Список переменных защит - ключ это код защиты (0 - 25)
     */
    typedef QMap<int, QPair<QString, QVector<protection_item_t> > > protection_list_item; // QString - имя защиты
    /*!
     * \brief The protection_t struct
     *
     * Структура хранилища по всем настройкам
     */
    struct protection_t
    {
        protection_list_item items;
        protection_list_set  sets;
    };
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
            void setJournalDescription(QVariant data);
            void setVisibleProperty(bool state = false);

            void tableClear() const;

            static QDateTime unpackDateTime(QVector<quint8>& data);
            static void      convertHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest);

        protected:
            void printCrash(const QVector<quint8>& data) const;
            void printEvent(const QVector<quint8>& data) const;

        signals:
            void clickedButtonRead(bool = false);

        private slots:
            void clickedItemTable(const QModelIndex& index);

        private:
            Ui::CJournalWidget* ui;
            QVariant            m_journal_data;
    };
    // регистрация новых пользовательских типов
    Q_DECLARE_METATYPE(event_t)
    Q_DECLARE_METATYPE(protection_t)
#endif // CJOURNALWIDGET_H
