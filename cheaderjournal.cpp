#include "cheaderjournal.h"
#include "ui_headerjournal.h"
//----------------------------------------------
CHeaderJournal::CHeaderJournal(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CHeaderJournal)
{
    ui->setupUi(this);

    clear();

    ui->pushbtnJournalRead->setEnabled(false);

    connect(ui->pushbtnJournalRead, &QPushButton::toggled, this, &CHeaderJournal::clickedButtonRead);
    connect(ui->pushbtnJournalClear, &QPushButton::clicked, this, &CHeaderJournal::clickedButtonClear);
    connect(ui->pushbtnJournalRead, &QPushButton::toggled, this, &CHeaderJournal::stateButtonReadChanged);
    connect(this, &CHeaderJournal::stateButtonReadOff, this, &CHeaderJournal::stateButtonReadChanged);
}
//-------------------------------
CHeaderJournal::~CHeaderJournal()
{
    delete ui;
}
//--------------------------
void CHeaderJournal::clear()
{
    ui->lineEditDeviceMessages->setText("0");
    ui->lineEditElapsedTime->setText("0");
    ui->lineEditTableMessages->setText("0");
}
/*!
 * \brief CHeaderJournal::setTextDeviceCountMessages
 * \param value Количество прочитанных записей из устройства
 * \param total Количество записей в устройстве
 */
void CHeaderJournal::setTextDeviceCountMessages(int value, int total)
{
    ui->lineEditDeviceMessages->setText(QString::number(value) + "/" + QString::number(total));
}
/*!
 * \brief CHeaderJournal::setTextElapsedTime
 * \param value Затраченное время на операцию
 */
void CHeaderJournal::setTextElapsedTime(int value)
{
    float time = float(value/1000.0f);

    ui->lineEditElapsedTime->setText(QString::number(time, 'f', 2) + tr(" сек."));
}
/*!
 * \brief CHeaderJournal::setTextTableCountMessages
 * \param value Количество записей в таблице
 */
void CHeaderJournal::setTextTableCountMessages(int value)
{
    ui->lineEditTableMessages->setText(QString::number(value));
}
/*!
 * \brief CHeaderJournal::stateCheckbox
 * \return истина, если чекбокс установлен
 */
bool CHeaderJournal::stateCheckbox()
{
    return ui->checkboxJournalTableScroll->isChecked();
}
/*!
 * \brief CHeaderJournal::stateButtonReadChanged
 * \param checked Текущее состояние кнопки
 *
 * Слот для изменения напдписи на кнопке при изменении ее состояния (нажата/отжата)
 */
void CHeaderJournal::stateButtonReadChanged(bool checked)
{
    ui->pushbtnJournalRead->setChecked(checked);

    if(checked)
    {
        ui->pushbtnJournalRead->setText(tr("Прервать чтение"));
    }
    else
    {
        ui->pushbtnJournalRead->setText(tr("Прочитать журнал"));
    }
}
/*!
 * \brief CHeaderJournal::stateEnabledButtonReadChanged
 * \param checked Текущее состояния активности кнопки
 *
 * Слот для изменения состояния активности кнопки (отслеживает состояние COM-порта)
 */
void CHeaderJournal::stateEnabledButtonReadChanged(bool checked)
{
    ui->pushbtnJournalRead->setEnabled(checked);
}
