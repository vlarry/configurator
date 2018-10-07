#ifndef DOCKWIDGET_H
    #define DOCKWIDGET_H
    //----------------
    #include <QWidget>
    #include <QDragEnterEvent>
    #include <QDropEvent>
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
            enum class WidgetType
            {
                WidgetVariable
            };

        public:
            explicit CDockWidget(QWidget* parent = nullptr);
            ~CDockWidget();
            CDockPanelItemCtrl* control();
            CContainerWidget *container(int id);
            void hideContent();
            void showContent();
            bool isContentHidden() const;
            void setVisibleContent(bool state);

        public slots:
            void addContainer(CContainerWidget* wgt);
            void removeItem(int id);

        protected:
            void dragEnterEvent(QDragEnterEvent* event);
            void dropEvent(QDropEvent* event);

        private:
            Ui::CDockWidget* ui;
    };
#endif // DOCKWIDGET_H
