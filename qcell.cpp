#include "qcell.h"
#include "ui_qcell.h"
//----------------------------
QCell::QCell(QWidget *parent):
    QWidget(parent),
    ui(new Ui::QCell)
{
    ui->setupUi(this);
}
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
    ui->lblCellName->clear();
    ui->lblCellName->setText(fontMetrics().elidedText(name, Qt::ElideRight, ui->lblCellName->width() - 10));
}
//-----------------------------------
void QCell::setCellValue(float value)
{
    ui->leCellValue->setText(QLocale::system().toString(value, 'f', 4));
}
//---------------------------------
void QCell::setCellValue(int value)
{
    ui->leCellValue->setText(QString::number(value));
}
//----------------------------
QSize QCell::labelSize() const
{
    return ui->lblCellName->size();
}
//----------------------------
QSize QCell::fieldSize() const
{
    return ui->leCellValue->size();
}
