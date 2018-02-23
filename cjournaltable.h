#ifndef CJOURNALTABLE_H
    #define CJOURNALTABLE_H
    //---------------------
    #include <QTableWidget>
    #include <QWidget>
    #include <QMetaType>
    #include <QDebug>
    /*!
     * \brief protection_set_t
     *
     * Хранение данных по настройкам защит ввиде:
     * ключ - код настройки защиты (передается - в один момент времени активна одна защита) - тип int
     * имя  - имя настройки защиты - тип QString
     *
     * список итемов - список пунктов настройки защиты:
     * имя итема - тип QString
     * имя индекса - тип QString
     */
    typedef QVector<QPair<QString, QString> > protection_item;  // список итемов (name_item, index_item)
    typedef QPair<QString, protection_item>   protection_t;     // Защита (protect_name, list_item)
    typedef QMap<int, protection_t>           protection_set_t;
    Q_DECLARE_METATYPE(protection_set_t) // регистрируем свой тип
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
    Q_DECLARE_METATYPE(event_t)
    /*!
     * \brief The CJournalTable class
     *
     * Наследник от QTableWidget. Реализует интерфесы для работы с журналами.
     */
    class CJournalTable: public QTableWidget
    {
        public:
            CJournalTable(QWidget* parent = nullptr);

            void setRowData(int row, QVariant value);

        private:
            QMap<int, QVariant> m_data_rows;
    };
#endif // CJOURNALTABLE_H
