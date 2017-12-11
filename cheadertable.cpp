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

    QFont font = painter->font();

    font.setBold(true);

    painter->setFont(font);

    QString text = model()->headerData(logicalIndex, m_orientation).toString();

    QRect r;

    painter->save();

        if(m_orientation == Qt::Vertical)
        {
            r = QRect(rect.x(), rect.y(), rect.width() - 1, rect.height() - 1);
        }
        else
        {
            painter->translate(rect.left(), rect.bottom());
            painter->rotate(-90);

            r = QRect(0, 0, rect.height() - 1, rect.width() - 1);
        }

        QRect t = r;
        t.setX(t.x() + 5);

        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        painter->fillRect(r, Qt::white);

        painter->drawText(t, Qt::AlignLeft | Qt::AlignVCenter, text);

    painter->restore();
}
//-----------------------------------------------------------------
QSize CHeaderTable::sectionSizeFromContents(int logicalIndex) const
{
    QString text = model()->headerData(logicalIndex, m_orientation).toString();

    QFont        font = this->font();
    QFontMetrics fm(font);

    int w = fm.width(text)*1.2 + 10;
    int h = fm.height()*1.2 + 5;

    if(m_orientation == Qt::Horizontal)
        return QSize(h, w);

    return QSize(w, h);
}
