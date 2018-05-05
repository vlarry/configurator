#ifndef COUTPUTALL_H
    #define COUTPUTALL_H
    //----------------
    #include <QWidget>
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

        protected:
            void closeEvent(QCloseEvent* event);

        signals:
            void buttonRead();
            void closeWindow(bool = false);

        private:
            Ui::COutputAll* ui;
    };
#endif // COUTPUTALL_H
