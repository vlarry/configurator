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
            CMatrixPurposeModel(QVector<QPair<QString, QString> >& row_labels, group_t& group,
                                QAbstractTableModel* parent = nullptr);
            CMatrixPurposeModel(QAbstractTableModel* parent = nullptr);
            void     updateData();
            CMatrix& matrixTable();
            void     setMatrixTable(CMatrix& matrix);

        private:
            int           rowCount(const QModelIndex& parent = QModelIndex()) const;
            int           columnCount(const QModelIndex& parent = QModelIndex()) const;
            bool          setData(const QModelIndex& index, const QVariant& value, int role);
            QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const;
            Qt::ItemFlags flags(const QModelIndex& index) const;
            void          fillHorizontalHeaderModel(QStandardItemModel& headerModel, group_t& group);
            void          fillVerticalHeaderModel(QStandardItemModel& headerModel,
                                                  const QVector<QPair<QString, QString> >& labels);

        private:
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
