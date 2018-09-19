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
    ui->lineEditUserCurrentPassword->setEchoMode(QLineEdit::Password);

    setPasswordMode(NORMAL_MODE);

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
//--------------------------------------------------
CUserDialog::user_t CUserDialog::currentUser() const
{
    return user_t({ ui->comboBoxUserLogin->currentText(), ui->lineEditUserCurrentPassword->text() });
}
//---------------------------------------------------------
void CUserDialog::setPasswordMode(CUserDialog::mode_t mode)
{
    if(mode == NORMAL_MODE)
    {
        ui->comboBoxUserLogin->hide();
        ui->labelUserLogin->hide();
        ui->labelUserCurrentPassword->hide();
        ui->lineEditUserCurrentPassword->hide();
    }
    else if(mode == EDIT_MODE)
    {
        setWindowTitle(tr("Смена пароля"));
        ui->labelUserCurrentPassword->show();
        ui->lineEditUserCurrentPassword->show();
        ui->labelUserPassword->setText(tr("Новый пароль"));
    }
}
