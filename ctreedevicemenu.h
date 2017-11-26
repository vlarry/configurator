#ifndef CTREEDEVICEMENU_H
    #define CTREEDEVICEMENU_H
    //--------------------
    #include <QTreeWidget>
    #include <QTreeWidgetItem>
    #include <QWidget>
    //---------------------------------------
    class CTreeDeviceMenu: public QTreeWidget
    {
        public:
            CTreeDeviceMenu(QWidget* parent = nullptr);
    };
#endif // CTREEDEVICEMENU_H
