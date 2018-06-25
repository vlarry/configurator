#include "cstatusinfo.h"
#include "ui_statusinfo.h"
//----------------------------------------
CStatusInfo::CStatusInfo(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CStatusInfo),
    m_timeout_update(nullptr)
{
    ui->setupUi(this);

    ui->tableWidgetModule->setRowCount(6);
    ui->tableWidgetModule->setColumnCount(3);
    ui->tableWidgetModule->setHorizontalHeaderLabels(QStringList() << ("МДВВ-01") << ("МДВВ-02") << ("МИК-01"));
    ui->tableWidgetModule->setVerticalHeaderLabels(QStringList() << tr("U пит") << tr("Ток потребления")
                                                                 << tr("Температура процессора")
                                                                 << tr("Температура внутри блока")
                                                                 << tr("Внешний датчик температуры 1")
                                                                 << tr("Внешний датчик температуры 2"));

    ui->splitterCentral->setChildrenCollapsible(true);
    ui->splitterCentral->setStretchFactor(0, 0);
    ui->splitterCentral->setStretchFactor(1, 1);

    ui->tableWidgetModule->horizontalHeader()->setStretchLastSection(true);

    m_timeout_update = new QTimer(this);

    connect(ui->pushButtonStatusInfoUpdate, &QPushButton::clicked, this, buttonUpdate);
    connect(ui->checkBoxUpdateData, &QCheckBox::clicked, this, &CStatusInfo::stateUpdateData);
    connect(ui->spinBoxUpdateData, SIGNAL(valueChanged(int)), this, SLOT(timeoutValueChanged(int)));
    connect(m_timeout_update, &QTimer::timeout, this, &CStatusInfo::timeoutUpdate);
}
//-------------------------
CStatusInfo::~CStatusInfo()
{
    delete ui;
}
//--------------------------------------------------
void CStatusInfo::setStatusList(status_list_t& list)
{
    m_status_list = list;
}
//--------------------------------------------
void CStatusInfo::updateMcpInfo(quint32 value)
{
    ui->listWidgetStatus->clear();
    ui->tableWidgetModule->clearContents();

    for(quint32 i = 0; i < sizeof(value)*8; i++)
    {
        bool status = value & (1 << i);

        if(status)
        {
            QListWidgetItem* item = new QListWidgetItem(m_status_list[i]);
            ui->listWidgetStatus->addItem(item);
        }
    }
}
//--------------------------------------------------------------
void CStatusInfo::updateModuleInfo(const QVector<quint16> &info)
{
    if(info.count() != 24)
        return;

    union
    {
        quint16 buf[2];
        float _float;
    } value;

    value.buf[0] = info[1];
    value.buf[1] = info[0];

    float MDVV_01_Upower = value._float;

    value.buf[0] = info[3];
    value.buf[1] = info[2];

    float MDVV_01_Icur = value._float;

    value.buf[0] = info[5];
    value.buf[1] = info[4];

    float MDVV_01_Tcpu = value._float;

    value.buf[0] = info[7];
    value.buf[1] = info[6];

    float MDVV_01_Tblock = value._float;

    value.buf[0] = info[9];
    value.buf[1] = info[8];

    float MDVV_02_Tint1 = value._float;

    value.buf[0] = info[11];
    value.buf[1] = info[10];

    float MDVV_02_Tint2 = value._float;

    value.buf[0] = info[13];
    value.buf[1] = info[12];

    float MDVV_02_Tcpu = value._float;

    value.buf[0] = info[21];
    value.buf[1] = info[20];

    float MIK_01_Tcpu = value._float;

    QTableWidgetItem* itemMDVV_01_Upower = new QTableWidgetItem(QLocale::system().toString(MDVV_01_Upower, 'f', 2));
    QTableWidgetItem* itemMDVV_01_Icur   = new QTableWidgetItem(QLocale::system().toString(MDVV_01_Icur, 'f', 2));
    QTableWidgetItem* itemMDVV_01_Tcpu   = new QTableWidgetItem(QLocale::system().toString(MDVV_01_Tcpu, 'f', 2));
    QTableWidgetItem* itemMDVV_01_Tblock = new QTableWidgetItem(QLocale::system().toString(MDVV_01_Tblock, 'f', 2));
    QTableWidgetItem* itemMDVV_02_Tint1  = new QTableWidgetItem(QLocale::system().toString(MDVV_02_Tint1, 'f', 2));
    QTableWidgetItem* itemMDVV_02_Tint2  = new QTableWidgetItem(QLocale::system().toString(MDVV_02_Tint2, 'f', 2));
    QTableWidgetItem* itemMDVV_02_Tcpu   = new QTableWidgetItem(QLocale::system().toString(MDVV_02_Tcpu, 'f', 2));
    QTableWidgetItem* itemMIK_01_Tcpu    = new QTableWidgetItem(QLocale::system().toString(MIK_01_Tcpu, 'f', 2));

    itemMDVV_01_Upower->setTextAlignment(Qt::AlignCenter);
    itemMDVV_01_Icur->setTextAlignment(Qt::AlignCenter);
    itemMDVV_01_Tcpu->setTextAlignment(Qt::AlignCenter);
    itemMDVV_01_Tblock->setTextAlignment(Qt::AlignCenter);
    itemMDVV_02_Tint1->setTextAlignment(Qt::AlignCenter);
    itemMDVV_02_Tint2->setTextAlignment(Qt::AlignCenter);
    itemMDVV_02_Tcpu->setTextAlignment(Qt::AlignCenter);
    itemMIK_01_Tcpu->setTextAlignment(Qt::AlignCenter);

    ui->tableWidgetModule->setItem(0, 0, itemMDVV_01_Upower);
    ui->tableWidgetModule->setItem(1, 0, itemMDVV_01_Icur);
    ui->tableWidgetModule->setItem(2, 0, itemMDVV_01_Tcpu);
    ui->tableWidgetModule->setItem(3, 0, itemMDVV_01_Tblock);
    ui->tableWidgetModule->setItem(4, 1, itemMDVV_02_Tint1);
    ui->tableWidgetModule->setItem(5, 1, itemMDVV_02_Tint2);
    ui->tableWidgetModule->setItem(2, 1, itemMDVV_02_Tcpu);
    ui->tableWidgetModule->setItem(2, 2, itemMIK_01_Tcpu);
}
//-------------------------------------------
void CStatusInfo::stateUpdateData(bool state)
{
    if(state)
        m_timeout_update->start(ui->spinBoxUpdateData->value());
    else
        m_timeout_update->stop();
}
//-------------------------------
void CStatusInfo::timeoutUpdate()
{
    emit buttonUpdate();
}
//--------------------------------------------------
void CStatusInfo::timeoutValueChanged(int new_value)
{
    if(ui->checkBoxUpdateData->isChecked())
    {
        m_timeout_update->stop();
        m_timeout_update->start(new_value);
    }
}
//----------------------------------------------
void CStatusInfo::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
    m_timeout_update->stop();
    emit closeWindow();
}
//--------------------------------------------
void CStatusInfo::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    if(ui->checkBoxUpdateData->isChecked())
        m_timeout_update->start(ui->spinBoxUpdateData->value());
}
