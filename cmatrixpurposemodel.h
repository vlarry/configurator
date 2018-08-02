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
    //---------------------------------------------------
    class CMatrixPurposeModel: public QAbstractTableModel
    {
        public:
            enum IO_Type
            {
                IO_INPUT,
                IO_OUTPUT
            };

        public:
            CMatrixPurposeModel(QVector<QPair<QString, QString> >& label_columns, group_t& group,
                                IO_Type io_type = IO_INPUT, QAbstractTableModel* parent = nullptr);
            CMatrixPurposeModel(const QStringList& labels, IO_Type io_type = IO_INPUT,
                                QAbstractTableModel* parent = nullptr);
            CMatrixPurposeModel(const QStringList& rows, const QStringList& columns, IO_Type io_type = IO_INPUT,
                                QAbstractTableModel* parent = nullptr);
            void     updateData();
            CMatrix& matrix();
            void     setMatrixTable(CMatrix& matrix);
            int      rowCount(const QModelIndex& parent = QModelIndex()) const;
            int      columnCount(const QModelIndex& parent = QModelIndex()) const;
            IO_Type  ioDataType() const;

        private:
            bool          setData(const QModelIndex& index, const QVariant& value, int role);
            QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const;
            Qt::ItemFlags flags(const QModelIndex& index) const;
            void          fillVerticalHeaderModel(QStandardItemModel& headerModel, group_t& group);
            void          fillHorizontalHeaderModel(QStandardItemModel& headerModel,
                                                  const QVector<QPair<QString, QString> >& labels);
            void          fillHeaderProtectionModel(const QStringList& labels);
            void          fillHeaderMonitorModel(const QStringList& rows, const QStringList& columns);

        private:
            CMatrix            m_matrix;
            QStandardItemModel m_horizontal_header;
            QStandardItemModel m_vertical_header;
            IO_Type            m_io_type;
    };
    //--------------------------------------------------
    class CTableItemDelegate: public QStyledItemDelegate
    {
        public:
            enum TableType
            {
                PURPOSE_TYPE,
                PROTECTION_TYPE
            };

        public:
            CTableItemDelegate(TableType table_type, bool inverse = false, QObject* parent = nullptr);

        protected:
            void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
            bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

        private:
            TableType m_table_type;
            bool      m_inverse;
    };
#endif // CMATRIXPURPOSEMODEL_H
