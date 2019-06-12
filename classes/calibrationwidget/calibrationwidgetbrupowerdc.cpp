#include "calibrationwidgetbrupowerdc.h"
#include "ui_calibrationwidgetbrupowerdc.h"
//--------------------------------------------------------------------------
CCalibrationWidgetBRUPowerDC::CCalibrationWidgetBRUPowerDC(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetBRUPowerDC)
{
    ui->setupUi(this);
}
//-----------------------------------------------------------
CCalibrationWidgetBRUPowerDC::~CCalibrationWidgetBRUPowerDC()
{
    delete ui;
}
//---------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Рисование отрезков перед фазами UA, UB, UC и U умножителя напряжения смещения
    QRect r = ui->gridLayoutShift->cellRect(1, 1);
    QPoint topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayoutShift->cellRect(2, 1);
    QPoint centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    QPoint centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayoutShift->cellRect(3, 1);
    QPoint bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayoutShift->cellRect(5, 1);
    QPoint leftCenter = QPoint(r.left(), r.top() + r.height()/2);
    QPoint rightCenter = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);
    painter.drawLine(leftCenter, rightCenter);

    // Рисование отрезков перед фазами UA, UB, UC и U умножителя напряжения наклона
    r = ui->gridLayoutIncline->cellRect(1, 1);
    topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayoutIncline->cellRect(2, 1);
    centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayoutIncline->cellRect(3, 1);
    bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayoutIncline->cellRect(5, 1);
    leftCenter = QPoint(r.left(), r.top() + r.height()/2);
    rightCenter = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);
    painter.drawLine(leftCenter, rightCenter);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
