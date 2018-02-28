#ifndef CJOURNALPROPERTYDELEGATE_H
    #define CJOURNALPROPERTYDELEGATE_H
    //----------------------
    #include <QItemDelegate>
    #include <QPainter>
    #include <QStyleOptionViewItem>
    //--------------------------------------------------
    class CJournalPropertyDelegate: public QItemDelegate
    {
        public:
            CJournalPropertyDelegate(QObject* parent = nullptr);
            void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    };
#endif // CJOURNALPROPERTYDELEGATE_H
