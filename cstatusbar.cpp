#include "cstatusbar.h"
#include "ui_statusbar.h"
//--------------------------------------
CStatusBar::CStatusBar(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CStatusBar),
    m_timerStatusMessage(nullptr),
    m_timerProgressbar(nullptr)
{
    ui->setupUi(this);

    m_timerStatusMessage = new QTimer(this);
    m_timerProgressbar   = new QTimer(this);

    ui->lblTitleProgressbar->hide();
    ui->progressbarProcess->hide();
    ui->progressbarProcess->setRange(0, 100);
    ui->progressbarProcess->setValue(0);

    connect(m_timerStatusMessage, &QTimer::timeout, this, &CStatusBar::timeoutStatusMessage);
    connect(m_timerProgressbar, &QTimer::timeout, this, &CStatusBar::timeoutProgressbar);
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
        return "0-0-0-0-0";

    return sn;
}
//--------------------------------------------------------
void CStatusBar::setProgressbarTitle(const QString& title)
{
    if(title.isEmpty())
        return;

    ui->lblTitleProgressbar->show();
    ui->progressbarProcess->show();
    ui->lblTitleProgressbar->setText(title);
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
        m_timerStatusMessage->start(timeout);
    }
}
//-------------------------------------
void CStatusBar::timeoutStatusMessage()
{
    m_timerStatusMessage->stop();
    ui->lblStatusMessage->clear();
}
//-----------------------------------
void CStatusBar::timeoutProgressbar()
{
    int value = ui->progressbarProcess->value();

    if(value == 75 && m_timerProgressbar->property("MODE").toBool())
    {
        m_timerProgressbar->stop();
        m_timerProgressbar->start(500);
    }
    else if(value == 99 && m_timerProgressbar->property("MODE").toBool())
        return;
    else if(value == 100 && !m_timerProgressbar->property("MODE").toBool())
    {
        m_timerProgressbar->stop();
        ui->progressbarProcess->hide();
        ui->lblTitleProgressbar->hide();
    }

    ui->progressbarProcess->setValue(++value);
}
//---------------------------------
void CStatusBar::startProgressbar()
{
    if(m_timerProgressbar->isActive())
        m_timerProgressbar->stop();

    ui->progressbarProcess->setRange(0, 100);
    ui->progressbarProcess->setValue(0);
    ui->progressbarProcess->show();
    ui->lblTitleProgressbar->show();

    m_timerProgressbar->setProperty("MODE", true); // MODE = true - старт (нормальная работа таймера), false - завершение
    m_timerProgressbar->start(100);
}
//--------------------------------
void CStatusBar::stopProgressbar()
{
    m_timerProgressbar->stop();
    m_timerProgressbar->setProperty("MODE", false);

    int value = ui->progressbarProcess->value();

    value = (100 - value)/10;
    value = ((value == 0)?50:value*5);

    m_timerProgressbar->start(50);
}
