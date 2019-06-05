#include "calibrationcontroller.h"
//------------------------------------------------------------------------------------------------------------------------------------
CCalibrationController::CCalibrationController(CCalibrationWidgetOfCurrent *widget_of_current, CCalibrationWidgetPower *widget_power):
    m_widget_of_current(widget_of_current),
    m_widget_power(widget_power)
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

}
