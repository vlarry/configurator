#include "cstatusbar.h"
#include "ui_statusbar.h"
//--------------------------------------
CStatusBar::CStatusBar(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CStatusBar),
    m_timer(nullptr)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, &CStatusBar::timeoutStatusMessage);
}
//-----------------------
CStatusBar::~CStatusBar()
{
    delete ui;
}
//----------------------------------------------
void CStatusBar::connectStateChanged(bool state)
{
    if(state)
    {
        ui->lblDeviceStatus->setPixmap(QPixmap(":/images/resource/images/connect.png"));
    }
    else
    {
        ui->lblDeviceStatus->setPixmap(QPixmap(":/images/resource/images/disconnect.png"));
    }

    ui->lblDeviceStatus->setProperty("STATE", state);
}
//----------------------------------
void CStatusBar::clearSerialNumber()
{
    ui->lblDeviceSerialNumber->clear();
}
//-----------------------------------
void CStatusBar::clearStatusMessage()
{
    ui->lblStatusMessage->clear();
}
//------------------------------
bool CStatusBar::isState() const
{
    return ui->lblDeviceStatus->property("STATE").toBool();
}
//------------------------------------------
bool CStatusBar::serialNumberIsEmpty() const
{
    return ui->lblDeviceSerialNumber->text().isEmpty();
}
//------------------------------------
QString CStatusBar::serialNumberText()
{
    QString sn = ui->lblDeviceSerialNumber->text();
    sn.remove("S/n:");
    sn.remove(" ");

    if(sn.isEmpty() || !isState())
        return "";

    return sn;
}
//------------------------------------------------------
void CStatusBar::setSerialNumber(const QString& sn_text)
{
    ui->lblDeviceSerialNumber->setText(sn_text);
}
//--------------------------------------------------------------------
void CStatusBar::setStatusMessage(const QString& message, int timeout)
{
    ui->lblStatusMessage->setText(message);

    if(timeout != 0)
    {
        m_timer->start(timeout);
    }
}
//-------------------------------------
void CStatusBar::timeoutStatusMessage()
{
    m_timer->stop();
    ui->lblStatusMessage->clear();
}
