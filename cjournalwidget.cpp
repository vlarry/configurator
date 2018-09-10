#include "cjournalwidget.h"
#include "ui_journalwidget.h"
//----------------------------------------------
CJournalWidget::CJournalWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CJournalWidget)
{
    ui->setupUi(this);

    ui->tableWidgetJournal->setShowGrid(true);
    ui->tableWidgetJournal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetJournal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetJournal->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    ui->listViewPropertyCrashJournal->hide(); // по умолчанию окно свойств журнала аварий скрыто
    ui->tableWidgetPropertyHalfhourJournal->hide(); // по умолчанию окно свойств журнала получасовок скрыто

    connect(ui->widgetJournalHeader, &CHeaderJournal::clickedButtonRead, this, &CJournalWidget::clickedButtonRead);
    connect(ui->tableWidgetJournal, &CJournalTable::clicked, this, &CJournalWidget::clickedItemTable);

    setAutoFillBackground(true);

    CJournalPropertyModel* model = new CJournalPropertyModel(ui->listViewPropertyCrashJournal);

    ui->listViewPropertyCrashJournal->setModel(model);
    ui->listViewPropertyCrashJournal->setItemDelegate(new CJournalPropertyDelegate(ui->listViewPropertyCrashJournal));
}
//-------------------------------
CJournalWidget::~CJournalWidget()
{
    delete ui;
}
//--------------------------------------------
CHeaderJournal* CJournalWidget::header() const
{
    return ui->widgetJournalHeader;
}
//--------------------------------------
void CJournalWidget::headerClear() const
{
    ui->widgetJournalHeader->clear();
}
//-----------------------------------------
CJournalTable* CJournalWidget::table() const
{
    return ui->tableWidgetJournal;
}
//------------------------------------------------
QListWidget* CJournalWidget::propertyJournal() const
{
    return /*ui->listWidgetPropertyJournal*/nullptr;
}
/*!
 * \brief CJournalWidget::print
 * \param data Данные для вывода на монитор
 *
 * Предназначен для выбора и запуска вывода данных на экран в зависимости от журнала
 */
void CJournalWidget::print(const QVector<quint16>& data) const
{
    QString journal_type = property("TYPE").toString();

    if(journal_type.isEmpty())
        return;

    QVector<quint8> bytes;

    convertHalfwordToBytes(data, bytes);

    if(journal_type == "CRASH")
    {
        printCrash(bytes);
    }
    else if(journal_type == "EVENT")
    {
        printEvent(bytes);
    }
    else if(journal_type == "HALFHOUR")
    {
        printHalfHour(bytes);
    }

    QTableWidgetItem* itemBeg = ui->tableWidgetJournal->item(0, 1);
    QTableWidgetItem* itemEnd = ui->tableWidgetJournal->item(ui->tableWidgetJournal->rowCount(), 1);

    if(itemBeg && itemEnd)
    {
        ui->widgetJournalHeader->setTextTableCountMessages(QString("%1 - %2/%3").arg(itemBeg->text()).
                                                           arg(itemEnd->text()).
                                                           arg(ui->tableWidgetJournal->rowCount()));
    }

    ui->tableWidgetJournal->resizeColumnsToContents();
    ui->tableWidgetJournal->horizontalHeader()->setStretchLastSection(true);
}
//----------------------------------------------------------------------------------------------------------
void CJournalWidget::setTableHeaders(CJournalWidget::PropertyType property_type, const QStringList& headers)
{
    m_property_type = property_type;

    ui->tableWidgetJournal->setColumnCount(headers.count());
    ui->tableWidgetJournal->setHorizontalHeaderLabels(headers);
}
//----------------------------------------------------------------
void CJournalWidget::setTableColumnWidth(const QVector<int>& list)
{
    if(list.isEmpty() || list.count() != ui->tableWidgetJournal->columnCount())
        return;

    for(int i = 0; i < list.count(); i++)
    {
        ui->tableWidgetJournal->setColumnWidth(i, list[i]);
    }
}
//-------------------------------------------------------------
void CJournalWidget::setTableColumnWidth(int column, int width)
{
    if(column < 0 || column >= ui->tableWidgetJournal->columnCount())
        return;

    ui->tableWidgetJournal->setColumnWidth(column, width);
}
//-------------------------------------------------------
void CJournalWidget::setJournalDescription(QVariant data)
{
    m_journal_data = data;
}
//------------------------------------------------------------------------
void CJournalWidget::setVisibleProperty(PropertyType property, bool state)
{
    if(property == CRASH_PROPERTY)
        ui->listViewPropertyCrashJournal->setVisible(state);
    else if(property == HALFHOUR_PROPERTY)
        ui->tableWidgetPropertyHalfhourJournal->setVisible(state);
}
//------------------------------------------------------------------------------------
void CJournalWidget::setHalfhourHeaders(const QVector<halfhour_item_t>& halfhour_cols,
                                        const QVector<halfhour_item_t>& halfhour_rows)
{
    QStringList hor_labels, ver_labels;

    for(const halfhour_item_t& item: halfhour_cols)
    {
        hor_labels << QString("%1, %2").arg(item.name).arg(item.description);
    }

    for(const halfhour_item_t& item: halfhour_rows)
    {
        ver_labels << QString("%1").arg(item.name);
    }

    ui->tableWidgetPropertyHalfhourJournal->setColumnCount(hor_labels.count());
    ui->tableWidgetPropertyHalfhourJournal->setRowCount(ver_labels.count());
    ui->tableWidgetPropertyHalfhourJournal->setHorizontalHeaderLabels(hor_labels);
    ui->tableWidgetPropertyHalfhourJournal->setVerticalHeaderLabels(ver_labels);
}
//---------------------------------------
void CJournalWidget::journalClear() const
{
    ui->tableWidgetJournal->clearContents();
    ui->tableWidgetJournal->setRowCount(0);

    CJournalPropertyModel* model = qobject_cast<CJournalPropertyModel*>(ui->listViewPropertyCrashJournal->model());

    if(model)
    {
        QVector<QPair<QString, QString> > pair;
        model->setDataModel(pair);
    }

    ui->tableWidgetPropertyHalfhourJournal->clearContents();
}
/*!
 * \brief CJournalWidget::unpackDateTime
 * \param  data Данные в которых хранится упакованные дата и время
 * \return Возвращает дату/время, если данные валидные, иначе пустую дату/время
 *
 * Метод работает с журналами для преобразования данных в дату/время
 */
