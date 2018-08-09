#include "cprogressbarwidget.h"
#include "ui_progressbarwidget.h"
//------------------------------------------------------
CProgressBarWidget::CProgressBarWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CProgressBarWidget)
{
    ui->setupUi(this);
    hide();

    ui->progressBar->setRange(0, 0);

    connect(this, &CProgressBarWidget::increment, this, &CProgressBarWidget::progressIncrement);
    connect(this, &CProgressBarWidget::settingChanged, this, &CProgressBarWidget::setSettings);
}
//---------------------------------------
CProgressBarWidget::~CProgressBarWidget()
{
    delete ui;
}
//--------------------------------------
void CProgressBarWidget::progressStart()
{
    show();
}
//-------------------------------------
void CProgressBarWidget::progressStop()
{
    ui->progressBar->reset();
    ui->progressBar->resetFormat();
    ui->progressBar->setFormat("");
    ui->progressBar->setRange(0, 0);
    ui->progressBar->setValue(0);
    ui->lblProgressbarTitle->clear();

    hide();
}
//-------------------------------------------
void CProgressBarWidget::progressTitleClear()
{
    ui->lblProgressbarTitle->clear();
}
//-------------------------------------
int CProgressBarWidget::progressValue()
{
    return ui->progressBar->value();
}
//-------------------------------------------------------------
void CProgressBarWidget::setProgressFormat(const QString& text)
{
    m_format = text;
}
//---------------------------------------------------------
void CProgressBarWidget::setProgressRange(int min, int max)
{
    ui->progressBar->setRange(min, max);
}
//-------------------------------------------------------------
void CProgressBarWidget::setProgressTitle(const QString& title)
{
    ui->lblProgressbarTitle->setText(title);
}
//--------------------------------------------------
void CProgressBarWidget::setProgressValue(int value)
{
    ui->progressBar->setValue(value);
}
//---------------------------------------------------------------------------
void CProgressBarWidget::setSettings(int min, int max, const QString& format)
{
    ui->progressBar->reset();
    ui->progressBar->resetFormat();
    ui->progressBar->setFormat("");
    ui->progressBar->setRange(min, max);
    ui->progressBar->setValue(min);

    if(!format.isEmpty())
        m_format = format;
}
//---------------------------------------------------
void CProgressBarWidget::progressIncrement(int value)
{
    int val = ui->progressBar->value();

    val += value;

    QString text = QString::number(val) + " " + tr("%1 из %2 %3").arg(m_format).arg(ui->progressBar->maximum()).
                                                                                arg(m_format);

    ui->progressBar->setValue(val);
    ui->progressBar->setFormat(text);
}
