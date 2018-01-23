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
}
//-----------------------------------
void CStatusBar::clearStatusMessage()
{
    ui->lblStatusMessage->clear();
}
//------------------------------------------------------
void CStatusBar::setSerialNumber(const QString& sn_text)
{

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
