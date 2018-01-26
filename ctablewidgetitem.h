#ifndef CTABLEWIDGETITEM_H
    #define CTABLEWIDGETITEM_H
    //-------------------------
    #include <QTableWidgetItem>
    #include <QDate>
    //---------------------------------------------
    class CTableWidgetItem: public QTableWidgetItem
    {
        public:
            CTableWidgetItem(const QString& text, int type = Type);
            bool operator < (const QTableWidgetItem& other) const;
    };
#endif // CTABLEWIDGETITEM_H
