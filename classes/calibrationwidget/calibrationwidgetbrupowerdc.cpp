#include "calibrationwidgetbrupowerdc.h"
#include "ui_calibrationwidgetbrupowerdc.h"
//--------------------------------------------------------------------------
CCalibrationWidgetBRUPowerDC::CCalibrationWidgetBRUPowerDC(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetBRUPowerDC)
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.000001, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPowerStandardPhaseShift->setValidator(validator);
    ui->lineEditPowerStandardPhaseIncline->setValidator(validator);
    ui->lineEditPowerStandardMultiplierShift->setValidator(validator);
    ui->lineEditPowerStandardMultiplierIncline->setValidator(validator);
    ui->lineEditFactorUAShift->setValidator(validator);
    ui->lineEditFactorUBShift->setValidator(validator);
    ui->lineEditFactorUCShift->setValidator(validator);
    ui->lineEditFactorUAIncline->setValidator(validator);
    ui->lineEditFactorUBIncline->setValidator(validator);
    ui->lineEditFactorUCIncline->setValidator(validator);
    ui->lineEditFactorUMultiplierShift->setValidator(validator);
    ui->lineEditFactorUMultiplierIncline->setValidator(validator);

    ui->progressBarDataSet->hide();

//    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::calibrationParameterStart);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::stateButton);
    connect(this, &CCalibrationWidgetBRUPowerDC::calibrationEnd, this, &CCalibrationWidgetBRUPowerDC::stateButton);
