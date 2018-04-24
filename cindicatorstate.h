#ifndef CINDICATORSTATE_H
    #define CINDICATORSTATE_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QTimer>
    #include <QDebug>
    #include "cindicatorcell.h"
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
            enum RelayType
            {
                RELAY_TYPE_ONE,
                RELAY_TYPE_TWO
            };
            //------------
            struct relay_t
            {
                RelayType type;
                QString   key;
                QString   description;
            };
            //------------------------------------
            typedef QVector<relay_t> relay_list_t;

        public:
            explicit CIndicatorState(QWidget* parent = nullptr);
            ~CIndicatorState();

            void setLists(const QStringList& led_list, relay_list_t& relay_list);

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
