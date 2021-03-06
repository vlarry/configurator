#include "cjournalwidget.h"
#include "ui_journalwidget.h"
//----------------------------------------------
CJournalWidget::CJournalWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CJournalWidget),
    m_row_start(1)
{
    ui->setupUi(this);

    ui->tableWidgetJournal->setShowGrid(true);
    ui->tableWidgetJournal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetJournal->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetJournal->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidgetJournal->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableWidgetJournal->verticalHeader()->hide();

    connect(ui->widgetJournalHeader, &CHeaderJournal::clickedButtonRead, this, &CJournalWidget::clickedButtonRead);
    connect(ui->tableWidgetJournal, &CJournalTable::clicked, this, &CJournalWidget::clickedItemTable);
    connect(this, &CJournalWidget::printFinished, this, &CJournalWidget::updateTableJournal);

    setAutoFillBackground(true);

    CJournalPropertyModel* model = new CJournalPropertyModel();

    ui->listViewPropertyCrashJournal->setModel(model);
    ui->listViewPropertyCrashJournal->setItemDelegate(new CJournalPropertyDelegate());

    ui->listViewPropertyCrashJournal->hide(); // по умолчанию окно свойств журнала аварий скрыто
    ui->tableWidgetPropertyHalfhourJournal->hide(); // по умолчанию окно свойств журнала получасовок скрыто

    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 2);

    qDebug() << "constructor JornalWidget";
}
//-------------------------------
CJournalWidget::~CJournalWidget()
{
    delete ui;
}
//--------------------------------------------
CHeaderJournal* CJournalWidget::header() const
{
    if(ui->widgetJournalHeader)
        return ui->widgetJournalHeader;
    else
        return nullptr;
}
//--------------------------------------
void CJournalWidget::headerClear() const
{
    ui->widgetJournalHeader->clear();
}
//------------------------------------------
CJournalTable* CJournalWidget::table() const
{
    return ui->tableWidgetJournal;
}
//----------------------------------------------
QWidget* CJournalWidget::propertyJournal() const
{
    if(m_property_type == CRASH_PROPERTY)
        return ui->listViewPropertyCrashJournal;
    else if(m_property_type == HALFHOUR_PROPERTY)
        return ui->tableWidgetPropertyHalfhourJournal;

    return nullptr;
}
/*!
 * \brief CJournalWidget::print
 * \param data Данные для вывода на монитор
 *
 * Предназначен для выбора и запуска вывода данных на экран в зависимости от журнала
 */
