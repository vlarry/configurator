#include "configuratorwindow.h"
#include "cvariablewidget.h"
//-----------------------------------------------------------------
QColor CVariableWidget::m_background_color = QColor(250, 250, 250);
//------------------------------------------------
CVariableWidget::CVariableWidget(QWidget* parent):
    QWidget(parent),
    m_variablelist(nullptr)
{
    m_variablelist = new CVaribaleList(this);

    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->setMargin(0);
    vlayout->setStretch(0, 0);
    vlayout->setSpacing(0);
    vlayout->addWidget(m_variablelist);

    setLayout(vlayout);

    m_variablelist->horizontalHeader()->setHighlightSections(false);
    m_variablelist->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_variablelist->setSelectionMode(QAbstractItemView::NoSelection);
    m_variablelist->setEditTriggers(QTableWidget::NoEditTriggers);
}
//---------------------------------------------------------
void CVariableWidget::setData(const QVector<quint16>& data)
{
    if(data.isEmpty() || (data.count()/2 != m_line_var.count()))
        return;

    union
    {
        quint16 buffer[2];
        float   value;
    } cell_val;

    for(int i = 0; i < m_line_var.count(); i++)
    {
        QLineEdit* lineEdit = m_line_var[i];
        int index = lineEdit->property("INDEX").toInt()*2;

        cell_val.buffer[1] = data[index];
        cell_val.buffer[0] = data[index + 1];

        lineEdit->setText(QLocale().system().toString(cell_val.value, 'g', 6));
    }
}
//------------------------------------------------
void CVariableWidget::init(QSqlDatabase& database)
{
    if(!loadGroups(database))
    {
        qWarning() << tr("Инициализация списка расчетных величин завершилась неудачно.");
        return;
    }

    int rowCount = 0;

    for(const var_group_t& group: m_groups)
    {
        rowCount += 2 + group.var_list.count();
    }

    m_variablelist->setRowCount(rowCount);

    int rows = 0;

    for(const var_group_t& group: m_groups)
    {
        rows = insertGroupHeader(group.name, rows);
        rows = insertColumnLabels(group.columns, rows);
        rows = insertGroupRows(group.var_list, rows);
    }

    m_variablelist->resizeColumnsToContents();
    m_variablelist->setShowGrid(false);
    m_variablelist->setColumnWidth(0, 75);
    m_variablelist->setColumnWidth(1, 100);
    m_variablelist->setColumnWidth(2, 100);
    m_variablelist->horizontalHeader()->hide();
    m_variablelist->verticalHeader()->hide();
}
//------------------------------------
int CVariableWidget::cellCount() const
{
    if(m_variablelist)
        return m_line_var.count();

    return -1;
}
//------------------------------------------------------
bool CVariableWidget::loadGroups(QSqlDatabase& database)
{
    if(!database.isOpen())
        return false;

    QSqlQuery query(database);

    if(query.exec(QString("SELECT * FROM calculate_group;")))
    {
        while(query.next())
        {
            int group_id = query.value("id").toInt();
            QString name = query.value("name").toString();
            QStringList header_list = loadColumns(database, group_id);
            var_list_t var_list = loadRows(database, group_id);

            m_groups[group_id] = var_group_t({ name, header_list, var_list });
        }
    }
    else
    {
        qWarning() << tr("Не удалось прочитать список групп -> %1.").arg(query.lastError().text());
        return false;
    }

    if(m_groups.isEmpty())
        return false;

    return true;
}
//----------------------------------------------------------------------------
QStringList CVariableWidget::loadColumns(QSqlDatabase& database, int group_id)
{
    QStringList list;

    if(!database.isOpen())
        return list;

    QSqlQuery query(database);
    if(query.exec(QString("SELECT * FROM calculate_column WHERE group_id=%1;").arg(group_id)))
    {
        while(query.next())
        {
            list << query.value("header").toString();
        }
    }
    else
        qWarning() << tr("Не удалось прочитать заголовки колонок (group_id: %1) -> %2.").arg(group_id).
                      arg(query.lastError().text());

    return list;
}
//-----------------------------------------------------------------------------------------
CVariableWidget::var_list_t CVariableWidget::loadRows(QSqlDatabase& database, int group_id)
{
    var_list_t list;

    if(!database.isOpen())
        return list;

    QSqlQuery query(database);

    if(query.exec(QString("SELECT * FROM calculate_row WHERE group_id=%1;").arg(group_id)))
    {
        while(query.next())
        {
            QString var_index1 = query.value("var_index1").toString();
            QString var_index2 = query.value("var_index2").toString();

            var_data_t var_data;

            if(var_index1.contains('=')) // индекс переменной содержит знак равенства - это выражение
            {
                QStringList expr = var_index1.split('=');

                if(expr.count() == 2)
                {
                    if(expr.at(0) == 's') // левая часть выражения равна 's' - это строка
                    {
                        var_data.var1.type = "STRING"; // тип строка
                        var_data.var1.name = expr.at(1);
                    }
                }
            }
            else if(!var_index1.isEmpty())
            {
                var_data.var1 = loadData(database, var_index1);
                var_data.var1.type = "VAR"; // тип переменная
            }
            else
            {
                var_data.var1.type = "EMPTY";
            }

            var_data.var2 = loadData(database, var_index2);
            var_data.var2.type = "VAR";

            list << var_data;
        }
    }
    else
        qWarning() << tr("Не удалось загрузить строки (group_id: %1) -> %2.").arg(group_id).
                      arg(query.lastError().text());

    return list;
}
//--------------------------------------------------------------------------------------------
CVariableWidget::var_t CVariableWidget::loadData(QSqlDatabase& database, const QString& index)
{
    var_t var;

    if(!database.isOpen())
        return var;

    QSqlQuery query(database);

    if(query.exec(QString("SELECT * FROM calculate_variable WHERE var_index=\'%1\';").arg(index)))
    {
        if(query.first())
        {
            var.id          = query.value("id").toInt();
            var.index       = index.toUpper();
            var.name        = query.value("name").toString();
            var.unit        = query.value("unit").toString();
            var.description = query.value("description").toString();
        }
    }
    else
        qWarning() << tr("Не удалось загрузить переменную с индексом \'%1\' -> %2").arg(index).
                      arg(query.lastError().text());

    return var;
}
//-------------------------------------------------------------------
int CVariableWidget::insertGroupHeader(const QString& text, int row)
{
    QTableWidgetItem* item = new QTableWidgetItem(text);

    if(item)
    {
        QLinearGradient gradient(0, 0, 0, m_variablelist->rowHeight(row));
        gradient.setColorAt(0, QColor(230, 230, 230));
        gradient.setColorAt(0.5, Qt::lightGray);
        gradient.setColorAt(1, QColor(230, 230, 230));

        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        item->setTextAlignment(Qt::AlignCenter);
        item->setBackground(QBrush(gradient));
        item->setData(Qt::UserRole + 100, "HEADER");

        m_variablelist->setItem(row, 0, item);
        m_variablelist->setSpan(row, 0, 1, m_variablelist->columnCount());
    }

    return row + 1;
}
//-----------------------------------------------------------------------
int CVariableWidget::insertColumnLabels(const QStringList& list, int row)
{
    for(int col = 0; col < m_variablelist->columnCount(); col++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(list.at(col));
        QFont font = item->font();
        font.setBold(true);
        item->setTextAlignment(Qt::AlignCenter);
        item->setBackground(QBrush(m_background_color));
        item->setFont(font);
        m_variablelist->setItem(row, col, item);
    }

    return row + 1;
}
//----------------------------------------------------------------------------------------
int CVariableWidget::insertGroupRows(const CVariableWidget::var_list_t& var_list, int row)
{
    for(int row_index = 0; row_index < var_list.count(); row_index++)
    {
        var_data_t data = var_list[row_index];
        QString separator_str = " ";

        if(data.var1.type == "VAR")
        {
            QLineEdit* lineEditVar1 = new QLineEdit("0.0000", m_variablelist);

            if(lineEditVar1)
            {
                QWidget* widgetVar1 = new QWidget;
                QHBoxLayout* layoutVar1 = new QHBoxLayout;
                int height = static_cast<int>(lineEditVar1->fontMetrics().height()*1.2f);
                int width = static_cast<int>(lineEditVar1->fontMetrics().width("0.000000")*1.5f);

                lineEditVar1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                lineEditVar1->setObjectName(QString("lineEdit%1").arg(data.var1.index));
                lineEditVar1->setAlignment(Qt::AlignRight);
                lineEditVar1->setReadOnly(true);
                lineEditVar1->setMaximumHeight(height);
                lineEditVar1->setToolTip(data.var1.description);
                lineEditVar1->setMaximumWidth(width);
                lineEditVar1->setMaxLength(8);
                lineEditVar1->setProperty("INDEX", data.var1.id);
                layoutVar1->setContentsMargins(0, 0, 0, 0);
                layoutVar1->addWidget(lineEditVar1);
                widgetVar1->setLayout(layoutVar1);
                widgetVar1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                QTableWidgetItem* itemWidgetVar1 = new QTableWidgetItem;
                itemWidgetVar1->setBackground(QBrush(m_background_color));
                m_variablelist->setItem(row + row_index, 2, itemWidgetVar1);
                m_variablelist->setCellWidget(row + row_index, 2, widgetVar1);

                m_line_var << lineEditVar1;
                separator_str = "/";
            }
        }
        else if(data.var1.type == "STRING")
        {
            QTableWidgetItem* item = new QTableWidgetItem(data.var1.name);
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(QBrush(m_background_color));
            m_variablelist->setItem(row + row_index, 2, item);
        }
        else
        {
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(QBrush(m_background_color));
            m_variablelist->setItem(row + row_index, 2, item);
        }

        QLineEdit* lineEditVar2 = new QLineEdit("0.0000", m_variablelist);

        if(lineEditVar2)
        {
            QWidget* widgetVar2 = new QWidget;
            QHBoxLayout* layoutVar2 = new QHBoxLayout;
            int height = static_cast<int>(lineEditVar2->fontMetrics().height()*1.2f);
            int width = static_cast<int>(lineEditVar2->fontMetrics().width("0.000000")*1.5f);
            lineEditVar2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lineEditVar2->setObjectName(QString("lineEdit%1").arg(data.var1.index));
            lineEditVar2->setAlignment(Qt::AlignRight);
            lineEditVar2->setReadOnly(true);
            lineEditVar2->setMaximumHeight(height);
            lineEditVar2->setToolTip(data.var2.description);
            lineEditVar2->setMaximumWidth(width);
            lineEditVar2->setMaxLength(8);
            lineEditVar2->setProperty("INDEX", data.var2.id);
            layoutVar2->setContentsMargins(0, 0, 0, 0);
            layoutVar2->addWidget(lineEditVar2);
            widgetVar2->setLayout(layoutVar2);
            widgetVar2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            QTableWidgetItem* itemWidgetVar2 = new QTableWidgetItem;
            itemWidgetVar2->setBackground(QBrush(m_background_color));
            m_variablelist->setItem(row + row_index, 4, itemWidgetVar2);
            m_variablelist->setCellWidget(row + row_index, 4, widgetVar2);

            m_line_var << lineEditVar2;
        }

        QString index_str = data.var2.index;
        QString name_str  = data.var2.name;
        QString unit_str  = data.var2.unit;

        if(!data.var1.index.isEmpty())
        {
            index_str = QString("%1/%2").arg(data.var1.index).arg(data.var2.index);
        }

        if(data.var2.index.contains("Y0")) // добавление греческих букв дельта-фи перед названием по их кодам
        {
            name_str = QString("%1%2(%3)").arg(QChar(0x394)).arg(QChar(0x3c6)).arg(data.var2.name);
        }

        if(!data.var1.unit.isEmpty()) // объединение названий переменных и единиц измерения, если единицы измерения не равны
        {
            if(data.var1.unit.toUpper() != data.var2.unit.toUpper())
            {
                unit_str = QString("%1/%2").arg(data.var1.unit).arg(data.var2.unit);
                name_str = QString("%1/%2").arg(data.var1.name).arg(data.var2.name);
            }
        }

        QTableWidgetItem* itemIndex = new QTableWidgetItem(index_str);
        QTableWidgetItem* itemName  = new QTableWidgetItem(name_str);
        QTableWidgetItem* itemUnit  = new QTableWidgetItem(unit_str);

        itemIndex->setTextAlignment(Qt::AlignCenter);
        itemName->setTextAlignment(Qt::AlignCenter);
        itemUnit->setTextAlignment(Qt::AlignCenter);
        itemIndex->setBackground(QBrush(m_background_color));
        itemName->setBackground(QBrush(m_background_color));
        itemUnit->setBackground(QBrush(m_background_color));

        m_variablelist->setItem(row + row_index, 0, itemIndex);
        m_variablelist->setItem(row + row_index, 1, itemName);
        m_variablelist->setItem(row + row_index, 5, itemUnit);

        QTableWidgetItem* itemSeparator = new QTableWidgetItem;
        itemSeparator->setBackground(QBrush(m_background_color));

        QWidget* widgetSeparator = new QWidget;
        QLabel* labelSeparator = new QLabel(separator_str, m_variablelist);
        QHBoxLayout* layoutSeparator = new QHBoxLayout;

        labelSeparator->setAlignment(Qt::AlignCenter);
        labelSeparator->setMargin(0);
        labelSeparator->setScaledContents(true);
        labelSeparator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        labelSeparator->setMaximumWidth(10);
        layoutSeparator->setContentsMargins(0, 0, 0, 0);
        layoutSeparator->setMargin(0);
        layoutSeparator->setSpacing(0);
        layoutSeparator->addWidget(labelSeparator);
        widgetSeparator->setLayout(layoutSeparator);
        widgetSeparator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        m_variablelist->setItem(row + row_index, 3, itemSeparator);
        m_variablelist->setCellWidget(row + row_index, 3, widgetSeparator);
    }

    return var_list.count() + row;
}
//------------------------------------------
int CVariableWidget::columnMaxWidth(int col)
{
    int result = 0;

    for(int row = 0; row < m_variablelist->rowCount(); row++)
    {
        QTableWidgetItem* item = m_variablelist->item(row, col);
        int w = 0;

        if(item)
        {
            if(item->data(Qt::UserRole + 100).toString().toUpper() != "HEADER")
            {
                QFontMetrics fm(item->font());
                w = fm.width(item->text());
            }
        }
        else
        {
            QWidget* widget = m_variablelist->cellWidget(row, col);

            if(widget)
                w = widget->width();
        }

        if(w > result)
            result = w;
    }

    return result;
}
