#include "calibrationwidgetpower.h"
#include "ui_calibrationwidgetpower.h"
//----------------------------------------------------------------
CCalibrationWidgetPower::CCalibrationWidgetPower(QWidget *parent):
    QWidget(parent),
    ui(new Ui::CCalibrationWidgetPower)
{
    ui->setupUi(this);

    ui->pushButtonCalibration->setDisabled(true);

    QDoubleValidator* validator = new QDoubleValidator(0.000001, 10000, 6, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    ui->lineEditPowerStandardPhase->setValidator(validator);
    ui->lineEditPowerStandardPhaseLinear->setValidator(validator);
    ui->lineEditPowerStandard3U->setValidator(validator); // эталонное значение постоянной состовляющей для 3U0S, 3US и 3U0
    ui->lineEditFactorUA->setValidator(validator);
    ui->lineEditFactorUB->setValidator(validator);
    ui->lineEditFactorUC->setValidator(validator);
    ui->lineEditFactorUAB->setValidator(validator);
    ui->lineEditFactorUBC->setValidator(validator);
    ui->lineEditFactorUCA->setValidator(validator);
    ui->lineEditFactor3U0S->setValidator(validator);
    ui->lineEditFactor3US->setValidator(validator);
    ui->lineEditFactor3U0->setValidator(validator);

    ui->progressBarDataSet->hide();

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetPower::calibrationParameterStart);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetPower::stateButton);
    connect(this, &CCalibrationWidgetPower::calibrationEnd, this, &CCalibrationWidgetPower::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetPower::calibrationWriteProcess);
    connect(ui->lineEditPowerStandardPhase, &CLineEdit::textChanged, this, &CCalibrationWidgetPower::valueCurrentStandardChanged);
//    connect(ui->lineEditPowerStandardPhaseLinear, &QLineEdit::textChanged, this, &CCalibrationWidgetPower::valueCurrentStandardChanged);
//    connect(ui->lineEditPowerStandardDC, &CLineEdit::textChanged, this, &CCalibrationWidgetPower::valueCurrentStandardChanged);
//    connect(ui->checkBoxIa, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->checkBoxIb, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->checkBoxIc, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
//    connect(ui->checkBox3I0, &QCheckBox::clicked, this, &CCalibrationWidgetPower::stateChoiceCurrentChannelChanged);
    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetPower::saveCalibrationToFlash);
