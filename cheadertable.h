#ifndef CHEADERTABLE_H
    #define CHEADERTABLE_H
    //--------------------
    #include <QHeaderView>
    #include <QPainter>
    #include <QFont>
    #include <QDebug>
    //------------------------------------
    class CHeaderTable: public QHeaderView
    {
        public:
            CHeaderTable(Qt::Orientation orientation, QWidget* parent = nullptr);

        private:
            void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const;
            QSize sectionSizeFromContents(int logicalIndex) const;

        private:
            Qt::Orientation m_orientation;
    };
#endif // CHEADERTABLE_H
