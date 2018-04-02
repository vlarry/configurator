#include "qpanel.h"
//------------------------------
QPanel::QPanel(QWidget* parent):
    QDockWidget(parent),
    m_central_wgt(Q_NULLPTR)
{
    m_central_wgt = new QListWidget(this);
    setWidget(m_central_wgt);
}
//------------------------------------------------
void QPanel::setData(const QVector<quint16>& data)
{
    if(data.isEmpty() || data.count() != 74)
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
//---------------------------------------------------------------
void QPanel::setVariableNames(const calc_value_list_t& calc_list)
{
    if(calc_list.isEmpty())
        return;

    for(const calc_value_t& value: calc_list)
    {
        QString cell_str = value.name;

        if(!value.description.isEmpty())
            cell_str += QString(" (%1)").arg(value.description);

        QCell* cell = new QCell(cell_str);

        cell->setToolTip(value.description);

        QListWidgetItem* item = new QListWidgetItem(m_central_wgt);
        item->setSizeHint(cell->sizeHint());
        m_central_wgt->setItemWidget(item, cell);

//        m_cell_list.append(cell);
    }
}
