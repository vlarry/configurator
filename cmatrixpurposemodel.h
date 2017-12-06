#ifndef CMATRIXPURPOSEMODEL_H
    #define CMATRIXPURPOSEMODEL_H
    //----------------------------
    #include <QAbstractTableModel>
    #include <QStyledItemDelegate>
    #include <QApplication>
    #include <QCheckBox>
    #include <QDebug>
    //-----------
    class CColumn
    {
        public:
            CColumn();

            bool state() const;
            void setState(bool state);

        private:
            bool m_state;
    };
    class CRow
    {
        public:
            CRow();
            CRow(const QString& header, int columnSize);

            const QString& header() const;

            CColumn&       operator [](int index);
            const CColumn& operator [](int index) const;

        private:
            QString          m_header;
            QVector<CColumn> m_columns;
    };
    //--------------
    class CDataTable
    {
        public:
            CDataTable(QVector<CRow>& rows, QStringList& columnHeaders);

            int count() const;
            int columnCounts() const;

            const QString& columnName(int index) const;

            CRow&       operator [](int index);
            const CRow& operator [](int index) const;

        private:
            QVector<CRow> m_rows;
            QStringList   m_columnHeaders;
    };
    //---------------------------------------------------
    class CMatrixPurposeModel: public QAbstractTableModel
    {
        public:
            CMatrixPurposeModel(CDataTable& data, QAbstractTableModel* parent = nullptr);
            CDataTable&   dataTable();

        private:
            int           rowCount(const QModelIndex& parent = QModelIndex()) const;
            int           columnCount(const QModelIndex& parent = QModelIndex()) const;
            bool          setData(const QModelIndex& index, const QVariant& value, int role);
            QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const;
            QVariant      headerData(int section, Qt::Orientation orientation, int role) const;
            Qt::ItemFlags flags(const QModelIndex& index) const;

        private:
            CDataTable m_data;
    };
#endif // CMATRIXPURPOSEMODEL_H
