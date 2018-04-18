#include "cdockpanel.h"
//--------------------------------------
CDockPanel::CDockPanel(QWidget* parent):
    QFrame(parent),
    m_header(nullptr),
    m_footer(nullptr),
    m_widget(nullptr),
    m_header_visible(true),
    m_footer_visible(false)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->setMargin(2);

    m_header = new QLabel(tr("Заголовок"));
    m_footer = new QLabel(tr("Футер"));
    m_widget = new QWidget;

    setFrameStyle(QFrame::NoFrame);
    m_header->setAlignment(Qt::AlignCenter);
    m_footer->setAlignment(Qt::AlignCenter);
    m_header->setFrameStyle(QFrame::StyledPanel);
    m_footer->setFrameStyle(QFrame::StyledPanel);
    m_header->setFrameShadow(QFrame::Plain);
    m_footer->setFrameShadow(QFrame::Plain);
    m_header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_footer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_header->setFixedHeight(m_header->fontMetrics().height()*2);
    m_footer->setFixedHeight(m_footer->fontMetrics().height()*2);

    setHeaderVisible(m_header_visible);
    setFooterVisible(m_footer_visible);

    vlayout->addWidget(m_header);
    vlayout->addWidget(m_widget);
    vlayout->addWidget(m_footer);

    setLayout(vlayout);
}
//-----------------------
CDockPanel::~CDockPanel()
{
    delete m_header;
    delete m_footer;
    delete m_widget;

    m_header = nullptr;
    m_footer = nullptr;
    m_widget = nullptr;
}
//--------------------------------
QLabel* CDockPanel::header() const
{
    return m_header;
}
//--------------------------------
QLabel* CDockPanel::footer() const
{
    return m_footer;
}
//---------------------------------
QWidget* CDockPanel::widget() const
{
    return m_widget;
}
//------------------------------------
QString CDockPanel::headerText() const
{
    QString str = "";

    if(m_header)
        str = m_header->text();

    return str;
}
//------------------------------------
QString CDockPanel::footerText() const
{
    QString str = "";

    if(m_footer)
        str = m_footer->text();

    return str;
}
//----------------------------------------
void CDockPanel::setHeader(QLabel* header)
{
    if(m_header)
    {
        delete m_header;
        m_header = nullptr;
    }

    m_header = header;
}
//----------------------------------------
void CDockPanel::setFooter(QLabel* footer)
{
    if(m_footer)
    {
        delete m_footer;
        m_footer = nullptr;
    }

    m_footer = footer;
}
//-----------------------------------------
void CDockPanel::setWidget(QWidget* widget)
{
    if(m_widget)
    {
        delete m_widget;
        m_widget = nullptr;
    }

    m_widget = widget;

    QVBoxLayout* l = static_cast<QVBoxLayout*>(layout());

    if(l)
    {
        l->addWidget(m_header);
        l->addWidget(m_widget);
        l->addWidget(m_footer);

        resize(QSize(m_widget->sizeHint().width() + 50, m_widget->sizeHint().height()));
    }
}
//-------------------------------------------------
void CDockPanel::setHeaderText(const QString& text)
{
    if(m_header)
        m_header->setText(text);
}
//-------------------------------------------------
void CDockPanel::setFooterText(const QString& text)
{
    if(m_footer)
        m_footer->setText(text);
}
//---------------------------------------------
void CDockPanel::setHeaderVisible(bool visible)
{
    m_header_visible = visible;

    if(m_header)
        m_header->setVisible(m_header_visible);
}
//---------------------------------------------
void CDockPanel::setFooterVisible(bool visible)
{
    m_footer_visible = visible;

    if(m_footer)
        m_footer->setVisible(m_footer_visible);
}
