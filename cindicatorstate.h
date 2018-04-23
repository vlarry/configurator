#ifndef CINDICATORSTATE_H
    #define CINDICATORSTATE_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    //----------
    namespace Ui
    {
        class CIndicatorState;
    }
    //-----------------------------------
    class CIndicatorState: public QWidget
    {
        Q_OBJECT

        public:
            explicit CIndicatorState(QWidget* parent = nullptr);
            ~CIndicatorState();

        signals:
            void closeWindowIndicator(int);

        protected:
            void closeEvent(QCloseEvent* event);

        private:
            Ui::CIndicatorState* ui;
    };
#endif // CINDICATORSTATE_H