void CJournalWidget::print(const QVector<quint16>& data) const
{
    ui->tableWidgetJournal->setSortingEnabled(false);

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
    else if(journal_type == "ISOLATION")
    {
        printIsolation(bytes);
    }
    else if(journal_type == "SET")
    {
        printSet(bytes);
    }
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

    ui->tableWidgetJournal->horizontalHeader()->setStretchLastSection(true);
}
//-------------------------------------------------------------
void CJournalWidget::setTableColumnWidth(int column, int width)
{
    if(column < 0 || column >= ui->tableWidgetJournal->columnCount())
        return;

    ui->tableWidgetJournal->setColumnWidth(column, width);

    ui->tableWidgetJournal->horizontalHeader()->setStretchLastSection(true);
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
    {
//        ui->listViewPropertyCrashJournal->setFixedWidth(300);
        ui->listViewPropertyCrashJournal->setVisible(state);
    }
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
//-----------------------------------------------------------------
void CJournalWidget::setPropertySettings(const set_property_t &set)
{
    m_set_property = set;
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
//-----------------------------------------
bool CJournalWidget::journalIsEmpty() const
{
    return !ui->tableWidgetJournal->rowCount();
}
/*!
 * \brief CJournalWidget::setRowStartTable
 * \param row номер строки с которой начинается порядковый номер в таблице
 */
void CJournalWidget::setRowStartTable(int row)
{
    m_row_start = row;
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
    qDebug() << QString("Вывод журнала аварий: Размер %1 байт").arg(data.count());

    int data_count = data.count();

    if(data_count < 256 || (data_count%256) != 0)
    {
        qDebug() << QString("Вывод журнала аварий: Размер журнала не кратен 256");

        if(data_count < 256 && (data_count%128) != 0) // размер не кратен 128, т.е. ошибка, то выходим (иначе не успели принять вторую половину пакета
            return;
        else
            data_count -= 128; // обрезаем размер данных, чтобы выводить только полную информацию
    }

    int row = ui->tableWidgetJournal->rowCount();

    for(int index = 0; index < data_count; index += 256)
    {
        QVector<quint8> msg_data = data.mid(index, 256);

        quint16 id = static_cast<quint16>((msg_data[1] << 8) | msg_data[0]);
        QDateTime dt = unpackDateTime(QVector<quint8>() << msg_data[2] << msg_data[3] << msg_data[4] << msg_data[5] << msg_data[6]);

        ui->tableWidgetJournal->insertRow(row);
        ui->tableWidgetJournal->setItem(row, 0, new CTableWidgetItem(QString("%1").arg(row + m_row_start)));
        ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(QString::number(id)));
        ui->tableWidgetJournal->setItem(row, 2, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
        ui->tableWidgetJournal->setItem(row, 3, new CTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));

        int protect_code = msg_data[7]; // код защиты

        protection_t protection = qvariant_cast<protection_t>(m_journal_data);

        if(protection.items.find(protect_code) == protection.items.end()) // код защиты не обнаружен
            return;

        QPair<QString, QVector<protection_item_t> > pair = protection.items[protect_code];

        ui->tableWidgetJournal->setItem(row, 4, new CTableWidgetItem(pair.first)); // вывод в таблицу имени защиты

        ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        // парсинг свойств (настроек защиты, переменные, расчетные значения) и занесение их в хранилище таблицы
        // Настройки защиты - хрантятся в 28 байтах с 8го по 35й включительно
        QVector<protection_item_t> items = pair.second; // получаем список переменных защиты

        if(items.isEmpty())
            return;

        property_list_t property_list;

        union // объединение для преобразования в тип float
        {
            quint8  bytes[4];
            float   value;
            quint32 _int;
        } val;

        // добавление в свойства настроек защит
        property_list << property_data_item_t({ ";", tr("Настройки защиты") });

        for(const protection_item_t& item: items)
        {
            if(item.type == "LIST") // если тип является списком, то получаем номер варианта настройки
            {
                int row_item = msg_data[item.first];

                if(protection.sets.find(item.index) != protection.sets.end()) // если такой набор настроек существует
                {
                    int     count    = protection.sets[item.index].count();
                    QString set_item = tr("Не определена");

                    if(count >= 0 && row_item < count)
                    {
                        set_item = protection.sets[item.index][row_item]; // получаем название варианта настройки
                    }

                    property_list << property_data_item_t({ item.name, set_item });
                }
            }
            else if(item.type == "FLOAT") // если тип является вещественным типом, то высчитываем значение
            {
                for(int i = 0; i < 4; i++) // размерность типа FLOAT 4 байта
                {
                    int pos = item.first + i;

                    val.bytes[i] = msg_data[pos];
                }

                property_list << property_data_item_t({ item.name, QString::number(static_cast<double>(val.value), 'f', 6) });
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
                    val.bytes[i] = msg_data[pos];
                }

                property_list << property_data_item_t({ value.name, QString::number(static_cast<double>(val.value), 'f', 6) });
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

                    val.bytes[j] = msg_data[pos];
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

                    val.bytes[j] = msg_data[pos];
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

                val.bytes[j] = msg_data[pos];
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

        ui->tableWidgetJournal->setRowData(row++, QVariant::fromValue(property_list));
    }

    emit printFinished();

    qDebug() << QString("Вывод журнала аварий завершен успешно!");
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

            ui->tableWidgetJournal->setItem(row, 0, new CTableWidgetItem(QString("%1").arg(row + m_row_start)));
            ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(QString::number(id)));
            ui->tableWidgetJournal->setItem(row, 2, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));

            ui->tableWidgetJournal->setItem(row, 3, new CTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));
            ui->tableWidgetJournal->setItem(row, 6, new CTableWidgetItem(CTableWidgetItem(etype_str + QString(" (%1)").
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

            ui->tableWidgetJournal->setItem(row, 5, new CTableWidgetItem(ecategory_str +
                                                    QString(" (%1)").arg(QString::number(category_event))));
            ui->tableWidgetJournal->setItem(row, 4, new CTableWidgetItem(eparameter_str +
                                                    QString(" (%1)").arg(QString::number(parameter_event))));

            ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        }
    }

    emit printFinished();
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

        ui->tableWidgetJournal->setItem(row, 0, new CTableWidgetItem(QString("%1").arg(row + m_row_start)));
        ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(QString("%1").arg(id)));
        ui->tableWidgetJournal->setItem(row, 2, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
        ui->tableWidgetJournal->setItem(row, 3, new CTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));

        ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        if(type < type_list.count())
        {
            ui->tableWidgetJournal->setItem(row, 4, new CTableWidgetItem(QString("%1").arg(type_list[type])));
            ui->tableWidgetJournal->item(row, 4)->setTextAlignment(Qt::AlignCenter);

            if(type == 0)
            {
                quint32 secs = static_cast<quint32>((data[i + 11] << 24) | (data[i + 10] << 16) | (data[i + 9] << 8) | data[i + 8]);

                if(ui->tableWidgetJournal->isColumnHidden(4))
                    ui->tableWidgetJournal->showColumn(4);

                date_t  t    = secsToDate(secs);
                QString time = tr("%1 дн. %2 ч. %3 мин. %4 сек.").arg(t.day).arg(t.hour).arg(t.min).arg(t.sec);

                ui->tableWidgetJournal->setItem(row, 5, new CTableWidgetItem(time));

                union
                {
                    quint8 buf[4];
                    float  value;
                } value;

                halfhour_t halfhour;

                for(int j = 0; j < 12; j++)
                {
                    int index = j*4 + 12;

                    value.buf[0] = data[index];
                    value.buf[1] = data[index + 1];
                    value.buf[2] = data[index + 2];
                    value.buf[3] = data[index + 3];

                    halfhour.values << value.value;
                }

                ui->tableWidgetJournal->setRowData(row, QVariant::fromValue(halfhour));
            }
        }
    }

    emit printFinished();
}
//--------------------------------------------------------------------
void CJournalWidget::printIsolation(const QVector<quint8> &data) const
{
    QVector<QString> type_list = QVector<QString>() << "D0" << "D1" << "D2" << "D3" << "D4" << "D5" << "D6" << "D7";

    for(int i = 0; i < data.count(); i += 64)
    {
        quint16   id = static_cast<quint16>((data[i + 1] << 8) | data[i]);
        QDateTime dt = unpackDateTime(QVector<quint8>() << data[i + 2] << data[i + 3] << data[i + 4] << data[i + 5] << data[i + 6]);
        quint8    type = data[i + 7];
        int       row  = ui->tableWidgetJournal->rowCount();

        ui->tableWidgetJournal->insertRow(row);

        ui->tableWidgetJournal->setItem(row, 0, new CTableWidgetItem(QString("%1").arg(row + m_row_start)));
        ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(QString("%1").arg(id)));
        ui->tableWidgetJournal->setItem(row, 2, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
        ui->tableWidgetJournal->setItem(row, 3, new CTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));

        ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        if(type < type_list.count())
        {
            ui->tableWidgetJournal->setItem(row, 4, new CTableWidgetItem(QString("%1").arg(type_list[type])));
            ui->tableWidgetJournal->item(row, 4)->setTextAlignment(Qt::AlignCenter);

            union
            {
                quint8 buf[4];
                float  value;
            } value;

            value.buf[0] = data[8];
            value.buf[1] = data[9];
            value.buf[2] = data[10];
            value.buf[3] = data[11];

            ui->tableWidgetJournal->setItem(row, 5, new CTableWidgetItem(QString("%1").arg(QLocale::system().toString(value.value, 'f', 2))));
            ui->tableWidgetJournal->item(row, 5)->setTextAlignment(Qt::AlignCenter);

            value.buf[0] = data[12];
            value.buf[1] = data[13];
            value.buf[2] = data[14];
            value.buf[3] = data[15];

            ui->tableWidgetJournal->setItem(row, 6, new CTableWidgetItem(QString("%1").arg(QLocale::system().toString(value.value, 'f', 2))));
            ui->tableWidgetJournal->item(row, 6)->setTextAlignment(Qt::AlignCenter);

            value.buf[0] = data[16];
            value.buf[1] = data[17];
            value.buf[2] = data[18];
            value.buf[3] = data[19];

            ui->tableWidgetJournal->setItem(row, 7, new CTableWidgetItem(QString("%1").arg(QLocale::system().toString(value.value, 'f', 2))));
            ui->tableWidgetJournal->item(row, 7)->setTextAlignment(Qt::AlignCenter);
        }
    }

    emit printFinished();
}
//--------------------------------------------------------------
void CJournalWidget::printSet(const QVector<quint8> &data) const
{
    union
    {
        quint8 buf[4];
        float  value;
    } Float;

    union
    {
        quint8 buf[2];
        quint16 value;
    } U16;

    QStringList source_str_list = QStringList() << tr("меню") << tr("АРМ (запись по MODBUS)") << tr("БЗУ (установка умолчаний)") << tr("БЗУ (прочее)");
    QStringList baudrate_list = QStringList() << "300" << "600" << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    QStringList parity_list = QStringList() << "None" << "Odd" << "Even";

    QVector<QPair<int, QString>> item_umin =
    {
        qMakePair(1, tr("Пуск МТЗ3")),
        qMakePair(0, tr("Пуск Пусковой"))
    };

    QVector<QPair<int, QString>> item_level =
    {
        qMakePair(3, tr("Пуск УРОВ по КЦУ")),
        qMakePair(2, tr("Отказ цепи отключения")),
        qMakePair(1, tr("Отказ после защиты")),
        qMakePair(0, tr("Работа ВАКУУМ"))
    };

    QVector<QPair<int, QString>> item_apv =
    {
        qMakePair(14, tr("Откл МТЗ1")),
        qMakePair(13, tr("Откл МТЗ2")),
        qMakePair(12, tr("Откл МТЗ3")),
        qMakePair(11, tr("Откл МТЗ4")),
        qMakePair(10, tr("Откл ОЗЗ1")),
        qMakePair(9, tr("Откл ОЗЗ2")),
        qMakePair(8, tr("Откл НЗЗ1")),
        qMakePair(7, tr("Откл НЗЗ2")),
        qMakePair(6, tr("Откл 3U0")),
        qMakePair(5, tr("Откл АЧР1")),
        qMakePair(4, tr("Откл АЧР2")),
        qMakePair(3, tr("Откл АЧР3")),
        qMakePair(2, tr("Откл ВНЕШНЯЯ1")),
        qMakePair(1, tr("Откл ВНЕШНЯЯ2")),
        qMakePair(0, tr("Откл ВНЕШНЯЯ3"))
    };

    for(int i = 0; i < data.count(); i += 24)
    {
        quint16   id  = static_cast<quint16>((data[i + 1] << 8) | data[i]);
        QDateTime dt  = unpackDateTime(QVector<quint8>() << data[i + 2] << data[i + 3] << data[i + 4] << data[i + 5] << data[i + 6]);
        int       row = ui->tableWidgetJournal->rowCount();

        ui->tableWidgetJournal->insertRow(row);

        ui->tableWidgetJournal->setItem(row, 0, new CTableWidgetItem(QString("%1").arg(row + m_row_start)));
        ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(QString("%1").arg(id)));
        ui->tableWidgetJournal->setItem(row, 2, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
        ui->tableWidgetJournal->setItem(row, 3, new CTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));

        ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 3)->setTextAlignment(Qt::AlignCenter);

        quint8 source = (data[i + 7]&0xF0) >> 4;
        quint8 code = (data[i + 7]&0x0F);
        quint8 source_index = 0;

        if(source < source_str_list.count())
            source_index = source;

        ui->tableWidgetJournal->setItem(row, 4, new CTableWidgetItem(QString("%1").arg(source_str_list[source_index])));
        ui->tableWidgetJournal->setItem(row, 5, new CTableWidgetItem(QString("%1").arg(code)));

        ui->tableWidgetJournal->item(row, 4)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetJournal->item(row, 5)->setTextAlignment(Qt::AlignCenter);

        quint16 valindex = static_cast<quint16>((data[i + 17] << 8) | data[i + 16]);

        if(m_set_property.find(valindex) != m_set_property.end())
        {
            set_param_t param = m_set_property[valindex];

            QString strval_prev = "";
            QString strval_next = "";

            ui->tableWidgetJournal->setItem(row, 6, new CTableWidgetItem(QString("%1").arg(param.description)));
            ui->tableWidgetJournal->item(row, 6)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            if(param.type.toUpper() == "FLOAT")
            {
                Float.buf[0] = data[i + 8];
                Float.buf[1] = data[i + 9];
                Float.buf[2] = data[i + 10];
                Float.buf[3] = data[i + 11];

                strval_prev = QLocale::system().toString(Float.value, 'f', 2);

                Float.buf[0] = data[i + 12];
                Float.buf[1] = data[i + 13];
                Float.buf[2] = data[i + 14];
                Float.buf[3] = data[i + 15];

                strval_next = QLocale::system().toString(Float.value, 'f', 2);
            }
            else if(param.type.toUpper() == "INT")
            {
                QVector<QString> items = param.item.items;

                U16.buf[0] = data[i + 8];
                U16.buf[1] = 0;

                if(!items.isEmpty() && U16.value <= items.count())
                {
                    int index = U16.value - 1;

                    if(index < 0)
                        index = 0;

                    strval_prev = items[index];

                    U16.buf[0] = data[i + 12];
                    U16.buf[1] = 0;

                    if(!items.isEmpty() && U16.value <= items.count())
                    {
                        index = U16.value - 1;

                        if(index < 0)
                            index = 0;

                        strval_next = items[index];
                    }
                }
            }
            else if(param.type.toUpper() == "MODE")
            {
                quint8 i11_prev = data[i + 8]&0x01;
                quint8 i17_prev = ((data[i + 8]&0x02) >> 1)&0x01;
                quint8 i11_next = data[i + 12]&0x01;
                quint8 i17_next = ((data[i + 12]&0x02) >> 1)&0x01;

                if(!i11_prev && !i17_prev) // 0 | 0
                    strval_prev = "Сигнал";
                else if(i11_prev && !i17_prev) // 0 | 1
                {
                    strval_prev = "Отключение";
                }
                else if(!i11_prev && i17_prev) // 1 | 0
                {
                    strval_prev = "Откл. с блок.";
                }
                else if(i11_prev && i17_prev) // 1 | 1
                {
                    strval_prev = "Запрещено!";
                }

                if(!i11_next && !i17_next) // 0 | 0
                    strval_next = "Сигнал";
                else if(i11_next && !i17_next) // 0 | 1
                {
                    strval_next = "Отключение";
                }
                else if(!i11_next && i17_next) // 1 | 0
                {
                    strval_next = "Откл. с блок.";
                }
                else if(i11_next && i17_next) // 1 | 1
                {
                    strval_next = "Запрещено!";
                }
            }
            else if(param.type.toUpper() == "BIND")
            {
                QVector<QPair<int, QString>> items;

                if(param.item.key.toUpper().contains("UMIN")) // umin1/2
                {
                    items = item_umin;
                }
                else if(param.item.key.toUpper().contains("LEVEL")) // level1/2
                {
                    items = item_level;
                }
                else if(param.item.key.toUpper().contains("APV")) // apv
                {
                    items = item_apv;
                }

                quint16 val_prev = ((data[i + 9] << 8) | data[i + 8]);
                quint16 val_next = ((data[i + 13] << 8) | data[i + 12]);

                for(int i = 0; i < items.count(); i++)
                {
                    bool val_bit_prev = (val_prev & (1 << items[i].first));
                    bool val_bin_next = (val_next & (1 << items[i].first));

                    strval_prev += QString("%1: %2").arg(items[i].second).arg((val_bit_prev)?"Вкл":"Откл");
                    strval_next += QString("%1: %2").arg(items[i].second).arg((val_bin_next)?"Вкл":"Откл");

                    if(i < items.count() - 1)
                    {
                        strval_prev += "\n";
                        strval_next += "\n";
                    }
                }
            }
            else if(param.type.toUpper() == "U8" || param.type.toUpper() == "U16") // настройки
            {
                U16.buf[0] = data[i + 8];
                U16.buf[1] = data[i + 9];

                if(param.item.key.toUpper() == "MODBUS_ISPEED")
                    strval_prev = baudrate_list[U16.value];
                else if(param.item.key.toUpper() == "MODBUS_IPARITY")
                    strval_prev = parity_list[U16.value];
                else if(param.item.key.toUpper() == "MODBUS_ISTOP")
                    strval_prev = QLocale::system().toString(U16.value + 1);
                else
                    strval_prev = QLocale::system().toString(U16.value);

                U16.buf[0] = data[i + 12];
                U16.buf[1] = data[i + 13];

                if(param.item.key.toUpper() == "MODBUS_ISPEED")
                    strval_next = baudrate_list[U16.value];
                else if(param.item.key.toUpper() == "MODBUS_IPARITY")
                    strval_next = parity_list[U16.value];
                else if(param.item.key.toUpper() == "MODBUS_ISTOP")
                    strval_next = QLocale::system().toString(U16.value + 1);
                else
                    strval_next = QLocale::system().toString(U16.value);
            }

            ui->tableWidgetJournal->setItem(row, 7, new CTableWidgetItem(QString("%1").arg(strval_prev)));
            ui->tableWidgetJournal->setItem(row, 8, new CTableWidgetItem(QString("%1").arg(strval_next)));

            if(param.type.toUpper() != "BIND")
            {
                ui->tableWidgetJournal->item(row, 7)->setTextAlignment(Qt::AlignCenter);
                ui->tableWidgetJournal->item(row, 8)->setTextAlignment(Qt::AlignCenter);
            }
            else
            {
                ui->tableWidgetJournal->item(row, 7)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                ui->tableWidgetJournal->item(row, 8)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            }
        }

        U16.buf[0] = data[i + 16];
        U16.buf[1] = data[i + 17];

        ui->tableWidgetJournal->setItem(row, 9, new CTableWidgetItem(QString("%1").arg(U16.value)));
        ui->tableWidgetJournal->item(row, 9)->setTextAlignment(Qt::AlignCenter);
    }

    emit printFinished();
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
//---------------------------------------------
void CJournalWidget::updateTableJournal() const
{
    ui->tableWidgetJournal->resizeColumnsToContents();
    ui->tableWidgetJournal->resizeRowsToContents();
    ui->tableWidgetJournal->horizontalHeader()->setStretchLastSection(true);
    ui->widgetJournalHeader->setTextTableCountMessages(ui->tableWidgetJournal->rowCount());
    ui->tableWidgetJournal->setSortingEnabled(true);
}
