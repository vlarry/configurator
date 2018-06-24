#ifndef CMATRIX_H
    #define CMATRIX_H
    //----------------
    #include <QString>
    #include <QVector>
    #include <QDebug>
    //-----------
    class CColumn
    {
        public:
            enum StateType
            {
                INACTIVE,
                INVERSE_ACTIVE,
                NORMAL_ACTIVE
            };

        public:
            CColumn();
            CColumn(const QString& name);
            CColumn(int bit, StateType state, const QString& key, const QString& name, const QString& description);

            int     bit() const;
            QString key() const;
            QString name() const;
            QString description() const;
            StateType state() const;

            void setBit(int bit);
            void setData(const QString& key, const QString& name, const QString& description);
            void setKey(const QString& key);
            void setName(const QString& name);
            void setDescription(const QString& description);
            void setState(StateType state);

        private:
            int       m_bit;
            StateType m_state;
            QString   m_key;
            QString   m_name;
            QString   m_description;
    };
    //--------
    class CRow
    {
        public:
            typedef QVector<CColumn> column_t;

        public:
            CRow();
            CRow(const QString& key, const QString& name, column_t& columns);

            void addColumn(CColumn& column);

            QString   key() const;
            QString   name() const;
            column_t& columns();

            void setKey(const QString& key);
            void setName(const QString& name);
            void setColumns(column_t& columns);

            CColumn&       operator [](int index);
            const CColumn& operator [](int index) const;

        private:
            QString  m_key;
            QString  m_name;
            column_t m_columns;
    };
    //-----------
    class CMatrix
    {
        public:
            typedef QVector<CRow> row_t;

        public:
            CMatrix();
            CMatrix(row_t& rows, int columnCount);

            void addRow(CRow& row);

            row_t& rows();

            int rowIndexByKey(const QString& key);
            int rowCount() const;
            int columnCount() const;

            void setRowCount(int count);
            void setColumnCount(int count);

            CRow&       operator [](int index);
            const CRow& operator [](int index) const;

        private:
            row_t m_rows;
            int   m_rowCount;
            int   m_columnCount;
    };
#endif // CMATRIX_H
