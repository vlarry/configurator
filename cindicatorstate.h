#ifndef CINDICATORSTATE_H
    #define CINDICATORSTATE_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QShowEvent>
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
                QString   name;
            };
            //------------------------------------
            typedef QVector<relay_t> relay_list_t;

        public:
            explicit CIndicatorState(QWidget* parent = nullptr);
            ~CIndicatorState();

            QStringList ledList();
            QStringList relayList();

            void setLists(const QStringList& led_list, const QStringList &relay_list);
            void setOutputStates(const QVector<quint16>& data);

        public slots:
            void stateUpdateData(bool state = false);
            void timeoutUpdate();
            void timeoutValueChanged(int new_value);

        signals:
            void buttonUpdate();
            void closeWindow(bool = false);

        protected:
            void closeEvent(QCloseEvent* event);
            void showEvent(QShowEvent* event);

        private:
            void resetStates();

        private:
            Ui::CIndicatorState* ui;
            QTimer*              m_timeout_update;
    };
#endif // CINDICATORSTATE_H
