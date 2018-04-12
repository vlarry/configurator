#include "cmatrix.h"
//-----------------
CMatrix::CMatrix():
    m_rows(row_t(0, CRowNew())),
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
void CMatrix::addRow(CRowNew& row)
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
//--------------------------------------
CRowNew& CMatrix::operator [](int index)
{
    return m_rows[index];
}
//--------------------------------------------------
const CRowNew& CMatrix::operator [](int index) const
{
    return m_rows[index];
}
//-----Класс колонка-----
//-----------------------
CColumnNew::CColumnNew():
    m_bit(-1),
    m_state(false),
    m_key(""),
    m_name(""),
    m_description("")
{

}
//---------------------------------------------------------------------------------------------------------------
CColumnNew::CColumnNew(int bit, bool state, const QString& key, const QString& name, const QString& description):
    m_bit(bit),
    m_state(state),
    m_key(key),
    m_name(name),
    m_description(description)
{

}
//-------------------------
int CColumnNew::bit() const
{
    return m_bit;
}
//-----------------------------
QString CColumnNew::key() const
{
    return m_key;
}
//------------------------------
QString CColumnNew::name() const
{
    return m_name;
}
//-------------------------------------
QString CColumnNew::description() const
{
    return m_description;
}
//----------------------------
bool CColumnNew::state() const
{
    return m_state;
}
//------------------------------
void CColumnNew::setBit(int bit)
{
    m_bit = bit;
}
//-------------------------------------------------------------------------------------------
void CColumnNew::setData(const QString& key, const QString& name, const QString& description)
{
    m_key         = key;
    m_name        = name;
    m_description = description;
}
//-----------------------------------------
void CColumnNew::setKey(const QString& key)
{
    m_key = key;
}
//-------------------------------------------
void CColumnNew::setName(const QString& name)
{
    m_name = name;
}
//---------------------------------------------------------
void CColumnNew::setDescription(const QString& description)
{
    m_description = description;
}
//-----------------------------------
void CColumnNew::setState(bool state)
{
    m_state = state;
}
//--Класс строка---
//-----------------
CRowNew::CRowNew():
    m_key(""),
    m_name(""),
    m_columns(column_t(0, CColumnNew()))
{

}
//------------------------------------------------------------------------------------
CRowNew::CRowNew(const QString& key, const QString& name, CRowNew::column_t& columns):
    m_key(key),
    m_name(name),
    m_columns(columns)
{

}
//-----------------------------------------
void CRowNew::addColumn(CColumnNew& column)
{
    m_columns.push_back(column);
}
//--------------------------
QString CRowNew::key() const
{
    return m_key;
}
//---------------------------
QString CRowNew::name() const
{
    return m_name;
}
//-----------------------------------
CRowNew::column_t& CRowNew::columns()
{
    return m_columns;
}
//--------------------------------------
void CRowNew::setKey(const QString &key)
{
    m_key = key;
}
//----------------------------------------
void CRowNew::setName(const QString& name)
{
    m_name = name;
}
//--------------------------------------------------
void CRowNew::setColumns(CRowNew::column_t& columns)
{
    m_columns = columns;
}
//-----------------------------------------
CColumnNew& CRowNew::operator [](int index)
{
    return m_columns[index];
}
//-----------------------------------------------------
const CColumnNew& CRowNew::operator [](int index) const
{
    return m_columns[index];
}
