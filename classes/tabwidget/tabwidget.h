#ifndef TABWIDGET_H
    #define TABWIDGET_H
    //-------------------
    #include <QTabWidget>
    #include <QEvent>
    #include <QMouseEvent>
    #include <QTabBar>
    #include <QDropEvent>
    #include <QDragEnterEvent>
    #include <QDragMoveEvent>
    #include <QDebug>
    #include "containerwidget.h"
    #include "terminalwindow.h"
    //---------------------------------
    class CTabWidget: public QTabWidget
    {
        Q_OBJECT

        public:
            CTabWidget(QWidget* parent = nullptr);
            void addContainer(CContainerWidget* container);
            void setSuperParent(QWidget* parent);

        private slots:
            void tabDoubleClicked(int index);
            void tabRemove(const QString &title);

        signals:
            void removeContainer(const QString&);

        protected:
            void dragEnterEvent(QDragEnterEvent* event);
            void dragMoveEvent(QDragMoveEvent* event);
            void dragLeaveEvent(QDragLeaveEvent *event);
            void dropEvent(QDropEvent* event);

        private:
            QWidget* m_superParent;
    };
#endif // TABWIDGET_H
