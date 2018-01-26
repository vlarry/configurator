#include "ctablewidgetitem.h"
//----------------------------------------------------------------
CTableWidgetItem::CTableWidgetItem(const QString& text, int type):
    QTableWidgetItem(text, type)
{

}
//---------------------------------------------------------------------
bool CTableWidgetItem::operator < (const QTableWidgetItem& other) const
{
    return(QDate::fromString(other.data(Qt::DisplayRole ).toString(), "dd.MM.yy") >
           QDate::fromString(data(Qt::DisplayRole ).toString(), "dd.MM.yy"));
}
