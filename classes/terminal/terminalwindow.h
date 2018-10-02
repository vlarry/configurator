#ifndef TERMINALWINDOW_H
    #define TERMINALWINDOW_H
    //----------------
    #include <QWidget>
    #include <QPlainTextEdit>
    //------------------------------------------
    class CTerminalWindow: public QPlainTextEdit
    {
        Q_OBJECT

        public:
            CTerminalWindow(QWidget* parent = nullptr);
    };
#endif // TERMINALWINDOW_H
