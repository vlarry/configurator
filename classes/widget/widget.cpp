#include "widget.h"
//--------------------------------
CWidget::CWidget(QWidget* parent):
    QWidget(parent),
    m_context_menu(nullptr),
    m_interval(nullptr),
    m_tim_interval(nullptr),
    m_repeat_time(500)
{
    m_context_menu = new QMenu(this);
    m_interval     = new CIntervalDialog(tr("Интервал опроса"), this);
    m_tim_interval = new QTimer(this);

    m_context_menu->addAction(tr("Очистить"), this, &CWidget::clear);
    m_context_menu->addAction(tr("Прочитать"), this, &CWidget::read);
    m_context_menu->addAction(tr("Интервал"), this, &CWidget::setInterval);

    connect(m_interval, &CIntervalDialog::repeat, this, &CWidget::repeatState);
    connect(m_tim_interval, &QTimer::timeout, this, &CWidget::repeatTimeout);
}
//-------------------------
void CWidget::setInterval()
{
    if(m_interval->exec() == QDialog::Accepted)
    {
        m_repeat_time = m_interval->interval();

        if(m_tim_interval->isActive())
        {
            m_tim_interval->stop();
            m_tim_interval->start(m_repeat_time);
        }
    }
}
//-----------------------------------
void CWidget::repeatState(bool state)
{
    if(state)
    {
        m_tim_interval->start(m_repeat_time);
    }
    else
        m_tim_interval->stop();
}
//---------------------------
void CWidget::repeatTimeout()
{
    emit read();
}
//------------------------------------------------------
void CWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if(m_context_menu)
        m_context_menu->exec(event->globalPos());
}
//-------------------------------------------
void CWidget::keyPressEvent(QKeyEvent* event)
{
    bool isAlt  = QGuiApplication::keyboardModifiers()&Qt::AltModifier;
    bool isCtrl = QGuiApplication::keyboardModifiers()&Qt::ControlModifier;
    int  key    = event->key();

    emit pressKey(isAlt, isCtrl, key);
}
//------------------------------------------
void CWidget::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    hide();
    m_tim_interval->stop();
}
//----------------------------------------
void CWidget::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    repeatState(m_interval->isRepeatChecked());
}
