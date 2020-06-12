#include "menucombobox.h"
//--------------------------------------------
CMenuComboBox::CMenuComboBox(QWidget *parent):
    QComboBox(parent),
    m_is_edit(false),
    m_def(0)
{
    connect(this, static_cast<void (CMenuComboBox::*)(int)>(&CMenuComboBox::currentIndexChanged), this, &CMenuComboBox::isCurrentIndexChanged);
}
//-------------------------------------
int CMenuComboBox::defaultIndex() const
{
    return m_def;
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
//----------------------------------
void CMenuComboBox::resetToDefault()
{
    if(m_def < count())
        setCurrentIndex(m_def);
}
//--------------------------------------------
void CMenuComboBox::setDefaultIndex(int index)
{
    m_def = index;
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
    // блокируем прокрутку в выпадающем списке
    return;
}
