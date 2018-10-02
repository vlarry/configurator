#ifndef CTERMINAL_H
    #define CTERMINAL_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QMouseEvent>
    #include <QMimeData>
    #include <QDrag>
    #include <QIntValidator>
    #include <QDebug>
    #include "terminalwindow.h"
    //----------
    namespace Ui
    {
        class CTerminal;
    }
    //-----------------------------
    class CTerminal: public QWidget
    {
        Q_OBJECT
    
        public:
            explicit CTerminal(QWidget* parent = nullptr);
            ~CTerminal();

            void show();
        
        signals:
            void closeTerminal(int);
            void sendDeviceCommand(int);
        
        public slots:
            void appendData(QByteArray& data, bool isRequest = true);

        private slots:
            void convertDeviceCommand();

        private:
            void closeEvent(QCloseEvent* event);
            bool eventFilter(QObject* watched, QEvent* event);
    
        private:
            Ui::CTerminal* ui;
    };
#endif // CTERMINAL_H
