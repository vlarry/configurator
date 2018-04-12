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
