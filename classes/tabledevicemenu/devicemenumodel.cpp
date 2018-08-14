#include "devicemenumodel.h"
//--------------------------------------------------------------
CDeviceMenuTableWidget::CDeviceMenuTableWidget(QWidget* parent):
    QTableWidget(parent)
{
    setShowGrid(false);
    horizontalHeader()->setHighlightSections(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection);
    verticalHeader()->hide();
    setColumnWidth(0, 200);
    setColumnWidth(1, 200);
    setColumnWidth(2, 200);

    connect(this, &CDeviceMenuTableWidget::itemClicked, this, &CDeviceMenuTableWidget::rowClicked);
}
//-----------------------------------------------------------------
void CDeviceMenuTableWidget::setColumns(const QStringList& columns)
{
    setColumnCount(columns.count());
    setHorizontalHeaderLabels(columns);
}
//---------------------------------------------------
void CDeviceMenuTableWidget::addGroup(group_t& group)
{
    int row = rowCount();
    QFont f = font();
    f.setBold(true);
    QLinearGradient gradient(0, 0, 0, 20);
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(0.5, Qt::gray);
    gradient.setColorAt(1, Qt::lightGray);
    QTableWidgetItem* item = new QTableWidgetItem;

    item->setText(group.name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setBackground(QBrush(gradient));
    item->setFont(f);
    item->setData(Qt::UserRole + 100, HEADER);
    insertRow(row);

    m_group_rows[row] = group.rows.count(); // количество строк в группе

    for(int i = 0; i < columnCount(); i++)
        setItem(row, i, item);

    setSpan(row, 0, 1, colorCount());

    row++;

    for(int i = 0; i < group.rows.count(); i++)
    {
        column_list_t columns = group.rows[i];

        if(columns.count() != columnCount())
            continue;

        int row_index = row + i;

        insertRow(row_index);

        for(int j = 0; j < columns.count(); j++)
        {
            item_t column = columns[j];

            if(column.item)
            {
                QWidget*     wgt = new QWidget;
                QHBoxLayout* l   = new QHBoxLayout(wgt);

                l->addWidget(column.item);
                l->setAlignment(Qt::AlignCenter);
                l->setContentsMargins(0, 0, 0, 0);

                setCellWidget(row_index, j, wgt);
            }
        }
    }
}
//-------------------------------------------------------------
void CDeviceMenuTableWidget::rowClicked(QTableWidgetItem* item)
{
    RowType type = static_cast<RowType>(item->data(Qt::UserRole + 100).toInt());

    if(type == HEADER)
    {
        if(m_group_rows.isEmpty())
            return;

        int  row_count = m_group_rows[item->row()];
        bool state     = true;

        if(isRowHidden(item->row() + 1))
            state = false;

        for(int i = item->row() + 1; i <= (item->row() + row_count); i++)
            setRowHidden(i, state);
    }
}
