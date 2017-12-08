#include "cheadertable.h"
//-----------------------------------------------------------------------
CHeaderTable::CHeaderTable(Qt::Orientation orientation, QWidget* parent):
    QHeaderView(orientation, parent),
    m_orientation(orientation)
{
    this->setSectionResizeMode(QHeaderView::Fixed);
}
//-------------------------------------------------------------------------------------------
void CHeaderTable::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
    if(!rect.isValid())
        return;

    QString text = model()->headerData(logicalIndex, m_orientation).toString();

    if(m_orientation == Qt::Vertical)
    {
        QRect r(rect.x() + 1, rect.y() + 1, rect.width() - 1, rect.height() - 1);

        painter->save();
            painter->setPen(Qt::black);
            QHeaderView::paintSection(painter, r, logicalIndex);
        painter->restore();
    }
    else
    {
        painter->save();
        painter->translate(rect.left(), rect.bottom());
        painter->rotate(-90);

        QRect r(0, 0, rect.height() - 1, rect.width() - 1);

        painter->setPen(Qt::black);
        painter->fillRect(r, Qt::white);

        QRect t = r;

        t.setX(t.x() + 5);

        painter->drawText(t, Qt::AlignLeft | Qt::AlignVCenter, text);
        painter->restore();
    }
}
//-----------------------------------------------------------------
QSize CHeaderTable::sectionSizeFromContents(int logicalIndex) const
{
    QSize s = QHeaderView::sectionSizeFromContents(logicalIndex);

    if(m_orientation == Qt::Horizontal)
        return QSize(s.height(), std::max(100, s.width()));

    return s;
}
