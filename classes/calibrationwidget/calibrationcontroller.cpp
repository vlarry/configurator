#include "calibrationcontroller.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CCalibrationController::CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power, QObject *parent):
    QObject(parent),
    m_widget_of_current(widget_of_current),
    m_widget_power(widget_power),
    m_calibration_type(TYPE_NONE)
{
    connect(m_widget_of_current, &CCalibrationWidgetOfCurrent::calibrationStart, this, &CCalibrationController::calibrationProcessStart);
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
    m_calibration_data << unit;

    if(m_calibration.request_count == m_calibration.request_all) // Приняты все данные по калибровке
    {
        if(m_calibration_type == TYPE_CURRENT)
            m_widget_of_current->calibrationDataProcess(m_calibration_data);
        else if(m_calibration_type == TYPE_POWER)
            m_widget_power->calibrationDataProcess(m_calibration_data);

        m_calibration_data.clear();
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

    if(m_calibration.request_all == 0)
        m_calibration.request_all = 1;

    if(m_calibration.request_count < m_calibration.request_all)
    {
        for(CModBusDataUnit& unit: m_calibration.units)
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
//---------------------------------------------------------------------------------------
void CCalibrationController::calibrationProcessStart(QVector<CModBusDataUnit> &unit_list)
{
    QWidget *widget = qobject_cast<QWidget*>(sender());

    if(!widget)
        return;

    if(widget == m_widget_of_current)
        m_calibration_type = TYPE_CURRENT;
    else if(widget == m_widget_power)
        m_calibration_type = TYPE_POWER;

    m_calibration.units       = unit_list;
    m_calibration.request_all = m_widget_of_current->dataSetCount();
    m_calibration.pause       = m_widget_of_current->timePauseRequest();
    m_calibration.timer       = new QTimer;

    connect(m_calibration.timer, &QTimer::timeout, this, &CCalibrationController::calibrationProcess);
}