QDateTime CJournalWidget::unpackDateTime(QVector<quint8>& data)
{
    if(data.isEmpty() || data.count() != 5) // упакованная дата размещается в 5ти байтах
        return QDateTime(); // возвращаем пусту дату, если нет данных или их размер меньше или больше 5ти байт

    quint16 year = ((data[0]&0xFC) >> 2);

    year += ((year < 2000)?2000:0); // приводит дату к 'yyyy'

    quint8 month = static_cast<quint8>((data[0]&0x03) << 2) | ((data[1]&0xC0) >> 6);
    quint8 day   = ((data[1]&0x3E) >> 1);

    quint8  hour    = static_cast<quint8>((data[1]&0x01) << 4) | ((data[2]&0xF0) >> 4);
    quint8  minute  = static_cast<quint8>((data[2]&0x0F) << 2) | ((data[3]&0xC0) >> 6);
    quint8  second  = (data[3]&0x3F);
    quint16 msecond = quint16(data[4]*3.90625f); // перевод 256 долей секунды в мс, т.е. 1000/256 = 3.90625

    QDateTime dt(QDate(year, month, day), QTime(hour, minute, second, msecond));

    return dt;
}
/*!
 * \brief CJournalWidget::convertHalfwordToBytes
 * \param source Источник данных - передает вектор полуслов
 * \param dest   Назначение - возвращает вектор байт
 *
 * Предназначен для конвертирования из 16ти битных данных в 8ми битные
 */
