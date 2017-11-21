#ifndef CTERMINAL_H
    #define CTERMINAL_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QDebug>
    #include "cdataunittype.h"
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
        
        signals:
            void close(int);
        
        public slots:
            void appendData(QByteArray& data);
            
        private:
            void closeEvent(QCloseEvent* event);
    
        private:
            Ui::CTerminal* ui;
    };
#endif // CTERMINAL_H
