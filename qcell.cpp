#include "qcell.h"
#include "ui_qcell.h"
//------------------------------------------------------
QCell::QCell(const QString& cell_name, QWidget *parent):
    QWidget(parent),
    ui(new Ui::QCell)
{
    ui->setupUi(this);
    
    ui->lblCellName->setText(cell_name);
}
//-------------
QCell::~QCell()
{
    delete ui;
}
//------------------------------------------
void QCell::setCellName(const QString& name)
{
    ui->lblCellName->setText(name);
}
//-----------------------------------
void QCell::setCellValue(float value)
{
    ui->leCellValue->setText(QString::number(value, 'f', 4));
}
