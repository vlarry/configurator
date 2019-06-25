#include "calibrationwidgetbruresistance.h"
#include "ui_calibrationwidgetbruresistance.h"
//--------------------------------------------------------------------------------
CCalibrationWidgetBRUResistance::CCalibrationWidgetBRUResistance(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetBRUResistance),
    m_calibration_type(CALIBRATION_NONE),
    m_calibration_min({ 0.0f, calibration_t() }),
    m_calibration_max({ 0.0f, calibration_t() })
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.000001, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPowerStandardPhaseMin->setValidator(validator);
    ui->lineEditPowerStandardPhaseMax->setValidator(validator);

    ui->lineEditMeasuredD38->setValidator(validator);
    ui->lineEditMeasuredD39->setValidator(validator);
    ui->lineEditMeasuredD40->setValidator(validator);

    ui->lineEditFactorRAShift->setValidator(validator);
    ui->lineEditFactorRBShift->setValidator(validator);
    ui->lineEditFactorRCShift->setValidator(validator);
    ui->lineEditFactorRAIncline->setValidator(validator);
    ui->lineEditFactorRBIncline->setValidator(validator);
    ui->lineEditFactorRCIncline->setValidator(validator);

    ui->progressBarDataSet->hide();

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::calibrationParameterStart);
    connect(ui->pushButtonCalibration, &QPushButton::toggled, this, &CCalibrationWidgetBRUResistance::stateButton);
    connect(this, &CCalibrationWidgetBRUResistance::calibrationEnd, this, &CCalibrationWidgetBRUResistance::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::calibrationWriteProcess);
    connect(ui->lineEditPowerStandardPhaseMin, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUResistance::valueCurrentStandardChanged);
    connect(ui->lineEditPowerStandardPhaseMax, &CLineEdit::textChanged, this, &CCalibrationWidgetBRUResistance::valueCurrentStandardChanged);
    connect(ui->checkBoxRA, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRB, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->checkBoxRC, &QCheckBox::clicked, this, &CCalibrationWidgetBRUResistance::stateChoiceChannelChanged);
    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetBRUResistance::saveCalibrationToFlash);
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
        case RESISTANCE_RA:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 124, 2); // чтение D38->Ra
            unit.setProperty("CHANNEL", RESISTANCE_RA);
            unit.setProperty("KEY", "RA");
        break;

        case RESISTANCE_RB:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 126, 2); // чтение D39->Rb
            unit.setProperty("CHANNEL", RESISTANCE_RB);
            unit.setProperty("KEY", "RB");
        break;

        case RESISTANCE_RC:
            unit = CModBusDataUnit(0, CModBusDataUnit::ReadInputRegisters, 128, 2); // чтение D40->Rc
            unit.setProperty("CHANNEL", RESISTANCE_RC);
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

    list << calculateValue(RESISTANCE_RA);
    list << calculateValue(RESISTANCE_RB);
    list << calculateValue(RESISTANCE_RC);
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
//-------------------------------------------------------------
float CCalibrationWidgetBRUResistance::standardPhaseMin() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseMin->text());
}
//-------------------------------------------------------------
float CCalibrationWidgetBRUResistance::standardPhaseMax() const
{
    return QLocale::system().toFloat(ui->lineEditPowerStandardPhaseMax->text());
}
//---------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateRa() const
{
    return ui->checkBoxRA->isChecked();
}
//---------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateRb() const
{
    return ui->checkBoxRB->isChecked();
}
//---------------------------------------------------
bool CCalibrationWidgetBRUResistance::stateRc() const
{
    return ui->checkBoxRC->isChecked();
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
//------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureRa() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD38->text());
}
//------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureRb() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD39->text());
}
//------------------------------------------------------
float CCalibrationWidgetBRUResistance::measureRc() const
{
    return QLocale::system().toFloat(ui->lineEditMeasuredD40->text());
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
//---------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureRa(float average)
{
    ui->lineEditMeasuredD38->setText(QLocale::system().toString(average, 'f', 6));
}
//---------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureRb(float average)
{
    ui->lineEditMeasuredD39->setText(QLocale::system().toString(average, 'f', 6));
}
//---------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setMeasureRc(float average)
{
    ui->lineEditMeasuredD40->setText(QLocale::system().toString(average, 'f', 6));
}
//------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationRa(float min, float max)
{
    ui->lineEditDeviationRA->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
}
//------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationRb(float min, float max)
{
    ui->lineEditDeviationRB->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
}
//------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setDeviationRc(float min, float max)
{
    ui->lineEditDeviationRC->setText(QString("%1 / %2").arg(QLocale::system().toString(min, 'f', 6)).arg(QLocale::system().toString(max, 'f', 6)));
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

    deviation = float(sqrt(double(double(deviation)/double(data.count()))));

    return QPointF(double(average), double(deviation));
}
//---------------------------------------------
void CCalibrationWidgetBRUResistance::display()
{
    qInfo() << tr("Калибровка БРУ по сопротивлению:");

    calibration_t data_min = m_calibration_min.data;
    calibration_t data_max = m_calibration_max.data;

    if(!data_min.ra.isEmpty() && !data_max.ra.isEmpty())
    {
        float Xsrcmin = m_calibration_min.value;
        float Xsrcmax = m_calibration_max.value;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        QPointF deviationMin = standardDeviation(data_min.ra);
        QPointF deviationMax = standardDeviation(data_max.ra);

        Xmeasmin = deviationMin.x();
        Xmeasmax = deviationMax.x();

        setDeviationRa(deviationMin.y(), deviationMax.y());

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftRa(K);
            qDebug() << QString("Новый коэффициент сдвига ARA рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента ARA: Знаменатель равен нулю");

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;
        numerator = ps - rq; // числитель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineRa(A);
            qDebug() << QString("Новый коэффициент наклона KRA рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRA: Знаменатель равен нулю");
    }

    if(!data_min.rb.isEmpty() && !data_max.rb.isEmpty())
    {
        float Xsrcmin = m_calibration_min.value;
        float Xsrcmax = m_calibration_max.value;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        QPointF deviationMin = standardDeviation(data_min.rb);
        QPointF deviationMax = standardDeviation(data_max.rb);

        Xmeasmin = deviationMin.x();
        Xmeasmax = deviationMax.x();

        setDeviationRb(deviationMin.y(), deviationMax.y());

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftRb(K);
            qDebug() << QString("Новый коэффициент сдвига ARB рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента ARB: Знаменатель равен нулю");

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;
        numerator = ps - rq; // числитель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineRb(A);
            qDebug() << QString("Новый коэффициент наклона KRB рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRB: Знаменатель равен нулю");
    }

    if(!data_min.rc.isEmpty() && !data_max.rc.isEmpty())
    {
        float Xsrcmin = m_calibration_min.value;
        float Xsrcmax = m_calibration_max.value;

        float Xmeasmin = 0.0f;
        float Xmeasmax = 0.0f;

        QPointF deviationMin = standardDeviation(data_min.rc);
        QPointF deviationMax = standardDeviation(data_max.rc);

        Xmeasmin = deviationMin.x();
        Xmeasmax = deviationMax.x();

        setDeviationRc(deviationMin.y(), deviationMax.y());

        float numerator = (Xmeasmax - Xmeasmin); // числитель
        float denominator (Xsrcmax - Xsrcmin); // знаменатель

        if(denominator > 0)
        {
            float K = numerator/denominator;
            setFactorShiftRc(K);
            qDebug() << QString("Новый коэффициент сдвига ARC рассчитан: %1 ").arg(K);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента ARC: Знаменатель равен нулю");

        float ps = Xsrcmax*Xmeasmin;
        float rq = Xsrcmin*Xmeasmax;
        numerator = ps - rq; // числитель

        if(denominator > 0)
        {
            float A = numerator/denominator;
            setFactorInclineRc(A);
            qDebug() << QString("Новый коэффициент наклона KRC рассчитан: %1 ").arg(A);
        }
        else
            qDebug() << QString("Ошибка при расчете коэффициента KRC: Знаменатель равен нулю");
    }

    m_calibration_type = CALIBRATION_NONE;
    m_calibration_min = { 0.0f, calibration_t() };
    m_calibration_max = { 0.0f, calibration_t() };
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
    float phaseMin = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseMin->text());
    float phaseMax = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseMax->text());

    if((stateRa() || stateRb() || stateRc()) && (phaseMin > 0 && phaseMax > 0) && (phaseMin < phaseMax))
    {
        ui->pushButtonCalibration->setEnabled(true);
        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
}
//---------------------------------------------------------------
void CCalibrationWidgetBRUResistance::calibrationParameterStart()
{
    if((!stateRa() && !stateRb() && !stateRc()) ||
       (standardPhaseMin() == 0 || standardPhaseMax() == 0))
    {
        m_calibration_type = CALIBRATION_NONE;
        m_calibration_min = { 0.0f, calibration_t() };
        m_calibration_max = { 0.0f, calibration_t() };

        QMessageBox::warning(this, tr("Калибровка БРУ по сопротивлению"), tr("Нет выбранных каналов для калибровки"));
        emit calibrationEnd();

        return;
    }

    if(m_calibration_type == CALIBRATION_MIN &&
      ((stateRa() || stateRb() || stateRc()) && standardPhaseMin() >= standardPhaseMax()))
    {
        m_calibration_type = CALIBRATION_NONE;
        m_calibration_min = { 0.0f, calibration_t() };
        m_calibration_max = { 0.0f, calibration_t() };

        QMessageBox::warning(this, tr("Калибровка БРУ по сопротивлению"), tr("Эталонное значение для максимума меньше или равно значения минимума"));
        emit calibrationEnd();

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

    if(stateRa())
    {
        unit_list << calculateValue(RESISTANCE_RA);
        param_count++;
    }
    if(stateRb())
    {
        unit_list << calculateValue(RESISTANCE_RB);
        param_count++;
    }
    if(stateRc())
    {
        unit_list << calculateValue(RESISTANCE_RC);
        param_count++;
    }

    if(unit_list.isEmpty())
        return;

    if(m_calibration_type == CALIBRATION_NONE)
    {
        m_calibration_type = CALIBRATION_MIN;
        m_calibration_min.value = standardPhaseMin();

        emit calibrationFactorAllStart();
    }
    else if(m_calibration_type == CALIBRATION_MIN)
    {
        m_calibration_type = CALIBRATION_MAX;
        m_calibration_max.value = standardPhaseMax();
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

        if(channel == RESISTANCE_RA)
            calibration_data.ra << value.f;
        else if(channel == RESISTANCE_RB)
            calibration_data.rb << value.f;
        else if(channel == RESISTANCE_RC)
            calibration_data.rc << value.f;
    }

    if(m_calibration_type == CALIBRATION_MIN)
    {
        m_calibration_min.data = calibration_data;
        QMessageBox::information(this, tr("Калибровка БРУ по сопротивлению"), tr("Калибровка минимума окончена.\n"
                                                                                 "Произведите калибровку максимума."));
    }
    else if(m_calibration_type == CALIBRATION_MAX)
    {
        m_calibration_max.data = calibration_data;
        QMessageBox::information(this, tr("Калибровка БРУ по сопротивлению"), tr("Калибровка максимума окончена."));
        display();
    }

    emit calibrationEnd();
}
//-------------------------------------------------------------
void CCalibrationWidgetBRUResistance::calibrationWriteProcess()
{
    float shiftRa   = 0.0f;
    float shiftRb   = 0.0f;
    float shiftRc   = 0.0f;
    float inclineRa = 0.0f;
    float inclineRb = 0.0f;
    float inclineRc = 0.0f;

    if(stateRa())
    {
        shiftRa = valueShiftRa();
        inclineRa = valueInclineRa();
    }
    if(stateRb())
    {
        shiftRb = valueShiftRb();
        inclineRb = valueInclineRb();
    }
    if(stateRc())
    {
        shiftRc = valueShiftRc();
        inclineRc = valueInclineRc();
    }

    if(shiftRa == 0.0f && shiftRb == 0.0f && shiftRc == 0.0f && inclineRa == 0.0f && inclineRb == 0.0f && inclineRc == 0.0f)
    {
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
    unit_ShiftRa.setProperty("KEY", "ARA");

    value.f = shiftRb;
    CModBusDataUnit unit_ShiftRb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftRb.setProperty("KEY", "ARB");

    value.f = shiftRc;
    CModBusDataUnit unit_ShiftRc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_ShiftRc.setProperty("KEY", "ARC");

    value.f = inclineRa;
    CModBusDataUnit unit_InclineRa(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineRa.setProperty("KEY", "KRA");

    value.f = inclineRb;
    CModBusDataUnit unit_InclineRb(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineRb.setProperty("KEY", "KRB");

    value.f = inclineRc;
    CModBusDataUnit unit_InclineRc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_InclineRc.setProperty("KEY", "KRC");

    QVector<CModBusDataUnit> units;

    if(shiftRa > 0.0f)
        units << unit_ShiftRa;
    if(shiftRb > 0.0f)
        units << unit_ShiftRb;
    if(shiftRc > 0.0f)
        units << unit_ShiftRc;
    if(inclineRa > 0.0f)
        units << unit_InclineRa;
    if(inclineRb > 0.0f)
        units << unit_InclineRb;
    if(inclineRc > 0.0f)
        units << unit_InclineRc;

    qInfo() << tr("Запись новых калибровочных коэффициентов БРУ по сопротивлению подтверждена");

    emit calibrationWriteStart(units);
}
//------------------------------------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::setCalibrartionFactorActual(const QString &key, float value)
{
    qDebug() << QString("Калибровочный коэффициент: %1, значение: %2").arg(key).arg(value);
    if(key == "KRA")
        setFactorInclineRa(value);
    else if(key == "KRB")
        setFactorInclineRb(value);
    else if(key == "KRC")
        setFactorInclineRc(value);
    else if(key == "ARA")
        setFactorShiftRa(value);
    else if(key == "ARB")
        setFactorShiftRb(value);
    else if(key == "ARC")
        setFactorShiftRc(value);
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
        setMeasureRa(value.f);
    }
    else if(channel == "RB")
    {
        setMeasureRb(value.f);
    }
    else if(channel == "RC")
    {
        setMeasureRc(value.f);
    }
}
//----------------------------------------------------------
void CCalibrationWidgetBRUResistance::progressBarIncrement()
{
    int count = ui->progressBarDataSet->value();
    int step  = 100/ui->spinBoxSetDataCount->value();
    ui->progressBarDataSet->setValue(count + step);
}
//------------------------------------------------------------------
void CCalibrationWidgetBRUResistance::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Рисование отрезков перед сопротивлениями RA, RB, и RC
    QRect r = ui->gridLayoutTable->cellRect(3, 1);
    QPoint topCenterLine = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint leftCenterTop = QPoint(r.left(), topCenterLine.y());
    QPoint rightCenterTop = QPoint(r.right(), topCenterLine.y());
    r = ui->gridLayoutTable->cellRect(4, 1);
    QPoint middleCenterLine = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint rightCenterMiddle = QPoint(r.right(), middleCenterLine.y());
    r = ui->gridLayoutTable->cellRect(5, 1);
    QPoint bottomCenterLine = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint leftCenterBottom = QPoint(r.left(), bottomCenterLine.y());
    QPoint rightCenterBottom = QPoint(r.right(), bottomCenterLine.y());

    painter.drawLine(topCenterLine, bottomCenterLine);
    painter.drawLine(leftCenterTop, rightCenterTop);
    painter.drawLine(middleCenterLine, rightCenterMiddle);
    painter.drawLine(leftCenterBottom, rightCenterBottom);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
