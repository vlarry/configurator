#include "calibrationcontroller.h"
//-----------------------------------------------------------------------------------------------------------------------------------
CCalibrationController::CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power,
                                               CCalibrationWidgetBRUPowerDC *widget_bru_power_dc, CCalibrationWidgetBRUResistance *widget_bru_resistance,
                                               QObject *parent):
    QObject(parent),
    m_widget_of_current(widget_of_current),
    m_widget_power(widget_power),
    m_widget_bru_power_dc(widget_bru_power_dc),
    m_widget_bru_resistance(widget_bru_resistance),
    m_calibration_type(TYPE_NONE)
{
    m_timer_caluculate = new QTimer(this);

    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationStart, this, &CCalibrationController::calibrationProcessStart);
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationFactorAllStart, this, &CCalibrationController::calibrationFactorAllRead);
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationWriteStart, this, &CCalibrationController::calibrationWrite);
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::saveToFlash, this, &CCalibrationController::calibrationSaveToFlash);
    connect(this, &CCalibrationController::calibrationFactorActual, m_widget_of_current, &CCalibrationWidgetOfCurrent::setCalibrartionFactorActual);
    connect(this, &CCalibrationController::calculateResponse, m_widget_of_current, &CCalibrationWidgetOfCurrent::setCalculateActualValue);
    connect(this, &CCalibrationController::dataIncrement, m_widget_of_current, &CCalibrationWidgetOfCurrent::progressBarIncrement);

    connect(m_widget_power, &CCalibrationWidgetPower::calibrationStart, this, &CCalibrationController::calibrationProcessStart);
    connect(m_widget_power, &CCalibrationWidgetPower::calibrationFactorAllStart, this, &CCalibrationController::calibrationFactorAllRead);
    connect(m_widget_power, &CCalibrationWidgetPower::calibrationWriteStart, this, &CCalibrationController::calibrationWrite);
    connect(m_widget_power, &CCalibrationWidgetPower::saveToFlash, this, &CCalibrationController::calibrationSaveToFlash);
    connect(this, &CCalibrationController::calibrationFactorActual, m_widget_power, &CCalibrationWidgetPower::setCalibrartionFactorActual);
    connect(this, &CCalibrationController::calculateResponse, m_widget_power, &CCalibrationWidgetPower::setCalculateActualValue);
    connect(this, &CCalibrationController::dataIncrement, m_widget_power, &CCalibrationWidgetPower::progressBarIncrement);

    connect(m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::calibrationStart, this, &CCalibrationController::calibrationProcessStart);
    connect(m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::calibrationFactorAllStart, this, &CCalibrationController::calibrationFactorAllRead);
    connect(m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::calibrationWriteStart, this, &CCalibrationController::calibrationWrite);
    connect(m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::saveToFlash, this, &CCalibrationController::calibrationSaveToFlash);
    connect(this, &CCalibrationController::calibrationFactorActual, m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::setCalibrartionFactorActual);
    connect(this, &CCalibrationController::calculateResponse, m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::setCalculateActualValue);
    connect(this, &CCalibrationController::dataIncrement, m_widget_bru_power_dc, &CCalibrationWidgetBRUPowerDC::progressBarIncrement);

    connect(m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::calibrationMeasureRead, this, &CCalibrationController::calibrationProcessStart);
    connect(m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::calibrationFactorAllStart, this, &CCalibrationController::calibrationFactorAllRead);
    connect(m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::calibrationWriteStart, this, &CCalibrationController::calibrationWrite);
    connect(m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::saveToFlash, this, &CCalibrationController::calibrationSaveToFlash);
    connect(m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::sendCommand, this, &CCalibrationController::bruResistanceSendCommand);
    connect(this, &CCalibrationController::calibrationFactorActual, m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::setCalibrartionFactorActual);
    connect(this, &CCalibrationController::calculateResponse, m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::setCalculateActualValue);
