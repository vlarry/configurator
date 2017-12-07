#include "cmatrixpurposemodel.h"
//--------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(CDataTable &data, QAbstractTableModel* parent):
    QAbstractTableModel(parent),
    m_data(data)
{

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
        Qt::CheckState state = static_cast<Qt::CheckState>((m_data[index.row()][index.column()].state())?Qt::Checked:
                                                                                                         Qt::Unchecked);

        return state;
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
//------------------------------------------
CDataTable& CMatrixPurposeModel::dataTable()
{
    return m_data;
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
//----------------------------------------------------
const QString& CDataTable::columnName(int index) const
{
    return m_columnHeaders.at(index);
}
//------------------------------------------------
const QStringList &CDataTable::columnNames() const
{
    return m_columnHeaders;
}
//---------------------------------------------
const QStringList CDataTable::rowNames() const
{
    QStringList names;

    for(const CRow& row: m_rows)
        names << row.header();

    return names;
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
    m_header(""),
    m_columns(QVector<CColumn>(0))
{

}
//------------------------------------------------
CRow::CRow(const QString& header, int columnSize):
    m_header(header),
    m_columns(QVector<CColumn>(columnSize, CColumn()))
{

}
//---------------------------------
const QString& CRow::header() const
{
    return m_header;
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
    m_state(false)
{

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
    bool data = index.model()->data(index, Qt::CheckStateRole).toBool();

    QStyleOptionButton checkboxstyle;

    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, &checkboxstyle);

    checkboxstyle.rect = option.rect;

    checkboxstyle.rect.setLeft(option.rect.x() + option.rect.width()/2 - checkbox_rect.width()/2);

    if(data)
        checkboxstyle.state = QStyle::State_On|QStyle::State_Enabled;
    else
        checkboxstyle.state = QStyle::State_Off|QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter);
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
