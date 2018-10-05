#include "containerwidget.h"
#include "ui_containerwidget.h"
//-------------------------------------------------------------------------------------------------------------------
CContainerWidget::CContainerWidget(const QString& title, QWidget* contentWidget, AnchorType anchor, QWidget* parent):
    QWidget(parent),
    ui(new Ui::CContainerWidget),
    m_anchor(anchor)
{
    ui->setupUi(this);
    ui->toolButtonHeader->hide();
    ui->labelHeader->setText(title);
    ui->labelHeader->setAlignment(Qt::AlignCenter);
    ui->labelHeader->setAutoFillBackground(true);

    QFont f(ui->labelHeader->font());
    f.setPointSize(12);
    f.setBold(true);
    ui->labelHeader->setFont(f);

    setWidget(contentWidget);
}
//-----------------------------------
CContainerWidget::~CContainerWidget()
{
    delete ui;
}
//-----------------------------------------------------------
CContainerWidget::AnchorType CContainerWidget::anchor() const
{
    return m_anchor;
}
//---------------------------------
QWidget* CContainerWidget::widget()
{
    return m_contentWidget;
}
//---------------------------------
void CContainerWidget::buttonHide()
{
    ui->toolButtonHeader->hide();
}
//---------------------------------
void CContainerWidget::buttonShow()
{
    ui->toolButtonHeader->show();
}
//--------------------------------------------
void CContainerWidget::setWidget(QWidget* wgt)
{
    m_contentWidget = wgt;

    QVBoxLayout* vlayout = new QVBoxLayout(ui->widgetContainer);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(m_contentWidget);
    ui->widgetContainer->setLayout(vlayout);
}
//-----------------------------------------------------------------------
void CContainerWidget::setHeaderBackground(const QColor& backgroundColor)
{
    QLinearGradient gradient(0, 0, 0, ui->labelHeader->height());
    gradient.setColorAt(0, backgroundColor);
    gradient.setColorAt(0.5, Qt::gray);
    gradient.setColorAt(1, backgroundColor);

    QPalette pal(ui->labelHeader->palette());
    pal.setBrush(QPalette::Window, QBrush(gradient));
    ui->labelHeader->setPalette(pal);
}
//-------------------------------------------------------------------
void CContainerWidget::setAnchor(CContainerWidget::AnchorType anchor)
{
    m_anchor = anchor;
}
