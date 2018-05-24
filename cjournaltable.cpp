#include "cjournaltable.h"
//--------------------------------------------
CJournalTable::CJournalTable(QWidget* parent):
    QTableWidget(parent)
{

}
/*!
 * \brief CJournalTable::rowData
 * \param row - номер строки
 * \return Возвращает значение QVariant (хранится структура описывающая определенный журнал)
 */
QVariant CJournalTable::rowData(int row) const
{
    QTableWidgetItem* i = item(row, 0);

    if(i)
        return i->data(Qt::UserRole + 100);

    return QVariant();
}
/*!
 * \brief CJournalTable::setRowData
 * \param row - номер строки, которая является ключом
 * \param value - данные журнала (структура данных зависит от журнала)
 */
void CJournalTable::setRowData(int row, QVariant value)
{
    QTableWidgetItem* i = item(row, 0);

    if(i)
        i->setData(Qt::UserRole + 100, value);
}
//-------------------------------------------------
void CJournalTable::keyPressEvent(QKeyEvent* event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) // копирование данных из журнала
    {
        if(rowCount() != 0)
        {
            QList<QTableWidgetSelectionRange> selected = selectedRanges();

            if(!selected.isEmpty())
            {
                QString clipboard_str = "";

                for(const QTableWidgetSelectionRange range: selected)
                {
                    for(int i = range.topRow(); i <= range.bottomRow(); i++)
                    {
                        for(int j = 0; j < columnCount(); j++)
                        {
                            clipboard_str += item(i, j)->text() + "\t";
                        }

                        clipboard_str += "\n";
                    }
                }

                QApplication::clipboard()->setText(clipboard_str);
            }
            else
            {
                QMessageBox::warning(this, tr("Копирование данных из таблицы"), tr("В таблице нет выделенных строк."));
            }
        }
        else
        {
            QMessageBox::warning(this, tr("Копирование данных из таблицы"), tr("Таблица пуста."));
        }
    }
}
