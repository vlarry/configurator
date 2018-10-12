#include "devicemenumodel.h"
//--------------------------------------------------------------
CDeviceMenuTableWidget::CDeviceMenuTableWidget(QWidget* parent):
    QTableWidget(parent)
{
    horizontalHeader()->setHighlightSections(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::NoSelection);
    verticalHeader()->hide();
    setEditTriggers(QTableWidget::NoEditTriggers);

    connect(this, &CDeviceMenuTableWidget::itemClicked, this, &CDeviceMenuTableWidget::rowClicked);
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

    setRowCount(cur_row + group.items.count() + rowCountSubgroup(group.subgroup) + 1);
    insertHeader(cur_row, group.name);

    m_group_rows[cur_row] = group.items.count() + rowCountSubgroup(group.subgroup) + 1;

    for(int i = 0; i < group.items.count(); i++)
    {
        item_t item = group.items[i];
        int    row  = cur_row + item.row + 1;

        if(item.type.toUpper() == "SUBGROUP")
        {
            insertHeader(row, item.name, SUBHEADER);

            int index = 0;
            if(item.key.contains('_')) // имя группы содержит знак подчеркивания - индекс переменной будет отличен от нуля (для связанных виджетов)
            {
                QStringList str_list = item.key.split('_');

                if(str_list.count() == 2)
                {
                    index = str_list.at(1).toInt();
                }
            }
            insertSubgroup(row, group.subgroup, index);

            m_group_rows[row] = rowCountSubgroup(group.subgroup) + 1;
        }
        else
        {
            insertItem(row, item);
        }
    }

    emit itemClicked(item(cur_row, 0));
}
//-------------------------------------------------------
void CDeviceMenuTableWidget::showEvent(QShowEvent* event)
{
    QTableWidget::showEvent(event);

    for(int i = 0; i < columnCount(); i++)
    {
        setColumnWidth(i, ((i == 2 || i == 3)?100:(i == 0)?400:300));
    }
}
//-----------------------------------------------------------------
void CDeviceMenuTableWidget::rowClicked(QTableWidgetItem* item_cur)
{
    RowType    rowType    = static_cast<RowType>(item_cur->data(Qt::UserRole + 100).toInt());
    GroupState groupState = static_cast<GroupState>(item_cur->data(Qt::UserRole + 101).toInt());

    if(rowType == HEADER || rowType == SUBHEADER)
    {
        int row_beg   = item_cur->row();
        int row_count = m_group_rows[item_cur->row()];

        for(int row = row_beg + 1; row < row_count + row_beg; row++)
        {
            if(groupState == CLOSE)
            {
                QTableWidgetItem* titem = item(row, 0);

                if(titem)
                {
                    if(static_cast<RowType>(titem->data(Qt::UserRole + 100).toInt()) == SUBHEADER)
                        titem->setData(Qt::UserRole + 101, OPEN);
                }

                setRowHidden(row, false);
                item_cur->setData(Qt::UserRole + 101, OPEN);
            }
            else
            {
                setRowHidden(row, true);
                item_cur->setData(Qt::UserRole + 101, CLOSE);
            }
        }
    }
}
//--------------------------------------------------------------------------------------------
int CDeviceMenuTableWidget::rowCountSubgroup(const CDeviceMenuTableWidget::group_list_t& list)
{
    int result = 0;

    for(const group_t& group: list)
    {
        result += group.items.count();

        if(!group.subgroup.isEmpty())
            result += rowCountSubgroup(group.subgroup);
    }

    return result;
}
//-----------------------------------------------------------------------------------
void CDeviceMenuTableWidget::insertHeader(int row, const QString& name, RowType type)
{
    QFont f = font();
    f.setBold(true);

    QLinearGradient gradient(0, 0, 0, rowHeight(row));

    if(type == HEADER)
    {
        gradient.setColorAt(0, QColor(190, 190, 190));
        gradient.setColorAt(0.5, Qt::gray);
        gradient.setColorAt(1, QColor(190, 190, 190));
    }
    else
    {
        gradient.setColorAt(0, QColor(230, 230, 230));
        gradient.setColorAt(0.5, Qt::lightGray);
        gradient.setColorAt(1, QColor(230, 230, 230));
    }

    QTableWidgetItem* item = new QTableWidgetItem;

    item->setText(name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFont(f);
    item->setData(Qt::UserRole + 100, type);
    item->setData(Qt::UserRole + 101, OPEN);
    item->setBackground(QBrush(gradient));

    setItem(row, 0, item);
    setSpan(row, 0, 1, columnCount());
}
//-----------------------------------------------------------------------------------------------------
void CDeviceMenuTableWidget::insertItem(int row, const CDeviceMenuTableWidget::item_t& item, int index)
{
    QWidget*     label_name  = new QWidget;
    QHBoxLayout* layout_name = new QHBoxLayout;
    QLabel*      unit        = new QLabel(item.name, label_name);
    QString      index_str   = ((index != 0)?QString("_%1").arg(index):"");

    layout_name->setObjectName(QString("layoutLabel%1%2").arg(item.key).arg(index_str));
    unit->setObjectName(QString("label%1%2").arg(item.key).arg(index_str));

    layout_name->addWidget(unit);
    layout_name->setAlignment(Qt::AlignCenter);
    layout_name->setContentsMargins(0, 0, 0, 0);
    layout_name->setSpacing(0);
    label_name->setLayout(layout_name);

    setCellWidget(row, 0, label_name);

    QWidget*     wgt        = new QWidget;
    QHBoxLayout* wgt_layout = new QHBoxLayout;
    QWidget*     widget     = nullptr;

    if(item.type.toUpper() == "LIST") // Тип равен СПИСОК, значит это комбобокс
    {
        QComboBox* cb = new QComboBox(wgt);

        cb->setObjectName(QString("comboBox%1%2").arg(item.key).arg(index_str));

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
        le->setObjectName(QString("lineEdit%1%2").arg(item.key).arg(index_str));
        le->setAlignment(Qt::AlignCenter);

        float min         = item.unit.min;
        float max         = item.unit.max;
        float val_default = item.unit.val_default;

        if(min == 0.0f && max == 0.0f)
        {
            min = 0.0f;
            max = 1000.0f;
        }

        if(item.type.toUpper() == "INT")
        {
            le->setValidator(new QIntValidator(static_cast<int>(min), static_cast<int>(max), le));
            le->setValidatorType(CLineEdit::INT);
            le->setText(QLocale::system().toString(static_cast<int>(val_default)));
        }
        else if(item.type.toUpper() == "FLOAT")
        {
            le->setValidator(new QDoubleValidator(static_cast<double>(min), static_cast<double>(max), 6, le));
            le->setValidatorType(CLineEdit::FLOAT);
            le->setText(QLocale::system().toString(val_default, 'f', 6));
        }

        widget = le;
    }

    if(widget)
    {
        widget->setMinimumWidth(200);
        widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        wgt_layout->setObjectName(QString("layoutWidget%1%2").arg(item.key).arg(index_str));
        wgt_layout->addWidget(widget);
        wgt_layout->setAlignment(Qt::AlignCenter);
        wgt_layout->setContentsMargins(50, 0, 50, 0);
        wgt_layout->setSpacing(0);
        wgt->setLayout(wgt_layout);

        setCellWidget(row, 1, wgt);
    }


    QWidget*     wgt_limit    = new QWidget;
    QHBoxLayout* layout_limit = new QHBoxLayout;
    QLabel*      label_limit  = new QLabel(QString("%1...%2").arg(static_cast<double>(item.unit.min)).
                                                               arg(static_cast<double>(item.unit.max)), wgt_limit);

    QWidget*     wgt_unit    = new QWidget;
    QHBoxLayout* layout_unit = new QHBoxLayout;
    QLabel*      label_unit  = new QLabel(QString("%1").arg(((item.unit.unit.isEmpty())?"":item.unit.unit)), wgt_limit);

    if(item.type.toUpper() == "INT" || item.type.toUpper() == "FLOAT")
    {
        label_limit->setObjectName(QString("label%1Limit%2").arg(item.key).arg(index_str));
        label_limit->setAlignment(Qt::AlignCenter);
        label_limit->setScaledContents(true);
        label_limit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        layout_limit->setObjectName(QString("layoutLabelLimit%1%2").arg(item.key).arg(index_str));
        layout_limit->addWidget(label_limit);
        layout_limit->setAlignment(Qt::AlignCenter);
        layout_limit->setContentsMargins(0, 0, 0, 0);
        layout_limit->setSpacing(0);
        wgt_limit->setLayout(layout_limit);

        label_unit->setObjectName(QString("label%1Unit%2").arg(item.key).arg(index_str));
        label_unit->setAlignment(Qt::AlignCenter);
        label_unit->setScaledContents(true);
        label_unit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        layout_unit->setObjectName(QString("layoutLabelUnit%1%2").arg(item.key).arg(index_str));
        layout_unit->addWidget(label_unit);
        layout_unit->setAlignment(Qt::AlignCenter);
        layout_unit->setContentsMargins(0, 0, 0, 0);
        layout_unit->setSpacing(0);
        wgt_unit->setLayout(layout_unit);

        setCellWidget(row, 2, wgt_limit);
        setCellWidget(row, 3, wgt_unit);
    }
}
//---------------------------------------------------------------------------------------------------------------
void CDeviceMenuTableWidget::insertSubgroup(int row, const CDeviceMenuTableWidget::group_list_t& list, int index)
{
    for(int i = 0; i < list.count(); i++)
    {
        group_t group = list[i];

        for(int j = 0; j < group.items.count(); j++)
        {
            item_t item      = group.items[j];
            int    row_index = row + item.row + i + 1;

            insertItem(row_index, item, index);
        }
    }
}
