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
    if(m_data_rows.find(row) != m_data_rows.end())
        return m_data_rows[row];

    return QVariant();
}
/*!
 * \brief CJournalTable::setRowData
 * \param row - номер строки, которая является ключом
 * \param value - данные журнала (структура данных зависит от журнала)
 */
void CJournalTable::setRowData(int row, QVariant value)
{
    m_data_rows[row] = value;
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
