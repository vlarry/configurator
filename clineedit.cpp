#include "clineedit.h"
//------------------------------------
CLineEdit::CLineEdit(QWidget* parent):
    QLineEdit(parent),
    m_focus(false)
{
    connect(this, &CLineEdit::textChanged, this, &CLineEdit::validateTextChanged);
}
//-------------------------------------------------------
bool CLineEdit::isValidateText(const QString& text) const
{
    const QDoubleValidator* v = static_cast<const QDoubleValidator*>(validator());

    bool   isNumber = false;
    double value    = QLocale::system().toDouble(text, &isNumber);

    if(isNumber && (value >= v->bottom() && value <= v->top()))
    {
        return true;
    }

    return false;
}
//---------------------------------------------------
void CLineEdit::mouseReleaseEvent(QMouseEvent* event)
{
    if(!m_focus)
    {
        m_focus = true;
        selectAll();
    }
    else
    {
        QString str = selectedText();

        if(str.isEmpty())
            selectAll();
    }

    QLineEdit::mouseReleaseEvent(event);
}
//---------------------------------------------
void CLineEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);

    if(event->key() == Qt::Key_Return)
    {
        selectAll();
    }
}
//-----------------------------------------------
void CLineEdit::focusOutEvent(QFocusEvent* event)
{
    m_focus = false;
    QLineEdit::focusOutEvent(event);
}
//------------------------------------------------------
void CLineEdit::validateTextChanged(const QString& text)
{
    QPalette t_palette = palette();

    if(isValidateText(text))
    {
        t_palette.setColor(QPalette::Text, QColor(Qt::black));
    }
    else
        t_palette.setColor(QPalette::Text, QColor(Qt::red));

    setPalette(t_palette);
}
