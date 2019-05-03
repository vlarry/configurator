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
    ui->progressBar->setMaximumWidth(300);
    ui->progressBar->setMinimumWidth(300);

    ui->lblProgressbarTitle->setAlignment(Qt::AlignCenter);

    connect(this, &CProgressBarWidget::increment, this, &CProgressBarWidget::progressIncrement);
    connect(this, &CProgressBarWidget::settingChanged, this, &CProgressBarWidget::setSettings);

    setWindowFlags(Qt::FramelessWindowHint);
}
//---------------------------------------
CProgressBarWidget::~CProgressBarWidget()
{
    delete ui;
}
//--------------------------------------
void CProgressBarWidget::progressStart()
{
    if(!parent())
        return;

    QWidget* widget = qobject_cast<QWidget*>(parent());

    if(!widget)
        return;

    QSize s = widget->geometry().size();

    QFont f = ui->lblProgressbarTitle->font();
    QFontMetrics fm(f);

    int w_label = fm.width(ui->lblProgressbarTitle->text())*1.2f;
    ui->lblProgressbarTitle->setFixedWidth(w_label);
    setFixedWidth(w_label + 300);

    int x = s.width()/2 - size().width()/2;
    int y = s.height()/2 - size().height()/2;

    move(x, y);
    show();
}
//-------------------------------------
void CProgressBarWidget::progressStop()
{
    Sleep(500);

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
    QString text = QString::number(value) + " " + tr("%1 из %2 %3").arg(m_format).arg(ui->progressBar->maximum()).arg(m_format);

    ui->progressBar->setFormat(text);
    ui->progressBar->setValue(value);
}
//---------------------------------------------------------------------------
void CProgressBarWidget::setSettings(int min, int max, const QString& format)
{
    ui->progressBar->reset();
    ui->progressBar->resetFormat();
    ui->progressBar->setFormat(format);
    ui->progressBar->setRange(min, max);
    ui->progressBar->setValue(min);

    if(!format.isEmpty())
        m_format = format;

    if(ui->lblProgressbarTitle->isHidden())
        ui->lblProgressbarTitle->setVisible(true);
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
