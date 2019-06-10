#include "calibrationwidgetpowerdc.h"
#include "ui_calibrationwidgetpowerdc.h"
//--------------------------------------------------------------------
CCalibrationWidgetPowerDC::CCalibrationWidgetPowerDC(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetPowerDC)
{
    ui->setupUi(this);
}
//-----------------------------------------------------
CCalibrationWidgetPowerDC::~CCalibrationWidgetPowerDC()
{
    delete ui;
}
//------------------------------------------------------------
void CCalibrationWidgetPowerDC::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}
