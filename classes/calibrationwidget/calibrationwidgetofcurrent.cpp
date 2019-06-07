#include "calibrationwidgetofcurrent.h"
#include "ui_calibrationwidgetofcurrent.h"
//------------------------------------------------------------------------
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

    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::calibrationParameterStart);
    connect(ui->pushButtonCalibration, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::stateButton);
    connect(this, &CCalibrationWidgetOfCurrent::calibrationEnd, this, &CCalibrationWidgetOfCurrent::stateButton);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::calibrationWriteProcess);
    connect(ui->lineEditCurrentStandardPhase, &CLineEdit::textChanged, this, &CCalibrationWidgetOfCurrent::valueCurrentStandardChanged);
    connect(ui->lineEditCurrentStandard3I0, &CLineEdit::textChanged, this, &CCalibrationWidgetOfCurrent::valueCurrentStandardChanged);
    connect(ui->checkBoxIa, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->checkBoxIb, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->checkBoxIc, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->checkBox3I0, &QCheckBox::clicked, this, &CCalibrationWidgetOfCurrent::stateChoiceCurrentChannelChanged);
    connect(ui->pushButtonSaveToFlash, &QPushButton::clicked, this, &CCalibrationWidgetOfCurrent::saveCalibrationToFlash);
    connect(this, &CCalibrationWidgetOfCurrent::dataIncrement, this, &CCalibrationWidgetOfCurrent::progressBarIncrement);
}
//---------------------------------------------------------
CCalibrationWidgetOfCurrent::~CCalibrationWidgetOfCurrent()
{
    delete ui;
}
//-----------------------------------------------
bool CCalibrationWidgetOfCurrent::stateIa() const
{
    return ui->checkBoxIa->isChecked();
}
//-----------------------------------------------
bool CCalibrationWidgetOfCurrent::stateIb() const
{
    return ui->checkBoxIb->isChecked();
}
//-----------------------------------------------
bool CCalibrationWidgetOfCurrent::stateIc() const
{
    return ui->checkBoxIc->isChecked();
}
//------------------------------------------------
bool CCalibrationWidgetOfCurrent::state3I0() const
{
    return ui->checkBox3I0->isChecked();
}
//------------------------------------------------
int CCalibrationWidgetOfCurrent::dataCount() const
{
    return ui->spinBoxSetDataCount->value();
}
//---------------------------------------------------
int CCalibrationWidgetOfCurrent::pauseRequest() const
{
    return ui->spinBoxPauseRequest->value();
}
//-------------------------------------------------------------
void CCalibrationWidgetOfCurrent::setStandardPhase(float value)
{
    ui->lineEditCurrentStandardPhase->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setStandard3I0(float value)
{
    ui->lineEditCurrentStandard3I0->setText(QLocale::system().toString(value, 'f', 6));
}
//------------------------------------------------------
void CCalibrationWidgetOfCurrent::setIaState(bool state)
{
    ui->checkBoxIa->setChecked(state);
}
//------------------------------------------------------
void CCalibrationWidgetOfCurrent::setIbState(bool state)
{
    ui->checkBoxIb->setChecked(state);
}
//------------------------------------------------------
void CCalibrationWidgetOfCurrent::setIcState(bool state)
{
    ui->checkBoxIc->setChecked(state);
}
//-------------------------------------------------------
void CCalibrationWidgetOfCurrent::set3I0State(bool state)
{
    ui->checkBox3I0->setChecked(state);
}
//-------------------------------------------------------
void CCalibrationWidgetOfCurrent::setDataCount(int count)
{
    ui->spinBoxSetDataCount->setValue(count);
}
//----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setPauseRequest(int pause)
{
    ui->spinBoxPauseRequest->setValue(pause);
}
//------------------------------------------------------
float CCalibrationWidgetOfCurrent::standardPhase() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandardPhase->text());
}
//----------------------------------------------------
float CCalibrationWidgetOfCurrent::standard3I0() const
{
    return QLocale::system().toFloat(ui->lineEditCurrentStandard3I0->text());
}
//------------------------------------------------
float CCalibrationWidgetOfCurrent::valueIa() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIa->text());
}
//------------------------------------------------
float CCalibrationWidgetOfCurrent::valueIb() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIb->text());
}
//------------------------------------------------
float CCalibrationWidgetOfCurrent::valueIc() const
{
    return QLocale::system().toFloat(ui->lineEditFactorIc->text());
}
//-------------------------------------------------
float CCalibrationWidgetOfCurrent::value3I0() const
{
    return QLocale::system().toFloat(ui->lineEditFactor3I0->text());
}
/*!
 * \brief CCalibrationWidgetOfCurrent::newCalibrationFactor
 * \param standard Эталонное значение тока
 * \param cur_factor Текущий коэффициент
 * \param measure_list Список значений
 * \return новый коэффициент
 *
 * Расчет нового калибровочного коэффициента
 */
