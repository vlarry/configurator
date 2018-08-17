#include "clineedit.h"
//------------------------------------
CLineEdit::CLineEdit(QWidget* parent):
    QLineEdit(parent),
    m_focus(false),
    m_validator_type(FLOAT)
{
    connect(this, &CLineEdit::textChanged, this, &CLineEdit::validateTextChanged);
}
//-------------------------------------------------------
bool CLineEdit::isValidateText(const QString& text) const
{
    bool   isNumber = false;
    double value    = QLocale::system().toDouble(text, &isNumber);

    float bottom = 0;
    float top    = 0;

    if(validatorType() == INT)
    {
        const QIntValidator* intValidator = dynamic_cast<const QIntValidator*>(validator());

        if(intValidator)
        {
            bottom = intValidator->bottom();
            top    = intValidator->top();
        }
    }
    else if(validatorType() == FLOAT)
    {
        const QDoubleValidator* floatValidator = dynamic_cast<const QDoubleValidator*>(validator());

        if(floatValidator)
        {
            bottom = floatValidator->bottom();
            top    = floatValidator->top();
        }
    }

    if(isNumber && (value >= bottom && value <= top))
    {
        return true;
    }

    return false;
}
//-------------------------------------------------------------
void CLineEdit::setValidatorType(CLineEdit::ValidatorType type)
{
    m_validator_type = type;
}
//-------------------------------------------------------
CLineEdit::ValidatorType CLineEdit::validatorType() const
{
    return m_validator_type;
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
