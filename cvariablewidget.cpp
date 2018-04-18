#include "cvariablewidget.h"
//------------------------------------------------
CVariableWidget::CVariableWidget(QWidget* parent):
    QWidget(parent),
    m_variablelist(nullptr)
{
    m_variablelist = new CVaribaleList;

    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->setMargin(0);
    vlayout->addWidget(m_variablelist);

    setLayout(vlayout);

    connect(m_variablelist, &CVaribaleList::resizeSize, this, &CVariableWidget::resizeSize);
}
//---------------------------------------------------------
void CVariableWidget::setData(const QVector<quint16>& data)
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

        QListWidgetItem* item = m_variablelist->item(j);
        QCell*           cell = static_cast<QCell*>(m_variablelist->itemWidget(item));

        cell->setCellValue(cell_t.value);
    }
}
//------------------------------------------------------------------------
void CVariableWidget::setVariableNames(const calc_value_list_t& calc_list)
{
    if(calc_list.isEmpty())
        return;

    for(const calc_value_t& value: calc_list)
    {
        QString cell_str = value.name;

        if(!value.description.isEmpty())
            cell_str += QString(" (%1)").arg(value.description);

        m_variables << cell_str;

        QCell* cell = new QCell;

        cell->setCellName(cell_str);

        cell->setToolTip(value.description);

        QListWidgetItem* item = new QListWidgetItem(m_variablelist);
        item->setSizeHint(cell->sizeHint());
        m_variablelist->setItemWidget(item, cell);
    }
}
//--------------------------------
void CVariableWidget::resizeSize()
{
    for(int i = 0; i < m_variablelist->count(); i++)
    {
        QListWidgetItem* item = m_variablelist->item(i);
        QCell*           cell = static_cast<QCell*>(m_variablelist->itemWidget(item));

        cell->setCellName(m_variables.at(i));
    }
}
