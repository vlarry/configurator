#include "dialogjournalreadresult.h"
#include "ui_dialogjournalreadresult.h"
//--------------------------------------------------------------------------------------
DialogJournalReadResult::DialogJournalReadResult(const QString &title, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogJournalReadResult)
{
    ui->setupUi(this);

    ui->labelMsgTotalValue->clear();
    ui->labelMsgReadDoneValue->clear();
    ui->labelTimeElapsedValue->clear();

    connect(ui->pushButton, &QPushButton::clicked, this, &DialogJournalReadResult::accept);

    setWindowTitle(title);
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
}
//-------------------------------------------------
DialogJournalReadResult::~DialogJournalReadResult()
{
    delete ui;
}
//----------------------------------------------------------------------
void DialogJournalReadResult::setValues(int total, int read, float time)
{
    ui->labelMsgTotalValue->setText(QLocale::system().toString(total));
    ui->labelMsgReadDoneValue->setText(QLocale::system().toString(read));
    ui->labelTimeElapsedValue->setText(QString("%1 сек").arg(QLocale::system().toString(time, 'f', 1)));
}
