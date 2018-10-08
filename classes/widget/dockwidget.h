#ifndef DOCKWIDGET_H
    #define DOCKWIDGET_H
    //----------------
    #include <QWidget>
    #include <QDragEnterEvent>
    #include <QDropEvent>
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
    class CDockWidget: public QWidget
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

        protected:
            void dragEnterEvent(QDragEnterEvent* event);
            void dropEvent(QDropEvent* event);

        private:
            Ui::CDockWidget*    ui;
            CDockPanelItemCtrl* m_controlItem;
    };
#endif // DOCKWIDGET_H
