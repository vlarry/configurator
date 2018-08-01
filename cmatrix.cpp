#include "cmatrix.h"
//-----------------
CColumn::CColumn():
    m_data({ "", "", "", -1, UNCHECKED })
{

}
//-----------------------------------
CColumn::CColumn(const unit_t& data):
    m_data(data)
{

}
//---------------------
unit_t& CColumn::data()
{
    return m_data;
}
//---------------------------------
const unit_t& CColumn::data() const
{
    return m_data;
}
//-----------
CRow::CRow():
    m_data({ "", "", "", -1, UNCHECKED }),
    m_column_array(0)
{

}
//---------------------------------------------------------------
CRow::CRow(const unit_t& data, const CRow::ColumnArray& columns):
    m_data(data),
    m_column_array(columns)
{

}
//------------------
unit_t& CRow::data()
{
    return m_data;
}
//------------------------------
const unit_t& CRow::data() const
{
    return m_data;
}
//---------------------
int CRow::count() const
{
    return m_column_array.count();
}
//-----------------------------------
CColumn& CRow::operator [](int index)
{
    Q_ASSERT(index >= 0 && index <= m_column_array.count());
    return m_column_array[index];
}
//-----------------------------------------------
const CColumn& CRow::operator [](int index) const
{
    Q_ASSERT(index >= 0 && index <= m_column_array.count());
    return m_column_array[index];
}
//-----------------
CMatrix::CMatrix():
    m_matrix(0)
{

}
//-----------------------------------
void CMatrix::addRow(const CRow& row)
{
    m_matrix.push_back(row);
}
//------------------------------
int CMatrix::columnCount() const
{
    if(m_matrix.isEmpty())
        return 0;

    return m_matrix[0].count();
}
//---------------------------
int CMatrix::rowCount() const
{
    return m_matrix.count();
}
//-----------------------------------
CRow& CMatrix::operator [](int index)
{
    Q_ASSERT(index >= 0 && index <= m_matrix.count());
    return m_matrix[index];
}
//-----------------------------------------------
const CRow& CMatrix::operator [](int index) const
{
    Q_ASSERT(index >= 0 && index <= m_matrix.count());
    return m_matrix[index];
}
