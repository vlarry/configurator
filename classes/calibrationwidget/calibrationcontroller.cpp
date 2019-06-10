#include "calibrationcontroller.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CCalibrationController::CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power, QObject *parent):
    QObject(parent),
    m_widget_of_current(widget_of_current),
    m_widget_power(widget_power),
    m_calibration_type(TYPE_NONE)
{
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationStart, this, &CCalibrationController::calibrationProcessStart);
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationFactorAllStart, this, &CCalibrationController::calibrationFactorAllRead);
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationWriteStart, this, &CCalibrationController::calibrationWrite);
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::saveToFlash, this, &CCalibrationController::calibrationSaveToFlash);
    connect(this, &CCalibrationController::calibrationFactorActual, m_widget_of_current, &CCalibrationWidgetOfCurrent::setCalibrartionFactorActual);

    connect(m_widget_power, &CCalibrationWidgetPower::calibrationStart, this, &CCalibrationController::calibrationProcessStart);
    connect(m_widget_power, &CCalibrationWidgetPower::calibrationFactorAllStart, this, &CCalibrationController::calibrationFactorAllRead);
    connect(m_widget_power, &CCalibrationWidgetPower::calibrationWriteStart, this, &CCalibrationController::calibrationWrite);
    connect(m_widget_power, &CCalibrationWidgetPower::saveToFlash, this, &CCalibrationController::calibrationSaveToFlash);
    connect(this, &CCalibrationController::calibrationFactorActual, m_widget_power, &CCalibrationWidgetPower::setCalibrartionFactorActual);
}
//-----------------------------------------------
CCalibrationController::~CCalibrationController()
{

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
//-------------------------------------------------------------
void CCalibrationController::dataIsReady(CModBusDataUnit &unit)
{
    m_calibration_data.data << unit;
    m_calibration_data.counter++;
qDebug() << QString("Получено сообщение №%1").arg(m_calibration_data.counter);
    if(m_calibration_data.counter == m_calibration_data.limit) // Приняты все данные по калибровке
    {
        if(m_calibration_type == TYPE_CURRENT)
            m_widget_of_current->calibrationDataProcess(m_calibration_data.data);
        else if(m_calibration_type == TYPE_POWER)
            m_widget_power->calibrationDataProcess(m_calibration_data.data);
qDebug() << QString("Калибровочные данные приняты в полном объеме: %1 из %2").arg(m_calibration_data.counter).arg(m_calibration.request_all);
        m_calibration_data = { 0, 0, QVector<CModBusDataUnit>(0) };
        m_calibration_type = TYPE_NONE;
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
        m_calibration_type = TYPE_CURRENT;
    else if(widget == m_widget_power)
        m_calibration_type = TYPE_POWER;

    m_calibration = { m_widget_of_current->dataCount(), 0, m_widget_of_current->pauseRequest(), unit_list, nullptr };
    m_calibration_data = { 0, param_count*m_calibration.request_all, QVector<CModBusDataUnit>(0) };

    m_calibration.timer = new QTimer;

    connect(m_calibration.timer, &QTimer::timeout, this, &CCalibrationController::calibrationProcess);
    calibrationProcess();
}
