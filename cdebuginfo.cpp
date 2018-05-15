#include "cdebuginfo.h"
#include "ui_debuginfo.h"
//------------------------------------------------------------
CDebugInfo::CDebugInfo(const QString& title, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CDebugInfo)
{
    ui->setupUi(this);

    setWindowTitle(title);
    setWindowFlag(Qt::Window);

    connect(ui->checkBoxSetMonitoring, &QCheckBox::clicked, this, &CDebugInfo::readInfoCtrl);
    connect(ui->spinBoxTimeoutMonitor, SIGNAL(valueChanged(int)), this, SLOT(timeoutChanged(int)));
}
//-----------------------
CDebugInfo::~CDebugInfo()
{
    delete ui;
}
//------------------------------------------------------------------------------
void CDebugInfo::setHedears(const QStringList& rows, const QStringList& columns)
{
    if(columns.isEmpty())
        return;

    ui->tableWidgetDebugValue->setRowCount(rows.count());
    ui->tableWidgetDebugValue->setColumnCount(columns.count());
    ui->tableWidgetDebugValue->setHorizontalHeaderLabels(columns);
    ui->tableWidgetDebugValue->setVerticalHeaderLabels(rows);
    ui->tableWidgetDebugValue->resizeColumnsToContents();
    ui->tableWidgetDebugValue->horizontalHeader()->setStretchLastSection(true);

    for(int i = 0; i < rows.count(); i++)
    {
        for(int j = 0; j < columns.count(); j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem("0");

            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetDebugValue->setItem(i, j, item);
        }
    }
}
//-----------------------------------------------------------------
void CDebugInfo::setData(int channel, const QVector<quint16>& data)
{
    if(channel < 0 || channel >= ui->tableWidgetDebugValue->rowCount() || data.isEmpty())
        return;

    QTableWidgetItem* itemState = ui->tableWidgetDebugValue->item(channel, 0);
    QTableWidgetItem* itemMin   = ui->tableWidgetDebugValue->item(channel, 1);
    QTableWidgetItem* itemMax   = ui->tableWidgetDebugValue->item(channel, 2);
    QTableWidgetItem* itemErr   = ui->tableWidgetDebugValue->item(channel, 3);
    QTableWidgetItem* itemSkp   = ui->tableWidgetDebugValue->item(channel, 4);

    if(itemState)
        itemState->setText(QString("%1").arg(quint8((data[0] >> 8)&0x01)));

    if(itemMin)
        itemMin->setText(QString("%1").arg(quint8(data[0]&0xFF)));

    if(itemMax)
        itemMax->setText(QString("%1").arg(quint8((data[1] >> 8)&0xFF)));

    if(itemErr)
        itemErr->setText(QString("%1").arg(quint8(data[1]&0xFF)));

    if(itemSkp)
        itemSkp->setText(QString("%1").arg(quint8((data[2] >> 8)&0xFF)));

    union
    {
        quint16 buf[2];
        float val;
    } value;

    for(int i = 5; i < data.count() - 1; i += 2)
    {
        value.buf[0] = data[i + 1];
        value.buf[1] = data[i];

        QTableWidgetItem* item = ui->tableWidgetDebugValue->item(channel, i);

        if(item)
            item->setText(QLocale::system().toString(value.val, 'f', 1));
    }
}
//---------------------------------------
void CDebugInfo::readInfoCtrl(bool state)
{
    if(state)
    {
        emit readInfo(ui->spinBoxTimeoutMonitor->value(), true);
    }
    else
    {
        emit readInfo(ui->spinBoxTimeoutMonitor->value());
    }
}
//------------------------------------------
void CDebugInfo::timeoutChanged(int newTime)
{
    emit readInfo(newTime, ui->checkBoxSetMonitoring->isChecked());
}
//---------------------------------------------
void CDebugInfo::closeEvent(QCloseEvent* event)
{
    ui->checkBoxSetMonitoring->setChecked(false);

    emit closeWindow();
    emit readInfo(0);

    QWidget::closeEvent(event);
}
