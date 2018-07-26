#include "calibrationwidget.h"
#include "ui_calibrationwidget.h"
//------------------------------------------------------
CCalibrationWidget::CCalibrationWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidget)
{
    ui->setupUi(this);

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidget::calibration);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidget::stateButton);
    connect(this, &CCalibrationWidget::calibrationEnd, this, &CCalibrationWidget::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidget::apply);
}
//---------------------------------------
CCalibrationWidget::~CCalibrationWidget()
{
    delete ui;
}
//-------------------------------------------
QCheckBox* CCalibrationWidget::ctrlIa() const
{
    return ui->checkBoxIa;
}
//-------------------------------------------
QCheckBox* CCalibrationWidget::ctrlIb() const
{
    return ui->checkBoxIb;
}
//-------------------------------------------
QCheckBox* CCalibrationWidget::ctrlIc() const
{
    return ui->checkBoxIc;
}
//--------------------------------------------
QCheckBox* CCalibrationWidget::ctrl3I0() const
{
    return ui->checkBox3I0;
}
//-----------------------------------------
int CCalibrationWidget::timeSetData() const
{
    return ui->spinBoxTimeSetData->value();
}
//----------------------------------------------
int CCalibrationWidget::timePauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//----------------------------------------------------
void CCalibrationWidget::addCalibrationIa(float value)
{
    m_calibration_current_data.Ia << value;
}
//----------------------------------------------------
void CCalibrationWidget::addCalibrationIb(float value)
{
    m_calibration_current_data.Ib << value;
}
//----------------------------------------------------
void CCalibrationWidget::addCalibrationIc(float value)
{
    m_calibration_current_data.Ic << value;
}
//-----------------------------------------------------
void CCalibrationWidget::addCalibration3I0(float value)
{
    m_calibration_current_data._3I0 << value;
}
//---------------------------------------------------------------------------------------------
const CCalibrationWidget::calibration_current_t& CCalibrationWidget::calibrationCurrent() const
{
    return m_calibration_current_data;
}
//----------------------------------------------------------
float CCalibrationWidget::calibrationCurrentStandard() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandard->text());
}
//-------------------------------------------------------------
float CCalibrationWidget::calibrationCurrentStandard3I0() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandard3I0->text());
}
//----------------------------------------------------
float CCalibrationWidget::calibrationCurrentIa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIa->text());
}
//----------------------------------------------------
float CCalibrationWidget::calibrationCurrentIb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIb->text());
}
//----------------------------------------------------
float CCalibrationWidget::calibrationCurrentIc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIc->text());
}
//-----------------------------------------------------
float CCalibrationWidget::calibrationCurrent3I0() const
{
    return QLocale::system().toFloat(ui->lineEditFactor3I0->text());
}
//-----------------------------------------------
void CCalibrationWidget::setFactorIa(float value)
{
    ui->lineEditFactorIa->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------
void CCalibrationWidget::setFactorIb(float value)
{
    ui->lineEditFactorIb->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------
void CCalibrationWidget::setFactorIc(float value)
{
    ui->lineEditFactorIc->setText(QLocale::system().toString(value, 'f', 6));
}
//------------------------------------------------
void CCalibrationWidget::setFactor3I0(float value)
{
    ui->lineEditFactor3I0->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------------------------------
void CCalibrationWidget::setMeasureIa(float average, float deviation)
{
    QString text = QString("%1 (%2)").arg(QLocale::system().toString(average, 'f', 6)).
                                    arg(QLocale::system().toString(deviation, 'f', 6));
    ui->lineEditMeasuredD01->setText(text);
}
//-------------------------------------------------------------------
void CCalibrationWidget::setMeasureIb(float average, float deviation)
{
    QString text = QString("%1 (%2)").arg(QLocale::system().toString(average, 'f', 6)).
                                    arg(QLocale::system().toString(deviation, 'f', 6));
    ui->lineEditMeasuredD02->setText(text);
}
//-------------------------------------------------------------------
void CCalibrationWidget::setMeasureIc(float average, float deviation)
{
    QString text = QString("%1 (%2)").arg(QLocale::system().toString(average, 'f', 6)).
                                    arg(QLocale::system().toString(deviation, 'f', 6));
    ui->lineEditMeasuredD03->setText(text);
}
//--------------------------------------------------------------------
void CCalibrationWidget::setMeasure3I0(float average, float deviation)
{
    QString text = QString("%1 (%2)").arg(QLocale::system().toString(average, 'f', 6)).
                                    arg(QLocale::system().toString(deviation, 'f', 6));
    ui->lineEditMeasuredD04->setText(text);
}
//-------------------------------------------
void CCalibrationWidget::setAmIa(float value)
{
    ui->lineEditAmCH03->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------
void CCalibrationWidget::setAmIb(float value)
{
    ui->lineEditAmCH04->setText(QLocale::system().toString(value, 'f', 6));
}
//-------------------------------------------
void CCalibrationWidget::setAmIc(float value)
{
    ui->lineEditAmCH05->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------
void CCalibrationWidget::setAm3I0(float value)
{
    ui->lineEditAmCH02->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------
void CCalibrationWidget::stateButton(bool state)
{
    ui->pushButtonCalibration->setEnabled(!state);
}
//-----------------------------------------------------
void CCalibrationWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    int hcenter        = ui->gridLayout->cellRect(1, 1).left() + ui->gridLayout->cellRect(1, 1).width()/2;
    int left           = ui->gridLayout->cellRect(1, 1).left();
    int right          = ui->gridLayout->cellRect(1, 1).right();
    int top_center     = ui->gridLayout->cellRect(1, 1).top() + ui->gridLayout->cellRect(1, 1).height()/2;
    int bottom_center1 = ui->gridLayout->cellRect(3, 1).top() + ui->gridLayout->cellRect(3, 1).height()/2;
    int vcenter        = ui->gridLayout->cellRect(2, 1).top() + ui->gridLayout->cellRect(2, 1).height()/2;
    int bottom_center2 = ui->gridLayout->cellRect(5, 1).top() + ui->gridLayout->cellRect(5, 1).height()/2;

    painter.drawLines(QVector<QLine>() << QLine(right, top_center, hcenter, top_center) <<
                                          QLine(hcenter, top_center, hcenter, bottom_center1) <<
                                          QLine(hcenter, bottom_center1, right, bottom_center1) <<
                                          QLine(left + 5, vcenter, right, vcenter) <<
                                          QLine(left + 5, bottom_center2, right, bottom_center2));
    painter.drawRect(ui->verticalLayoutCenter->geometry());
}
