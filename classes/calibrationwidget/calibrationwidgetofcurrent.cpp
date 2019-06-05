#include "calibrationwidgetofcurrent.h"
#include "ui_calibrationwidgetofcurrent.h"
//------------------------------------------------------
CCalibrationWidgetOfCurrent::CCalibrationWidgetOfCurrent(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetOfCurrent)
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

    ui->progressBarDataSet->hide();

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::calibration);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::stateButton);
    connect(this, &CCalibrationWidgetOfCurrent::calibrationEnd, this, &CCalibrationWidgetOfCurrent::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::apply);
    connect(ui->lineEditCurrentStandardPhase, &QLineEdit::textChanged, this, &CCalibrationWidgetOfCurrent::valueCurrentStandardChanged);
    connect(ui->lineEditCurrentStandard3I0, &QLineEdit::textChanged, this, &CCalibrationWidgetOfCurrent::valueCurrentStandardChanged);
    connect(ui->checkBoxIa, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->checkBoxIb, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->checkBoxIc, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->checkBox3I0, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::saveCalibrationToFlash);
    connect(this, &CCalibrationWidgetOfCurrent::dataIncrement, this, &CCalibrationWidgetOfCurrent::progressBarIncrement);
}
//---------------------------------------
CCalibrationWidgetOfCurrent::~CCalibrationWidgetOfCurrent()
{
    delete ui;
}
//-------------------------------------------
QCheckBox* CCalibrationWidgetOfCurrent::ctrlIa() const
{
    return ui->checkBoxIa;
}
//-------------------------------------------
QCheckBox* CCalibrationWidgetOfCurrent::ctrlIb() const
{
    return ui->checkBoxIb;
}
//-------------------------------------------
QCheckBox* CCalibrationWidgetOfCurrent::ctrlIc() const
{
    return ui->checkBoxIc;
}
//--------------------------------------------
QCheckBox* CCalibrationWidgetOfCurrent::ctrl3I0() const
{
    return ui->checkBox3I0;
}
//------------------------------------------
int CCalibrationWidgetOfCurrent::dataSetCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//----------------------------------------------
int CCalibrationWidgetOfCurrent::timePauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::addCalibrationIa(float value)
{
    m_calibration_current_data.Ia << value;
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::addCalibrationIb(float value)
{
    m_calibration_current_data.Ib << value;
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::addCalibrationIc(float value)
{
    m_calibration_current_data.Ic << value;
}
//-----------------------------------------------------
void CCalibrationWidgetOfCurrent::addCalibration3I0(float value)
{
    m_calibration_current_data._3I0 << value;
}
//---------------------------------------------------------------------------------------------
const CCalibrationWidgetOfCurrent::calibration_current_t& CCalibrationWidgetOfCurrent::calibrationCurrent() const
{
    return m_calibration_current_data;
}
//---------------------------------------------------------------
float CCalibrationWidgetOfCurrent::calibrationCurrentStandardPhase() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandardPhase->text());
}
//-------------------------------------------------------------
float CCalibrationWidgetOfCurrent::calibrationCurrentStandard3I0() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandard3I0->text());
}
//----------------------------------------------------
float CCalibrationWidgetOfCurrent::calibrationCurrentIa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIa->text());
}
//----------------------------------------------------
float CCalibrationWidgetOfCurrent::calibrationCurrentIb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIb->text());
}
//----------------------------------------------------
float CCalibrationWidgetOfCurrent::calibrationCurrentIc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIc->text());
}
//-----------------------------------------------------
float CCalibrationWidgetOfCurrent::calibrationCurrent3I0() const
{
    return QLocale::system().toFloat(ui->lineEditFactor3I0->text());
}
//------------------------------------------------
void CCalibrationWidgetOfCurrent::calibrationCurrentClear()
{
    m_calibration_current_data = calibration_current_t({ calibration_data_t(0), calibration_data_t(0), calibration_data_t(0),
                                                         calibration_data_t(0) });
}
//--------------------------------------------------------
bool CCalibrationWidgetOfCurrent::calibrationCurrentIaState() const
{
    return ui->checkBoxIa->isChecked();
}
//--------------------------------------------------------
bool CCalibrationWidgetOfCurrent::calibrationCurrentIbState() const
{
    return ui->checkBoxIb->isChecked();
}
//--------------------------------------------------------
bool CCalibrationWidgetOfCurrent::calibrationCurrentIcState() const
{
    return ui->checkBoxIc->isChecked();
}
//---------------------------------------------------------
bool CCalibrationWidgetOfCurrent::calibrationCurrent3I0State() const
{
    return ui->checkBox3I0->isChecked();
}
//---------------------------------------------------------
int CCalibrationWidgetOfCurrent::calibrationCurrentDataCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//------------------------------------------------------------
int CCalibrationWidgetOfCurrent::calibrationCurrentPauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentStandardPhase(float value)
{
    ui->lineEditCurrentStandardPhase->setText(QLocale::system().toString(value, 'f', 6));
}
//---------------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentStandard3I0(float value)
{
    ui->lineEditCurrentStandard3I0->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentIaState(bool state)
{
    ui->checkBoxIa->setChecked(state);
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentIbState(bool state)
{
    ui->checkBoxIb->setChecked(state);
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentIcState(bool state)
{
    ui->checkBoxIc->setChecked(state);
}
//----------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrent3I0State(bool state)
{
    ui->checkBox3I0->setChecked(state);
}
//-----------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentDataCount(int count)
{
    ui->spinBoxSetDataCount->setValue(count);
}
//--------------------------------------------------------
void CCalibrationWidgetOfCurrent::setCurrentPauseRequest(int pause)
{
    ui->spinBoxPauseRequest->setValue(pause);
}
//-----------------------------------------------
void CCalibrationWidgetOfCurrent::setFactorIa(float value)
{
    ui->lineEditFactorIa->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------
void CCalibrationWidgetOfCurrent::setFactorIb(float value)
{
    ui->lineEditFactorIb->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------
void CCalibrationWidgetOfCurrent::setFactorIc(float value)
{
    ui->lineEditFactorIc->setText(QLocale::system().toString(value, 'f', 6));
}
//------------------------------------------------
void CCalibrationWidgetOfCurrent::setFactor3I0(float value)
{
    ui->lineEditFactor3I0->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasureIa(float average)
{
    ui->lineEditMeasuredD01->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasureIb(float average)
{
    ui->lineEditMeasuredD02->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasureIc(float average)
{
    ui->lineEditMeasuredD03->setText(QLocale::system().toString(average, 'f', 6));
}
//---------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasure3I0(float average)
{
    ui->lineEditMeasuredD04->setText(QLocale::system().toString(average, 'f', 6));
}
//--------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviationIa(float value)
{
    ui->lineEditDeviationIa->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviationIb(float value)
{
    ui->lineEditDeviationIb->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviationIc(float value)
{
    ui->lineEditDeviationIc->setText(QLocale::system().toString(value, 'f', 6));
}
//---------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviation3I0(float value)
{
    ui->lineEditDeviation3I0->setText(QLocale::system().toString(value, 'f', 6));
}
//----------------------------------------------
void CCalibrationWidgetOfCurrent::stateButton(bool state)
{
    ui->pushButtonCalibration->setEnabled(!state);
    ui->pushButtonApply->setEnabled(!state);
    ui->pushButtonSaveToFlash->setEnabled(!state);
    ui->pushButtonCalibration->setChecked(state);
    ui->progressBarDataSet->setVisible(state);

    if(state)
        ui->progressBarDataSet->setValue(0);
}
//------------------------------------------------------------------
void CCalibrationWidgetOfCurrent::valueCurrentStandardChanged(const QString&)
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
void CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged(bool)
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
//-----------------------------------------------
void CCalibrationWidgetOfCurrent::saveCalibrationToFlash()
{
    int answer = QMessageBox::question(this, tr("Запись калибровок по току в устройство"),
                                             tr("Вы действительно хотите сохранить "
                                                "\nновые калибровочные коэффициенты "
                                                "\nв память устройства?"));
    if(answer == QMessageBox::Yes)
    {
        qInfo() << tr("Сохранение калибровочных коэффициентов по току пользователем во флеш.");
        emit saveToFlash(2);
    }
    else
        qInfo() << tr("Отказ от сохранения калибровочных коэффициентов по току во флеш.");
}
//---------------------------------------------
void CCalibrationWidgetOfCurrent::progressBarIncrement()
{
    int count = ui->progressBarDataSet->value();
    int step  = 100/ui->spinBoxSetDataCount->value();
    ui->progressBarDataSet->setValue(count + step);
}
//--------------------------------------------------------------
void CCalibrationWidgetOfCurrent::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    QRect r = ui->gridLayout->cellRect(1, 1);
    QPoint topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout->cellRect(2, 1);
    QPoint centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    QPoint centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout->cellRect(3, 1);
    QPoint bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout->cellRect(5, 1);
    QPoint leftCenter3I0 = QPoint(r.left(), r.top() + r.height()/2);
    QPoint rightCenter3I0 = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);
    painter.drawLine(leftCenter3I0, rightCenter3I0);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
