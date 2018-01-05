#include "cmatrixpurposemodel.h"
//--------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(CDataTable &data, QAbstractTableModel* parent):
    QAbstractTableModel(parent),
    m_data(data)
{
    int pos = 0;

    for(int i = 0; i < m_data.columnCounts(); i++) // сортировка активных ячеек - перенос в начало
    {
        bool pos_changed = false;

        for(int j = 0; j < m_data.count(); j++) // проход по всем строкам
        {
            CColumn column = m_data[j][i];

            if(column.active())
            {
                if(pos != i)
                {
                    CColumn tcolumn = m_data[j][pos];

                    m_data[j][pos] = column;
                    m_data[j][i] = tcolumn;

                    if(!pos_changed)
                        pos_changed = true;
                }
                else
                {
                    if(!pos_changed)
                        pos_changed = true;
                }
            }
        }

        if(pos_changed)
        {
            QString columnName = m_data.columnName(i);

            m_data.setColumnName(i, m_data.columnName(pos));
            m_data.setColumnName(pos, columnName);

            pos++;
        }
    }
}
//------------------------------------
void CMatrixPurposeModel::updateData()
{
    // обновление модели
    QModelIndex topLeft     = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(m_data.count() - 1, m_data.columnCounts() - 1);

    emit dataChanged(topLeft, bottomRight);
}
//------------------------------------------
CDataTable& CMatrixPurposeModel::dataTable()
{
    return m_data;
}
//------------------------------------------------------
void CMatrixPurposeModel::setDataTable(CDataTable& data)
{
    m_data = data;
}
//----------------------------------------------------------------
int CMatrixPurposeModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}
//-------------------------------------------------------------------
int CMatrixPurposeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_data.columnCounts();
}
//------------------------------------------------------------------------------------------
bool CMatrixPurposeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid() || !value.isValid())
        return false;

    if(role == Qt::CheckStateRole)
    {
        if(!m_data[index.row()][index.column()].active())
            return false;

        bool state = ((static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked)?true:false);

        m_data[index.row()][index.column()].setState(state);

        emit dataChanged(index, index);

        return true;
    }

    return false;
}
//--------------------------------------------------------------------------
QVariant CMatrixPurposeModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::CheckStateRole)
    {
        Qt::CheckState state;

        if(m_data[index.row()][index.column()].active())
            state = static_cast<Qt::CheckState>((m_data[index.row()][index.column()].state())?Qt::Checked:Qt::Unchecked);
        else
            state = Qt::Unchecked;

        return state;
    }
    else if(role == Qt::UserRole)
    {
        return m_data[index.row()][index.column()].active();
    }

    return QVariant();
}
//------------------------------------------------------------------------------------------------
QVariant CMatrixPurposeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal)
        return m_data.columnName(section);
    else if(orientation == Qt::Vertical)
        return m_data[section].header();

    return QVariant();
}
//----------------------------------------------------------------------
Qt::ItemFlags CMatrixPurposeModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags itemFlags = QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;

    return itemFlags;
}
//----------------------
//---class CDataTable---
//----------------------------------------------------------------------
CDataTable::CDataTable(QVector<CRow>& rows, QStringList& columnHeaders):
    m_rows(rows),
    m_columnHeaders(columnHeaders)
{

}
//---------------------------
int CDataTable::count() const
{
    return m_rows.count();
}
//----------------------------------
int CDataTable::columnCounts() const
{
    return m_columnHeaders.count();
}
//-------------------------------------------------
int CDataTable::indexRowFromKey(const QString& key)
{
    for(int i = 0; i < m_rows.count(); i++)
    {
        if(m_rows[i].key().toUpper() == key.toUpper())
            return i;
    }

    return -1;
}
//----------------------------------------------------
const QString& CDataTable::columnName(int index) const
{
    return m_columnHeaders.at(index);
}
//-----------------------------------------------------
QVector<int> CDataTable::columnIndexListActive(int row)
{
    QVector<int> list = QVector<int>();

    if(!m_rows.isEmpty())
    {
        for(int i = 0; i < columnCounts(); i++)
        {
            if(m_rows[row][i].active())
                list << i;
        }
    }

    return list;
}
//------------------------------------------------------------
void CDataTable::setColumnName(int index, const QString& name)
{
    m_columnHeaders.replace(index, name);
}
//-------------------------------------------------------------
void CDataTable::setDisableColumns(int row, QVector<int>& list)
{
    m_rows[row].setInactiveColumnList(list);
}
//------------------------------------------------------------
void CDataTable::setEnableColumns(int row, QVector<int>& list)
{
    m_rows[row].setActiveColumnList(list);
}
//--------------------------------------
CRow& CDataTable::operator [](int index)
{
    return m_rows[index];
}
//--------------------------------------------------
const CRow& CDataTable::operator [](int index) const
{
    return m_rows[index];
}
//-------------------------
//-------class CRow--------
//-----------
CRow::CRow():
    m_key(""),
    m_header(""),
    m_columns(QVector<CColumn>(0))
{

}
//--------------------------------------------------------------------
CRow::CRow(const QString& key, const QString& header, int columnSize):
    m_key(key),
    m_header(header),
    m_columns(QVector<CColumn>(columnSize, CColumn()))
{

}
//-----------------------
int CRow::columns() const
{
    return m_columns.count();
}
//------------------------------
const QString& CRow::key() const
{
    return m_key;
}
//---------------------------------
const QString& CRow::header() const
{
    return m_header;
}
//--------------------------------------------------
void CRow::setInactiveColumnList(QVector<int>& list)
{
    if(list.isEmpty())
        return;

    for(int index: list)
        m_columns[index].setActive(false);
}
//------------------------------------------------
void CRow::setActiveColumnList(QVector<int>& list)
{
    if(list.isEmpty())
        return;

    for(int index: list)
        m_columns[index].setActive(true);
}
//-----------------------------------
CColumn& CRow::operator [](int index)
{
    return m_columns[index];
}
//-----------------------------------------------
const CColumn& CRow::operator [](int index) const
{
    return m_columns[index];
}
//-----------------
//--class CColumn--
//-----------------
CColumn::CColumn():
    m_state(false),
    m_active(false)
{

}
//--------------------------
bool CColumn::active() const
{
    return m_active;
}
//-------------------------
bool CColumn::state() const
{
    return m_state;
}
//--------------------------------
void CColumn::setState(bool state)
{
    m_state = state;
}
//----------------------------------
void CColumn::setActive(bool active)
{
    m_active = active;
}
//--------------------------------
//------class CItemDelegate-------
//------------------------------------------------------
CTableItemDelegate::CTableItemDelegate(QObject* parent):
    QStyledItemDelegate(parent)
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

    if(!index.data(Qt::UserRole).toBool())
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
