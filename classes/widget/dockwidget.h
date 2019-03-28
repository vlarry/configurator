#ifndef DOCKWIDGET_H
    #define DOCKWIDGET_H
    //---------------
    #include <QFrame>
    #include <QDragEnterEvent>
    #include <QDropEvent>
    #include <QDrag>
    #include <QToolButton>
    #include <QDebug>
    #include "cdockpanelitemctrl.h"
    #include "containerwidget.h"
    //----------
    namespace Ui
    {
        class CDockWidget;
    }
    //-------------------------------
    class CDockWidget: public QFrame
    {
        Q_OBJECT

        public:
            explicit CDockWidget(QWidget* parent = nullptr);
            ~CDockWidget();
            CDockPanelItemCtrl* control();
            CContainerWidget *container(int id);
            void hideContent();
            void showContent();
            bool isContentHidden() const;
            void setVisibleContent(bool state);
            void setControlItemDir(CDockPanelItemCtrl::DirType dir);

        public slots:
            void addContainer(CContainerWidget* wgt);
            void removeItem(int id);

        signals:
            void isContainerEmpty(bool = false);

        protected:
            void dragEnterEvent(QDragEnterEvent* event);
            void dragMoveEvent(QDragMoveEvent* event);
            void dragLeaveEvent(QDragLeaveEvent* event);
            void dropEvent(QDropEvent* event);

        private:
            Ui::CDockWidget*    ui;
            CDockPanelItemCtrl* m_controlItem;
            static int          m_idCount;
    };
#endif // DOCKWIDGET_H