//    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::calibrationWriteProcess);
    connect(ui->lineEditPowerStandardPhaseShift, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardPhaseIncline, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardMultiplierShift, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardMultiplierIncline, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->checkBoxUAShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUBShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUCShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUAIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUBIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUCIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUMultiplierShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUMultiplierIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
//    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::saveCalibrationToFlash);
//    connect(this, &CCalibrationWidgetBRUPowerDC::dataIncrement, this, &CCalibrationWidgetBRUPowerDC::progressBarIncrement);
}
//-----------------------------------------------------------
CCalibrationWidgetBRUPowerDC::~CCalibrationWidgetBRUPowerDC()
{
    delete ui;
}
//-------------------------------------------------------------------------------------------------------------
CModBusDataUnit CCalibrationWidgetBRUPowerDC::calculateValue(CCalibrationWidgetBRUPowerDC::ChannelType channel)
{
    CModBusDataUnit unit;

    switch(channel)
    {
        case POWER_SHIFT_UA:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 118, 2); // чтение D35->Ua dc
            unit.setProperty("CHANNEL", POWER_SHIFT_UA);
            unit.setProperty("KEY", "UA");
        break;

        case POWER_SHIFT_UB:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 120, 2); // чтение D36->Ub dc
            unit.setProperty("CHANNEL", POWER_SHIFT_UB);
            unit.setProperty("KEY", "UB");
        break;

        case POWER_SHIFT_UC:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 122, 2); // чтение D37->Uc dc
            unit.setProperty("CHANNEL", POWER_SHIFT_UC);
            unit.setProperty("KEY", "UC");
        break;

        case POWER_SHIFT_MULTIPLIER:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 116, 2); // чтение D34->Uумн
            unit.setProperty("CHANNEL", POWER_SHIFT_MULTIPLIER);
            unit.setProperty("KEY", "UMULTIPLIER");
        break;

        default: return CModBusDataUnit();
    }

    return unit;
}
//-------------------------------------------------------------------------
QVector<CModBusDataUnit> CCalibrationWidgetBRUPowerDC::calculateValueList()
{
    QVector<CModBusDataUnit> list;

    list << calculateValue(POWER_SHIFT_UA);
    list << calculateValue(POWER_SHIFT_UB);
    list << calculateValue(POWER_SHIFT_UC);
    list << calculateValue(POWER_SHIFT_MULTIPLIER);

    return list;
}
//-------------------------------------------------
int CCalibrationWidgetBRUPowerDC::dataCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//----------------------------------------------------
int CCalibrationWidgetBRUPowerDC::pauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseShift() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseShift->text());
}
//--------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseIncline() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseIncline->text());
}
//----------------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseMultiplierShift() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardMultiplierShift->text());
}
//------------------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseMultiplierIncline() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardMultiplierIncline->text());
}
//-----------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateShiftUa() const
{
    return ui->checkBoxUAShift->isChecked();
}
//-----------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateShiftUb() const
{
    return ui->checkBoxUBShift->isChecked();
}
//-----------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateShiftUc() const
{
    return ui->checkBoxUCShift->isChecked();
}
//-------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateInclineUa() const
{
    return ui->checkBoxUAIncline->isChecked();
}
//-------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateInclineUb() const
{
    return ui->checkBoxUBIncline->isChecked();
}
//-------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateInclineUc() const
{
    return ui->checkBoxUCIncline->isChecked();
}
//--------------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateUMultiplierShift() const
{
    return ui->checkBoxUMultiplierShift->isChecked();
}
//----------------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateUMultiplierIncline() const
{
    return ui->checkBoxUMultiplierIncline->isChecked();
}
//------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueShiftUa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUAShift->text());
}
//------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueShiftUb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUBShift->text());
}
//------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueShiftUc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUCShift->text());
}
//--------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueInclineUa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUAIncline->text());
}
//--------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueInclineUb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUBIncline->text());
}
//--------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueInclineUc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUCIncline->text());
}
//---------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueShiftUMultiplier() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUMultiplierShift->text());
}
//-----------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::valueInclineUMultiplier() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUMultiplierIncline->text());
}
//--------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureShiftUa() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD35Shift->text());
}
//--------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureShiftUb() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD36Shift->text());
}
//--------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureShiftUc() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD37Shift->text());
}
//----------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureInclineUa() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD35Incline->text());
}
//----------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureInclineUb() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD36Incline->text());
}
//----------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureInclineUc() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD37Incline->text());
}
//-----------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureShiftUMultiplier() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD34Shift->text());
}
//-------------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureInclineUMultiplier() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD34Incline->text());
}
//-------------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateCalculateUpdate() const
{
    return ui->checkBoxCalculateValueUpdate->isChecked();
}
//--------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorShiftUa(float value)
{
    ui->lineEditFactorUAShift->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorShiftUb(float value)
{
    ui->lineEditFactorUBShift->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorShiftUc(float value)
{
    ui->lineEditFactorUCShift->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorInclineUa(float value)
{
    ui->lineEditFactorUAIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorInclineUb(float value)
{
    ui->lineEditFactorUBIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorInclineUc(float value)
{
    ui->lineEditFactorUCIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorShiftUMultiplier(float value)
{
    ui->lineEditFactorUMultiplierShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setFactorInclineUMultiplier(float value)
{
    ui->lineEditFactorUMultiplierIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureShiftUa(float average)
{
    ui->lineEditMeasuredD35Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureShiftUb(float average)
{
    ui->lineEditMeasuredD36Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureShiftUc(float average)
{
    ui->lineEditMeasuredD37Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureInclineUa(float average)
{
    ui->lineEditMeasuredD35Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureInclineUb(float average)
{
    ui->lineEditMeasuredD36Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureInclineUc(float average)
{
    ui->lineEditMeasuredD37Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureShiftUMultiplier(float average)
{
    ui->lineEditMeasuredD34Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//----------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureInclineUMultiplier(float average)
{
    ui->lineEditMeasuredD34Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationShiftUa(float value)
{
    ui->lineEditDeviationUAShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationShiftUb(float value)
{
    ui->lineEditDeviationUBShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationShiftUc(float value)
{
    ui->lineEditDeviationUCShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationInclineUa(float value)
{
    ui->lineEditDeviationUAIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationInclineUb(float value)
{
    ui->lineEditDeviationUBIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationInclineUc(float value)
{
    ui->lineEditDeviationUCIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationShiftUMultiplier(float value)
{
    ui->lineEditDeviationUMultiplierShift->setText(QLocale::system().toString(value, 'f', 6));
}
//---------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationInclineMultiplier(float value)
{
    ui->lineEditDeviationUMultiplierIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::showMessageError(const QString &message)
{
    QMessageBox::warning(this, tr("Калибровка БРУ по напряжению DC"), message);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::newCalibrationFactor(float standard, float power_factor, const CCalibrationWidgetBRUPowerDC::calibration_data_t &measure_list)
{
    float measure = 0;

    if(measure_list.count() > 1)
        standard *= measure_list.count();

    for(float value: measure_list)
        measure += value;

    return (standard/measure)*power_factor;
}
//-------------------------------------------------------------------------------------------------------------------
QPointF CCalibrationWidgetBRUPowerDC::standardDeviation(const CCalibrationWidgetBRUPowerDC::calibration_data_t &data)
{
    float average = 0;

    for(float value: data)
        average += value;

    average /= data.count();

    float deviation = 0;

    for(float value: data)
        deviation += (value - average)*(value - average);

    deviation = float(sqrt(double(double(deviation)/double(data.count()) - 1.0)));

    return QPointF(double(average), double(deviation));
}
//--------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::stateButton(bool state)
{
    ui->pushButtonCalibration->setEnabled(!state);
    ui->pushButtonApply->setEnabled(!state);
    ui->pushButtonSaveToFlash->setEnabled(!state);
    ui->pushButtonCalibration->setChecked(state);
    ui->progressBarDataSet->setVisible(state);

    if(state)
        ui->progressBarDataSet->setValue(0);
}
//----------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged(const QString&)
{
    stateChoiceChannelChanged(false); // аргумент не имеет значения, т.к. не используется
}
//----------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged(bool)
{
    float phaseShift = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseShift->text());
    float phaseIncline = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseIncline->text());
    float phaseMultiplierShift = QLocale::system().toFloat(ui->lineEditPowerStandardMultiplierShift->text());
    float phaseMultiplierIncline = QLocale::system().toFloat(ui->lineEditPowerStandardMultiplierIncline->text());

    if((ui->checkBoxUAShift->isChecked() ||
        ui->checkBoxUBShift->isChecked() ||
        ui->checkBoxUCShift->isChecked()) && phaseShift > 0)
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }
    else if((ui->checkBoxUAIncline->isChecked() ||
             ui->checkBoxUBIncline->isChecked() ||
             ui->checkBoxUCIncline->isChecked()) && phaseIncline > 0)
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }
    else if(ui->checkBoxUMultiplierShift->isChecked() && phaseMultiplierShift > 0)
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }
    else if(ui->checkBoxUMultiplierIncline->isChecked() && phaseMultiplierIncline > 0)
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
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
