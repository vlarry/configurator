#include "cindicatorstate.h"
#include "ui_indicatorstate.h"
//------------------------------------------------
CIndicatorState::CIndicatorState(QWidget* parent):
    QWidget(parent),
    ui(new Ui::CIndicatorState)
{
    ui->setupUi(this);

    setWindowTitle(tr("Состояния индикаторов"));
    setWindowFlag(Qt::Window);
}
//---------------------------------
CIndicatorState::~CIndicatorState()
{
    delete ui;
}
//--------------------------------------------------
void CIndicatorState::closeEvent(QCloseEvent* event)
{
    emit closeWindowIndicator(Qt::Unchecked);

    QWidget::closeEvent(event);
}