//    connect(this, &CCalibrationWidgetPower::dataIncrement, this, &CCalibrationWidgetPower::progressBarIncrement);
}
//-------------------------------------------------
CCalibrationWidgetPower::~CCalibrationWidgetPower()
{
    delete ui;
}
//--------------------------------------------
int CCalibrationWidgetPower::dataCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//-----------------------------------------------
int CCalibrationWidgetPower::pauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//-------------------------------------------
bool CCalibrationWidgetPower::stateUa() const
{
    return ui->checkBoxUA->isChecked();
}
//-------------------------------------------
bool CCalibrationWidgetPower::stateUb() const
{
    return ui->checkBoxUB->isChecked();
}
//-------------------------------------------
bool CCalibrationWidgetPower::stateUc() const
{
    return ui->checkBoxUC->isChecked();
}
//--------------------------------------------
bool CCalibrationWidgetPower::stateUab() const
{
    return ui->checkBoxUAB->isChecked();
}
//--------------------------------------------
bool CCalibrationWidgetPower::stateUbc() const
{
    return ui->checkBoxUBC->isChecked();
}
//--------------------------------------------
bool CCalibrationWidgetPower::stateUca() const
{
    return ui->checkBoxUCA->isChecked();
}
//---------------------------------------------
bool CCalibrationWidgetPower::state3U0S() const
{
    return ui->checkBox3U0S->isChecked();
}
//--------------------------------------------
bool CCalibrationWidgetPower::state3US() const
{
    return ui->checkBox3US->isChecked();
}
//--------------------------------------------
bool CCalibrationWidgetPower::state3I0() const
{
    return ui->checkBox3U0->isChecked();
}
//--------------------------------------------
float CCalibrationWidgetPower::valueUa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUA->text());
}
//--------------------------------------------
float CCalibrationWidgetPower::valueUb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUB->text());
}
//--------------------------------------------
float CCalibrationWidgetPower::valueUc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUC->text());
}
//---------------------------------------------
float CCalibrationWidgetPower::valueUab() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUAB->text());
}
//---------------------------------------------
float CCalibrationWidgetPower::valueUbc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUBC->text());
}
//---------------------------------------------
float CCalibrationWidgetPower::valueUca() const
{
    return QLocale::system().toFloat(ui->lineEditFactorUCA->text());
}
//----------------------------------------------
float CCalibrationWidgetPower::value3U0S() const
{
    return QLocale::system().toFloat(ui->lineEditFactor3U0S->text());
}
//---------------------------------------------
float CCalibrationWidgetPower::value3US() const
{
    return QLocale::system().toFloat(ui->lineEditFactor3US->text());
}
//---------------------------------------------
float CCalibrationWidgetPower::value3U0() const
{
    return QLocale::system().toFloat(ui->lineEditFactor3U0->text());
}
//---------------------------------------------------
void CCalibrationWidgetPower::stateButton(bool state)
{
    ui->pushButtonCalibration->setEnabled(!state);
    ui->pushButtonApply->setEnabled(!state);
    ui->pushButtonSaveToFlash->setEnabled(!state);
    ui->pushButtonCalibration->setChecked(state);
    ui->progressBarDataSet->setVisible(state);

    if(state)
        ui->progressBarDataSet->setValue(0);
}
//----------------------------------------------------
void CCalibrationWidgetPower::saveCalibrationToFlash()
{
    int answer = QMessageBox::question(this, tr("Запись калибровок по напряжению в устройство"),
                                             tr("Вы действительно хотите сохранить "
                                                "\nновые калибровочные коэффициенты "
                                                "\nв память устройства?"));
    if(answer == QMessageBox::Yes)
    {
        qInfo() << tr("Сохранение калибровочных коэффициентов по напряжению пользователем во флеш.");
        emit saveToFlash();
    }
    else
        qInfo() << tr("Отказ от сохранения калибровочных коэффициентов по напряжению во флеш.");
}
//-----------------------------------------------------------------------
void CCalibrationWidgetPower::valueCurrentStandardChanged(const QString&)
{
    CLineEdit* le    = qobject_cast<CLineEdit*>(sender());
    float      phase = QLocale::system().toFloat(ui->lineEditPowerStandardPhase->text());
    float      linear = QLocale::system().toFloat(ui->lineEditPowerStandardPhaseLinear->text());
    float      _3U   = QLocale::system().toFloat(ui->lineEditPowerStandard3U->text());

    if(le == ui->lineEditPowerStandardPhase)
    {
        if((ui->checkBoxUA->isChecked() ||
            ui->checkBoxUB->isChecked() ||
            ui->checkBoxUC->isChecked()) && phase > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if((ui->checkBoxUAB->isChecked() ||
                 ui->checkBoxUBC->isChecked() ||
                 ui->checkBoxUCA->isChecked()) && linear > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if((ui->checkBox3U0S->isChecked() ||
                 ui->checkBox3US->isChecked()  ||
                 ui->checkBox3U0->isChecked()) && _3U > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }
    else if(le == ui->lineEditPowerStandardPhaseLinear)
    {
        if((ui->checkBoxUAB->isChecked() ||
            ui->checkBoxUBC->isChecked() ||
            ui->checkBoxUCA->isChecked()) && linear > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if((ui->checkBoxUA->isChecked() ||
                 ui->checkBoxUB->isChecked() ||
                 ui->checkBoxUC->isChecked()) && phase > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if((ui->checkBox3U0S->isChecked() ||
                 ui->checkBox3US->isChecked()  ||
                 ui->checkBox3U0->isChecked()) && _3U > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }
    else if(le == ui->lineEditPowerStandard3U)
    {
        if((ui->checkBox3U0S->isChecked() ||
            ui->checkBox3US->isChecked()  ||
            ui->checkBox3U0->isChecked()) && _3U > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else if((ui->checkBoxUA->isChecked() ||
                 ui->checkBoxUB->isChecked() ||
                 ui->checkBoxUC->isChecked()) && phase > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        if((ui->checkBoxUAB->isChecked() ||
            ui->checkBoxUBC->isChecked() ||
            ui->checkBoxUCA->isChecked()) && linear > 0)
        {
            ui->pushButtonCalibration->setEnabled(true);
        }
        else
            ui->pushButtonCalibration->setDisabled(true);

        return;
    }

    ui->pushButtonCalibration->setDisabled(true);
}
//-------------------------------------------------------
void CCalibrationWidgetPower::calibrationParameterStart()
{
    if(!ui->checkBoxUA->isChecked() &&
       !ui->checkBoxUB->isChecked() &&
       !ui->checkBoxUC->isChecked() &&
       !ui->checkBoxUAB->isChecked() &&
       !ui->checkBoxUBC->isChecked() &&
       !ui->checkBoxUCA->isChecked() &&
       !ui->checkBox3U0S->isChecked() &&
       !ui->checkBox3US->isChecked() &&
       !ui->checkBox3U0->isChecked())
    {
        QMessageBox::warning(this, tr("Калибровка по напряжению"), tr("Нет выбранных каналов для калибровки"));
        return;
    }

    CModBusDataUnit unit_Ua(0, CModBusDataUnit::ReadInputRegisters, 80, 2);
    CModBusDataUnit unit_Ub(0, CModBusDataUnit::ReadInputRegisters, 82, 2);
    CModBusDataUnit unit_Uc(0, CModBusDataUnit::ReadInputRegisters, 84, 2);
    CModBusDataUnit unit_Uab(0, CModBusDataUnit::ReadInputRegisters, 130, 2);
    CModBusDataUnit unit_Ubc(0, CModBusDataUnit::ReadInputRegisters, 132, 2);
    CModBusDataUnit unit_Uca(0, CModBusDataUnit::ReadInputRegisters, 134, 2);
    CModBusDataUnit unit_3U0S(0, CModBusDataUnit::ReadInputRegisters, -1, 2);
    CModBusDataUnit unit_3US(0, CModBusDataUnit::ReadInputRegisters, -1, 2);
    CModBusDataUnit unit_3U0(0, CModBusDataUnit::ReadInputRegisters, -1, 2);

    unit_Ua.setProperty("CHANNEL", CURRENT_UA);
    unit_Ub.setProperty("CHANNEL", CURRENT_UB);
    unit_Uc.setProperty("CHANNEL", CURRENT_UC);
    unit_Uab.setProperty("CHANNEL", CURRENT_UAB);
    unit_Ubc.setProperty("CHANNEL", CURRENT_UBC);
    unit_Uca.setProperty("CHANNEL", CURRENT_UCA);
    unit_3U0S.setProperty("CHANNEL", CURRENT_3U0S);
    unit_3US.setProperty("CHANNEL", CURRENT_3US);
    unit_3U0.setProperty("CHANNEL", CURRENT_3U0);

    QVector<CModBusDataUnit> unit_list;
    int param_count = 0;

    if(ui->checkBoxUA->isChecked())
    {
        unit_list << unit_Ua;
        param_count++;
    }
    if(ui->checkBoxUB->isChecked())
    {
        unit_list << unit_Ub;
        param_count++;
    }
    if(ui->checkBoxUC->isChecked())
    {
        unit_list << unit_Uc;
        param_count++;
    }
    if(ui->checkBoxUAB->isChecked())
    {
        unit_list << unit_Uab;
        param_count++;
    }
    if(ui->checkBoxUBC->isChecked())
    {
        unit_list << unit_Ubc;
        param_count++;
    }
    if(ui->checkBoxUCA->isChecked())
    {
        unit_list << unit_Uca;
        param_count++;
    }
    if(ui->checkBox3U0S->isChecked())
    {
        unit_list << unit_3U0S;
        param_count++;
    }
    if(ui->checkBox3US->isChecked())
    {
        unit_list << unit_3US;
        param_count++;
    }
    if(ui->checkBox3U0->isChecked())
    {
        unit_list << unit_3U0;
        param_count++;
    }

    emit calibrationFactorAllStart();
    emit calibrationStart(unit_list, param_count);
}
//----------------------------------------------------------------------------------
void CCalibrationWidgetPower::calibrationDataProcess(QVector<CModBusDataUnit> &data)
{
    if(data.isEmpty())
        return;
}
//-----------------------------------------------------
void CCalibrationWidgetPower::calibrationWriteProcess()
{
    float Ua    = 0;
    float Ub    = 0;
    float Uc    = 0;
    float Uab   = 0;
    float Ubc   = 0;
    float Uca   = 0;
    float _3U0S = 0;
    float _3US  = 0;
    float _3U0  = 0;

    if(ui->checkBoxUA->isChecked())
        Ua = valueUa();
    if(ui->checkBoxUB->isChecked())
        Ub = valueUb();
    if(ui->checkBoxUC->isChecked())
        Uc = valueUc();
    if(ui->checkBoxUAB->isChecked())
        Uab = valueUab();
    if(ui->checkBoxUBC->isChecked())
        Ubc = valueUbc();
    if(ui->checkBoxUCA->isChecked())
        Uca = valueUca();
    if(ui->checkBox3U0S->isChecked())
        _3U0S = value3U0S();
    if(ui->checkBox3US->isChecked())
        _3US = value3US();
    if(ui->checkBox3U0->isChecked())
        _3U0 = value3U0();

    if(Ua == 0.0f && Ub == 0.0f && Uc == 0.0f && Uab == 0.0f && Ubc == 0.0f && Uca == 0.0f && _3U0S == 0.0f && _3US == 0.0f && _3U0 == 0.0f)
        return;

    QString str;
    QString textValue;

    textValue += ((Ua != 0.0f)?QString("Ua = %1\n").arg(QLocale::system().toString(Ua, 'f', 6)):"");
    textValue += ((Ub != 0.0f)?QString("Ub = %1\n").arg(QLocale::system().toString(Ub, 'f', 6)):"");
    textValue += ((Uc != 0.0f)?QString("Uc = %1\n").arg(QLocale::system().toString(Uc, 'f', 6)):"");
    textValue += ((Uab != 0.0f)?QString("Uab = %1\n").arg(QLocale::system().toString(Uab, 'f', 6)):"");
    textValue += ((Ubc != 0.0f)?QString("Ubc = %1\n").arg(QLocale::system().toString(Ubc, 'f', 6)):"");
    textValue += ((Uca != 0.0f)?QString("Uca = %1\n").arg(QLocale::system().toString(Uca, 'f', 6)):"");
    textValue += ((_3U0S != 0.0f)?QString("3U0S = %1\n").arg(QLocale::system().toString(_3U0S, 'f', 6)):"");
    textValue += ((_3US != 0.0f)?QString("3US = %1\n").arg(QLocale::system().toString(_3US, 'f', 6)):"");
    textValue += ((_3U0 != 0.0f)?QString("3U0 = %1\n").arg(QLocale::system().toString(_3U0, 'f', 6)):"");

    str = tr("Вы хотите сохранить новые калибровки?\n%1").arg(textValue);
    int res = QMessageBox::question(this, tr("Запись калибровок по напряжению"), str);

    qInfo() << tr("Запись новых калибровочных коэффициентов по напряжению:\n%1").arg(textValue);

    if(res == QMessageBox::No)
    {
        qInfo() << tr("Отказ пользователя от записи калибровочных коэффициетов по напряжению");
        return;
    }

    union
    {
        quint16 i[2];
        float   f;
    } value;

    value.f = Ua;
    CModBusDataUnit unit_Ua(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ua.setProperty("KEY", "KUA");

    value.f = Ub;
    CModBusDataUnit unit_Ub(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ub.setProperty("KEY", "KUB");

    value.f = Uc;
    CModBusDataUnit unit_Uc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Uc.setProperty("KEY", "KUC");

    value.f = Uab;
    CModBusDataUnit unit_Uab(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Uab.setProperty("KEY", "KUAB");

    value.f = Ubc;
    CModBusDataUnit unit_Ubc(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ubc.setProperty("KEY", "KUBC");

    value.f = Uca;
    CModBusDataUnit unit_Uca(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Uca.setProperty("KEY", "KUCA");

    value.f = _3U0S;
    CModBusDataUnit unit_3U0S(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_3U0S.setProperty("KEY", "K3U0S");

    value.f = _3US;
    CModBusDataUnit unit_3US(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_3US.setProperty("KEY", "K3US");

    value.f = _3U0;
    CModBusDataUnit unit_3U0(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_3U0.setProperty("KEY", "K3U0");

    QVector<CModBusDataUnit> units;

    if(Ua != 0.0f)
        units << unit_Ua;
    if(Ub != 0.0f)
        units << unit_Ub;
    if(Uc != 0.0f)
        units << unit_Uc;
    if(Uab != 0.0f)
        units << unit_Uab;
    if(Ubc != 0.0f)
        units << unit_Ubc;
    if(Uca != 0.0f)
        units << unit_Uca;
    if(_3U0S != 0.0f)
        units << unit_3U0S;
    if(_3US != 0.0f)
        units << unit_3US;
    if(_3U0 != 0.0f)
        units << unit_3U0;

    qInfo() << tr("Запись новых калибровочных коэффициентов по напряжению подтверждена");

    emit calibrationWriteStart(units);
}
//----------------------------------------------------------
void CCalibrationWidgetPower::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    // Рисование отрезков перед фазами UA, UB и UC
    QRect r = ui->gridLayout_1->cellRect(1, 1);
    QPoint topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_1->cellRect(2, 1);
    QPoint centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    QPoint centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_1->cellRect(3, 1);
    QPoint bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    QPoint bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    // Рисование отрезков перед фазами UAB, UBC и UCA
    r = ui->gridLayout_2->cellRect(0, 1);
    topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_2->cellRect(1, 1);
    centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_2->cellRect(2, 1);
    bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    // Рисование отрезков перед постоянной составляющей 3U0S, 3US и 3U0
    r = ui->gridLayout_3->cellRect(0, 1);
    topCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    topRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_3->cellRect(1, 1);
    centerLeft = QPoint(r.left(), r.top() + r.height()/2);
    centerRight = QPoint(r.right(), r.top() + r.height()/2);
    r = ui->gridLayout_3->cellRect(2, 1);
    bottomCenter = QPoint(r.left() + r.width()/2, r.top() + r.height()/2);
    bottomRight = QPoint(r.right(), r.top() + r.height()/2);
    painter.drawLine(topCenter, bottomCenter);
    painter.drawLine(topCenter, topRight);
    painter.drawLine(bottomCenter, bottomRight);
    painter.drawLine(centerLeft, centerRight);

    painter.drawRect(ui->verticalLayoutCentral->geometry());
}
