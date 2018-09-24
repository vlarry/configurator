#include "configuratorwindow.h"
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
    if(data.isEmpty() || (data.count()/2 != m_variablelist->count()))
        return;

    union
    {
        quint16 buffer[2];
        float   value;
    } cell_val;

    for(int i = 0; i < m_variablelist->count(); i++)
    {
        QListWidgetItem* item = m_variablelist->item(i);

        if(item)
        {
            QCell* cell = qobject_cast<QCell*>(m_variablelist->itemWidget(item));

            if(cell)
            {
                int index = cell->property("INDEX").toInt()*2;

                cell_val.buffer[1] = data[index];
                cell_val.buffer[0] = data[index + 1];

                cell->setCellValue(cell_val.value);
            }
        }
    }
}
//------------------------------------------------------------------------
void CVariableWidget::setVariableNames(const calc_value_list_t& calc_list)
{
    if(calc_list.isEmpty())
        return;

    calc_value_list_t list = calc_list;
    std::sort(list.begin(), list.end(), [](const calc_value_t& val1, const calc_value_t& val2) { return val1.sort_id < val2.sort_id; });

    for(const calc_value_t& value: list)
    {
        QString cell_str = value.name;

        if(!value.description.isEmpty())
            cell_str += QString(" (%1)").arg(value.description);

        m_variables << cell_str;

        QCell* cell = new QCell;
        cell->setProperty("INDEX", value.id);

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
//------------------------------------
int CVariableWidget::cellCount() const
{
    if(m_variablelist)
        return m_variablelist->count();

    return -1;
}
