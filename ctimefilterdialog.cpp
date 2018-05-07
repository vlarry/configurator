#include "ctimefilterdialog.h"
#include "ui_timefilterdialog.h"
//----------------------------------------------------
CTimeFilterDialog::CTimeFilterDialog(QWidget* parent):
    QDialog(parent),
    ui(new Ui::CTimeFilterDialog)
{
    ui->setupUi(this);

    setModal(true);
}
//-------------------------------------
CTimeFilterDialog::~CTimeFilterDialog()
{
    delete ui;
}
//-----------------------------------
QTime CTimeFilterDialog::time() const
{
    return ui->timeEditFilter->time();
}
