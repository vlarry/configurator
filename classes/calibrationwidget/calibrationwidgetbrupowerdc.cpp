#include "calibrationwidgetbrupowerdc.h"
#include "ui_calibrationwidgetbrupowerdc.h"
//--------------------------------------------------------------------------
CCalibrationWidgetBRUPowerDC::CCalibrationWidgetBRUPowerDC(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetBRUPowerDC),
    m_calibration_type(CALIBRATION_NONE),
    m_calibration_min({ 0.0f, 0.0f, calibration_t() }),
    m_calibration_max({ 0.0f, 0.0f, calibration_t() })
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.0f, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPowerStandardPhaseMin->setValidator(validator);
    ui->lineEditPowerStandardPhaseMax->setValidator(validator);
    ui->lineEditPowerStandardMultiplierMin->setValidator(validator);
    ui->lineEditPowerStandardMultiplierMax->setValidator(validator);

    ui->lineEditMeasuredD34->setValidator(validator);
    ui->lineEditMeasuredD35->setValidator(validator);
    ui->lineEditMeasuredD36->setValidator(validator);
    ui->lineEditMeasuredD37->setValidator(validator);

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

    connect(ui->lineEditPowerStandardPhaseMin, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardPhaseMax, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardMultiplierMin, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardMultiplierMax, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUPowerDC::valueCurrentStandardChanged);

    connect(ui->checkBoxUA, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUB, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUC, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
    connect(ui->checkBoxUMultiplier, &QCheckBox::clicked, this, &CCalibrationWidgetBRUPowerDC::stateChoiceChannelChanged);
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
        case POWER_UA:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 118, 2); // чтение D35->Ua dc
            unit.setProperty("CHANNEL", POWER_UA);
            unit.setProperty("KEY", "UA");
        break;

        case POWER_UB:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 120, 2); // чтение D36->Ub dc
            unit.setProperty("CHANNEL", POWER_UB);
            unit.setProperty("KEY", "UB");
        break;

        case POWER_UC:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 122, 2); // чтение D37->Uc dc
            unit.setProperty("CHANNEL", POWER_UC);
            unit.setProperty("KEY", "UC");
        break;

        case POWER_MULTIPLIER:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 116, 2); // чтение D34->Uумн
            unit.setProperty("CHANNEL", POWER_MULTIPLIER);
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

    list << calculateValue(POWER_UA);
    list << calculateValue(POWER_UB);
    list << calculateValue(POWER_UC);
    list << calculateValue(POWER_MULTIPLIER);

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
//----------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseMin() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseMin->text());
}
//----------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseMax() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseMax->text());
}
//--------------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseMultiplierMin() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardMultiplierMin->text());
}
//--------------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::standardPhaseMultiplierMax() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardMultiplierMax->text());
}
//------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateUa() const
{
    return ui->checkBoxUA->isChecked();
}
//------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateUb() const
{
    return ui->checkBoxUB->isChecked();
}
//------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateUc() const
{
    return ui->checkBoxUC->isChecked();
}
//---------------------------------------------------------
bool CCalibrationWidgetBRUPowerDC::stateUMultiplier() const
{
    return ui->checkBoxUMultiplier->isChecked();
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
//---------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureUa() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD35->text());
}
//---------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureUb() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD36->text());
}
//---------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureUc() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD37->text());
}
//------------------------------------------------------------
float CCalibrationWidgetBRUPowerDC::measureUMultiplier() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD34->text());
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
//------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureUa(float average)
{
    ui->lineEditMeasuredD35->setText(QLocale::system().toString(average, 'f', 6));
}
//------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureUb(float average)
{
    ui->lineEditMeasuredD36->setText(QLocale::system().toString(average, 'f', 6));
}
//------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureUc(float average)
{
    ui->lineEditMeasuredD37->setText(QLocale::system().toString(average, 'f', 6));
}
//---------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setMeasureUMultiplier(float average)
{
    ui->lineEditMeasuredD34->setText(QLocale::system().toString(average, 'f', 6));
}
//---------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationUa(float min, float max)
{
    ui->lineEditDeviationUA->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
}
//---------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationUb(float min, float max)
{
    ui->lineEditDeviationUB->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
}
//---------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationUc(float min, float max)
{
    ui->lineEditDeviationUC->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
}
//------------------------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::setDeviationUMultiplier(float min, float max)
{
    ui->lineEditDeviationUMultiplier->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
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

    deviation = float(sqrt(double(double(deviation)/double(data.count()))));

    return QPointF(double(average), double(deviation));
}
//------------------------------------------
void CCalibrationWidgetBRUPowerDC::display()
{
    qInfo() << tr("Калибровка БРУ по напряжению DC:");

    calibration_t data_min = m_calibration_min.data;
    calibration_t data_max = m_calibration_max.data;

    if(!data_min.ua.isEmpty() && !data_max.ua.isEmpty())
    {
        float Xsrcmin = m_calibration_min.value;
        float Xsrcmax = m_calibration_max.value;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        QPointF deviationMin = standardDeviation(data_min.ua);
        QPointF deviationMax = standardDeviation(data_max.ua);

        Xmeasmin = deviationMin.x();
        Xmeasmax = deviationMax.x();

        setDeviationUa(deviationMin.y(), deviationMax.y());

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

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        numerator = ps - rq; // числитель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUa(A);
            qDebug() << QString("Новый коэффициент наклона AUADC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KUADC: Знаменатель равен нулю");
    }

    if(!data_min.ub.isEmpty() && !data_max.ub.isEmpty())
    {
        float Xsrcmin = m_calibration_min.value;
        float Xsrcmax = m_calibration_max.value;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.ub).x();
        Xmeasmax = standardDeviation(data_max.ub).x();

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

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        numerator = ps - rq; // числитель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUb(A);
            qDebug() << QString("Новый коэффициент наклона AUBDC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента AUBDC: Знаменатель равен нулю");
    }

    if(!data_min.uc.isEmpty() && !data_max.uc.isEmpty())
    {
        float Xsrcmin = m_calibration_min.value;
        float Xsrcmax = m_calibration_max.value;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.uc).x();
        Xmeasmax = standardDeviation(data_max.uc).x();

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

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        numerator = ps - rq; // числитель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineUc(A);
            qDebug() << QString("Новый коэффициент наклона AUCDC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента AUCDC: Знаменатель равен нулю");
    }

    if(!data_min.multiplier.isEmpty() && !data_max.multiplier.isEmpty())
    {
        float Xsrcmin = m_calibration_min.multyplierValue;
        float Xsrcmax = m_calibration_max.multyplierValue;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        Xmeasmin = standardDeviation(data_min.multiplier).x();
        Xmeasmax = standardDeviation(data_max.multiplier).x();

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

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;

        numerator = ps - rq; // числитель

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
    m_calibration_min = { 0.0f, 0.0f, calibration_t() };
    m_calibration_max = { 0.0f, 0.0f, calibration_t() };
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
    float phaseMin = standardPhaseMin();
    float phaseMax = standardPhaseMax();
    float phaseMultiplierMin = standardPhaseMultiplierMin();
    float phaseMultiplierMax = standardPhaseMultiplierMin();

    if((stateUa() || stateUb() || stateUc()) && ((phaseMin > 0 && phaseMax > 0) && (phaseMin < phaseMax)))
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }
    else if(stateUMultiplier() && ((phaseMultiplierMin > 0 && phaseMultiplierMax > 0) && (phaseMultiplierMin < phaseMultiplierMax)))
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
}
//------------------------------------------------------------
void CCalibrationWidgetBRUPowerDC::calibrationParameterStart()
{
    if(((!stateUa() && !stateUb() && !stateUc()) || (standardPhaseMin() == 0 || standardPhaseMax() == 0)) &&
            (!stateUMultiplier() || (standardPhaseMultiplierMin() == 0 || standardPhaseMultiplierMax() == 0)))
    {
        m_calibration_type = CALIBRATION_NONE;
        m_calibration_min = { 0.0f, 0.0f, calibration_t() };
        m_calibration_max = { 0.0f, 0.0f, calibration_t() };

        QMessageBox::warning(this, tr("Калибровка БРУ по напряжению DC"), tr("Нет выбранных каналов для калибровки"));
        emit calibrationEnd();

        return;
    }

    if(m_calibration_type == CALIBRATION_MIN &&
      (((stateUa() || stateUb() || stateUc()) && standardPhaseMin() >= standardPhaseMax()) ||
      (stateUMultiplier() && standardPhaseMultiplierMin() >= standardPhaseMultiplierMax())))
    {
        m_calibration_type = CALIBRATION_NONE;
        m_calibration_min = { 0.0f, 0.0f, calibration_t() };
        m_calibration_max = { 0.0f, 0.0f, calibration_t() };

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

    if(stateUa())
    {
        unit_list << calculateValue(POWER_UA);
        param_count++;
    }
    if(stateUb())
    {
        unit_list << calculateValue(POWER_UB);
        param_count++;
    }
    if(stateUc())
    {
        unit_list << calculateValue(POWER_UC);
        param_count++;
    }
    if(stateUMultiplier())
    {
        unit_list << calculateValue(POWER_MULTIPLIER);
        param_count++;
    }

    if(unit_list.isEmpty())
        return;

    if(m_calibration_type == CALIBRATION_NONE)
    {
        m_calibration_type = CALIBRATION_MIN;
        m_calibration_min.value = standardPhaseMin();
        m_calibration_min.multyplierValue = standardPhaseMultiplierMin();

        emit calibrationFactorAllStart();
    }
    else if(m_calibration_type == CALIBRATION_MIN)
    {
        m_calibration_type = CALIBRATION_MAX;
        m_calibration_max.value = standardPhaseMax();
        m_calibration_max.multyplierValue = standardPhaseMultiplierMax();
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

        if(channel == POWER_UA)
            calibration_data.ua << value.f;
        else if(channel == POWER_UB)
            calibration_data.ub << value.f;
        else if(channel == POWER_UC)
            calibration_data.uc << value.f;
        else if(channel == POWER_MULTIPLIER)
            calibration_data.multiplier << value.f;
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
    float shift_ua           = 0.0f;
    float shift_ub           = 0.0f;
    float shift_uc           = 0.0f;
    float incline_ua         = 0.0f;
    float incline_ub         = 0.0f;
    float incline_uc         = 0.0f;
    float shift_multiplier   = 0.0f;
    float incline_multiplier = 0.0f;


    if(stateUa())
    {
        shift_ua = valueShiftUa();
        incline_ua = valueInclineUa();
    }
    if(stateUb())
    {
        shift_ub = valueShiftUb();
        incline_ub = valueInclineUb();
    }
    if(stateUc())
    {
        shift_uc = valueShiftUc();
        incline_uc = valueShiftUc();
    }
    if(stateUMultiplier())
    {
        shift_multiplier = valueShiftUMultiplier();
        incline_multiplier = valueInclineUMultiplier();
    }

    QString str;
    QString textValue;

    textValue += ((shift_ua != 0.0f)?QString("Ua сдвига = %1\n").arg(QLocale::system().toString(shift_ua, 'f', 6)):"");
    textValue += ((shift_ub != 0.0f)?QString("Ub сдвига = %1\n").arg(QLocale::system().toString(shift_ub, 'f', 6)):"");
    textValue += ((shift_uc != 0.0f)?QString("Uc сдвига = %1\n").arg(QLocale::system().toString(shift_uc, 'f', 6)):"");
    textValue += ((incline_ua != 0.0f)?QString("Ua наклона = %1\n").arg(QLocale::system().toString(incline_ua, 'f', 6)):"");
    textValue += ((incline_ub != 0.0f)?QString("Ub наклона = %1\n").arg(QLocale::system().toString(incline_ub, 'f', 6)):"");
    textValue += ((incline_uc != 0.0f)?QString("Uc наклона = %1\n").arg(QLocale::system().toString(incline_uc, 'f', 6)):"");
    textValue += ((shift_multiplier != 0.0f)?QString("Uумн сдвига = %1\n").arg(QLocale::system().toString(shift_multiplier, 'f', 6)):"");
    textValue += ((incline_multiplier != 0.0f)?QString("Uумн наклона = %1\n").arg(QLocale::system().toString(incline_multiplier, 'f', 6)):"");

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

    value.f = shift_ua;
    CModBusDataUnit unit_ShiftUa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftUa.setProperty("KEY", "AUADC");

    value.f = shift_ub;
    CModBusDataUnit unit_ShiftUb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftUb.setProperty("KEY", "AUBDC");

    value.f = shift_uc;
    CModBusDataUnit unit_ShiftUc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftUc.setProperty("KEY", "AUCDC");

    value.f = incline_ua;
    CModBusDataUnit unit_InclineUa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineUa.setProperty("KEY", "KUADC");

    value.f = incline_ub;
    CModBusDataUnit unit_InclineUb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineUb.setProperty("KEY", "KUBDC");

    value.f = incline_uc;
    CModBusDataUnit unit_InclineUc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineUc.setProperty("KEY", "KUCDC");

    value.f = shift_multiplier;
    CModBusDataUnit unit_ShiftMultiplier(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftMultiplier.setProperty("KEY", "AUMDC");

    value.f = incline_multiplier;
    CModBusDataUnit unit_InclineMultiplier(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineMultiplier.setProperty("KEY", "KUMDC");

    QVector<CModBusDataUnit> units;

    if(shift_ua >= 0.0f)
        units << unit_ShiftUa;
    if(shift_ub >= 0.0f)
        units << unit_ShiftUb;
    if(shift_uc >= 0.0f)
        units << unit_ShiftUc;
    if(incline_ua >= 0.0f)
        units << unit_InclineUa;
    if(incline_ub >= 0.0f)
        units << unit_InclineUb;
    if(incline_uc >= 0.0f)
        units << unit_InclineUc;
    if(shift_multiplier >= 0.0f)
        units << unit_ShiftMultiplier;
    if(incline_multiplier >= 0.0f)
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
        setMeasureUa(value.f);
    }
    else if(channel == "UB")
    {
        setMeasureUb(value.f);
    }
    else if(channel == "UC")
    {
        setMeasureUc(value.f);
    }
    else if(channel == "UMULTIPLIER")
    {
        setMeasureUMultiplier(value.f);
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
    QRect r = ui->gridLayoutTable->cellRect(1, 1);
    QPoint topCenterPowerPhase = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint leftTopMin = QPoint(r.left(), topCenterPowerPhase.y());
    r = ui->gridLayoutTable->cellRect(6, 1);
    QPoint bottomCenterPowerPhase = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    r = ui->gridLayoutTable->cellRect(3, 1);
    QPoint leftBottomCenterMin = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint leftBottomMin = QPoint(r.left(), leftBottomCenterMin.y());
    r = ui->gridLayoutTable->cellRect(4, 1);
    QPoint centerPowerUa = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint rightCenterPowerUa = QPoint(r.right(), centerPowerUa.y());
    r = ui->gridLayoutTable->cellRect(5, 1);
    QPoint centerPowerUb = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint rightCenterPowerUb = QPoint(r.right(), centerPowerUb.y());
    r = ui->gridLayoutTable->cellRect(6, 1);
    QPoint centerPowerUc = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint rightCenterPowerUc = QPoint(r.right(), centerPowerUc.y());
    r = ui->gridLayoutTable->cellRect(7, 1);
    QPoint topCenterPowerMultiplier = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint rightCenterMultiplier = QPoint(r.right(), topCenterPowerMultiplier.y());
    r = ui->gridLayoutTable->cellRect(8, 1);
    QPoint centerPowerMultiplierMin = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint leftCenterPowerMultiplierMin = QPoint(r.left(), centerPowerMultiplierMin.y());
    r = ui->gridLayoutTable->cellRect(10, 1);
    QPoint bottomCenterPowerMultiplier = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint leftCenterPowerMultiplier = QPoint(r.left(), bottomCenterPowerMultiplier.y());

    painter.drawLine(topCenterPowerPhase, bottomCenterPowerPhase);
    painter.drawLine(leftTopMin, topCenterPowerPhase);
    painter.drawLine(leftBottomMin, leftBottomCenterMin);
    painter.drawLine(centerPowerUa, rightCenterPowerUa);
    painter.drawLine(centerPowerUb, rightCenterPowerUb);
    painter.drawLine(centerPowerUc, rightCenterPowerUc);
    painter.drawLine(topCenterPowerMultiplier, bottomCenterPowerMultiplier);
    painter.drawLine(topCenterPowerMultiplier, rightCenterMultiplier);
    painter.drawLine(leftCenterPowerMultiplierMin, centerPowerMultiplierMin);
    painter.drawLine(leftCenterPowerMultiplier, bottomCenterPowerMultiplier);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
