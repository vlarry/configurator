#include "ctablewidgetitem.h"
//----------------------------------------------------------------
CTableWidgetItem::CTableWidgetItem(const QString& text, int type):
    QTableWidgetItem(text, type)
{

}
//---------------------------------------------------------------------
bool CTableWidgetItem::operator < (const QTableWidgetItem& other) const
{
    switch(other.column())
    {
        case 0: // Сортировка по ID
        {
            return data(Qt::DisplayRole ).toInt() < other.data(Qt::DisplayRole ).toInt();
        }

        case 1: // Сортировка по дате
        {
            return QDate::fromString(data(Qt::DisplayRole ).toString(), "dd.MM.yyyy") <
                   QDate::fromString(other.data(Qt::DisplayRole ).toString(), "dd.MM.yyyy");
        }

        case 2: // Сортировка по времени
        {
            return QTime::fromString(data(Qt::DisplayRole ).toString(), "HH:mm:ss.zzz") <
                   QTime::fromString(other.data(Qt::DisplayRole ).toString(), "HH:mm:ss.zzz");
        }

        default:
        {
            return data(Qt::DisplayRole ).toString() < other.data(Qt::DisplayRole ).toString();
        }
    }
}
