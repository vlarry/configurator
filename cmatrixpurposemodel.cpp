#include "cmatrixpurposemodel.h"
//-----------------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(QVector<QPair<QString, QString> >& row_labels, group_t& group,
                                         QAbstractTableModel* parent):
    QAbstractTableModel(parent)
{
    fillHorizontalHeaderModel(m_horizontal_header, group);
    fillVerticalHeaderModel(m_vertical_header, row_labels);

    CRow::column_t columns;

    for(int key: group.keys())
    {
        group_item_t item = group[key];

        for(var_t& var: item.var_list)
        {
            columns << CColumn(var.bit, false, var.key, var.name, var.description);
        }
    }

    for(const QPair<QString, QString>& label: row_labels)
    {
        CRow row(label.first, label.second, columns);
        m_matrix.addRow(row);
    }

    m_matrix.setRowCount(row_labels.count());
    m_matrix.setColumnCount(columns.count());
}
//-----------------------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(QVector<QPair<QString, int> >& labels, QAbstractTableModel* parent):
    QAbstractTableModel(parent)
{
    QStringList list;

    for(QPair<QString, int>& label: labels)
        list << label.first;

    fillHeaderProtectionModel(list);

    CRow::column_t columns;

    for(const QString& label: list)
    {
        columns << CColumn(label);
    }

    for(const QString& label: list)
    {
        CRow row("", label, columns);
        m_matrix.addRow(row);
    }

    m_matrix.setRowCount(list.count());
    m_matrix.setColumnCount(list.count());
}
//-------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(const QStringList& rows, const QStringList& columns,
                                         QAbstractTableModel* parent):
    QAbstractTableModel(parent)
{
    fillHeaderMonitorModel(rows, columns);

    CRow::column_t column_list;

    for(const QString& column_label: columns)
        column_list << CColumn(column_label);

    for(const QString& row_label: rows)
    {
        CRow row("", row_label, column_list);
        m_matrix.addRow(row);
    }

    m_matrix.setRowCount(rows.count());
    m_matrix.setColumnCount(columns.count());
}
//------------------------------------
void CMatrixPurposeModel::updateData()
{
    // обновление модели
    QModelIndex topLeft     = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(m_matrix.rowCount() - 1, m_matrix.columnCount() - 1);

    emit dataChanged(topLeft, bottomRight);
}
//-----------------------------------------
CMatrix& CMatrixPurposeModel::matrixTable()
{
    return m_matrix;
}
//-------------------------------------------------------
void CMatrixPurposeModel::setMatrixTable(CMatrix& matrix)
{
    m_matrix = matrix;
}
//----------------------------------------------------------------
int CMatrixPurposeModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_matrix.rowCount();
}
//-------------------------------------------------------------------
int CMatrixPurposeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_matrix.columnCount();
}
//------------------------------------------------------------------------------------------
bool CMatrixPurposeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid() || !value.isValid())
        return false;

    if(role == Qt::CheckStateRole)
    {
//        if(!m_data[index.row()][index.column()].status())
//            return false;

        bool state = ((static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked)?true:false);

//        m_data[index.row()][index.column()].setState(state);
        m_matrix[index.row()][index.column()].setState(state);

        emit dataChanged(index, index);

        return true;
    }

    return false;
}
//--------------------------------------------------------------------------
QVariant CMatrixPurposeModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::CheckStateRole)
    {
        return static_cast<Qt::CheckState>((m_matrix[index.row()][index.column()].state())?Qt::Checked:Qt::Unchecked);
    }

    if(role == Qt::ToolTipRole)
    {
        return QString("%1: %2").arg(m_matrix[index.row()].name()).arg(m_matrix[index.row()][index.column()].description());
    }

    if(role == HierarchicalHeaderView::HorizontalHeaderDataRole)
    {
        QVariant v;
        v.setValue((QObject*)&m_horizontal_header);
        return v;
    }

    if(role == HierarchicalHeaderView::VerticalHeaderDataRole)
    {
        QVariant v;
        v.setValue((QObject*)&m_vertical_header);
        return v;
    }

    return QVariant();
}
//----------------------------------------------------------------------
Qt::ItemFlags CMatrixPurposeModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags itemFlags = QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;

    return itemFlags;
}
//--------------------------------------------------------------------------------------------------
void CMatrixPurposeModel::fillHorizontalHeaderModel(QStandardItemModel& headerModel, group_t& group)
{
    if(group.isEmpty())
        return;

    int keys = 0;

    for(int key: group.keys())
    {
        group_item_t   group_item = group[key];
        QStandardItem* itemGroup  = new QStandardItem(group_item.name);
        QVector<var_t> var_list   = group_item.var_list;

        if(var_list.isEmpty())
            continue;

        for(var_t var: var_list)
        {
            QStandardItem* cell = new QStandardItem(var.name + QString(3, ' ') + QString("(%1)").arg(var.key));
            cell->setData(1, Qt::UserRole);

            itemGroup->appendColumn(QList<QStandardItem*>() << cell);
        }

        headerModel.setItem(0, keys++, itemGroup);
    }
}
//--------------------------------------------------------------------------------
void CMatrixPurposeModel::fillVerticalHeaderModel(QStandardItemModel& headerModel,
                                                  const QVector<QPair<QString, QString> >& labels)
{
    int columns = 0;

    for(const QPair<QString, QString>& label: labels)
    {
        headerModel.setItem(0, columns++, new QStandardItem(label.second));
    }
}
//----------------------------------------------------------------------------
void CMatrixPurposeModel::fillHeaderProtectionModel(const QStringList& labels)
{
    int columns = 0;

    for(const QString& label: labels)
    {
        QStandardItem* hitem = new QStandardItem(label);
        QStandardItem* vitem = new QStandardItem(label);

        hitem->setData(1, Qt::UserRole);

        m_horizontal_header.setItem(0, columns, hitem);
        m_vertical_header.setItem(0, columns++, vitem);
    }
}
//---------------------------------------------------------------------------------------------------
void CMatrixPurposeModel::fillHeaderMonitorModel(const QStringList& rows, const QStringList& columns)
{
    int counter = 0;

    for(const QString& row: rows)
    {
        QStandardItem* item = new QStandardItem(row);

        m_vertical_header.setItem(0, counter++, item);
    }

    counter = 0;

    for(const QString& column: columns)
    {
        QStandardItem* item = new QStandardItem(column);

        item->setData(1, Qt::UserRole);

        m_horizontal_header.setItem(0, counter++, item);
    }
}
//--------------------------------
//------class CItemDelegate-------
//----------------------------------------------------------------------------
CTableItemDelegate::CTableItemDelegate(TableType table_type, QObject* parent):
    QStyledItemDelegate(parent),
    m_table_type(table_type)
{

}
//-------------------------------------------------------------------------------------------------------------------
void CTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionButton checkboxstyle;

    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    QPoint ct(option.rect.x() + option.rect.width()/2, option.rect.y() + option.rect.height()/2);
    QRect  rect;

    rect.setTopLeft(QPoint(ct.x() - checkbox_rect.width()/2, ct.y() - checkbox_rect.height()/2));
    rect.setBottomRight(QPoint(ct.x() + checkbox_rect.width()/2, ct.y() + checkbox_rect.height()/2));

    rect.setTopLeft(QPoint(rect.left() - 1, rect.top() - 2));
    rect.setBottomRight(QPoint(rect.right() + 1, rect.bottom()));

    checkboxstyle.rect = option.rect;

    checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()/2 - checkbox_rect.width()/2);

    bool state = index.model()->data(index, Qt::CheckStateRole).toBool();

    if(state)
        checkboxstyle.state = QStyle::State_On|QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off|QStyle::State_Enabled;

    if(m_table_type == PROTECTION_TYPE && index.row() == index.column())
    {
        int cx = option.rect.x() + option.rect.width()/2;
        int cy = option.rect.y() + option.rect.height()/2;

        QPoint line_topLeft(cx - checkbox_rect.width()/2, cy - checkbox_rect.height()/2 - 1);
        QPoint line_bottomRight(cx + checkbox_rect.width()/2, cy + checkbox_rect.height()/2 -1);
        QPoint line_bottomLeft(cx - checkbox_rect.width()/2, cy + checkbox_rect.height()/2);
        QPoint line_topRight(cx + checkbox_rect.width()/2, cy - checkbox_rect.height()/2);

        painter->save();
            painter->setPen(Qt::gray);
            painter->drawRect(line_topLeft.x(), line_topLeft.y(), checkbox_rect.width(), checkbox_rect.height());
            painter->drawLine(line_topLeft, line_bottomRight);
            painter->drawLine(line_bottomLeft, line_topRight);
        painter->restore();
    }
    else
    {
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
    }
}
//----------------------------------------------------------------------------------------------------------------
bool CTableItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option,
                                     const QModelIndex& index)
{
    Qt::ItemFlags flags = model->flags(index);

    if(!(flags & Qt::ItemIsUserCheckable))
        return false;

    QVariant value = index.data(Qt::CheckStateRole);

    if(!value.isValid())
        return false;

    if(event->type() == QEvent::MouseButtonRelease)
    {
        QStyleOptionButton checkboxstyle;

        QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &option);
        checkboxstyle.rect  = option.rect;

        QPoint ct(option.rect.x() + option.rect.width()/2, option.rect.y() + option.rect.height()/2);

        checkboxstyle.rect.setTopLeft(QPoint(ct.x() - checkbox_rect.width()/2, ct.y() - checkbox_rect.height()/2));
        checkboxstyle.rect.setBottomRight(QPoint(ct.x() + checkbox_rect.width()/2, ct.y() + checkbox_rect.height()/2));

        QPoint point = static_cast<QMouseEvent*>(event)->pos();

        if(!checkboxstyle.rect.contains(point))
            return false;

        Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked?Qt::Unchecked:Qt::Checked);

        return model->setData(index, state, Qt::CheckStateRole);
    }

    return false;
}
