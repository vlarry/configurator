#ifndef CINDICATORSTATE_H
    #define CINDICATORSTATE_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QTimer>
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

            void setLists(const QStringList& led_list, const QStringList& relay_list);

        private slots:
            void changeState();

        signals:
            void closeWindowIndicator(int);

        protected:
            void closeEvent(QCloseEvent* event);

        private:
            Ui::CIndicatorState* ui;
            QTimer               m_timer;
            bool                 m_state;
    };
#endif // CINDICATORSTATE_H