void CJournalWidget::convertHalfwordToBytes(const QVector<quint16>& source, QVector<quint8>& dest)
{
    for(quint16 value: source)
    {
        dest << (quint8(((value >> 8)&0x00FF))) << (quint8((value&0x00FF)));
    }
}
//---------------------------------------------
date_t CJournalWidget::secsToDate(quint32 secs)
{
    date_t date;

    date.day  = secs/86400;
    date.hour = (secs = secs%86400)/3600;
    date.min  = (secs = secs%3600)/60;
    date.sec  = (secs%60);

    return date;
}
//----------------------------------------------------------------
void CJournalWidget::printCrash(const QVector<quint8>& data) const
{
    if(data.count() != 256)
        return;

    quint16 id = static_cast<quint16>((data[1] << 8) | data[0]);

    QDateTime dt = unpackDateTime(QVector<quint8>() << data[2] << data[3] << data[4] << data[5] << data[6]);

    int row = ui->tableWidgetJournal->rowCount();

    ui->tableWidgetJournal->insertRow(row);
    ui->tableWidgetJournal->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
    ui->tableWidgetJournal->setItem(row, 2, new QTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));

    int protect_code = data[7]; // код защиты

    protection_t protection = qvariant_cast<protection_t>(m_journal_data);

    if(protection.items.find(protect_code) == protection.items.end()) // код защиты не обнаружен
        return;

    QPair<QString, QVector<protection_item_t> > pair = protection.items[protect_code];

    ui->tableWidgetJournal->setItem(row, 3, new QTableWidgetItem(pair.first)); // вывод в таблицу имени защиты

    // парсинг свойств (настроек защиты, переменные, расчетные значения) и занесение их в хранилище таблицы
    // Настройки защиты - хрантятся в 28 байтах с 8го по 35й включительно
    QVector<protection_item_t> items = pair.second; // получаем список переменных защиты

    if(items.isEmpty())
        return;

    property_list_t property_list;

    union // объединение для преобразования в тип float
    {
        quint8  bytes[4];
        float   _float;
        quint32 _int;
    } val;

    // добавление в свойства настроек защит
    property_list << property_data_item_t({ ";", tr("Настройки защиты") });

    for(const protection_item_t& item: items)
    {
        if(item.type == "LIST") // если тип является списком, то получаем номер варианта настройки
        {
            int row = data[item.first];

            if(protection.sets.find(item.index) != protection.sets.end()) // если такой набор настроек существует
            {
                int     count    = protection.sets[item.index].count();
                QString set_item = tr("Не определена");

                if(count >= 0 && row < count)
                {
                    set_item = protection.sets[item.index][row]; // получаем название варианта настройки
                }

                property_list << property_data_item_t({ item.name, set_item });
            }
        }
        else if(item.type == "FLOAT") // если тип является вещественным типом, то высчитываем значение
        {
            for(int i = 0; i < 4; i++) // размерность типа FLOAT 4 байта
            {
                int pos = item.first + i;

                val.bytes[i] = data[pos];
            }

            property_list << property_data_item_t({ item.name, QString::number(static_cast<double>(val._float), 'f', 6) });
        }
    }

    property_list << property_data_item_t({ ";", tr("Расчетные величины") }); // добавление разделителя между блоками переменных

    // добавление в свойства расчетных величин
    calc_value_list_t calc_value_list = protection.calc;

    if(!calc_value_list.isEmpty())
    {
        for(const calc_value_t& value: calc_value_list)
        {
            for(int i = 0; i < 4; i++)
            {
                int pos = value.first + i;
                val.bytes[i] = data[pos];
            }

            property_list << property_data_item_t({ value.name, QString::number(static_cast<double>(val._float), 'f', 6) });
        }
    }

    property_list << property_data_item_t({ ";", tr("Внутренние переменные: выходы") }); // добавление разделителя между блоками переменных

    variable_list_t variable_list = protection.variable;

    if(!variable_list.isEmpty())
    {
        for(int i = 0; i < 48; i += 4) // переход через 4 байта (32 бита) - 32*12 = 384 максимальное количество переменных,
                                       // но их пока всего 358
        {
            for(int j = 0; j < 4; j++) // размерность типа INT 4 байта
            {
                int pos = 36 + i + j;

                val.bytes[j] = data[pos];
            }

            quint32 value = val._int;

            for(int k = 0; k < 32; k++) // обработка переменной состояний из 32 бит
            {
                int ivar = i*8 + k; // вычисляем индекс переменной

                if(ivar >= 358)
                    break;

                bool    state = (value&(1 << k));
                QString str   = ((state)?tr("Да"):tr("Нет"));

                property_list << property_data_item_t({ QString("%1 (%2)").arg(variable_list[ivar].index).arg(variable_list[ivar].name),
                                                        str });
            }
        }
    }

    property_list << property_data_item_t({ ";", tr("Реле, светодиоды, модифицируемые переменные") });

    io_list_t out_list = protection.out;

    if(!out_list.isEmpty())
    {
        for(int i = 0; i < 8; i += 4)
        {
            for(int j = 0; j < 4; j++) // размерность типа INT 4 байта
            {
                int pos = 84 + i + j;

                val.bytes[j] = data[pos];
            }

            quint32 value = val._int;

            for(int k = 0; k < 32; k++) // обработка переменной состояний из 32 бит
            {
                int ivar = i*8 + k; // вычисляем индекс переменной

                if(ivar >= out_list.count())
                    break;

                bool    state = (value&(1 << k));
                QString str   = ((state)?tr("Да"):tr("Нет"));

                property_list << property_data_item_t({ QString("%1 (%2)").arg(out_list[ivar].index).arg(out_list[ivar].description),
                                                        str });
            }
        }
    }

    property_list << property_data_item_t({ ";", tr("Входы") });

    io_list_t input_list = protection.input;

    if(!input_list.isEmpty())
    {
        for(int j = 0; j < 4; j++) // размерность типа INT 4 байта
        {
            int pos = 92 + j;

            val.bytes[j] = data[pos];
        }

        quint32 value = val._int;

        for(int k = 0; k < 32; k++) // обработка переменной состояний из 32 бит
        {
            if(k >= 20)
                break;

            bool    state = (value&(1 << k));
            QString str   = ((state)?tr("Да"):tr("Нет"));

            property_list << property_data_item_t({ QString("%1 (%2)").arg(input_list[k].index).arg(input_list[k].description),
                                                    str });
        }
    }

    ui->tableWidgetJournal->setRowData(row, QVariant::fromValue(property_list));
}
//----------------------------------------------------------------
void CJournalWidget::printEvent(const QVector<quint8>& data) const
{
    for(int i = 0; i < data.count(); i += 16)
    {
        quint16 id = static_cast<quint16>((data[i + 1] << 8) | data[i]);

        QDateTime dt = unpackDateTime(QVector<quint8>() << data[i + 2] << data[i + 3] << data[i + 4] << data[i + 5] << data[i + 6]);

        quint8  type_event      = data[i + 7];
        quint8  category_event  = data[i + 8];
        quint16 parameter_event = static_cast<quint16>(data[i + 9] | (data[i + 10] << 8));

        QVector<QString> category_error_list = QVector<QString>() << tr("Ok") << tr("не ACK") << tr("Привязка") <<
                                                                     tr("Контрольная сумма") << tr("9 бит") <<
                                                                     tr("Ответ не полный") << tr("Нет ответа") << tr("HAL");

        QVector<event_t> etype = qvariant_cast<QVector<event_t> >(m_journal_data);

        if(!etype.isEmpty())
        {
            QVector<event_t> ecategory  = QVector<event_t>();
            QVector<event_t> eparameter = QVector<event_t>();

            if(etype.count() > type_event)
                ecategory = etype[type_event].sub_event;

            if(ecategory.count() > category_event)
                eparameter = ecategory[category_event].sub_event;

            int row = ui->tableWidgetJournal->rowCount();

            ui->tableWidgetJournal->insertRow(row);

            QString etype_str = tr("Неизвестный тип");

            if(etype.count() > type_event)
                etype_str = etype[type_event].name;

            ui->tableWidgetJournal->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
            ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));

            ui->tableWidgetJournal->setItem(row, 2, new QTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));
            ui->tableWidgetJournal->setItem(row, 3, new QTableWidgetItem(QTableWidgetItem(etype_str + QString(" (%1)").
                                                                                  arg(type_event))));

            QString ecategory_str  = (ecategory.isEmpty())?tr("Неизвестная категория"):ecategory[category_event].name;
            QString eparameter_str = "";

            if(type_event == 3) // если событие - Ошибка выполнения команды модулем
            {
                int error_code  = ((parameter_event&0xFF00) >> 8);
                parameter_event = parameter_event&0x00FF;
                eparameter_str  = (error_code >= category_error_list.count())?tr("Неизвестная ошибка"):tr("Ошибка \'%1\': ").
                                                                              arg(category_error_list[error_code]);
            }

            eparameter_str += ((eparameter.isEmpty() || (eparameter.count() <= parameter_event))?
                               tr("Неизвестный параметр"):eparameter[parameter_event].name);

            ui->tableWidgetJournal->setItem(row, 4, new QTableWidgetItem(ecategory_str +
                                                    QString(" (%1)").arg(QString::number(category_event))));
            ui->tableWidgetJournal->setItem(row, 5, new QTableWidgetItem(eparameter_str +
                                                    QString(" (%1)").arg(QString::number(parameter_event))));

            ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
