#include "calibrationwidget.h"
#include "ui_calibrationwidget.h"
//------------------------------------------------------
CCalibrationWidget::CCalibrationWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidget)
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.000001, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditCurrentStandardPhase->setValidator(validator);
    ui->lineEditCurrentStandard3I0->setValidator(validator);
    ui->lineEditFactorIa->setValidator(validator);
    ui->lineEditFactorIb->setValidator(validator);
    ui->lineEditFactorIc->setValidator(validator);
    ui->lineEditFactor3I0->setValidator(validator);

    connect(ui->pushButtonCalibration, &QPushButton::toggled, this, &CCalibrationWidget::calibration);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidget::stateButton);
    connect(this, &CCalibrationWidget::calibrationEnd, this, &CCalibrationWidget::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidget::apply);
    connect(ui->lineEditCurrentStandardPhase, &QLineEdit::textChanged, this, &CCalibrationWidget::valueCurrentStandardChanged);
    connect(ui->lineEditCurrentStandard3I0, &QLineEdit::textChanged, this, &CCalibrationWidget::valueCurrentStandardChanged);
    connect(ui->checkBoxIa, &QCheckBox::clicked, this, &CCalibrationWidget::stateChoiceCurrentChannelChanged);
    connect(ui->checkBoxIb, &QCheckBox::clicked, this, &CCalibrationWidget::stateChoiceCurrentChannelChanged);
    connect(ui->checkBoxIc, &QCheckBox::clicked, this, &CCalibrationWidget::stateChoiceCurrentChannelChanged);
    connect(ui->checkBox3I0, &QCheckBox::clicked, this, &CCalibrationWidget::stateChoiceCurrentChannelChanged);
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
//---------------------------------------------------------------
float CCalibrationWidget::calibrationCurrentStandardPhase() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandardPhase->text());
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
//------------------------------------------------
void CCalibrationWidget::calibrationCurrentClear()
{
    m_calibration_current_data = calibration_current_t({ calibration_data_t(0), calibration_data_t(0), calibration_data_t(0),
                                                         calibration_data_t(0) });
}
//-----------------------------------------------------------
void CCalibrationWidget::setCurrentStandardPhase(float value)
{
    ui->lineEditCurrentStandardPhase->setText(QLocale::system().toString(value, 'f', 6));
}
//---------------------------------------------------------
void CCalibrationWidget::setCurrentStandard3I0(float value)
{
    ui->lineEditCurrentStandard3I0->setText(QLocale::system().toString(value, 'f', 6));
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
    ui->pushButtonApply->setEnabled(!state);
    ui->pushButtonCalibration->setChecked(state);
}
//------------------------------------------------------------------
void CCalibrationWidget::valueCurrentStandardChanged(const QString&)
{
    QLineEdit* le    = qobject_cast<QLineEdit*>(sender());
    float      phase = QLocale::system().toFloat(ui->lineEditCurrentStandardPhase->text());
    float      _3I0  = QLocale::system().toFloat(ui->lineEditCurrentStandard3I0->text());

    if(le == ui->lineEditCurrentStandardPhase)
    {
        if((ui->checkBoxIa->isChecked() || ui->checkBoxIb->isChecked() || ui->checkBoxIc->isChecked()) && phase > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if(ui->checkBox3I0->isChecked() && _3I0 > 0)
            ui->pushButtonCalibration->setEnabled(true);
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }
    else if(le == ui->lineEditCurrentStandard3I0)
    {
        if(ui->checkBox3I0->isChecked() && _3I0 > 0)
            ui->pushButtonCalibration->setEnabled(true);
        else if((ui->checkBoxIa->isChecked() || ui->checkBoxIb->isChecked() || ui->checkBoxIc->isChecked()) && phase > 0)
            ui->pushButtonCalibration->setEnabled(true);
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
}
//-------------------------------------------------------------
void CCalibrationWidget::stateChoiceCurrentChannelChanged(bool)
{
    float phase = QLocale::system().toFloat(ui->lineEditCurrentStandardPhase->text());
    float _3I0  = QLocale::system().toFloat(ui->lineEditCurrentStandard3I0->text());

    if(ui->checkBoxIa->isChecked() || ui->checkBoxIb->isChecked() || ui->checkBoxIc->isChecked())
    {
        if(phase > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if(ui->checkBox3I0->isChecked() && _3I0 > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }

    if(ui->checkBox3I0->isChecked())
    {
        if(_3I0 > 0)
            ui->pushButtonCalibration->setEnabled(true);
        else if((ui->checkBoxIa->isChecked() || ui->checkBoxIb->isChecked() || ui->checkBoxIc->isChecked()) &&
                (phase > 0))
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
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
