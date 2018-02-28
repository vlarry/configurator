#ifndef CJOURNALPROPERTYMODEL_H
    #define CJOURNALPROPERTYMODEL_H
    //---------------------------
    #include <QAbstractListModel>
    //----------------------------------------------------
    class CJournalPropertyModel: public QAbstractListModel
    {
        Q_OBJECT

        public:
            CJournalPropertyModel(QObject* parent = nullptr);
            int      rowCount(const QModelIndex& parent = QModelIndex()) const;
            QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
            void     setDataModel(QVector<QPair<QString, QString> >& data);

        private:
            QVector<QPair<QString, QString> > m_data;
    };
#endif // CJOURNALPROPERTYMODEL_H
