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
    if(role == Qt::CheckStateRole)
    {
        bool state = ((value == Qt::Checked)?true:false);

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
        if(m_data[index.row()][index.column()].state())
            return Qt::Checked;
        else
            return Qt::Unchecked;
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
//--------------------------------------------
