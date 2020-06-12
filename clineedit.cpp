#include "clineedit.h"
//------------------------------------
CLineEdit::CLineEdit(QWidget* parent):
    QLineEdit(parent),
    m_focus(false),
    m_validator_type(FLOAT),
    m_is_edit(false),
    m_range({ 0.0f, 0.0f, 0.0f })
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
            bottom = static_cast<float>(floatValidator->bottom());
            top    = static_cast<float>(floatValidator->top());
        }
        else
            return true;
    }

    if(isNumber && ((value + 0.000001) >= static_cast<double>(bottom) && (value - 0.000001) <= static_cast<double>(top))) // 0.000001 из-за округления (0.1 представляется как 0.09)
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
//----------------------------
bool CLineEdit::isEdit() const
{
    return m_is_edit;
}
//--------------------------------------------------
const CLineEdit::RangeType &CLineEdit::range() const
{
    return m_range;
}
//---------------------------
void CLineEdit::resetIsEdit()
{
    m_is_edit = false;
}
//------------------------------
void CLineEdit::resetToDefault()
{
    if(validatorType() == INT)
    {
        setText(QLocale::system().toString(static_cast<int>(m_range.def)));
    }
    else if(validatorType() == FLOAT)
    {
        setText(QLocale::system().toString(static_cast<double>(m_range.def), 'f', 2));
    }
}
//---------------------------------------------------
void CLineEdit::setRange(CLineEdit::RangeType &range)
{
    m_range = range;
}
//------------------------------------
void CLineEdit::setRangeMin(float min)
{
    m_range.min = min;
}
//------------------------------------
void CLineEdit::setRangeMax(float max)
{
    m_range.max = max;
}
//----------------------------------------
void CLineEdit::setRangeDefault(float def)
{
    m_range.def = def;
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
        m_is_edit = true;
        emit editValue(); // сигнал редактирования значения
    }
    else
    {
        m_is_edit = false;
        t_palette.setColor(QPalette::Text, QColor(Qt::red));
    }

    setPalette(t_palette);
}
