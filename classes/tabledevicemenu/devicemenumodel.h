#ifndef DEVICEMENUMODEL_H
    #define DEVICEMENUMODEL_H
    //----------------------------
    #include <QAbstractTableModel>
    //------------------------------------------------
    class CDeviceMenuModel: public QAbstractTableModel
    {
        public:
            CDeviceMenuModel(QAbstractTableModel* parent = nullptr);
    };
#endif // DEVICEMENUMODEL_H
