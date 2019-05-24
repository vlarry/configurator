#include "ccheckboxinternalvariable.h"
//-----------------------------------------------------------------------------------------
CCheckBoxInternalVariable::CCheckBoxInternalVariable(const QString &text, QWidget *parent):
    QCheckBox(text, parent),
    m_is_changed(false)
{

}
//---------------------------------------------------------
void CCheckBoxInternalVariable::setChangedState(bool state)
{
    m_is_changed = state;
    QTimer::singleShot(500, this, &CCheckBoxInternalVariable::timeoutStateChanged);
}
//---------------------------------------------------
void CCheckBoxInternalVariable::timeoutStateChanged()
{
    m_is_changed = false;
    update();
}
//------------------------------------------------------------
void CCheckBoxInternalVariable::paintEvent(QPaintEvent *event)
{
    QCheckBox::paintEvent(event);

    if(m_is_changed)
    {
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.fillRect(event->rect(), QBrush(QColor(0, 255, 0, 64)));
    }
}
