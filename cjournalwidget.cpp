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

    ui->tableWidgetJournal->installEventFilter(this);

    ui->textEditPropertyJournal->hide(); // по умолчанию окно свойств скрыто

    connect(ui->widgetJournalHeader, &CHeaderJournal::clickedButtonRead, this, &CJournalWidget::clickedButtonRead);
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
QTableWidget* CJournalWidget::table() const
{
    return ui->tableWidgetJournal;
}
//------------------------------------------------
QTextEdit* CJournalWidget::propertyJournal() const
{
    return ui->textEditPropertyJournal;
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
        printCrash(bytes);
    else if(journal_type == "EVENT")
        printEvent(bytes);
}
//--------------------------------------------------------------
void CJournalWidget::setTableHeaders(const QStringList& headers)
{
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
//-------------------------------------------------------------------------------
void CJournalWidget::setEventList(const QVector<CJournalWidget::event_t>& events)
{
    m_event_list = events;
}
//-------------------------------------------------
void CJournalWidget::setVisibleProperty(bool state)
{
    ui->textEditPropertyJournal->setVisible(state);
}
//-------------------------------------
void CJournalWidget::tableClear() const
{
    ui->tableWidgetJournal->clearContents();
    ui->tableWidgetJournal->setRowCount(0);
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

    quint8 month = ((data[0]&0x03) << 2) | ((data[1]&0xC0) >> 6);
    quint8 day   = ((data[1]&0x3E) >> 1);

    quint8  hour    = ((data[1]&0x01) << 4) | ((data[2]&0xF0) >> 4);
    quint8  minute  = ((data[2]&0x0F) << 2) | ((data[3]&0xC0) >> 6);
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
//----------------------------------------------------------
void CJournalWidget::printCrash(const QVector<quint8>& data) const
{
    if(data.count() != 256)
        return;

    quint16 id = ((data[1] << 8) | data[0]);

    QDateTime dt = unpackDateTime(QVector<quint8>() << data[2] << data[3] << data[4] << data[5] << data[6]);

    int row = ui->tableWidgetJournal->rowCount();

    ui->tableWidgetJournal->insertRow(row);
    ui->tableWidgetJournal->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
    ui->tableWidgetJournal->setItem(row, 1, new CTableWidgetItem(dt.date().toString("dd.MM.yyyy")));
    ui->tableWidgetJournal->setItem(row, 2, new QTableWidgetItem(dt.time().toString("HH:mm:ss.zzz")));
}
//----------------------------------------------------------
void CJournalWidget::printEvent(const QVector<quint8>& data) const
{
    for(int i = 0; i < data.count(); i += 16)
    {
        quint16 id = ((data[i + 1] << 8) | data[i]);

        QDateTime dt = unpackDateTime(QVector<quint8>() << data[i + 2] << data[i + 3] << data[i + 4] << data[i + 5] << data[i + 6]);

        quint8  type_event      = data[i + 7];
        quint8  category_event  = data[i + 8];
        quint16 parameter_event = data[i + 9] | (data[i + 10] << 8);

        QVector<event_t> etype = ((!m_event_list.isEmpty())?m_event_list:QVector<event_t>());

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
            QString eparameter_str = ((eparameter.isEmpty() || (eparameter.count() <= parameter_event))?
                                       tr("Неизвестный параметр"):eparameter[parameter_event].name);

            ui->tableWidgetJournal->setItem(row, 4, new QTableWidgetItem(ecategory_str +
                                                    QString(" (%1)").arg(QString::number(category_event))));
            ui->tableWidgetJournal->setItem(row, 5, new QTableWidgetItem(eparameter_str +
                                                    QString(" (%1)").arg(QString::number(parameter_event))));

            ui->tableWidgetJournal->item(row, 0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 1)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetJournal->item(row, 2)->setTextAlignment(Qt::AlignCenter);

            if(ui->widgetJournalHeader->stateCheckbox())
                ui->tableWidgetJournal->scrollToBottom();
        }
    }
}
//--------------------------------------------------------------
bool CJournalWidget::eventFilter(QObject* object, QEvent* event)
{
    if(object->metaObject()->className() == QString("QTableWidget") && event->type() == QEvent::KeyPress)
    {
        QTableWidget* table    = qobject_cast<QTableWidget*>(object);
        QKeyEvent*    keyEvent = (QKeyEvent*)event;

        if(keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_C)
        {
            if(table->rowCount() != 0)
            {
                QList<QTableWidgetSelectionRange> selected = table->selectedRanges();

                if(!selected.isEmpty())
                {
                    QString clipboard_str = "";

                    for(const QTableWidgetSelectionRange range: selected)
                    {
                        for(int i = range.topRow(); i <= range.bottomRow(); i++)
                        {
                            for(int j = 0; j < table->columnCount(); j++)
                            {
                                clipboard_str += table->item(i, j)->text() + "\t";
                            }

                            clipboard_str += "\n";
                        }
                    }

                    QApplication::clipboard()->setText(clipboard_str);
                }
                else
                {
                    QMessageBox::warning(this, tr("Копирование данных из таблицы"), tr("В таблице нет выделенных строк."));
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Копирование данных из таблицы"), tr("Таблица пуста."));
            }
        }
    }

    return true;
}
