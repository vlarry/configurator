#include "cmatrixpurposemodel.h"
//--------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(QAbstractTableModel *parent):
    QAbstractTableModel(parent)
{

}
//--------------------------------------------------------------------------------------------
CMatrixPurposeModel::CMatrixPurposeModel(QVector<RowType>& data, QAbstractTableModel* parent):
    QAbstractTableModel(parent)
{
    m_data = data;
}
//----------------------------------------------------------------
int CMatrixPurposeModel::rowCount(const QModelIndex& parent) const
{
    return /*m_data.count()*/5;
}
//-------------------------------------------------------------------
int CMatrixPurposeModel::columnCount(const QModelIndex& parent) const
{
//    if(m_data.isEmpty())
//        return 0;

    return /*m_data[0].column.count()*/3;
}
//--------------------------------------------------------------------------
QVariant CMatrixPurposeModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::DisplayRole)
        return tr("HELLO");

    return QVariant();
}
