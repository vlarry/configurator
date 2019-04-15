#include "journalprogress.h"
#include "ui_journalprogress.h"
//---------------------------------------------------------------------------------
JournalProgress::JournalProgress(int total, const QString &title, QWidget *parent):
    QWidget(parent),
    ui(new Ui::JournalProgress),
    m_total(total)
{
    ui->setupUi(this);
    setWindowTitle(title);

    ui->progressBarReadDone->setRange(0, 100);
    ui->progressBarReadDone->setValue(0);
    ui->labelMsgReadCurrent->clear();
    ui->labelMsgReadDone->clear();

    connect(ui->pushButtonMsgReadCancel, &QPushButton::clicked, this, &JournalProgress::readCancel);
    setWindowFlag(Qt::Window);
}
//---------------------------------
JournalProgress::~JournalProgress()
{
    delete ui;
}
//-----------------------------------------------
void JournalProgress::setProgressValue(int value)
{
    int percent = value*100/m_total;
    ui->progressBarReadDone->setValue(percent);
    ui->labelMsgReadCurrent->setText(tr("Вычитано %1 из %2 сообщений").arg(value).arg(m_total));
    ui->labelMsgReadDone->setText(tr("Выполнено: %2%").arg(percent));
}
//--------------------------------
void JournalProgress::readCancel()
{
    emit cancel(false);
}