//    connect(this, &CCalibrationController::dataIncrement, m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::progressBarIncrement);

    connect(m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::stateVariable, this, &CCalibrationController::bruResistanceStateVariable);
    connect(this, &CCalibrationController::bruResistanceStateVariableIsReady, m_widget_bru_resistance, &CCalibrationWidgetBRUResistance::processStateVariable);

    connect(m_timer_caluculate, &QTimer::timeout, this, &CCalibrationController::calculateValueRead);
}
//-----------------------------------------------
CCalibrationController::~CCalibrationController()
{
    if(m_timer_caluculate)
        delete m_timer_caluculate;
}
//---------------------------------------------------------------------------------------------
void CCalibrationController::setWidgetCalibrationOfCurrent(CCalibrationWidgetOfCurrent *widget)
{
    m_widget_of_current = widget;
}
//-------------------------------------------------------------------------------------
void CCalibrationController::setWidgetCalibrationPower(CCalibrationWidgetPower *widget)
{
    m_widget_power = widget;
}
//-----------------------------------------------------------------------------------------------
void CCalibrationController::setWidgetCalibrationBRUPowerDC(CCalibrationWidgetBRUPowerDC *widget)
{
    m_widget_bru_power_dc = widget;
}
//-----------------------------------------------------------------------------------------------------
void CCalibrationController::setWidgetCalibrationBRUResistance(CCalibrationWidgetBRUResistance *widget)
{
    m_widget_bru_resistance = widget;
}
//-------------------------------------------------------------
void CCalibrationController::dataIsReady(CModBusDataUnit &unit)
{
    m_calibration_data.data << unit;
    m_calibration_data.counter++;

    emit dataIncrement();

qDebug() << QString("Получено сообщение №%1").arg(m_calibration_data.counter);
    if(m_calibration_data.counter == m_calibration_data.limit) // Приняты все данные по калибровке
    {
        if(m_calibration_type == TYPE_CURRENT)
            m_widget_of_current->calibrationDataProcess(m_calibration_data.data);
        else if(m_calibration_type == TYPE_POWER_AC)
            m_widget_power->calibrationDataProcess(m_calibration_data.data);
        else if(m_calibration_type == TYPE_BRU_POWER_DC)
            m_widget_bru_power_dc->calibrationDataProcess(m_calibration_data.data);
        else if(m_calibration_type == TYPE_BRU_RESISTANCE)
            m_widget_bru_resistance->calibrationDataProcess(m_calibration_data.data);
qDebug() << QString("Калибровочные данные приняты в полном объеме: %1 из %2").arg(m_calibration_data.counter).arg(m_calibration.request_all);
        m_calibration_data = { 0, 0, QVector<CModBusDataUnit>(0) };
        m_calibration_type = TYPE_NONE;

        if(m_calculate_type != TYPE_NONE)
            m_timer_caluculate->start(500);
    }
}
/*!
 * \brief CCalibrationController::calibrationProcess
 *
 * Обработчик таймера для отправки запроса
 */
void CCalibrationController::calibrationProcess()
{
    m_calibration.timer->stop();
qDebug() << QString("запросов всего = %1, запросов отправлено = %2").arg(m_calibration.request_all).arg(m_calibration.request_count);
    if(m_calibration.request_all == 0)
        m_calibration.request_all = 1;

    if(m_calibration.request_count < m_calibration.request_all)
    {
        for(CModBusDataUnit &unit: m_calibration.units)
            emit calibration(unit);

        m_calibration.request_count++;
        m_calibration.timer->start(m_calibration.pause);
    }
    else
    {
        disconnect(m_calibration.timer, &QTimer::timeout, this, &CCalibrationController::calibrationProcess);
        delete m_calibration.timer;
    }
}
//--------------------------------------------------------------------------------------------------------
void CCalibrationController::calibrationProcessStart(QVector<CModBusDataUnit> &unit_list, int param_count)
{
    QWidget *widget = qobject_cast<QWidget*>(sender());

    if(!widget)
        return;

    if(widget == m_widget_of_current)
    {
        m_calibration_type = TYPE_CURRENT;
        m_calibration = { m_widget_of_current->dataCount(), 0, m_widget_of_current->pauseRequest(), unit_list, nullptr };
    }
    else if(widget == m_widget_power)
    {
        m_calibration_type = TYPE_POWER_AC;
        m_calibration = { m_widget_power->dataCount(), 0, m_widget_power->pauseRequest(), unit_list, nullptr };
    }
    else if(widget == m_widget_bru_power_dc)
    {
        m_calibration_type = TYPE_BRU_POWER_DC;
        m_calibration = { m_widget_bru_power_dc->dataCount(), 0, m_widget_bru_power_dc->pauseRequest(), unit_list, nullptr };
    }
    else if(widget == m_widget_bru_resistance)
    {
        m_calibration_type = TYPE_BRU_RESISTANCE;
        m_calibration = { m_widget_bru_resistance->dataCount(), 0, m_widget_bru_resistance->pauseRequest(), unit_list, nullptr };
    }

    m_calibration_data = { 0, param_count*m_calibration.request_all, QVector<CModBusDataUnit>(0) };

    m_calibration.timer = new QTimer;

    m_timer_caluculate->stop(); // останавливаем таймер
    connect(m_calibration.timer, &QTimer::timeout, this, &CCalibrationController::calibrationProcess);
    calibrationProcess();
}
//-----------------------------------------------
void CCalibrationController::calculateValueRead()
{
    QVector<CModBusDataUnit> list;

    if(m_widget_of_current->stateCalculateUpdate() && m_calculate_type == TYPE_CURRENT)
        list = m_widget_of_current->calculateValueList();
    if(m_widget_power->stateCalculateUpdate() && m_calculate_type == TYPE_POWER_AC)
        list = m_widget_power->calculateValueList();
    if(m_widget_bru_power_dc->stateCalculateUpdate() && m_calculate_type == TYPE_BRU_POWER_DC)
        list = m_widget_bru_power_dc->calculateValueList();
    if(m_widget_bru_resistance->stateCalculateUpdate() && m_calculate_type == TYPE_BRU_RESISTANCE)
        list = m_widget_bru_resistance->calculateValueList();
qDebug() << "Список расчетных величин для калибровок: " << list.count();
    if(!list.isEmpty())
        calculate(list);
}
//------------------------------------------------------------------------------------------
void CCalibrationController::setCalculateState(bool state, CalibrationType type = TYPE_NONE)
{
    m_calculate_type = type;

    if(state)
    {
        if(!m_timer_caluculate->isActive())
            m_timer_caluculate->start(500);
    }
    else
    {
        if(m_timer_caluculate->isActive())
            m_timer_caluculate->stop();
    }
}
