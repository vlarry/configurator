#include "coutputallcell.h"
#include "ui_outputallcell.h"
//-------------------------------------------------------------------
COutputAllCell::COutputAllCell(const QString& name, QWidget* parent):
    QWidget(parent),
    ui(new Ui::COutputAllCell)
{
    ui->setupUi(this);

    ui->labelName->setText(name);
}
//-------------------------------
COutputAllCell::~COutputAllCell()
{
    delete ui;
}
//----------------------------------
QString COutputAllCell::name() const
{
    return ui->labelName->text();
}
//--------------------------------
bool COutputAllCell::state() const
{
    return bool(ui->labelState->text().toInt());
}
//-----------------------------------------------
void COutputAllCell::setName(const QString& name)
{
    ui->labelName->setText(name);
}
//---------------------------------------
void COutputAllCell::setState(bool state)
{
    ui->labelState->setText(QString("%1").arg(state));
}
