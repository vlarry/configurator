#include "calibrationwidget.h"
#include "ui_calibrationwidget.h"
//------------------------------------------------------
CCalibrationWidget::CCalibrationWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidget)
{
    ui->setupUi(this);
}
//---------------------------------------
CCalibrationWidget::~CCalibrationWidget()
{
    delete ui;
}
//-----------------------------------------------------
void CCalibrationWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    int hcenter        = ui->gridLayout->cellRect(1, 1).left() + ui->gridLayout->cellRect(1, 1).width()/2;
    int left           = ui->gridLayout->cellRect(1, 1).left();
    int right          = ui->gridLayout->cellRect(1, 1).right();
    int top_center     = ui->gridLayout->cellRect(1, 1).top() + ui->gridLayout->cellRect(1, 1).height()/2;
    int bottom_center1 = ui->gridLayout->cellRect(3, 1).top() + ui->gridLayout->cellRect(3, 1).height()/2;
    int vcenter        = ui->gridLayout->cellRect(2, 1).top() + ui->gridLayout->cellRect(2, 1).height()/2;
    int bottom_center2 = ui->gridLayout->cellRect(5, 1).top() + ui->gridLayout->cellRect(5, 1).height()/2;

    painter.drawLines(QVector<QLine>() << QLine(right, top_center, hcenter, top_center) <<
                                          QLine(hcenter, top_center, hcenter, bottom_center1) <<
                                          QLine(hcenter, bottom_center1, right, bottom_center1) <<
                                          QLine(left + 5, vcenter, right, vcenter) <<
                                          QLine(left + 5, bottom_center2, right, bottom_center2));
    painter.drawRect(ui->verticalLayoutCenter->geometry());
}
