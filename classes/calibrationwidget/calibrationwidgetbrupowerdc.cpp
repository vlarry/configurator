#include "calibrationwidgetbrupowerdc.h"
#include "ui_calibrationwidgetbrupowerdc.h"
//--------------------------------------------------------------------------
CCalibrationWidgetBRUPowerDC::CCalibrationWidgetBRUPowerDC(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetBRUPowerDC),
    m_calibration_type(CALIBRATION_NONE),
    m_calibration_min({ 0.0f, 0.0f, 0.0f, 0.0f, calibration_t() }),
    m_calibration_max({ 0.0f, 0.0f, 0.0f, 0.0f, calibration_t() })
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

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::calibrationParameterStart);
    connect(ui->pushButtonCalibration, &QPushButton::toggled, this, &CCalibrationWidgetBRUPowerDC::stateButton);
    connect(this, &CCalibrationWidgetBRUPowerDC::calibrationEnd, this, &CCalibrationWidgetBRUPowerDC::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::calibrationWriteProcess);
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
    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetBRUPowerDC::saveCalibrationToFlash);
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
//------------------------------------------
void CCalibrationWidgetBRUPowerDC::display()
{
    qInfo() << tr("Калибровка БРУ по напряжению DC:");

    calibration_t data_min = m_calibration_min.data;
    calibration_t data_max = m_calibration_max.data;

    if(!data_min.shiftUa.isEmpty() && !data_max.shiftUa.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftValue;
        float Xsrcmax = m_calibration_max.shiftValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftUa).x();
        Xmeasmax = standardDeviation(data_max.shiftUa).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftUa(K);
            qDebug() << QString("Новый коэффициент сдвига KUADC рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KUADC: Знаменатель равен нулю");
    }

    if(!data_min.shiftUb.isEmpty() && !data_max.shiftUb.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftValue;
        float Xsrcmax = m_calibration_max.shiftValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftUb).x();
        Xmeasmax = standardDeviation(data_max.shiftUb).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftUb(K);
            qDebug() << QString("Новый коэффициент сдвига KUBDC рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KUBDC: Знаменатель равен нулю");
    }

    if(!data_min.shiftUc.isEmpty() && !data_max.shiftUc.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftValue;
        float Xsrcmax = m_calibration_max.shiftValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftUc).x();
        Xmeasmax = standardDeviation(data_max.shiftUc).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftUc(K);
            qDebug() << QString("Новый коэффициент сдвига KUCDC рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KUCDC: Знаменатель равен нулю");
    }

    if(!data_min.shiftMultiplier.isEmpty() && !data_max.shiftMultiplier.isEmpty())
    {
        float Xsrcmin = m_calibration_min.shiftMultyplierValue;
        float Xsrcmax = m_calibration_max.shiftMultyplierValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.shiftMultiplier).x();
        Xmeasmax = standardDeviation(data_max.shiftMultiplier).x();

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftUMultiplier(K);
            qDebug() << QString("Новый коэффициент сдвига KUMDC рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KUMDC: Знаменатель равен нулю");
    }

    if(!data_min.inclineUa.isEmpty() && !data_max.inclineUa.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineValue;
        float Xsrcmax = m_calibration_max.inclineValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineUa).x();
        Xmeasmax = standardDeviation(data_max.inclineUa).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUa(A);
            qDebug() << QString("Новый коэффициент наклона AUADC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KUADC: Знаменатель равен нулю");
    }

    if(!data_min.inclineUb.isEmpty() && !data_max.inclineUb.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineValue;
        float Xsrcmax = m_calibration_max.inclineValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineUb).x();
        Xmeasmax = standardDeviation(data_max.inclineUb).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUb(A);
            qDebug() << QString("Новый коэффициент наклона AUBDC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента AUBDC: Знаменатель равен нулю");
    }

    if(!data_min.inclineUc.isEmpty() && !data_max.inclineUc.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineValue;
        float Xsrcmax = m_calibration_max.inclineValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineUc).x();
        Xmeasmax = standardDeviation(data_max.inclineUc).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUc(A);
            qDebug() << QString("Новый коэффициент наклона AUCDC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента AUCDC: Знаменатель равен нулю");
    }

    if(!data_min.inclineMultiplier.isEmpty() && !data_max.inclineMultiplier.isEmpty())
    {
        float Xsrcmin = m_calibration_min.inclineMultyplierValue;
        float Xsrcmax = m_calibration_max.inclineMultyplierValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.inclineMultiplier).x();
        Xmeasmax = standardDeviation(data_max.inclineMultiplier).x();

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        float numerator = ps - rq; // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUMultiplier(A);
            qDebug() << QString("Новый коэффициент наклона AUMDC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента AUMDC: Знаменатель равен нулю");
    }

    m_calibration_type = CALIBRATION_NONE;
    m_calibration_min = { 0.0f, 0.0f, 0.0f, 0.0f, calibration_t() };
    m_calibration_max = { 0.0f, 0.0f, 0.0f, 0.0f, calibration_t() };
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
//---------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::saveCalibrationToFlash()
{
    int answer = QMessageBox::question(this, tr("Запись калибровок БРУ по напряжению DC в устройство"),
                                             tr("Вы действительно хотите сохранить "
                                                "\nновые калибровочные коэффициенты "
                                                "\nв память устройства?"));
    if(answer == QMessageBox::Yes)
    {
        qInfo() << tr("Сохранение калибровочных коэффициентов БРУ по напряжению DC пользователем во флеш.");
        emit saveToFlash();
    }
    else
        qInfo() << tr("Отказ от сохранения калибровочных коэффициентов БРУ по напряжению DC во флеш.");
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
//------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::calibrationParameterStart()
{
    if(!ui->checkBoxUAShift->isChecked() &&
       !ui->checkBoxUBShift->isChecked() &&
       !ui->checkBoxUCShift->isChecked() &&
       !ui->checkBoxUAIncline->isChecked() &&
       !ui->checkBoxUBIncline->isChecked() &&
       !ui->checkBoxUCIncline->isChecked() &&
       !ui->checkBoxUMultiplierShift->isChecked() &&
       !ui->checkBoxUMultiplierIncline->isChecked())
    {
        QMessageBox::warning(this, tr("Калибровка БРУ по напряжению DC"), tr("Нет выбранных каналов для калибровки"));
        return;
    }

    if(m_calibration_type == CALIBRATION_MIN &&
      (((ui->checkBoxUAShift->isChecked() || ui->checkBoxUBShift->isChecked() || ui->checkBoxUCShift->isChecked()) &&
       standardPhaseShift() <= m_calibration_min.shiftValue) ||
       ((ui->checkBoxUAIncline->isChecked() || ui->checkBoxUBIncline->isChecked() || ui->checkBoxUCIncline->isChecked()) &&
       standardPhaseIncline() <= m_calibration_min.inclineValue) ||
       (ui->checkBoxUMultiplierShift->isChecked() && standardPhaseMultiplierShift() <= m_calibration_min.shiftMultyplierValue) ||
       (ui->checkBoxUMultiplierIncline->isChecked() && standardPhaseMultiplierIncline() <= m_calibration_min.inclineMultyplierValue)))
    {
        m_calibration_type = CALIBRATION_NONE;
        m_calibration_min = { 0.0f, 0.0f, 0.0f, 0.0f,calibration_t() };
        m_calibration_max = { 0.0f, 0.0f, 0.0f, 0.0f,calibration_t() };

        QMessageBox::warning(this, tr("Калибровка БРУ по напряжению"), tr("Эталонное значение для максимума меньше или равно значения минимума"));
        emit calibrationEnd();

        return;
    }

    int answer = QMessageBox::information(this, tr("Калибровка БРУ по напряжению DC"), tr("Сейчас будет произведена калибровка %1").
                                          arg((m_calibration_type == CALIBRATION_NONE)?tr("минимума"):tr("максимума")), QMessageBox::Ok | QMessageBox::Cancel);

    if(answer == QMessageBox::Cancel)
    {
        m_calibration_type = CALIBRATION_NONE;
        return;
    }

    QVector<CModBusDataUnit> unit_list;
    int param_count = 0;

    if(ui->checkBoxUAShift->isChecked())
    {
        if(measureShiftUa() >= 20.0f)
        {
            unit_list << calculateValue(POWER_SHIFT_UA);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения сдвига Ua (Ua < 20В)"));
    }
    if(ui->checkBoxUBShift->isChecked())
    {
        if(measureShiftUb() >= 20.0f)
        {
            unit_list << calculateValue(POWER_SHIFT_UB);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения сдвига Ub (Ub < 20В)"));
    }
    if(ui->checkBoxUCShift->isChecked())
    {
        if(measureShiftUc() >= 20.0f)
        {
            unit_list << calculateValue(POWER_SHIFT_UC);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения сдвига Uc (Uc < 20В)"));
    }
    if(ui->checkBoxUAIncline->isChecked())
    {
        if(measureInclineUa() >= 20.0f)
        {
            unit_list << calculateValue(POWER_INCLINE_UA);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения наклона Ua (Uab < 20В)"));
    }
    if(ui->checkBoxUBIncline->isChecked())
    {
        if(measureInclineUb() >= 20.0f)
        {
            unit_list << calculateValue(POWER_INCLINE_UB);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения наклона Ub (Ub < 20В)"));
    }
    if(ui->checkBoxUCIncline->isChecked())
    {
        if(measureInclineUc() >= 20.0f)
        {
            unit_list << calculateValue(POWER_INCLINE_UC);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения наклона Uc (Uc < 20В)"));
    }
    if(ui->checkBoxUMultiplierShift->isChecked())
    {
        if(measureShiftUMultiplier() >= 20.0f)
        {
            unit_list << calculateValue(POWER_SHIFT_MULTIPLIER);
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения сдвига умножителя (UU < 20В)"));
    }
    if(ui->checkBoxUMultiplierIncline->isChecked())
    {
        if(measureInclineUMultiplier() >= 20.0f)
        {
            unit_list << calculateValue(POWER_INCLINE_MULTIPLIER);;
            param_count++;
        }
        else
            showMessageError(tr("Нельзя произвести калибровку напряжения наклона умножителя (UU < 20В)"));
    }

    if(unit_list.isEmpty())
        return;

    if(m_calibration_type == CALIBRATION_NONE)
    {
        m_calibration_type = CALIBRATION_MIN;
        m_calibration_min.shiftValue = standardPhaseShift();
        m_calibration_min.inclineValue = standardPhaseIncline();
        m_calibration_min.shiftMultyplierValue = standardPhaseMultiplierShift();
        m_calibration_min.inclineMultyplierValue = standardPhaseMultiplierIncline();

        emit calibrationFactorAllStart();
    }
    else if(m_calibration_type == CALIBRATION_MIN)
    {
        m_calibration_type = CALIBRATION_MAX;
        m_calibration_max.shiftValue = standardPhaseShift();
        m_calibration_max.inclineValue = standardPhaseIncline();
        m_calibration_max.shiftMultyplierValue = standardPhaseMultiplierShift();
        m_calibration_max.inclineMultyplierValue = standardPhaseMultiplierIncline();
    }

    emit calibrationStart(unit_list, param_count);
}
//---------------------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::calibrationDataProcess(QVector<CModBusDataUnit> &data)
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

        if(channel == POWER_SHIFT_UA)
            calibration_data.shiftUa << value.f;
        else if(channel == POWER_SHIFT_UB)
            calibration_data.shiftUb << value.f;
        else if(channel == POWER_SHIFT_UC)
            calibration_data.shiftUc << value.f;
        else if(channel == POWER_INCLINE_UA)
            calibration_data.inclineUa << value.f;
        else if(channel == POWER_INCLINE_UB)
            calibration_data.inclineUb << value.f;
        else if(channel == POWER_INCLINE_UC)
            calibration_data.inclineUc << value.f;
        else if(channel == POWER_SHIFT_MULTIPLIER)
            calibration_data.shiftMultiplier << value.f;
        else if(channel == POWER_INCLINE_MULTIPLIER)
            calibration_data.inclineMultiplier << value.f;
    }

    if(m_calibration_type == CALIBRATION_MIN)
    {
        m_calibration_min.data = calibration_data;
        QMessageBox::information(this, tr("Калибровка БРУ по напряжению DC"), tr("Калибровка минимума окончена.\n"
                                                                                 "Произведите калибровку максимума."));
    }
    else if(m_calibration_type == CALIBRATION_MAX)
    {
        m_calibration_max.data = calibration_data;
        QMessageBox::information(this, tr("Калибровка БРУ по напряжению DC"), tr("Калибровка максимума окончена."));
        display();
    }

    emit calibrationEnd();
}
//----------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::calibrationWriteProcess()
{
    float shiftUa    = 0;
    float shiftUb    = 0;
    float shiftUc    = 0;
    float inclineUa   = 0;
    float inclineUb   = 0;
    float inclineUc   = 0;
    float shiftMultiplier = 0;
    float inclineMultiplier  = 0;

    if(ui->checkBoxUAShift->isChecked())
        shiftUa = valueShiftUa();
    if(ui->checkBoxUBShift->isChecked())
        shiftUb = valueShiftUb();
    if(ui->checkBoxUCShift->isChecked())
        shiftUc = valueShiftUc();
    if(ui->checkBoxUAIncline->isChecked())
        inclineUa = valueInclineUa();
    if(ui->checkBoxUBIncline->isChecked())
        inclineUb = valueInclineUb();
    if(ui->checkBoxUCIncline->isChecked())
        inclineUc = valueInclineUc();
    if(ui->checkBoxUMultiplierShift->isChecked())
        shiftMultiplier = valueShiftUMultiplier();
    if(ui->checkBoxUMultiplierIncline->isChecked())
        inclineMultiplier = valueInclineUMultiplier();

    QString messageError = tr("Напряжение на входе не должно быть меньше 20В");

    if(shiftUa == 0.0f && shiftUb == 0.0f && shiftUc == 0.0f && inclineUa == 0.0f && inclineUb == 0.0f && inclineUc == 0.0f &&
       shiftMultiplier == 0.0f && inclineMultiplier == 0.0f)
    {
        showMessageError(messageError);
        return;
    };

    QString str;
    QString textValue;

    textValue += ((shiftUa != 0.0f)?QString("Ua сдвига = %1\n").arg(QLocale::system().toString(shiftUa, 'f', 6)):"");
    textValue += ((shiftUb != 0.0f)?QString("Ub сдвига = %1\n").arg(QLocale::system().toString(shiftUb, 'f', 6)):"");
    textValue += ((shiftUc != 0.0f)?QString("Uc сдвига = %1\n").arg(QLocale::system().toString(shiftUc, 'f', 6)):"");
    textValue += ((inclineUa != 0.0f)?QString("Ua наклона = %1\n").arg(QLocale::system().toString(inclineUa, 'f', 6)):"");
    textValue += ((inclineUb != 0.0f)?QString("Ub наклона = %1\n").arg(QLocale::system().toString(inclineUb, 'f', 6)):"");
    textValue += ((inclineUc != 0.0f)?QString("Uc наклона = %1\n").arg(QLocale::system().toString(inclineUc, 'f', 6)):"");
    textValue += ((shiftMultiplier != 0.0f)?QString("Uумн сдвига = %1\n").arg(QLocale::system().toString(shiftMultiplier, 'f', 6)):"");
    textValue += ((inclineMultiplier != 0.0f)?QString("Uумн наклона = %1\n").arg(QLocale::system().toString(inclineMultiplier, 'f', 6)):"");

    str = tr("Вы хотите сохранить новые калибровки?\n%1").arg(textValue);
    int res = QMessageBox::question(this, tr("Запись калибровок БРУ по напряжению DC"), str);

    qInfo() << tr("Запись новых калибровочных коэффициентов БРУ по напряжению DC:\n%1").arg(textValue);

    if(res == QMessageBox::No)
    {
        qInfo() << tr("Отказ пользователя от записи калибровочных коэффициетов БРУ по напряжению DC");
        return;
    }

    union
    {
        quint16 i[2];
        float   f;
    } value;

    value.f = shiftUa;
    CModBusDataUnit unit_ShiftUa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftUa.setProperty("KEY", "AUADC");

    value.f = shiftUb;
    CModBusDataUnit unit_ShiftUb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftUb.setProperty("KEY", "AUBDC");

    value.f = shiftUc;
    CModBusDataUnit unit_ShiftUc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftUc.setProperty("KEY", "AUCDC");

    value.f = inclineUa;
    CModBusDataUnit unit_InclineUa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineUa.setProperty("KEY", "KUADC");

    value.f = inclineUb;
    CModBusDataUnit unit_InclineUb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineUb.setProperty("KEY", "KUBDC");

    value.f = inclineUc;
    CModBusDataUnit unit_InclineUc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineUc.setProperty("KEY", "KUCDC");

    value.f = shiftMultiplier;
    CModBusDataUnit unit_ShiftMultiplier(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftMultiplier.setProperty("KEY", "AUMDC");

    value.f = inclineMultiplier;
    CModBusDataUnit unit_InclineMultiplier(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineMultiplier.setProperty("KEY", "KUMDC");

    QVector<CModBusDataUnit> units;

    if(shiftUa >= 20.0f)
        units << unit_ShiftUa;
    if(shiftUb >= 20.0f)
        units << unit_ShiftUb;
    if(shiftUc >= 20.0f)
        units << unit_ShiftUc;
    if(inclineUa >= 20.0f)
        units << unit_InclineUa;
    if(inclineUb >= 20.0f)
        units << unit_InclineUb;
    if(inclineUc >= 20.0f)
        units << unit_InclineUc;
    if(shiftMultiplier >= 20.0f)
        units << unit_ShiftMultiplier;
    if(inclineMultiplier >= 20.0f)
        units << unit_InclineMultiplier;

    if(units.isEmpty())
    {
        showMessageError("Список калибровочных значений пуст.\nНапряжение на входе должно быть больше или равно 20В.");
        return;
    }

    qInfo() << tr("Запись новых калибровочных коэффициентов БРУ по напряжению DC подтверждена");

    emit calibrationWriteStart(units);
}
//---------------------------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setCalibrartionFactorActual(const QString &key, float value)
{
    qDebug() << QString("Калибровочный коэффициент: %1, значение: %2").arg(key).arg(value);
    if(key == "AUADC")
        setFactorShiftUa(value);
    else if(key == "AUBDC")
        setFactorShiftUb(value);
    else if(key == "AUCDC")
        setFactorShiftUc(value);
    else if(key == "KUADC")
        setFactorInclineUa(value);
    else if(key == "KUBDC")
        setFactorInclineUb(value);
    else if(key == "KUCDC")
        setFactorInclineUc(value);
    else if(key == "AUMDC")
        setFactorShiftUMultiplier(value);
    else if(key == "KUMDC")
        setFactorInclineUMultiplier(value);
}
//-------------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setCalculateActualValue(CModBusDataUnit &unit)
{
    QString channel = unit.property("KEY").toString();

    union
    {
        quint16 v[2];
        float   f;
    } value;

    value.v[0] = unit[1];
    value.v[1] = unit[0];

    if(channel == "UA")
    {
        setMeasureShiftUa(value.f);
        setMeasureInclineUa(value.f);
    }
    else if(channel == "UB")
    {
        setMeasureShiftUb(value.f);
        setMeasureInclineUb(value.f);
    }
    else if(channel == "UC")
    {
        setMeasureShiftUc(value.f);
        setMeasureInclineUc(value.f);
    }
    else if(channel == "UMULTIPLIER")
    {
        setMeasureShiftUMultiplier(value.f);
        setMeasureInclineUMultiplier(value.f);
    }
}
//-------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::progressBarIncrement()
{
    int count = ui->progressBarDataSet->value();
    int step  = 100/ui->spinBoxSetDataCount->value();
    ui->progressBarDataSet->setValue(count + step);
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
