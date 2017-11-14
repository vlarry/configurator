#include "qpanel.h"
//------------------------------
QPanel::QPanel(QWidget* parent):
    QDockWidget(parent),
    m_central_wgt(Q_NULLPTR)
{
    m_cell_list.append(new QCell(tr("Ia")));
    m_cell_list.append(new QCell(tr("Ib")));
    m_cell_list.append(new QCell(tr("Ic")));
    m_cell_list.append(new QCell(tr("3I0")));
    m_cell_list.append(new QCell(tr("Uab")));
    m_cell_list.append(new QCell(tr("Ubc")));
    m_cell_list.append(new QCell(tr("Uca")));
    m_cell_list.append(new QCell(tr("3U0")));
    m_cell_list.append(new QCell(tr("Ua")));
    m_cell_list.append(new QCell(tr("Ub")));
    m_cell_list.append(new QCell(tr("Uc")));
    m_cell_list.append(new QCell(tr("F")));
    m_cell_list.append(new QCell(tr("Uлин")));
    m_cell_list.append(new QCell(tr("T1")));
    m_cell_list.append(new QCell(tr("T2")));
    m_cell_list.append(new QCell(tr("Ia")));
    m_cell_list.append(new QCell(tr("Ib")));
    m_cell_list.append(new QCell(tr("Ic")));
    m_cell_list.append(new QCell(tr("3I0")));
    m_cell_list.append(new QCell(tr("Uab")));
    m_cell_list.append(new QCell(tr("Ubc")));
    m_cell_list.append(new QCell(tr("Uca")));
    m_cell_list.append(new QCell(tr("3U0")));
    m_cell_list.append(new QCell(tr("Ua")));
    m_cell_list.append(new QCell(tr("Ub")));
    m_cell_list.append(new QCell(tr("Uc")));
    m_cell_list.append(new QCell(tr("F")));
    m_cell_list.append(new QCell(tr("Uлин")));
    m_cell_list.append(new QCell(tr("Uax")));
    m_cell_list.append(new QCell(tr("Ubx")));
    m_cell_list.append(new QCell(tr("Ucx")));
    m_cell_list.append(new QCell(tr("Ra")));
    m_cell_list.append(new QCell(tr("Rb")));
    m_cell_list.append(new QCell(tr("Rc")));
    m_cell_list.append(new QCell(tr("Ia")));
    m_cell_list.append(new QCell(tr("Ib")));
    m_cell_list.append(new QCell(tr("Ic")));
    m_cell_list.append(new QCell(tr("3I0")));
    m_cell_list.append(new QCell(tr("Uan(Uab)")));
    m_cell_list.append(new QCell(tr("Ubn(Ubc)")));
    m_cell_list.append(new QCell(tr("Ucn(Uca)")));
    m_cell_list.append(new QCell(tr("3U0")));
    m_cell_list.append(new QCell(tr("Ua")));
    m_cell_list.append(new QCell(tr("Ub")));
    m_cell_list.append(new QCell(tr("Uc")));
    m_cell_list.append(new QCell(tr("Uумн")));
    m_cell_list.append(new QCell(tr("Uab")));
    m_cell_list.append(new QCell(tr("Ubc")));
    m_cell_list.append(new QCell(tr("Uca")));
    m_cell_list.append(new QCell(tr("T1")));
    m_cell_list.append(new QCell(tr("T2")));
    m_cell_list.append(new QCell(tr("Ia^Ubc")));
    m_cell_list.append(new QCell(tr("Ib^Uca")));
    m_cell_list.append(new QCell(tr("Ic^Uab")));
    m_cell_list.append(new QCell(tr("3I0^3U0")));
    
    m_central_wgt = new QListWidget(this);
    setWidget(m_central_wgt);
    
    for(QCell* cell: m_cell_list)
    {
        QListWidgetItem* item = new QListWidgetItem(m_central_wgt);
        item->setSizeHint(cell->sizeHint());
        m_central_wgt->setItemWidget(item, cell);
    }
}
//------------------------------------------------
void QPanel::setData(const QVector<quint16>& data)
{
    if(data.isEmpty() || data.count() != 0x6E)
        return;
    
    union
    {
        quint16 buffer[2];
        float   value;
    } cell_t;
     
    for(quint8 i = 0, j = 0; i < data.count() - 1; i += 2, j++)
    {
        cell_t.buffer[1] = data.at(i);
        cell_t.buffer[0] = data.at(i + 1);
        
        QListWidgetItem* item = m_central_wgt->item(j);
        QCell*           cell = static_cast<QCell*>(m_central_wgt->itemWidget(item));
        
        cell->setCellValue(cell_t.value);
    }
}
