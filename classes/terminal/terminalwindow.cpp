#include "terminalwindow.h"
//------------------------------------------------
CTerminalWindow::CTerminalWindow(QWidget* parent):
    QPlainTextEdit(parent)
{
    QPalette tpalette = palette();

    tpalette.setColor(QPalette::Base, QColor(Qt::black));
    tpalette.setColor(QPalette::Text, QColor(Qt::green));

    setPalette(tpalette);
    setReadOnly(true);
}
