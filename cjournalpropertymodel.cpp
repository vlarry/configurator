#include "cjournalpropertymodel.h"
//------------------------------------------------------------
CJournalPropertyModel::CJournalPropertyModel(QObject* parent):
    QAbstractListModel(parent)
{

}
//------------------------------------------------------------------
int CJournalPropertyModel::rowCount(const QModelIndex& parent) const
{
    return m_data.count();
}
//----------------------------------------------------------------------------
QVariant CJournalPropertyModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid() && index.row() >= 0 && index.row() < m_data.count())
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant::fromValue(m_data[index.row()]);
        }
    }

    return QVariant();
}
//-------------------------------------------------------------------------------
void CJournalPropertyModel::setDataModel(QVector<QPair<QString, QString> >& data)
{
    m_data = data;

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(data.count(), 0);

    emit dataChanged(topLeft, bottomRight);
}
