#include "coutputall.h"
#include "ui_outputall.h"
//------------------------------------------------------------
COutputAll::COutputAll(const QString& title, QWidget* parent):
    QWidget(parent),
    ui(new Ui::COutputAll),
    m_timeout_update(nullptr)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Window);
    setWindowTitle(title);

    m_timeout_update = new QTimer(this);

    connect(ui->pushButtonRead, &QPushButton::clicked, this, &COutputAll::buttonUpdate);
    connect(ui->pushButtonReset, &QPushButton::clicked, this, &COutputAll::outputStateReset);
    connect(ui->checkBoxUpdateData, &QCheckBox::clicked, this, &COutputAll::stateUpdateData);
    connect(ui->spinBoxUpdateData, SIGNAL(valueChanged(int)), this, SLOT(timeoutValueChanged(int)));
    connect(m_timeout_update, &QTimer::timeout, this, &COutputAll::timeoutUpdate);
}
//-----------------------
COutputAll::~COutputAll()
{
    delete ui;
}
//--------------------------------------------------
void COutputAll::createList(const QStringList& list)
{
    if(list.isEmpty())
        return;

    for(const QString& name: list)
    {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidgetOutput);
        COutputAllCell*  cell = new COutputAllCell(name);

        ui->listWidgetOutput->setItemWidget(item, cell);
    }
}
//------------------------------------------------------------
void COutputAll::setOutputStates(const QVector<quint16>& data)
{
    if(data.isEmpty() || ui->listWidgetOutput->count() == 0)
        return;

    for(int i = 0; i < data.count(); i++)
    {
        for(quint16 j = 0; j < sizeof(quint16)*8; j++)
        {
            int row = i*16 + j;

            if(row >= ui->listWidgetOutput->count())
                break;

            QListWidgetItem* item = ui->listWidgetOutput->item(row);

            if(!item)
                continue;

            COutputAllCell* cell = qobject_cast<COutputAllCell*>(ui->listWidgetOutput->itemWidget(item));

            if(cell)
            {
                bool state = (data[i]&(1 << j));
                cell->setState(state);
            }
        }
    }
}
//---------------------------------
void COutputAll::outputStateReset()
{
    for(quint16 i = 0; i < ui->listWidgetOutput->count(); i++)
    {
        QListWidgetItem* item = ui->listWidgetOutput->item(i);

        if(!item)
            continue;

        COutputAllCell* cell = qobject_cast<COutputAllCell*>(ui->listWidgetOutput->itemWidget(item));

        if(cell)
        {
            cell->setState(false);
        }
    }
}
//------------------------------------------
void COutputAll::stateUpdateData(bool state)
{
    if(state)
        m_timeout_update->start(ui->spinBoxUpdateData->value());
    else
        m_timeout_update->stop();
}
//------------------------------
void COutputAll::timeoutUpdate()
{
    emit buttonUpdate();
}
//-------------------------------------------------
void COutputAll::timeoutValueChanged(int new_value)
{
    if(ui->checkBoxUpdateData->isChecked())
    {
        m_timeout_update->stop();
        m_timeout_update->start(new_value);
    }
}
//---------------------------------------------
void COutputAll::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);

    emit closeWindow();
    m_timeout_update->stop();
}
//-------------------------------------------
void COutputAll::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if(ui->checkBoxUpdateData->isChecked())
        m_timeout_update->start(ui->spinBoxUpdateData->value());
}
