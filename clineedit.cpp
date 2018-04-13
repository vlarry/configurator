#include "clineedit.h"
//------------------------------------
CLineEdit::CLineEdit(QWidget* parent):
    QLineEdit(parent)
{

}
//---------------------------------------------------
void CLineEdit::mouseReleaseEvent(QMouseEvent* event)
{
    QLineEdit::mouseReleaseEvent(event);

    selectAll();
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
