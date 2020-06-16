#ifndef CJOURNALWIDGET_H
    #define CJOURNALWIDGET_H
    //----------------
    #include <QWidget>
    #include <QTableWidget>
    #include <QListWidget>
    #include <QListWidgetItem>
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
    #include <QLabel>
    #include <QDebug>
    #include <QVariant>
    #include "cheaderjournal.h"
    #include "ctablewidgetitem.h"
    #include "cjournaltable.h"
    #include <cjournalpropertymodel.h>
    #include <cjournalpropertydelegate.h>
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
     * \brief The calc_value_t struct
     *
     * Стуктура описывающая расчетные величины.
     */
    struct calc_value_t
    {
        int     id; // id расчетной величины - порядковый номер в буфере значений при чтении
        QString name; // имя расчетной величины
        int     sort_id; // индекс сортировки - разбивание измеряемых величин на логические группы
        int     first; // позиция первого байта
        QString description; // описание расчетной величины
    };
    /*!
     * \brief The variable_t struct
     *
     * Структура описывает внутреннюю переменную.
     */
    struct variable_t
    {
        QString index; // индекс переменной
        QString name; // имя переменной
        QString description; // описание переменной
    };
    /*!
     * \brief variable_list_t
     *
     * Список внутренних переменных
     */
    typedef QVector<variable_t> variable_list_t;
    /*!
     * \brief The io_t struct
     *
     * Структура описывающая выходы.
     */
    struct io_t
    {
        QString index;
        QString description;
    };
    /*!
     * \brief io_list_t
     *
     * Список выходов
     */
    typedef QVector<io_t> io_list_t;
    /*!
     * \brief calc_value_list_t
     *
     * Список расчетных величин
     */
    typedef QVector<calc_value_t> calc_value_list_t;
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
        variable_list_t      variable;
        io_list_t            out;
        io_list_t            input;
        calc_value_list_t    calc;
    };
    /*!
     * \brief The date_t struct
     *
     * Структура для хранения количества дней, часов, минут и секунд
     */
    struct date_t
    {
        int day;
        int hour;
        int min;
        int sec;
    };
    //---------------
    struct halfhour_t
    {
        QVector<float> values;
    };
    //--------------------
    struct halfhour_item_t
    {
        QString name;
        QString description;
    };
    //--------------
    struct set_key_t
    {
        QString key;
        QVector<QString> items;
    };
    //----------------
    struct set_param_t
    {
        set_key_t item;
        QString description;
        QString type;
    };
    //-------------------------------------------------
    typedef QVector<halfhour_item_t> halfhour_labels_t;
    //--------------------------------------------
    typedef QMap<int, set_param_t> set_property_t; // карта для журнала уставок (привязка значение к текстовому описанию контейнера значения)
    //----------------------------------
    class CJournalWidget: public QWidget
    {
        Q_OBJECT

        public:
            enum PropertyType
            {
                EVENT_PROPERTY,
                CRASH_PROPERTY,
                HALFHOUR_PROPERTY,
                ISOLATION_PROPERTY,
                SET_PROPERTY
            };

        public:
            explicit CJournalWidget(QWidget* parent = nullptr);
            ~CJournalWidget();

            CHeaderJournal* header() const;
            void            headerClear() const;
            CJournalTable*  table() const;
            QWidget*        propertyJournal() const;
            void            print(const QVector<quint16>& data) const;

            void setTableHeaders(PropertyType property_type, const QStringList& headers);
            void setTableColumnWidth(const QVector<int>& list);
            void setTableColumnWidth(int column, int width);
            void setJournalDescription(QVariant data);
            void setVisibleProperty(PropertyType property, bool state = false);
            void setHalfhourHeaders(const QVector<halfhour_item_t>& halfhour_cols, const QVector<halfhour_item_t>& halfhour_rows);
            void setPropertySettings(const set_property_t &set);

            void journalClear() const;
            bool journalIsEmpty() const;

            void setRowStartTable(int row);

            static QDateTime unpackDateTime(QVector<quint8>& data);
            static void      convertHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest);
            static date_t    secsToDate(quint32 secs);

        protected:
            void printCrash(const QVector<quint8> &data) const;
            void printEvent(const QVector<quint8> &data) const;
            void printHalfHour(const QVector<quint8> &data) const;
            void printIsolation(const QVector<quint8> &data) const;
            void printSet(const QVector<quint8> &data) const;

        signals:
            void clickedButtonRead(bool = false);
            void printFinished() const;

        public slots:
            void updateTableJournal() const;

        private slots:
            void clickedItemTable(const QModelIndex& index);

        private:
            Ui::CJournalWidget*   ui;
            QVariant              m_journal_data;
            PropertyType          m_property_type;
            int                   m_row_start; // старт порядкового номера строк в таблице
            set_property_t        m_set_property; // для хранения привязок журнала уставок
    };
    // регистрация новых пользовательских типов
    Q_DECLARE_METATYPE(event_t)
    Q_DECLARE_METATYPE(protection_t)
    Q_DECLARE_METATYPE(halfhour_t)
    Q_DECLARE_METATYPE(halfhour_labels_t)
    Q_DECLARE_METATYPE(set_property_t)
#endif // CJOURNALWIDGET_H
