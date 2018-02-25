#ifndef CJOURNALTABLE_H
    #define CJOURNALTABLE_H
    //---------------------
    #include <QTableWidget>
    #include <QWidget>
    #include <QClipboard>
    #include <QMessageBox>
    #include <QMetaType>
    #include <QKeyEvent>
    #include <QApplication>
    #include <QDebug>
    /*!
     * \brief The CJournalTable class
     *
     * Наследник от QTableWidget. Реализует интерфесы для работы с журналами.
     */
    class CJournalTable: public QTableWidget
    {
        public:
            CJournalTable(QWidget* parent = nullptr);

            QVariant rowData(int row) const;
            void     setRowData(int row, QVariant value);

        protected:
            void keyPressEvent(QKeyEvent* event);

        private:
            QMap<int, QVariant> m_data_rows;
    };
#endif // CJOURNALTABLE_H
