#include "cheaderjournal.h"
#include "ui_headerjournal.h"
//----------------------------------------------
CHeaderJournal::CHeaderJournal(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CHeaderJournal)
{
    ui->setupUi(this);
}
//-------------------------------
CHeaderJournal::~CHeaderJournal()
{
    delete ui;
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
    float time = value/1000;

    ui->lineEditElapsedTime->setText(QString::number(time, 'f', 2));
}
/*!
 * \brief CHeaderJournal::setTextTableCountMessages
 * \param value Количество записей в таблице
 */
void CHeaderJournal::setTextTableCountMessages(int value)
{
    ui->lineEditTableMessages->setText(QString::number(value));
}
