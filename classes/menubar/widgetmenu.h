#ifndef WIDGETMENU_H
    #define WIDGETMENU_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class CWidgetMenu;
    }
    //-------------------------------
    class CWidgetMenu: public QWidget
    {
        Q_OBJECT

        public:
            explicit CWidgetMenu(QWidget* parent = nullptr);
            ~CWidgetMenu();

        private:
            Ui::CWidgetMenu* ui;
    };
#endif // WIDGETMENU_H