float CCalibrationWidgetOfCurrent::newCalibrationFactor(float standard, float cur_factor, const calibration_data_t &measure_list)
{
    float measure = 0;

    if(measure_list.count() > 1)
        standard *= measure_list.count();

    for(float value: measure_list)
        measure += value;

    return (standard/measure)*cur_factor;
}
/*!
 * \brief CCalibrationWidgetOfCurrent::standardDeviation
 * \param data Набор данных
 * \return Среднее значение + среднеквадратичное отклонение ввиде QPointF(среднее значение, среднеквадратичное отклонение)
 */
QPointF CCalibrationWidgetOfCurrent::standardDeviation(const CCalibrationWidgetOfCurrent::calibration_data_t &data)
{
    float average = 0;

    for(float value: data)
        average += value;

    average /= data.count();

    float deviation = 0;

    for(float value: data)
        deviation += (value - average)*(value - average);

    deviation = float(sqrt(double(double(deviation)/double(data.count()) - 1.0)));

    return QPointF(double(average), double(deviation));
}
//--------------------------------------------------------
void CCalibrationWidgetOfCurrent::setFactorIa(float value)
{
    ui->lineEditFactorIa->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------
void CCalibrationWidgetOfCurrent::setFactorIb(float value)
{
    ui->lineEditFactorIb->setText(QLocale::system().toString(value, 'f', 6));
}
//--------------------------------------------------------
void CCalibrationWidgetOfCurrent::setFactorIc(float value)
{
    ui->lineEditFactorIc->setText(QLocale::system().toString(value, 'f', 6));
}
//---------------------------------------------------------
void CCalibrationWidgetOfCurrent::setFactor3I0(float value)
{
    ui->lineEditFactor3I0->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasureIa(float average)
{
    ui->lineEditMeasuredD01->setText(QLocale::system().toString(average, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasureIb(float average)
{
    ui->lineEditMeasuredD02->setText(QLocale::system().toString(average, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasureIc(float average)
{
    ui->lineEditMeasuredD03->setText(QLocale::system().toString(average, 'f', 6));
}
//------------------------------------------------------------
void CCalibrationWidgetOfCurrent::setMeasure3I0(float average)
{
    ui->lineEditMeasuredD04->setText(QLocale::system().toString(average, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviationIa(float value)
{
    ui->lineEditDeviationIa->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviationIb(float value)
{
    ui->lineEditDeviationIb->setText(QLocale::system().toString(value, 'f', 6));
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviationIc(float value)
{
    ui->lineEditDeviationIc->setText(QLocale::system().toString(value, 'f', 6));
}
//------------------------------------------------------------
void CCalibrationWidgetOfCurrent::setDeviation3I0(float value)
{
    ui->lineEditDeviation3I0->setText(QLocale::system().toString(value, 'f', 6));
}
/*!
 * \brief CCalibrationWidgetOfCurrent::display
 * \param data - Данные по калибровке
 *
 * Расчет и вывод новых калибровочных коэффициентов
 */
void CCalibrationWidgetOfCurrent::display(const calibration_t &data)
{
    qInfo() << tr("Калибровка по току:");

    if(!data.Ia.isEmpty())
    {
        float   standard   = QLocale::system().toFloat(ui->lineEditCurrentStandardPhase->text());
        float   cur_factor = QLocale::system().toFloat(ui->lineEditFactorIa->text());
        float   newFactor  = newCalibrationFactor(standard, cur_factor, data.Ia);
        QPointF deviation  = standardDeviation(data.Ia);

        setFactorIa(newFactor);
        setMeasureIa(float(deviation.x()));
        setDeviationIa(float(deviation.y()));

        qInfo() << tr("Калибровка тока фазы А");

        for(float value: data.Ia)
            qInfo() << QString("Значение: %1").arg(QLocale::system().toString(value, 'f', 6));
        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                      arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                      arg(QLocale::system().toString(deviation.y(), 'f', 6));
        qInfo() << tr("Старое калибровочное значение: %1").arg(double(cur_factor));
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    if(!data.Ib.isEmpty())
    {
        float   standard   = standardPhase();
        float   cur_factor = QLocale::system().toFloat(ui->lineEditFactorIb->text());
        float   newFactor  = newCalibrationFactor(standard, cur_factor, data.Ib);
        QPointF deviation  = standardDeviation(data.Ib);

        setFactorIb(newFactor);
        setMeasureIb(float(deviation.x()));
        setDeviationIb(float(deviation.y()));

        qInfo() << tr("Калибровка тока фазы B");

        for(float value: data.Ib)
            qInfo() << QString("Значение: %1").arg(QLocale::system().toString(value, 'f', 6));

        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));
        qInfo() << tr("Старое калибровочное значение: %1").arg(double(cur_factor));
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    if(!data.Ic.isEmpty())
    {
        float   standard   = standardPhase();
        float   cur_factor = QLocale::system().toFloat(ui->lineEditFactorIc->text());
        float   newFactor  = newCalibrationFactor(standard, cur_factor, data.Ic);
        QPointF deviation  = standardDeviation(data.Ic);

        setFactorIc(newFactor);
        setMeasureIc(float(deviation.x()));
        setDeviationIc(float(deviation.y()));

        qInfo() << tr("Калибровка тока фазы C");

        for(float value: data.Ic)
            qInfo() << QString("Значение: %1").arg(QLocale::system().toString(value, 'f', 6));

        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));

        qInfo() << tr("Старое калибровочное значение: %1").arg(double(cur_factor));
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    if(!data._3I0.isEmpty())
    {
        float   standard   = standard3I0();
        float   cur_factor = QLocale::system().toFloat(ui->lineEditFactor3I0->text());
        float   newFactor  = newCalibrationFactor(standard, cur_factor, data._3I0);
        QPointF deviation  = standardDeviation(data._3I0);

        setFactor3I0(newFactor);
        setMeasure3I0(float(deviation.x()));
        setDeviation3I0(float(deviation.y()));

        qInfo() << tr("Калибровка среднего тока 3I0");

        for(float value: data._3I0)
            qInfo() << QString("Значение: %1").arg(QLocale::system().toString(value, 'f', 6));

        qInfo() << QString("Среднее арифметическое: %1 / Среднеквадратическое отклонение: %2").
                   arg(QLocale::system().toString(deviation.x(), 'f', 6)).
                   arg(QLocale::system().toString(deviation.y(), 'f', 6));

        qInfo() << tr("Старое калибровочное значение: %1").arg(double(cur_factor));
        qInfo() << tr("Новое калибровочное значение: %1").arg(QLocale::system().toString(newFactor, 'f', 6));
    }

    emit calibrationEnd(false);
}
//-------------------------------------------------------
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
//---------------------------------------------------------------------------
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
//----------------------------------------------------------------------
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
//--------------------------------------------------------
void CCalibrationWidgetOfCurrent::saveCalibrationToFlash()
{
    int answer = QMessageBox::question(this, tr("Запись калибровок по току в устройство"),
                                             tr("Вы действительно хотите сохранить "
                                                "\nновые калибровочные коэффициенты "
                                                "\nв память устройства?"));
    if(answer == QMessageBox::Yes)
    {
        qInfo() << tr("Сохранение калибровочных коэффициентов по току пользователем во флеш.");
        emit saveToFlash();
    }
    else
        qInfo() << tr("Отказ от сохранения калибровочных коэффициентов по току во флеш.");
}
//------------------------------------------------------
void CCalibrationWidgetOfCurrent::progressBarIncrement()
{
    int count = ui->progressBarDataSet->value();
    int step  = 100/ui->spinBoxSetDataCount->value();
    ui->progressBarDataSet->setValue(count + step);
}
//-----------------------------------------------------------
void CCalibrationWidgetOfCurrent::calibrationParameterStart()
{
    if(!ui->checkBoxIa->isChecked() &&
       !ui->checkBoxIb->isChecked() &&
       !ui->checkBoxIc->isChecked() &&
       !ui->checkBox3I0->isChecked())
    {
        QMessageBox::warning(this, tr("Калибровка по току"), tr("Нет выбранных каналов для калибровки"));
        return;
    }

    CModBusDataUnit unit_Ia(0, CModBusDataUnit::ReadInputRegisters, 64, 2);
    CModBusDataUnit unit_Ib(0, CModBusDataUnit::ReadInputRegisters, 66, 2);
    CModBusDataUnit unit_Ic(0, CModBusDataUnit::ReadInputRegisters, 68, 2);
    CModBusDataUnit unit_3I0(0, CModBusDataUnit::ReadInputRegisters, 70, 2);

    unit_Ia.setProperty("CHANNEL", CURRENT_IA);
    unit_Ib.setProperty("CHANNEL", CURRENT_IB);
    unit_Ic.setProperty("CHANNEL", CURRENT_IC);
    unit_3I0.setProperty("CHANNEL", CURRENT_3I0);

    QVector<CModBusDataUnit> unit_list;
    int param_count = 0;

    if(ui->checkBoxIa->isChecked())
    {
        unit_list << unit_Ia;
        param_count++;
    }
    if(ui->checkBoxIb->isChecked())
    {
        unit_list << unit_Ib;
        param_count++;
    }
    if(ui->checkBoxIc->isChecked())
    {
        unit_list << unit_Ic;
        param_count++;
    }
    if(ui->checkBox3I0->isChecked())
    {
        unit_list << unit_3I0;
        param_count++;
    }

    emit calibrationFactorAllStart();
    emit calibrationStart(unit_list, param_count);
}
//--------------------------------------------------------------------------------------
void CCalibrationWidgetOfCurrent::calibrationDataProcess(QVector<CModBusDataUnit> &data)
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

        if(channel == CURRENT_IA)
            calibration_data.Ia << value.f;
        else if(channel == CURRENT_IB)
            calibration_data.Ib << value.f;
        else if(channel == CURRENT_IC)
            calibration_data.Ic << value.f;
        else if(channel == CURRENT_3I0)
            calibration_data._3I0 << value.f;
    }

    display(calibration_data);
}
//---------------------------------------------------------
void CCalibrationWidgetOfCurrent::calibrationWriteProcess()
{
    float Ia   = 0;
    float Ib   = 0;
    float Ic   = 0;
    float _3I0 = 0;

    if(ui->checkBoxIa->isChecked())
        Ia  = valueIa();
    if(ui->checkBoxIb->isChecked())
        Ib = valueIb();
    if(ui->checkBoxIc->isChecked())
        Ic  = valueIc();
    if(ui->checkBox3I0->isChecked())
        _3I0 = value3I0();

    if(Ia == 0.0f && Ib == 0.0f && Ic == 0.0f && _3I0 == 0.0f)
        return;

    QString str;
    QString textValue;

    textValue += ((Ia != 0.0f)?QString("Ia = %1\n").arg(QLocale::system().toString(Ia, 'f', 6)):"");
    textValue += ((Ib != 0.0f)?QString("Ib = %1\n").arg(QLocale::system().toString(Ib, 'f', 6)):"");
    textValue += ((Ic != 0.0f)?QString("Ic = %1\n").arg(QLocale::system().toString(Ic, 'f', 6)):"");
    textValue += ((_3I0 != 0.0f)?QString("3I0 = %1\n").arg(QLocale::system().toString(_3I0, 'f', 6)):"");

    str = tr("Вы хотите сохранить новые калибровки?\n%1").arg(textValue);
    int res = QMessageBox::question(this, tr("Запись калибровок по току"), str);

    qInfo() << tr("Запись новых калибровочных коэффициентов по току:\n%1").arg(textValue);

    if(res == QMessageBox::No)
    {
        qInfo() << tr("Отказ пользователя от записи калибровочных коэффициетов по току");
        return;
    }

    union
    {
        quint16 i[2];
        float   f;
    } value;

    value.f = Ia;
    CModBusDataUnit unit_Ia(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ia.setProperty("KEY", "KIA");

    value.f = Ib;
    CModBusDataUnit unit_Ib(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ia.setProperty("KEY", "KIB");

    value.f = Ic;
    CModBusDataUnit unit_Ic(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ia.setProperty("KEY", "KIC");

    value.f = _3I0;
    CModBusDataUnit unit_3I0(0, CModBusDataUnit::WriteMultipleRegisters, 0, QVector<quint16>() << value.i[1] << value.i[0]);
    unit_Ia.setProperty("KEY", "K3I0");

    QVector<CModBusDataUnit> units;

    if(Ia != 0.0f)
        units << unit_Ia;
    if(Ib != 0.0f)
        units << unit_Ib;
    if(Ic != 0.0f)
        units << unit_Ic;
    if(_3I0!= 0.0f)
        units << unit_3I0;

    qInfo() << tr("Запись новых калибровочных коэффициентов по току подтверждена");

    emit calibrationWriteStart(units);
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
