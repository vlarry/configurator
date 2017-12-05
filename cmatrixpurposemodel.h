#ifndef CMATRIXPURPOSEMODEL_H
    #define CMATRIXPURPOSEMODEL_H
    //----------------------------
    #include <QAbstractTableModel>
    #include <QCheckBox>
    //---------------------------------------------------
    class CMatrixPurposeModel: public QAbstractTableModel
    {
        public:
            typedef QVector<QPair<QString*, QCheckBox*> > cell_t;
            //------------
            struct RowType
            {
                QString         header;
                QVector<cell_t> column;
            };

        public:
            CMatrixPurposeModel(QAbstractTableModel* parent = nullptr);
            CMatrixPurposeModel(QVector<RowType>& data, QAbstractTableModel* parent = nullptr);

        private:
            int      rowCount(const QModelIndex& parent = QModelIndex()) const;
            int      columnCount(const QModelIndex& parent = QModelIndex()) const;
            QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

        private:
            QVector<RowType> m_data;
    };
#endif // CMATRIXPURPOSEMODEL_H
