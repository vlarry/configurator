#include "calibrationwidgetpower.h"
#include "ui_calibrationwidgetpower.h"
//----------------------------------------------------------------
CCalibrationWidgetPower::CCalibrationWidgetPower(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetPower)
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.000001, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPowerStandardPhase->setValidator(validator);
    ui->lineEditPowerStandardPhase_2->setValidator(validator);
    ui->lineEditPowerStandardDC->setValidator(validator); // эталонное значение постоянной состовляющей для 3U0S, 3US и 3U0
    ui->lineEditFactorUA->setValidator(validator);
    ui->lineEditFactorUB->setValidator(validator);
    ui->lineEditFactorUC->setValidator(validator);
    ui->lineEditFactorUAB->setValidator(validator);
    ui->lineEditFactorUBC->setValidator(validator);
    ui->lineEditFactorUCA->setValidator(validator);
    ui->lineEditFactor3U0S->setValidator(validator);
    ui->lineEditFactor3US->setValidator(validator);
    ui->lineEditFactor3U0->setValidator(validator);

    ui->progressBarDataSet->hide();

//    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetPower::calibration);
//    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetPower::stateButton);
//    connect(this, &CCalibrationWidgetOfCurrent::calibrationEnd, this, &CCalibrationWidgetPower::stateButton);
//    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetPower::apply);
//    connect(ui->lineEditCurrentStandardPhase, &QLineEdit::textChanged, this, &CCalibrationWidgetPower::valueCurrentStandardChanged);
//    connect(ui->lineEditCurrentStandard3I0, &QLineEdit::textChanged, this, &CCalibrationWidgetPower::valueCurrentStandardChanged);
//    connect(ui->checkBoxIa, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->checkBoxIb, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->checkBoxIc, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->checkBox3I0, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetPower::saveCalibrationToFlash);
//    connect(this, &CCalibrationWidgetOfCurrent::dataIncrement, this, &CCalibrationWidgetPower::progressBarIncrement);
}
//-------------------------------------------------
CCalibrationWidgetPower::~CCalibrationWidgetPower()
{
    delete ui;
}
//-----------------------------------------------
int CCalibrationWidgetPower::dataSetCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//---------------------------------------------------
int CCalibrationWidgetPower::timePauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//----------------------------------------------------------------------------------
void CCalibrationWidgetPower::calibrationDataProcess(QVector<CModBusDataUnit> &data)
{
    if(data.isEmpty())
        return;
}
//----------------------------------------------------------
void CCalibrationWidgetPower::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Рисование отрезков перед фазами UA, UB и UC
    QRect r = ui->gridLayout_1->cellRect(1, 1);
    QPoint topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_1->cellRect(2, 1);
    QPoint centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    QPoint centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_1->cellRect(3, 1);
    QPoint bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    // Рисование отрезков перед фазами UAB, UBC и UCA
    r = ui->gridLayout_2->cellRect(0, 1);
    topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_2->cellRect(1, 1);
    centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_2->cellRect(2, 1);
    bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    // Рисование отрезков перед постоянной составляющей 3U0S, 3US и 3U0
    r = ui->gridLayout_3->cellRect(0, 1);
    topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_3->cellRect(1, 1);
    centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_3->cellRect(2, 1);
    bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
