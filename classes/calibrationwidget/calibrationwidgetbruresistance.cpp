#include "calibrationwidgetbruresistance.h"
#include "ui_calibrationwidgetbruresistance.h"
//--------------------------------------------------------------------------------
CCalibrationWidgetBRUResistance::CCalibrationWidgetBRUResistance(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetBRUResistance),
    m_calibration_type(CALIBRATION_NONE),
    m_calibration_min({ 0.0f, 0.0f, calibration_t() }),
    m_calibration_max({ 0.0f, 0.0f, calibration_t() })
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.000001, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPowerStandardPhaseShift->setValidator(validator);
    ui->lineEditPowerStandardPhaseIncline->setValidator(validator);
    ui->lineEditFactorRAShift->setValidator(validator);
    ui->lineEditFactorRBShift->setValidator(validator);
    ui->lineEditFactorRCShift->setValidator(validator);
    ui->lineEditFactorRAIncline->setValidator(validator);
    ui->lineEditFactorRBIncline->setValidator(validator);
    ui->lineEditFactorRCIncline->setValidator(validator);

    ui->progressBarDataSet->hide();

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::calibrationParameterStart);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::stateButton);
    connect(this, &CCalibrationWidgetBRUResistance::calibrationEnd, this, &CCalibrationWidgetBRUResistance::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::calibrationWriteProcess);
    connect(ui->lineEditPowerStandardPhaseShift, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUResistance::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardPhaseIncline, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUResistance::valueCurrentStandardChanged);
    connect(ui->checkBoxRAShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRBShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRCShift, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRAIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRBIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRCIncline, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::saveCalibrationToFlash);
