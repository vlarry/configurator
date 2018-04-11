#ifndef CMATRIXPURPOSEMODEL_H
    #define CMATRIXPURPOSEMODEL_H
    //----------------------------
    #include <QAbstractTableModel>
    #include <QStyledItemDelegate>
    #include <QApplication>
    #include <QCheckBox>
    #include <QMouseEvent>
    #include <QPainter>
    #include <QStandardItemModel>
    #include <QStandardItem>
    #include <QDebug>
    #include "HierarchicalHeaderView.h"
    #include "cheadertable.h"
    #include "cmatrix.h"
    //----------
    struct var_t
    {
        QString key;
        int     group_id;
        int     bit;
        QString name;
        QString description;
    };
    //-----------------
    struct group_item_t
    {
        QString        name;
        QString        description;
        QVector<var_t> var_list;
    };
    //--------------------------------------
    typedef QMap<int, group_item_t> group_t;
    //-----------
    class CColumn
    {
        public:
            typedef QPair<QString, QPair<QString, QString> > column_t; // column data <key, <name, description> >

        public:
            CColumn();
            CColumn(bool state, bool status);

            bool status() const;
            bool state() const;
            void setState(bool state);
            void setStatus(bool status);

        private:
            bool m_state;
            bool m_status;
    };
    //--------
    class CRow
    {
        public:
            CRow();
            CRow(const QString& key, const QString& header, int columnSize);
            CRow(const QString& key, const QString& header, QVector<CColumn>& columns);

            void addColumns(QVector<CColumn>& columns);

            int   columns() const;
            const QString& key() const;
            const QString& header() const;

            void setInactiveColumnList(QVector<int>& list);
            void setActiveColumnList(QVector<int>& list);

            CColumn&       operator [](int index);
            const CColumn& operator [](int index) const;

        private:
            QString          m_key;
            QString          m_header;
            QVector<CColumn> m_columns;
    };
    //--------------
    class CDataTable
    {
        public:
            CDataTable();
            CDataTable(QVector<CRow>& rows, QVector<CColumn::column_t>& columnHeaders);
            CDataTable(QVector<CRow>& rows, QVector<CColumn::column_t>& columnHeaders, group_t& group);

            void addRow(CRow& row);

            int count() const;
            int columnCounts() const;

            int               indexRowFromKey(const QString& key);
            CColumn::column_t columnData(int index) const;
            const QString&    columnName(int index) const;
            QVector<int>      columnIndexListActive(int row);
            QVector<int>      columnIndexListInactive(int row);
            group_t&          group();

            void setColumnHeaders(QVector<CColumn::column_t>& headers);

            void setDisableColumns(int row, QVector<int>& list);
            void setEnableColumns(int row, QVector<int>& list);

            CRow&       operator [](int index);
            const CRow& operator [](int index) const;

        private:
            QVector<CRow>              m_rows;
            QVector<CColumn::column_t> m_columnHeaders;
            group_t                    m_group;

    };
    //---------------------------------------------------
    class CMatrixPurposeModel: public QAbstractTableModel
    {
        public:
            CMatrixPurposeModel(CDataTable& data, QAbstractTableModel* parent = nullptr);
            CMatrixPurposeModel(const QStringList& row_labels, group_t& group, QAbstractTableModel* parent = nullptr);
            CMatrixPurposeModel(QAbstractTableModel* parent = nullptr);
            void updateData();
            CDataTable& dataTable();
            CMatrix&    dataTableNew();
            void setDataTable(CDataTable& data);

        private:
            int           rowCount(const QModelIndex& parent = QModelIndex()) const;
            int           columnCount(const QModelIndex& parent = QModelIndex()) const;
            bool          setData(const QModelIndex& index, const QVariant& value, int role);
            QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const;
            Qt::ItemFlags flags(const QModelIndex& index) const;
            void          fillHorizontalHeaderModel(QStandardItemModel& headerModel, group_t& group);
            void          fillVerticalHeaderModel(QStandardItemModel& headerModel, const QStringList& labels);

        private:
            CDataTable         m_data;
            CMatrix            m_matrix;
            QStandardItemModel m_horizontal_header;
            QStandardItemModel m_vertical_header;
    };
    //--------------------------------------------------
    class CTableItemDelegate: public QStyledItemDelegate
    {
        public:
            CTableItemDelegate(QObject* parent = nullptr);

        protected:
            void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
            bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
    };
#endif // CMATRIXPURPOSEMODEL_H
