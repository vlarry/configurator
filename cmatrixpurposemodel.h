#ifndef CMATRIXPURPOSEMODEL_H
    #define CMATRIXPURPOSEMODEL_H
    //----------------------------
    #include <QAbstractTableModel>
    #include <QStyledItemDelegate>
    #include <QApplication>
    #include <QCheckBox>
    #include <QMouseEvent>
    #include <QPainter>
    #include <QDebug>
    #include "cheadertable.h"
    //-----------
    class CColumn
    {
        public:
            CColumn();

            bool active() const;
            bool state() const;
            void setState(bool state);
            void setActive(bool active);

        private:
            bool m_state;
            bool m_active;
    };
    class CRow
    {
        public:
            CRow();
            CRow(const QString& header, int columnSize);

            const QString& header() const;

            void setInactiveColumnList(QVector<int>& list);

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

            void setDisableColumns(int row, QVector<int>& list);

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
            void updateData();
            CDataTable& dataTable();

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
