#include "userdialog.h"
#include "ui_userdialog.h"
//------------------------------------------------------------------
CUserDialog::CUserDialog(const QStringList& users, QWidget* parent):
    QDialog(parent),
    ui(new Ui::CUserDialog)
{
    ui->setupUi(this);

    ui->lineEditUserPassword->setEchoMode(QLineEdit::Password);
    ui->comboBoxUserLogin->addItems(users);
    ui->comboBoxUserLogin->hide();
    ui->labelUserLogin->hide();
    ui->labelUserMessage->hide();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Авторизация"));
}
//-------------------------
CUserDialog::~CUserDialog()
{
    delete ui;
}
//-------------------------------------------
CUserDialog::user_t CUserDialog::user() const
{
    return user_t({ ui->comboBoxUserLogin->currentText(), ui->lineEditUserPassword->text() });
}
//-----------------------------------------------
void CUserDialog::setMessage(const QString& text)
{
    ui->labelUserMessage->show();
    ui->labelUserMessage->setText(text);
}