//    connect(this, &CCalibrationWidgetBRUPowerDC::dataIncrement, this, &CCalibrationWidgetBRUPowerDC::progressBarIncrement);
}
//-----------------------------------------------------------------
CCalibrationWidgetBRUResistance::~CCalibrationWidgetBRUResistance()
{
    delete ui;
}
//-------------------------------------------------------------------------------------------------------------------
CModBusDataUnit CCalibrationWidgetBRUResistance::calculateValue(CCalibrationWidgetBRUResistance::ChannelType channel)
{
    CModBusDataUnit unit;

    switch(channel)
    {
        case RESISTANCE_SHIFT_RA:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 124, 2); // чтение D38->Ra
            unit.setProperty("CHANNEL", RESISTANCE_SHIFT_RA);
            unit.setProperty("KEY", "RA");
        break;

        case RESISTANCE_SHIFT_RB:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 126, 2); // чтение D39->Rb
            unit.setProperty("CHANNEL", RESISTANCE_SHIFT_RB);
            unit.setProperty("KEY", "RB");
        break;

        case RESISTANCE_SHIFT_RC:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 128, 2); // чтение D40->Rc
            unit.setProperty("CHANNEL", RESISTANCE_SHIFT_RC);
            unit.setProperty("KEY", "RC");
        break;

        case RESISTANCE_INCLINE_RA:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 124, 2); // чтение D38->Ra
            unit.setProperty("CHANNEL", RESISTANCE_INCLINE_RA);
            unit.setProperty("KEY", "RA");
        break;

        case RESISTANCE_INCLINE_RB:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 126, 2); // чтение D39->Rb
            unit.setProperty("CHANNEL", RESISTANCE_INCLINE_RB);
            unit.setProperty("KEY", "RB");
        break;

        case RESISTANCE_INCLINE_RC:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 128, 2); // чтение D40->Rc
            unit.setProperty("CHANNEL", RESISTANCE_INCLINE_RC);
            unit.setProperty("KEY", "RC");
        break;

        default: return CModBusDataUnit();
    }

    return unit;
}
//----------------------------------------------------------------------------
QVector<CModBusDataUnit> CCalibrationWidgetBRUResistance::calculateValueList()
{
    QVector<CModBusDataUnit> list;

    list << calculateValue(RESISTANCE_SHIFT_RA);
    list << calculateValue(RESISTANCE_SHIFT_RB);
    list << calculateValue(RESISTANCE_SHIFT_RC);
qDebug() << QString("Создание списка расчетных величин: размер списка = %1->").arg(list.count());
    return list;
}
//----------------------------------------------------
int CCalibrationWidgetBRUResistance::dataCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//-------------------------------------------------------
int CCalibrationWidgetBRUResistance::pauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//---------------------------------------------------------------
float CCalibrationWidgetBRUResistance::standardPhaseShift() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseShift->text());
}
//-----------------------------------------------------------------
float CCalibrationWidgetBRUResistance::standardPhaseIncline() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseIncline->text());
}
//--------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateShiftRa() const
{
    return ui->checkBoxRAShift->isChecked();
}
//--------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateShiftRb() const
{
    return ui->checkBoxRBShift->isChecked();
}
//--------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateShiftRc() const
{
    return ui->checkBoxRCShift->isChecked();
}
//----------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateInclineRa() const
{
    return ui->checkBoxRAIncline->isChecked();
}
//----------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateInclineRb() const
{
    return ui->checkBoxRBIncline->isChecked();
}
//----------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateInclineRc() const
{
    return ui->checkBoxRCIncline->isChecked();
}
//---------------------------------------------------------
float CCalibrationWidgetBRUResistance::valueShiftRa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorRAShift->text());
}
//---------------------------------------------------------
float CCalibrationWidgetBRUResistance::valueShiftRb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorRBShift->text());
}
//---------------------------------------------------------
float CCalibrationWidgetBRUResistance::valueShiftRc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorRCShift->text());
}
//-----------------------------------------------------------
float CCalibrationWidgetBRUResistance::valueInclineRa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorRAIncline->text());
}
//-----------------------------------------------------------
float CCalibrationWidgetBRUResistance::valueInclineRb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorRBIncline->text());
}
//-----------------------------------------------------------
float CCalibrationWidgetBRUResistance::valueInclineRc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorRCIncline->text());
}
//-----------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureShiftRa() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD38Shift->text());
}
//-----------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureShiftRb() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD39Shift->text());
}
//-----------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureShiftRc() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD40Shift->text());
}
//-------------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureInclineRa() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD38Incline->text());
}
//-------------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureInclineRb() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD39Incline->text());
}
//-------------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureInclineRc() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD40Incline->text());
}
//----------------------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateCalculateUpdate() const
{
    return ui->checkBoxCalculateValueUpdate->isChecked();
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setFactorShiftRa(float value)
{
    ui->lineEditFactorRAShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setFactorShiftRb(float value)
{
    ui->lineEditFactorRBShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setFactorShiftRc(float value)
{
    ui->lineEditFactorRCShift->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setFactorInclineRa(float value)
{
    ui->lineEditFactorRAIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setFactorInclineRb(float value)
{
    ui->lineEditFactorRBIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setFactorInclineRc(float value)
{
    ui->lineEditFactorRCIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureShiftRa(float average)
{
    ui->lineEditMeasuredD38Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureShiftRb(float average)
{
    ui->lineEditMeasuredD39Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureShiftRc(float average)
{
    ui->lineEditMeasuredD40Shift->setText(QLocale::system().toString(average, 'f', 6));
}
//----------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureInclineRa(float average)
{
    ui->lineEditMeasuredD38Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//----------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureInclineRb(float average)
{
    ui->lineEditMeasuredD39Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//----------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureInclineRc(float average)
{
    ui->lineEditMeasuredD40Incline->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationShiftRa(float value)
{
    ui->lineEditDeviationRAShift->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationShiftRb(float value)
{
    ui->lineEditDeviationRBShift->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationShiftRc(float value)
{
    ui->lineEditDeviationRCShift->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationInclineRa(float value)
{
    ui->lineEditDeviationRAIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationInclineRb(float value)
{
    ui->lineEditDeviationRBIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationInclineRc(float value)
{
    ui->lineEditDeviationRCIncline->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::showMessageError(const QString &message)
{
    QMessageBox::warning(this, tr("Калибровка БРУ по сопротивлению"), message);
}
//---------------------------------------------------------------------------------------------
float CCalibrationWidgetBRUResistance::newCalibrationFactor(float standard, float power_factor,
                                                            const CCalibrationWidgetBRUResistance::calibration_data_t &measure_list)
{
    float measure = 0;

    if(measure_list.count() > 1)
        standard *= measure_list.count();

    for(float value: measure_list)
        measure += value;

    return (standard/measure)*power_factor;
}
//-------------------------------------------------------------------------------------------------------------------------
QPointF CCalibrationWidgetBRUResistance::standardDeviation(const CCalibrationWidgetBRUResistance::calibration_data_t &data)
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
//---------------------------------------------
void CCalibrationWidgetBRUResistance::display()
{
    qInfo() << tr("Калибровка БРУ по сопротивлению:");

    calibration_t data_min = m_calibration_min.data;
    calibration_t data_max = m_calibration_max.data;

    if(!data_min.shiftRa.isEmpty() && !data_max.shiftRa.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftValue;
        float Xsrcmax = m_calibration_max.shiftValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftRa).x();
        Xmeasmax = standardDeviation(data_max.shiftRa).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftRa(K);
            qDebug() << QString("Новый коэффициент сдвига KRA рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRA: Знаменатель равен нулю");
    }

    if(!data_min.shiftRb.isEmpty() && !data_max.shiftRb.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftValue;
        float Xsrcmax = m_calibration_max.shiftValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftRb).x();
        Xmeasmax = standardDeviation(data_max.shiftRb).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftRa(K);
            qDebug() << QString("Новый коэффициент сдвига KRB рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRB: Знаменатель равен нулю");
    }

    if(!data_min.shiftRc.isEmpty() && !data_max.shiftRc.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftValue;
        float Xsrcmax = m_calibration_max.shiftValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftRc).x();
        Xmeasmax = standardDeviation(data_max.shiftRc).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftRa(K);
            qDebug() << QString("Новый коэффициент сдвига KRC рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRC: Знаменатель равен нулю");
    }

    if(!data_min.inclineRa.isEmpty() && !data_max.inclineRa.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineValue;
        float Xsrcmax = m_calibration_max.inclineValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineRa).x();
        Xmeasmax = standardDeviation(data_max.inclineRa).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorShiftRa(A);
            qDebug() << QString("Новый коэффициент наклона ARA рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRA: Знаменатель равен нулю");
    }

    if(!data_min.inclineRb.isEmpty() && !data_max.inclineRb.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineValue;
        float Xsrcmax = m_calibration_max.inclineValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineRb).x();
        Xmeasmax = standardDeviation(data_max.inclineRb).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorShiftRa(A);
            qDebug() << QString("Новый коэффициент наклона ARB рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRB: Знаменатель равен нулю");
    }

    if(!data_min.inclineRc.isEmpty() && !data_max.inclineRc.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineValue;
        float Xsrcmax = m_calibration_max.inclineValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineRc).x();
        Xmeasmax = standardDeviation(data_max.inclineRc).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorShiftRa(A);
            qDebug() << QString("Новый коэффициент наклона ARC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRC: Знаменатель равен нулю");
    }

    m_calibration_type = CALIBRATION_NONE;
    m_calibration_min = { 0.0f, 0.0f, calibration_t() };
    m_calibration_max = { 0.0f, 0.0f, calibration_t() };
}
//-----------------------------------------------------------
void CCalibrationWidgetBRUResistance::stateButton(bool state)
{
    ui->pushButtonCalibration->setEnabled(!state);
    ui->pushButtonApply->setEnabled(!state);
    ui->pushButtonSaveToFlash->setEnabled(!state);
    ui->pushButtonCalibration->setChecked(state);
    ui->progressBarDataSet->setVisible(state);

    if(state)
        ui->progressBarDataSet->setValue(0);
}
//------------------------------------------------------------
void CCalibrationWidgetBRUResistance::saveCalibrationToFlash()
{
    int answer = QMessageBox::question(this, tr("Запись калибровок БРУ по сопротивлению в устройство"),
                                             tr("Вы действительно хотите сохранить "
                                                "\nновые калибровочные коэффициенты "
                                                "\nв память устройства?"));
    if(answer == QMessageBox::Yes)
    {
        qInfo() << tr("Сохранение калибровочных коэффициентов БРУ по сопротивлению пользователем во флеш.");
        emit saveToFlash();
    }
    else
        qInfo() << tr("Отказ от сохранения калибровочных коэффициентов БРУ по сопротивлению во флеш.");
}
//-------------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::valueCurrentStandardChanged(const QString&)
{
    stateChoiceChannelChanged(false); // аргумент не имеет значения, т.к. не используется
}
//-------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::stateChoiceChannelChanged(bool)
{
    float phaseShift = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseShift->text());
    float phaseIncline = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseIncline->text());

    if((ui->checkBoxRAShift->isChecked() ||
        ui->checkBoxRBShift->isChecked() ||
        ui->checkBoxRCShift->isChecked()) && phaseShift > 0)
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }
    else if((ui->checkBoxRAIncline->isChecked() ||
             ui->checkBoxRBIncline->isChecked() ||
             ui->checkBoxRCIncline->isChecked()) && phaseIncline > 0)
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
}
//---------------------------------------------------------------
void CCalibrationWidgetBRUResistance::calibrationParameterStart()
{
    if(!ui->checkBoxRAShift->isChecked() &&
       !ui->checkBoxRBShift->isChecked() &&
       !ui->checkBoxRCShift->isChecked() &&
       !ui->checkBoxRAIncline->isChecked() &&
       !ui->checkBoxRBIncline->isChecked() &&
       !ui->checkBoxRCIncline->isChecked())
    {
        QMessageBox::warning(this, tr("Калибровка БРУ по сопротивлению"), tr("Нет выбранных каналов для калибровки"));
        return;
    }

    int answer = QMessageBox::information(this, tr("Калибровка БРУ по сопротивлению"), tr("Сейчас будет произведена калибровка %1").
                                          arg((m_calibration_type == CALIBRATION_NONE)?tr("минимума"):tr("максимума")), QMessageBox::Ok | QMessageBox::Cancel);

    if(answer == QMessageBox::Cancel)
    {
        m_calibration_type = CALIBRATION_NONE;
        return;
    }

    QVector<CModBusDataUnit> unit_list;
    int param_count = 0;

    if(ui->checkBoxRAShift->isChecked())
    {
        unit_list << calculateValue(RESISTANCE_SHIFT_RA);
        param_count++;
    }
    if(ui->checkBoxRBShift->isChecked())
    {
        unit_list << calculateValue(RESISTANCE_SHIFT_RB);
        param_count++;
    }
    if(ui->checkBoxRCShift->isChecked())
    {
        unit_list << calculateValue(RESISTANCE_SHIFT_RC);
        param_count++;
    }
    if(ui->checkBoxRAIncline->isChecked())
    {
        unit_list << calculateValue(RESISTANCE_INCLINE_RA);
        param_count++;
    }
    if(ui->checkBoxRBIncline->isChecked())
    {
        unit_list << calculateValue(RESISTANCE_INCLINE_RB);
        param_count++;
    }
    if(ui->checkBoxRCIncline->isChecked())
    {
        unit_list << calculateValue(RESISTANCE_INCLINE_RC);
        param_count++;
    }

    if(unit_list.isEmpty())
        return;

    if(m_calibration_type == CALIBRATION_NONE)
    {
        m_calibration_type = CALIBRATION_MIN;
        m_calibration_min.shiftValue = standardPhaseShift();
        m_calibration_min.inclineValue = standardPhaseIncline();

        emit calibrationFactorAllStart();
    }
    else if(m_calibration_type == CALIBRATION_MIN)
    {
        m_calibration_type = CALIBRATION_MAX;
        m_calibration_max.shiftValue = standardPhaseShift();
        m_calibration_max.inclineValue = standardPhaseIncline();
    }

    emit calibrationStart(unit_list, param_count);
}
//------------------------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::calibrationDataProcess(QVector<CModBusDataUnit> &data)
{
    if(data.isEmpty())
        return;

    calibration_t calibration_data;

    union
    {
        quint16 v[2];
        float   f;
    } value;
qDebug() << QString("Разбор калибровочных данных: размер = %1").arg(data.count());
    for(const CModBusDataUnit &unit: data)
    {
        if(unit.count() != 2)
            continue;

        ChannelType channel = static_cast<ChannelType>(unit.property("CHANNEL").toInt());

        value.v[0] = unit[1];
        value.v[1] = unit[0];

        if(channel == RESISTANCE_SHIFT_RA)
            calibration_data.shiftRa << value.f;
        else if(channel == RESISTANCE_SHIFT_RB)
            calibration_data.shiftRb << value.f;
        else if(channel == RESISTANCE_SHIFT_RC)
            calibration_data.shiftRc << value.f;
        else if(channel == RESISTANCE_INCLINE_RA)
            calibration_data.inclineRa << value.f;
        else if(channel == RESISTANCE_INCLINE_RB)
            calibration_data.inclineRb << value.f;
        else if(channel == RESISTANCE_INCLINE_RC)
            calibration_data.inclineRc << value.f;
    }

    if(m_calibration_type == CALIBRATION_MIN)
        m_calibration_min.data = calibration_data;
    else if(m_calibration_type == CALIBRATION_MAX)
    {
        m_calibration_max.data = calibration_data;
        display();
    }

    emit calibrationEnd();
}
//-------------------------------------------------------------
void CCalibrationWidgetBRUResistance::calibrationWriteProcess()
{
    float shiftRa    = 0;
    float shiftRb    = 0;
    float shiftRc    = 0;
    float inclineRa   = 0;
    float inclineRb   = 0;
    float inclineRc   = 0;

    if(ui->checkBoxRAShift->isChecked())
        shiftRa = valueShiftRa();
    if(ui->checkBoxRBShift->isChecked())
        shiftRb = valueShiftRb();
    if(ui->checkBoxRCShift->isChecked())
        shiftRc = valueShiftRc();
    if(ui->checkBoxRAIncline->isChecked())
        inclineRa = valueInclineRa();
    if(ui->checkBoxRBIncline->isChecked())
        inclineRb = valueInclineRb();
    if(ui->checkBoxRCIncline->isChecked())
        inclineRc = valueInclineRc();

//    QString messageError = tr("Напряжение на входе не должно быть меньше 20В");

    if(shiftRa == 0.0f && shiftRb == 0.0f && shiftRc == 0.0f && inclineRa == 0.0f && inclineRb == 0.0f && inclineRc == 0.0f)
    {
//        showMessageError(messageError);
        return;
    };

    QString str;
    QString textValue;

    textValue += ((shiftRa != 0.0f)?QString("Ra сдвига = %1\n").arg(QLocale::system().toString(shiftRa, 'f', 6)):"");
    textValue += ((shiftRb != 0.0f)?QString("Rb сдвига = %1\n").arg(QLocale::system().toString(shiftRb, 'f', 6)):"");
    textValue += ((shiftRc != 0.0f)?QString("Rc сдвига = %1\n").arg(QLocale::system().toString(shiftRc, 'f', 6)):"");
    textValue += ((inclineRa != 0.0f)?QString("Ra наклона = %1\n").arg(QLocale::system().toString(inclineRa, 'f', 6)):"");
    textValue += ((inclineRb != 0.0f)?QString("Rb наклона = %1\n").arg(QLocale::system().toString(inclineRb, 'f', 6)):"");
    textValue += ((inclineRc != 0.0f)?QString("Rc наклона = %1\n").arg(QLocale::system().toString(inclineRc, 'f', 6)):"");

    str = tr("Вы хотите сохранить новые калибровки?\n%1").arg(textValue);
    int res = QMessageBox::question(this, tr("Запись калибровок БРУ по сопротивлению"), str);

    qInfo() << tr("Запись новых калибровочных коэффициентов БРУ по сопротивлению:\n%1").arg(textValue);

    if(res == QMessageBox::No)
    {
        qInfo() << tr("Отказ пользователя от записи калибровочных коэффициетов БРУ по сопротивлению");
        return;
    }

    union
    {
        quint16 i[2];
        float   f;
    } value;

    value.f = shiftRa;
    CModBusDataUnit unit_ShiftRa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftRa.setProperty("KEY", "KRA");

    value.f = shiftRb;
    CModBusDataUnit unit_ShiftRb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftRb.setProperty("KEY", "KRB");

    value.f = shiftRc;
    CModBusDataUnit unit_ShiftRc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftRc.setProperty("KEY", "KRC");

    value.f = inclineRa;
    CModBusDataUnit unit_InclineRa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineRa.setProperty("KEY", "ARA");

    value.f = inclineRb;
    CModBusDataUnit unit_InclineRb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineRb.setProperty("KEY", "ARB");

    value.f = inclineRc;
    CModBusDataUnit unit_InclineRc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineRc.setProperty("KEY", "ARC");

    QVector<CModBusDataUnit> units;

    if(shiftRa > 0.0f)
        units << unit_ShiftRa;
    if(shiftRb >= 0.0f)
        units << unit_ShiftRb;
    if(shiftRc >= 0.0f)
        units << unit_ShiftRc;
    if(inclineRa >= 0.0f)
        units << unit_InclineRa;
    if(inclineRb >= 0.0f)
        units << unit_InclineRb;
    if(inclineRc >= 0.0f)
        units << unit_InclineRc;

    qInfo() << tr("Запись новых калибровочных коэффициентов БРУ по сопротивлению подтверждена");

    emit calibrationWriteStart(units);
}
//------------------------------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setCalibrartionFactorActual(const QString &key, float value)
{
    qDebug() << QString("Калибровочный коэффициент: %1, значение: %2").arg(key).arg(value);
    if(key == "KRA")
        setFactorShiftRa(value);
    else if(key == "KRB")
        setFactorShiftRb(value);
    else if(key == "KRC")
        setFactorShiftRc(value);
    else if(key == "ARA")
        setFactorInclineRa(value);
    else if(key == "ARB")
        setFactorInclineRb(value);
    else if(key == "ARC")
        setFactorInclineRc(value);
}
//----------------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setCalculateActualValue(CModBusDataUnit &unit)
{
    QString channel = unit.property("KEY").toString();

    union
    {
        quint16 v[2];
        float   f;
    } value;

    value.v[0] = unit[1];
    value.v[1] = unit[0];

    if(channel == "RA")
    {
        setMeasureShiftRa(value.f);
        setMeasureInclineRa(value.f);
    }
    else if(channel == "RB")
    {
        setMeasureShiftRb(value.f);
        setMeasureInclineRb(value.f);
    }
    else if(channel == "RC")
    {
        setMeasureShiftRc(value.f);
        setMeasureInclineRc(value.f);
    }
}
//------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::paintEvent(QPaintEvent *event)
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
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

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
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
