#ifndef CMATRIX_H
    #define CMATRIX_H
    //----------------
    #include <QString>
    #include <QVector>
    #include <QDebug>
    //--------------
    class CColumnNew
    {
        public:
            CColumnNew();
            CColumnNew(int bit, bool state, const QString& key, const QString& name, const QString& description);

            int     bit() const;
            QString key() const;
            QString name() const;
            QString description() const;
            bool    state() const;

            void setBit(int bit);
            void setData(const QString& key, const QString& name, const QString& description);
            void setKey(const QString& key);
            void setName(const QString& name);
            void setDescription(const QString& description);
            void setState(bool state);

        private:
            int     m_bit;
            bool    m_state;
            QString m_key;
            QString m_name;
            QString m_description;
    };
    //-----------
    class CRowNew
    {
        public:
            typedef QVector<CColumnNew> column_t;

        public:
            CRowNew();
            CRowNew(const QString& name, column_t& columns);

            void addColumn(CColumnNew& column);

            QString   name() const;
            column_t& columns();

            void setName(const QString& name);
            void setColumns(column_t& columns);

            CColumnNew&       operator [](int index);
            const CColumnNew& operator [](int index) const;

        private:
            QString  m_name;
            column_t m_columns;
    };
    //-----------
    class CMatrix
    {
        public:
            typedef QVector<CRowNew> row_t;

        public:
            CMatrix();
            CMatrix(row_t& rows, int columnCount);

            void addRow(CRowNew& row);

            row_t& rows();

            int rowCount() const;
            int columnCount() const;

            void setRowCount(int count);
            void setColumnCount(int count);

            CRowNew&       operator [](int index);
            const CRowNew& operator [](int index) const;

        private:
            row_t m_rows;
            int   m_rowCount;
            int   m_columnCount;
    };
#endif // CMATRIX_H
