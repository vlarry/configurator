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
    setEditTriggers(QTableWidget::NoEditTriggers);

//    connect(this, &CDeviceMenuTableWidget::itemClicked, this, &CDeviceMenuTableWidget::rowClicked);
}
//-----------------------------------------------------------------
void CDeviceMenuTableWidget::setColumns(const QStringList& columns)
{
    setColumnCount(columns.count());
    setHorizontalHeaderLabels(columns);
}
//---------------------------------------------------------------------------------
void CDeviceMenuTableWidget::addGroup(const CDeviceMenuTableWidget::group_t& group)
{
    int cur_row = rowCount();

    setRowCount(cur_row + group.items.count() + 1);
    insertHeader(cur_row, group.name);

    for(int i = 0; i < group.items.count(); i++)
    {
        item_t item = group.items[i];
        int    row  = cur_row + item.row + 1;

        if(item.type.toUpper() == "SUBGROUP")
        {
            insertHeader(row, item.name);
            insertSubgroup(row, group.subgroup);
        }
        else
        {
            insertItem(row, item);
        }
    }
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
//---------------------------------------------------------------------
void CDeviceMenuTableWidget::insertHeader(int row, const QString& name)
{
    QFont f = font();
    f.setBold(true);

    QLinearGradient gradient(0, 0, 0, rowHeight(row));
    gradient.setColorAt(0, QColor(230, 230, 230));
    gradient.setColorAt(0.5, Qt::lightGray);
    gradient.setColorAt(1, QColor(230, 230, 230));

    QTableWidgetItem* item = new QTableWidgetItem;

    item->setText(name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(f);
    item->setData(Qt::UserRole + 100, HEADER);
    item->setBackground(QBrush(gradient));

    setItem(row, 0, item);
    setSpan(row, 0, 1, 3);
}
//------------------------------------------------------------------------------------------
void CDeviceMenuTableWidget::insertItem(int row, const CDeviceMenuTableWidget::item_t& item)
{
    QWidget*     label_name  = new QWidget;
    QHBoxLayout* layout_name = new QHBoxLayout;
    QLabel*      unit        = new QLabel(item.name, label_name);

    layout_name->setObjectName(QString("layoutLabel%1").arg(item.key));
    unit->setObjectName(QString("label%1").arg(item.key));

    layout_name->addWidget(unit);
    layout_name->setAlignment(Qt::AlignCenter);
    layout_name->setContentsMargins(0, 0, 0, 0);
    label_name->setLayout(layout_name);

    setCellWidget(row, 0, label_name);

    QWidget*     wgt        = new QWidget;
    QHBoxLayout* wgt_layout = new QHBoxLayout;
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

    if(widget)
    {
        widget->setMinimumWidth(100);
        widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        wgt_layout->setObjectName(QString("layoutWidget%1").arg(item.key));
        wgt_layout->addWidget(widget);
        wgt_layout->setAlignment(Qt::AlignCenter);
        wgt_layout->setContentsMargins(75, 0, 75, 0);
        wgt->setLayout(wgt_layout);

        setCellWidget(row, 1, wgt);
    }

    if(item.unit.max != 0 && item.unit.min != 0)
    {
        QWidget*     lable_unit   = new QWidget;
        QHBoxLayout* layout_unit = new QHBoxLayout;
        QLabel*      unit        = new QLabel(QString("%1...%2%3").arg(item.unit.min).arg(item.unit.max).arg(item.unit.unit),
                                              lable_unit);

        unit->setObjectName(QString("label%1Unit").arg(item.key));

        layout_unit->setObjectName(QString("layoutLabelUnit%1").arg(item.key));
        layout_unit->addWidget(unit);
        layout_unit->setAlignment(Qt::AlignCenter);
        layout_unit->setContentsMargins(0, 0, 0, 0);
        lable_unit->setLayout(layout_unit);

        setCellWidget(row, 2, lable_unit);
    }
}
//----------------------------------------------------------------------------------------------------
void CDeviceMenuTableWidget::insertSubgroup(int row, const CDeviceMenuTableWidget::group_list_t& list)
{
    for(int i = 0; i < list.count(); i++)
    {
        group_t group = list[i];

        for(int j = 0; j < group.items.count(); j++)
        {
            item_t item      = group.items[j];
            int    row_index = row + item.row + i + 1;

            insertItem(row_index, item);
        }
    }
}
