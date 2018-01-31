#ifndef CEVENTLISTJOURNALS_H
    #define CEVENTLISTJOURNALS_H
    //----------------
    #include <QDialog>
    #include <QListWidget>
    #include <QListWidgetItem>
    #include <QModelIndex>
    #include <QDebug>
    //----------
    namespace Ui
    {
        class CEventListJournals;
    }
    //--------------------------------------
    class CEventListJournals: public QDialog
    {
        Q_OBJECT

        public:
            typedef struct
            {
                QString name;
                int     id;
            } cell_t;

        public:
            explicit CEventListJournals(QVector<cell_t>& list, QWidget* parent = nullptr);
            ~CEventListJournals();

            int currentId() const;

        public slots:
            void slotDoubleClicked(QListWidgetItem* item);

        private:
            Ui::CEventListJournals* ui;
            QVector<cell_t>         m_cells;
    };

#endif // CEVENTLISTJOURNALS_H
