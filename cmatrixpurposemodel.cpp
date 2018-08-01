#include "cmatrixpurposemodel.h"
//--------------------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(QVector<QPair<QString, QString> >& label_columns, group_t& group,
                                         IO_Type io_type, QAbstractTableModel* parent):
    QAbstractTableModel(parent),
    m_io_type(io_type)
{
    fillVerticalHeaderModel(m_vertical_header, group);
    fillHorizontalHeaderModel(m_horizontal_header, label_columns);

    CRow::ColumnArray columns;

    for(const QPair<QString, QString>& column: label_columns) // создаем список колонок таблицы
    {
        columns << CColumn(unit_t({ column.first, "", column.second, -1, UNCHECKED }));
    }

    for(int group_id: group.keys())
    {
        group_item_t item = group[group_id];

        for(const var_t& var: item.var_list)
        {
            CRow row(unit_t({ var.key, var.name, var.description, var.bit, UNCHECKED }), columns);
            m_matrix.addRow(row);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(const QStringList& labels, IO_Type io_type, QAbstractTableModel* parent):
    QAbstractTableModel(parent),
    m_io_type(io_type)
{
    fillHeaderProtectionModel(labels);

    CRow::ColumnArray columns;

    for(const QString& label: labels)
    {
        columns << CColumn(unit_t({ label, "", "", -1, UNCHECKED }));
    }

    for(const QString& label: labels)
    {
        CRow row(unit_t({ "", label, "", -1, UNCHECKED }), columns);
        m_matrix.addRow(row);
    }
}
//------------------------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(const QStringList& columns, const QStringList& rows, IO_Type io_type,
                                         QAbstractTableModel* parent):
    QAbstractTableModel(parent),
    m_io_type(io_type)
{
    fillHeaderMonitorModel(rows, columns);

    CRow::ColumnArray column_array;

    for(const QString& column_label: columns)
        column_array << CColumn(unit_t({ column_label, "", "", -1, UNCHECKED }));

    for(const QString& row_label: rows)
    {
        CRow row(unit_t({ "", row_label, "", -1, UNCHECKED }), column_array);
        m_matrix.addRow(row);
    }
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
//------------------------------------------------------------------
CMatrixPurposeModel::IO_Type CMatrixPurposeModel::ioDataType() const
{
    return m_io_type;
}
//------------------------------------------------------------------------------------------
bool CMatrixPurposeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid() || !value.isValid())
        return false;

    if(role == Qt::CheckStateRole)
    {
        StateType state = static_cast<StateType>(value.toInt());

        m_matrix[index.row()].data().state = state;

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
        return static_cast<Qt::CheckState>(m_matrix[index.row()].data().state);
    }

    if(role == Qt::ToolTipRole)
    {
        return QString("%1: %2").arg(m_matrix[index.row()].data().name).
                                 arg(m_matrix[index.row()].data().description);
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
//------------------------------------------------------------------------------------------------
void CMatrixPurposeModel::fillVerticalHeaderModel(QStandardItemModel& headerModel, group_t& group)
{
    if(group.isEmpty())
        return;

    int keys = 0;

    if(m_io_type == IO_OUTPUT)
    {
        QStandardItem* itemGroup = new QStandardItem("");
        QStandardItem* cell      = new QStandardItem(tr("Запоминание выходов"));

        itemGroup->setData(1, Qt::UserRole);

        itemGroup->appendColumn(QList<QStandardItem*>() << cell);
        headerModel.setItem(0, keys++, itemGroup);
    }

    for(int key: group.keys())
    {
        group_item_t   group_item = group[key];
        QStandardItem* itemGroup  = new QStandardItem(group_item.name);
        QVector<var_t> var_list   = group_item.var_list;

        itemGroup->setData(1, Qt::UserRole);

        if(var_list.isEmpty())
            continue;

        for(var_t var: var_list)
        {
            QStandardItem* cell = new QStandardItem(var.name + QString(3, ' ') + QString("(%1)").arg(var.key));

            itemGroup->appendColumn(QList<QStandardItem*>() << cell);
        }

        headerModel.setItem(0, keys++, itemGroup);
    }
}
//----------------------------------------------------------------------------------
void CMatrixPurposeModel::fillHorizontalHeaderModel(QStandardItemModel& headerModel,
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
    QStandardItem* hitemGroup = new QStandardItem(tr("Заблокировать защиту"));
    m_horizontal_header.setItem(0, 0, hitemGroup);
    QStandardItem* vitemGroup = new QStandardItem(tr("Блокирующая защита"));
    vitemGroup->setData(1, Qt::UserRole);
    m_vertical_header.setItem(0, 0, vitemGroup);

    for(const QString& label: labels)
    {
        QStandardItem* hitem = new QStandardItem(label);
        QStandardItem* vitem = new QStandardItem(label);

        hitem->setData(1, Qt::UserRole);

        hitemGroup->appendColumn(QList<QStandardItem*>() << hitem);
        vitemGroup->appendColumn(QList<QStandardItem*>() << vitem);
    }

    m_horizontal_header.setItem(0, 0, hitemGroup);
    m_vertical_header.setItem(0, 0, vitemGroup);
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
//------------------------------------------------------------------------------------------
CTableItemDelegate::CTableItemDelegate(TableType table_type, bool inverse, QObject* parent):
    QStyledItemDelegate(parent),
    m_table_type(table_type),
    m_inverse(inverse)
{

}
//-------------------------------------------------------------------------------------------------------------------
void CTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionButton checkboxstyle;

    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    int cx = option.rect.x() + option.rect.width()/2;
    int cy = option.rect.y() + option.rect.height()/2;

    QPoint line_topLeft(cx - checkbox_rect.width()/2, cy - checkbox_rect.height()/2 - 1);
    QPoint line_bottomRight(cx + checkbox_rect.width()/2, cy + checkbox_rect.height()/2 -1);
    QPoint line_bottomLeft(cx - checkbox_rect.width()/2, cy + checkbox_rect.height()/2);
    QPoint line_topRight(cx + checkbox_rect.width()/2, cy - checkbox_rect.height()/2);

    QRect rect = QRect(line_topLeft.x(), line_topLeft.y(), checkbox_rect.width(), checkbox_rect.height());

    painter->save();
        painter->drawRect(rect);

        if(m_table_type == PROTECTION_TYPE && index.row() == index.column())
        {
            painter->setPen(Qt::gray);
            painter->drawLine(line_topLeft, line_bottomRight);
            painter->drawLine(line_bottomLeft, line_topRight);
        }
        else
        {
            StateType state = static_cast<StateType>(index.model()->data(index, Qt::CheckStateRole).toInt());

            switch(state)
            {
                case UNCHECKED:
                break;

                case CHECKED:
                    painter->drawPixmap(rect, QPixmap(":/images/resource/images/checbox_tick.png"));
                break;

                case INVERSE:
                    int x = rect.left() + rect.width()/4;
                    int y = rect.top() + rect.height()/4;
                    int w = rect.width()/2;
                    int h = rect.height()/2;

                    painter->fillRect(x, y, w + 1, h + 1, Qt::SolidPattern);
                break;
            }
        }
    painter->restore();
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

        StateType state = static_cast<StateType>(value.toInt());

        if(state == UNCHECKED)
        {
            if(m_inverse)
                state = INVERSE;
            else
                state = CHECKED;
        }
        else if(state == INVERSE)
            state = CHECKED;
        else if(state == CHECKED)
            state = UNCHECKED;

        return model->setData(index, state, Qt::CheckStateRole);
    }

    return false;
}
