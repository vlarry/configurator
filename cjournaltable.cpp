#include "cjournaltable.h"
//--------------------------------------------
CJournalTable::CJournalTable(QWidget* parent):
    QTableWidget(parent)
{

}
//-----------------------------------------------------
void CJournalTable::setRowData(int row, QVariant value)
{
    m_data_rows[row] = value;
}
