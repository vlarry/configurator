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
    m_group_rows[row] = group.items.count(); // количество строк в группе

    if(!group.subgroup.isEmpty())
    {
        for(CDeviceMenuTableWidget::group_t subgroup: group.subgroup)
            m_group_rows[row] += subgroup.items.count();
    }

    QFont f = font();
    setRowCount(row + group.items.count() + 1);
    f.setBold(true);

    QTableWidgetItem* item = new QTableWidgetItem;

    item->setText(group.name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(f);
    item->setData(Qt::UserRole + 100, HEADER);

    QLinearGradient gradient(0, 0, 0, rowHeight(row));
    gradient.setColorAt(0, QColor(230, 230, 230));
    gradient.setColorAt(0.5, Qt::lightGray);
    gradient.setColorAt(1, QColor(230, 230, 230));
    item->setBackground(QBrush(gradient));

    for(int i = 0; i < columnCount(); i++)
        setItem(row, i, item);

    setSpan(row, 0, 1, columnCount());

    addItems(group, row);

    emit rowClicked(item);
}
//-------------------------------------------------------
void CDeviceMenuTableWidget::showEvent(QShowEvent* event)
{
    QTableWidget::showEvent(event);

    for(int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, 300);
    }
}
//------------------------------------------------------------------------------------------------
void CDeviceMenuTableWidget::addItems(const CDeviceMenuTableWidget::group_t& group, int group_row)
{
    for(int i = 0; i < group.items.count(); i++)
    {
        item_t item = group.items[i];
        int row_index = group_row + 1 + item.row;

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

        if(item.type.toUpper() == "LIST") // Тип равен СПИСОК, значит это комбобокс
        {
            QComboBox* cb = new QComboBox(wgt);

            cb->setObjectName(QString("comboBox%1").arg(item.key));

            QStringList subitemlist;

            for(const CDeviceMenuTableWidget::item_t& subitem: item.subitems)
                subitemlist << subitem.name;

            if(!subitemlist.isEmpty())
                cb->addItems(subitemlist);

            widget = cb;
        }
        else if(item.type.toUpper() == "FLOAT" || item.type.toUpper() == "INT") // иначе поле ввода
        {
            CLineEdit* le = new CLineEdit(wgt);
            le->setObjectName(QString("lineEdit%1").arg(item.key));
            le->setAlignment(Qt::AlignCenter);

            float min = item.unit.min;
            float max = item.unit.max;

            if(min == 0 && max == 0)
            {
                min = 0.0f;
                max = 1000.0f;
            }

            if(item.type.toUpper() == "INT")
            {
                le->setValidator(new QIntValidator(min, max, le));
                le->setText(QLocale::system().toString(((min < 0.0f)?0:int(min))));
                le->setValidatorType(CLineEdit::INT);
            }
            else if(item.type.toUpper() == "FLOAT")
            {
                le->setValidator(new QDoubleValidator(min, max, 6, le));
                le->setText(QLocale::system().toString(((min < 0.0f)?0:min), 'f', 6));
                le->setValidatorType(CLineEdit::FLOAT);
            }

            widget = le;
        }
        else if(item.type.toUpper() == "SUBGROUP") // если подгруппа
        {
            QPushButton* button = new QPushButton(wgt);
            button->setText(item.name);
            button->setIcon(QIcon(":/images/resource/images/branch_close.png"));
            button->setProperty("STATE", false);

            connect(button, &QPushButton::clicked, this, &CDeviceMenuTableWidget::subgroupClicked);

            widget = button;

            if(!group.subgroup.isEmpty())
                addItems(group.subgroup[0], row_index);
        }

        widget->setMinimumWidth(100);
        widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        wgt_layout->addWidget(widget);
        wgt_layout->setAlignment(Qt::AlignCenter);
        wgt_layout->setContentsMargins(75, 0, 75, 0);
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
//--------------------------------------------
void CDeviceMenuTableWidget::subgroupClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    if(button)
    {
        bool state = button->property("STATE").toBool();

        if(state)
        {
            button->setIcon(QIcon(":/images/resource/images/branch_close.png"));
        }
        else
        {
            button->setIcon(QIcon(":/images/resource/images/branch_open.png"));
        }

        button->setProperty("STATE", !state);
    }
}