//-------------------------------------------------------------------
void CJournalWidget::printHalfHour(const QVector<quint8>& data) const
{
    QVector<QString> type_list = QVector<QString>() << tr("Данные") << tr("Установка даты/времени") << tr("Новые дата/время") <<
                                                       tr("Обнуление счетчика");

    for(int i = 0; i < data.count(); i += 64)
    {
        quint16   id = static_cast<quint16>((data[i + 1] << 8) | data[i]);
        QDateTime dt = unpackDateTime(QVector<quint8>() << data[i + 2] << data[i + 3] << data[i + 4] << data[i + 5] << data[i + 6]);
        quint8    type = data[i + 7];
        int       row  = ui->tableWidgetJournal->rowCount();

        ui->tableWidgetJournal->insertRow(row);

        ui->tableWidgetJournal->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(id)));
        ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
        ui->tableWidgetJournal->setItem(row, 2, new QTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));

        ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);

        if(type < type_list.count())
        {
            ui->tableWidgetJournal->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(type_list[type])));
            ui->tableWidgetJournal->item(row, 3)->setTextAlignment(Qt::AlignCenter);

            if(type == 0)
            {
                quint32 secs = static_cast<quint32>((data[i + 11] << 24) | (data[i + 10] << 16) | (data[i + 9] << 8) | data[i + 8]);

                if(ui->tableWidgetJournal->isColumnHidden(4))
                    ui->tableWidgetJournal->showColumn(4);

                date_t  t    = secsToDate(secs);
                QString time = tr("%1 дн. %2 ч. %3 мин. %4 сек.").arg(t.day).arg(t.hour).arg(t.min).arg(t.sec);

                ui->tableWidgetJournal->setItem(row, 4, new QTableWidgetItem(time));

                union
                {
                    quint8 buf[4];
                    float  _float;
                } value;

                halfhour_t halfhour;

                for(int j = 0; j < 12; j++)
                {
                    int index = j*4 + 12;

                    value.buf[0] = data[index];
                    value.buf[1] = data[index + 1];
                    value.buf[2] = data[index + 2];
                    value.buf[3] = data[index + 3];

                    halfhour.values << value._float;
                }

                ui->tableWidgetJournal->setRowData(row, QVariant::fromValue(halfhour));
            }
        }
    }
}
//-------------------------------------------------------------
void CJournalWidget::clickedItemTable(const QModelIndex& index)
{
    QString journal_type = property("TYPE").toString();

    if(!index.isValid() || journal_type.isEmpty())
        return;

    if(journal_type == "CRASH")
    {
        property_list_t property_list = qvariant_cast<property_list_t>(ui->tableWidgetJournal->rowData(index.row()));

        if(property_list.isEmpty())
            return;

        QVector<QPair<QString, QString> > list;

        for(const property_data_item_t& item: property_list)
        {
            list << qMakePair(item.name, item.value);
        }

        if(!list.isEmpty())
        {
            CJournalPropertyModel* model = qobject_cast<CJournalPropertyModel*>(ui->listViewPropertyCrashJournal->model());

            if(model)
            {
                model->setDataModel(list);
            }
        }
    }
    else if(journal_type == "HALFHOUR")
    {
        ui->tableWidgetPropertyHalfhourJournal->clearContents();

        halfhour_t halfhour = qvariant_cast<halfhour_t>(ui->tableWidgetJournal->rowData(index.row()));

        if(halfhour.values.count() != 12)
            return;

        for(int col = 0; col < ui->tableWidgetPropertyHalfhourJournal->columnCount(); col++)
        {
            for(int row = 0; row < ui->tableWidgetPropertyHalfhourJournal->rowCount(); row++)
            {
                QTableWidgetItem* item = ui->tableWidgetPropertyHalfhourJournal->item(row, col);

                int     index     = col*4 + row;
                QString str_value = QLocale::system().toString(halfhour.values[index], 'f', 1);

                if(!item)
                {
                    item = new QTableWidgetItem;
                    ui->tableWidgetPropertyHalfhourJournal->setItem(row, col, item);
                }

                item->setTextAlignment(Qt::AlignCenter);
                item->setText(str_value);
            }
        }
    }
}
