#include "intervaldialog.h"
#include "ui_intervaldialog.h"
//----------------------------------------------------------------------
CIntervalDialog::CIntervalDialog(const QString& title, QWidget* parent):
    QDialog(parent),
    ui(new Ui::CIntervalDialog)
{
    ui->setupUi(this);

    setWindowTitle(title);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui->checkBoxRepeat, &QCheckBox::toggled, this, &CIntervalDialog::repeat);
}
//---------------------------------
CIntervalDialog::~CIntervalDialog()
{
    delete ui;
}
//-----------------------------------
int CIntervalDialog::interval() const
{
    return ui->spinBoxInterval->value();
}
//-------------------------------------------
bool CIntervalDialog::isRepeatChecked() const
{
    return ui->checkBoxRepeat->isChecked();
}
