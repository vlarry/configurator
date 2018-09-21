#ifndef WIDGET_H
    #define WIDGET_H
    //----------------
    #include <QWidget>
    #include <QGuiApplication>
    #include <QContextMenuEvent>
    #include <QKeyEvent>
    #include <QMenu>
    #include <QTimer>
    #include <QCloseEvent>
    #include <QShowEvent>
    #include <QDebug>
    #include "intervaldialog.h"
    //---------------------------
    class CWidget: public QWidget
    {
        Q_OBJECT

        public:
            explicit CWidget(QWidget* parent = nullptr);

        signals:
            void clear();
            void read();
            void interval();
            void pressKey(bool, bool, int);

        public slots:
            void setInterval();
            void repeatState(bool state);
            void repeatTimeout();

        protected:
            void contextMenuEvent(QContextMenuEvent* event);
            void keyPressEvent(QKeyEvent* event);
            void closeEvent(QCloseEvent* event);
            void showEvent(QShowEvent* event);

        private:
            QMenu*           m_context_menu;
            CIntervalDialog* m_interval;
            QTimer*          m_tim_interval;
            int              m_repeat_time;
    };
#endif // WIDGET_H
