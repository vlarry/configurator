#include "cjournalpropertydelegate.h"
//------------------------------------------------------------------
CJournalPropertyDelegate::CJournalPropertyDelegate(QObject* parent):
    QItemDelegate(parent)
{

}
//-------------------------------------------------------------------------------------------------------------------------
void CJournalPropertyDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if(index.data().canConvert<QPair<QString, QString> >())
    {
        QPair<QString, QString> pair = qvariant_cast<QPair<QString, QString> >(index.data());

        if(pair.first == ";")
        {
            painter->fillRect(option.rect, Qt::gray);
            painter->setPen(Qt::white);
            painter->drawText(option.rect, Qt::AlignCenter, pair.second);
        }
        else
        {
            if(option.state & QStyle::State_Selected)
            {
                painter->fillRect(option.rect, option.palette.highlight());
                painter->setPen(Qt::white);
            }
            else
                painter->setPen(Qt::black);

            painter->drawText(option.rect, Qt::AlignVCenter|Qt::AlignLeft, pair.first);
            painter->drawText(option.rect, Qt::AlignVCenter|Qt::AlignRight, pair.second);
        }
    }
}
