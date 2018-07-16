#ifndef MENUBAR_H
    #define MENUBAR_H
    //----------------
    #include <QWidget>
    //----------
    namespace Ui
    {
        class CMenuBar;
    }
    //----------------------------
    class CMenuBar: public QWidget
    {
        Q_OBJECT

        public:
            explicit CMenuBar(QWidget* parent = nullptr);
            ~CMenuBar();

        signals:
            void closeWindow();
            void expandedWindow();
            void minimizeWindow();

        private:
            Ui::CMenuBar* ui;
    };
#endif // MENUBAR_H
