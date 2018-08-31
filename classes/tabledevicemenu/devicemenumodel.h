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
    #include <QToolButton>
    #include <QPushButton>
    #include <QLinearGradient>
    #include "clineedit.h"
    //-----------------------------------------------
    class CDeviceMenuTableWidget: public QTableWidget
    {
        public:
            enum RowType
            {
                HEADER,
                SUBHEADER,
                ITEM
            };
            //-------------
            enum GroupState
            {
                CLOSE,
                OPEN
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
            struct group_t;
            typedef QVector<item_t> item_list_t;
            typedef QVector<group_t> group_list_t;
            //-----------
            struct item_t
            {
                QString     key;
                int         address;
                measure_t   unit;
                QString     type;
                QString     name;
                int         row;
                item_list_t subitems;
            };
            //------------
            struct group_t
            {
                QString      name;
                item_list_t  items;
                group_list_t subgroup;
            };

        public:
            CDeviceMenuTableWidget(QWidget* parent = nullptr);
            void setColumns(const QStringList& columns);
            void addGroup(const group_t& group);

        protected:
            void showEvent(QShowEvent* event);

        private slots:
            void rowClicked(QTableWidgetItem* item_cur);

        private:
            int  rowCountSubgroup(const group_list_t& list);
            void insertHeader(int row, const QString& name, RowType type = HEADER);
            void insertItem(int row, const item_t& item, int index = 0);
            void insertSubgroup(int row, const group_list_t& list, int index);

        private:
            QMap<int, int> m_group_rows;
    };
#endif // DEVICEMENUMODEL_H
