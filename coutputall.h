#ifndef COUTPUTALL_H
    #define COUTPUTALL_H
    //----------------
    #include <QWidget>
    #include <QCloseEvent>
    #include <QShowEvent>
    #include <QTimer>
    #include <QDebug>
    #include "coutputallcell.h"
    //----------
    namespace Ui
    {
        class COutputAll;
    }
    //------------------------------
    class COutputAll: public QWidget
    {
        Q_OBJECT

        public:
            explicit COutputAll(const QString& title, QWidget* parent = nullptr);
            ~COutputAll();

            void createList(const QStringList& list);
            void setOutputStates(const QVector<quint16>& data);

        public slots:
            void outputStateReset();
            void stateUpdateData(bool state = false);
            void timeoutUpdate();
            void timeoutValueChanged(int new_value);

        protected:
            void closeEvent(QCloseEvent* event);
            void showEvent(QShowEvent* event);

        signals:
            void buttonUpdate();
            void closeWindow(bool = false);

        private:
            Ui::COutputAll* ui;
            QTimer*         m_timeout_update;
    };
#endif // COUTPUTALL_H
