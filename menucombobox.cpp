#include "menucombobox.h"
//--------------------------------------------
CMenuComboBox::CMenuComboBox(QWidget *parent):
    QComboBox(parent),
    m_is_edit(false)
{
    connect(this, static_cast<void (CMenuComboBox::*)(int)>(&CMenuComboBox::currentIndexChanged), this, &CMenuComboBox::isCurrentIndexChanged);
}
//--------------------------------
bool CMenuComboBox::isEdit() const
{
    return m_is_edit;
}
//-------------------------------
void CMenuComboBox::resetIsEdit()
{
    m_is_edit = false;
}
//--------------------------------------------
void CMenuComboBox::isCurrentIndexChanged(int)
{
    m_is_edit = true;
    emit editValue();
}
//------------------------------------------
void CMenuComboBox::wheelEvent(QWheelEvent*)
{
    return;
}
