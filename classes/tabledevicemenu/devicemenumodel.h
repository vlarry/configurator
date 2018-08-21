#ifndef DEVICEMENUMODEL_H
    #define DEVICEMENUMODEL_H
    //---------------------
    #include <QTableWidget>
    #include <QTableWidgetItem>
    #include <QHeaderView>
    #include <QHBoxLayout>
    #include <QShowEvent>
    #include <QPaintEvent>
    #include <QPainter>
    #include <QLabel>
    #include <QComboBox>
    #include "clineedit.h"
    //-----------------------------------------------
    class CDeviceMenuTableWidget: public QTableWidget
    {
        public:
            enum RowType
            {
                HEADER,
                ITEM
            };
            //--------------
            struct measure_t
            {
                float   min;
                float   max;
                QString unit;
            };
            //------------
            struct item_t;
            typedef QVector<item_t> item_list_t;
            //-----------
            struct item_t
            {
                QString     key;
                int         address;
                QString     obj_name;
                measure_t   unit;
                QString     type;
                QString     name;
                int         row;
                item_list_t subitems;
            };
            //------------
            struct group_t
            {
                QString     name;
                item_list_t items;
            };

        public:
            CDeviceMenuTableWidget(QWidget* parent = nullptr);
            void setColumns(const QStringList& columns);
            void addGroup(group_t& group);

        protected:
            void showEvent(QShowEvent* event);

        private slots:
            void rowClicked(QTableWidgetItem* item);

        private:
            QMap<int, int> m_group_rows;
    };
#endif // DEVICEMENUMODEL_H
