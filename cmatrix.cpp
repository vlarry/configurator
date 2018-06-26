#include "cmatrix.h"
//-----------------
CMatrix::CMatrix():
    m_rows(row_t(0, CRow())),
    m_rowCount(0),
    m_columnCount(0)
{

}
//------------------------------------------------------
CMatrix::CMatrix(CMatrix::row_t& rows, int columnCount):
    m_rows(rows),
    m_rowCount(rows.count()),
    m_columnCount(columnCount)
{

}
//--------------------------------
void CMatrix::addRow(CRow& row)
{
    m_rows.push_back(row);
}
//-----------------------------
CMatrix::row_t& CMatrix::rows()
{
    return m_rows;
}
//--------------------------------------------
int CMatrix::rowIndexByKey(const QString& key)
{
    int index = -1;

    for(int i = 0; i < m_rowCount; i++)
    {
        if(m_rows[i].key().toUpper() == key.toUpper())
        {
            index = i;
            break;
        }
    }

    return index;
}
//---------------------------
int CMatrix::rowCount() const
{
    return m_rowCount;
}
//------------------------------
int CMatrix::columnCount() const
{
    return m_columnCount;
}
//----------------------------------
void CMatrix::setRowCount(int count)
{
    m_rowCount = count;
}
//-------------------------------------
void CMatrix::setColumnCount(int count)
{
    m_columnCount = count;
}
//-----------------------------------
CRow& CMatrix::operator [](int index)
{
    return m_rows[index];
}
//-----------------------------------------------
const CRow& CMatrix::operator [](int index) const
{
    return m_rows[index];
}
//-----Класс колонка-----
//-----------------------
CColumn::CColumn():
    m_bit(-1),
    m_state(StateType::UNCHECKED),
    m_key(""),
    m_name(""),
    m_description("")
{

}
//------------------------------------
CColumn::CColumn(const QString& name):
    m_bit(-1),
    m_state(StateType::UNCHECKED),
    m_key(""),
    m_name(name),
    m_description("")
{

}
//--------------------------------------------------------------------------------------------------------------
CColumn::CColumn(int bit, StateType state, const QString& key, const QString& name, const QString& description):
    m_bit(bit),
    m_state(state),
    m_key(key),
    m_name(name),
    m_description(description)
{

}
//----------------------
int CColumn::bit() const
{
    return m_bit;
}
//--------------------------
QString CColumn::key() const
{
    return m_key;
}
//---------------------------
QString CColumn::name() const
{
    return m_name;
}
//----------------------------------
QString CColumn::description() const
{
    return m_description;
}
//---------------------------------------
CColumn::StateType CColumn::state() const
{
    return m_state;
}
//---------------------------
void CColumn::setBit(int bit)
{
    m_bit = bit;
}
//----------------------------------------------------------------------------------------
void CColumn::setData(const QString& key, const QString& name, const QString& description)
{
    m_key         = key;
    m_name        = name;
    m_description = description;
}
//--------------------------------------
void CColumn::setKey(const QString& key)
{
    m_key = key;
}
//----------------------------------------
void CColumn::setName(const QString& name)
{
    m_name = name;
}
//------------------------------------------------------
void CColumn::setDescription(const QString& description)
{
    m_description = description;
}
//-------------------------------------
void CColumn::setState(StateType state)
{
    m_state = state;
}
//--Класс строка---
//-----------------
CRow::CRow():
    m_key(""),
    m_name(""),
    m_columns(column_t(0, CColumn()))
{

}
//---------------------------------------------------------------------------
CRow::CRow(const QString& key, const QString& name, CRow::column_t& columns):
    m_key(key),
    m_name(name),
    m_columns(columns)
{

}
//-----------------------------------
void CRow::addColumn(CColumn& column)
{
    m_columns.push_back(column);
}
//-----------------------
QString CRow::key() const
{
    return m_key;
}
//------------------------
QString CRow::name() const
{
    return m_name;
}
//-----------------------------
CRow::column_t& CRow::columns()
{
    return m_columns;
}
//-----------------------------------
void CRow::setKey(const QString &key)
{
    m_key = key;
}
//-------------------------------------
void CRow::setName(const QString& name)
{
    m_name = name;
}
//--------------------------------------------
void CRow::setColumns(CRow::column_t& columns)
{
    m_columns = columns;
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
