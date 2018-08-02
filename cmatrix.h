#ifndef CMATRIX_H
    #define CMATRIX_H
    //----------------
    #include <QString>
    #include <QVector>
    #include <QDebug>
    //------------
    enum StateType
    {
        UNCHECKED,
        INVERSE,
        CHECKED
    };
    //-----------
    struct unit_t
    {
        QString   key;
        QString   name;
        QString   description;
        int       position;
        StateType state;
    };
    //-----------
    class CColumn
    {
        public:
            CColumn();
            CColumn(const unit_t& data);

            unit_t& data();
            const unit_t& data() const;

        private:
            unit_t m_data;
    };
    //--------
    class CRow
    {
        public:
            typedef QVector<CColumn> ColumnArray;

        public:
            CRow();
            CRow(const unit_t& data, const ColumnArray& columns = ColumnArray(0));

            unit_t& data();
            const unit_t& data() const;

            int count() const;
            ColumnArray& columns();
            const ColumnArray& columns() const;

            CColumn&       operator [](int index);
            const CColumn& operator [](int index) const;

        private:
            unit_t      m_data;
            ColumnArray m_column_array;
    };
    //-----------
    class CMatrix
    {
        public:
            typedef QVector<CRow> RowArray;

        public:
            CMatrix();

            void addRow(const CRow& row);
            int  columnCount() const;
            int  rowCount() const;
            int  columnIndexByKey(const QString& key);

            RowArray&       rows();
            const RowArray& rows() const;

            CRow&       operator [](int index);
            const CRow& operator [](int index) const;

        private:
            RowArray m_matrix;
    };
#endif // CMATRIX_H
