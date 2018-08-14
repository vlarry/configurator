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
    //-----------------------------------------------
    class CDeviceMenuTableWidget: public QTableWidget
    {
        public:
            enum RowType
            {
                HEADER,
                ITEM
            };
            //-----------
            struct item_t
            {
                QString  name;
                QWidget* item;
            };
            //------------------------------------
            typedef QVector<item_t> column_list_t;
            typedef QVector<column_list_t> row_list_t;
            //------------
            struct group_t
            {
                QString    name;
                row_list_t rows;
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
