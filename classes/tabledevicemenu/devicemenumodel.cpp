#include "devicemenumodel.h"
//--------------------------------------------------------------
CDeviceMenuTableWidget::CDeviceMenuTableWidget(QWidget* parent):
    QTableWidget(parent)
{
//    setShowGrid(false);
    horizontalHeader()->setHighlightSections(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection);
    verticalHeader()->hide();

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

    QTableWidgetItem* item = new QTableWidgetItem;

    item->setText(group.name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(f);
    item->setData(Qt::UserRole + 100, HEADER);
    insertRow(row);

    QLinearGradient gradient(0, 0, 0, rowHeight(row));
    gradient.setColorAt(0, QColor(230, 230, 230));
    gradient.setColorAt(0.5, Qt::lightGray);
    gradient.setColorAt(1, QColor(230, 230, 230));
    item->setBackground(QBrush(gradient));

    m_group_rows[row] = group.items.count(); // количество строк в группе

    for(int i = 0; i < columnCount(); i++)
        setItem(row, i, item);

    setSpan(row, 0, 1, columnCount());

    row++;

    for(int i = 0; i < group.items.count(); i++)
    {
        item_t item = group.items[i];
        int row_index = row + i;
        insertRow(row_index);

        QWidget*     label_name  = new QWidget;
        QHBoxLayout* layout_name = new QHBoxLayout(label_name);
        QLabel*      unit        = new QLabel(item.name, label_name);

        unit->setObjectName(QString("label%1").arg(item.key));

        layout_name->addWidget(unit);
        layout_name->setAlignment(Qt::AlignCenter);
        layout_name->setContentsMargins(0, 0, 0, 0);
        label_name->setLayout(layout_name);

        setCellWidget(row_index, 0, label_name);

        QWidget*     wgt        = new QWidget;
        QHBoxLayout* wgt_layout = new QHBoxLayout(label_name);
        QWidget*     widget     = nullptr;

        if(item.type.toUpper() == "NUM") // Тип равен ЧИСЛО, значит это комбобокс
        {
            widget = new QComboBox(wgt);
            widget->setObjectName(QString("comboBox%1").arg(item.key));
        }
        else // иначе поле ввода
        {
            widget = new CLineEdit(wgt);
            widget->setObjectName(QString("lineEdit%1").arg(item.key));
        }

        wgt_layout->addWidget(widget);
        wgt_layout->setAlignment(Qt::AlignCenter);
        wgt_layout->setContentsMargins(0, 0, 0, 0);
        wgt->setLayout(wgt_layout);

        setCellWidget(row_index, 1, wgt);

        if(item.unit.max != 0 && item.unit.min != 0)
        {
            QWidget*     lable_unit   = new QWidget;
            QHBoxLayout* layout_unit = new QHBoxLayout(lable_unit);
            QLabel*      unit        = new QLabel(QString("%1...%2%3").arg(item.unit.min).arg(item.unit.max).arg(item.unit.unit),
                                                  lable_unit);

            unit->setObjectName(QString("label%1Unit").arg(item.key));

            layout_unit->addWidget(unit);
            layout_unit->setAlignment(Qt::AlignCenter);
            layout_unit->setContentsMargins(0, 0, 0, 0);
            lable_unit->setLayout(layout_unit);

            setCellWidget(row_index, 2, lable_unit);
        }
    }

    emit rowClicked(item);
}
//-------------------------------------------------------
void CDeviceMenuTableWidget::showEvent(QShowEvent* event)
{
    QTableWidget::showEvent(event);

    for(int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, 250);
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
