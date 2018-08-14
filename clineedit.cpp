#include "clineedit.h"
//------------------------------------
CLineEdit::CLineEdit(QWidget* parent):
    QLineEdit(parent),
    m_focus(false)
{
    setText(QLocale::system().toString(0.0f, 'f', 6));
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
