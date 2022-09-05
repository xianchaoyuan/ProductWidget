#include "ProductDelegate.h"

#include <qdrawutil.h>
#include <QDesktopServices>
#include <QEasingCurve>
#include <QHoverEvent>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmapCache>
#include <QTimer>
#include <QDebug>

constexpr int GridItemWidth = 230;
constexpr int GridItemHeight = 230;
constexpr int GridItemGap = 10;
constexpr int TagsSeparatorY = GridItemHeight - 60;


static QFont sizedFont(int size, const QWidget *widget)
{
    QFont f = widget->font();
    f.setPixelSize(size);
    return f;
}

ProductDelegate::ProductDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{
    lightColor = QColor(221, 220, 220);
    backgroundColor = Qt::white;
    foregroundColor1 = Qt::black;
    foregroundColor2 = Qt::gray;
}

ProductDelegate::~ProductDelegate()
{

}

void ProductDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    const ProductItem *item = index.data(ProductModel::ItemRole).value<ProductItem *>();
    if (!item)
        return;

    const QRect rc = option.rect;

    const int d = 10;
    const int x = rc.x() + d;
    const int y = rc.y() + d;
    const int w = rc.width() - 2 * d - GridItemGap;
    const int h = rc.height() - 2 * d;
    const bool hovered = option.state & QStyle::State_MouseOver;

    const int tagsBase = TagsSeparatorY + 10;
    const int shiftY   = TagsSeparatorY - 20;
    const int nameY    = TagsSeparatorY - 20;

    const QRect textRect = QRect(x, y + nameY, w, h);

    QTextOption wrapped;
    wrapped.setWrapMode(QTextOption::WordWrap);
    int offset = 0;
    float animationProgress = 0; // Linear increase from 0.0 to 1.0 during hover animation
    if (hovered)
    {
        if (index != m_previousIndex)
        {
            m_previousIndex = index;
            m_startTime.start();
            m_currentArea = rc;
            m_currentWidget = qobject_cast<QAbstractItemView *>(const_cast<QWidget *>(option.widget));
        }
        animationProgress = m_startTime.elapsed() / 200.0; // Duration 200 ms.
        static const QEasingCurve animationCurve(QEasingCurve::OutQuad);
        offset = animationCurve.valueForProgress(animationProgress) * shiftY;

        if (offset < shiftY)
        {
            QTimer::singleShot(1, this, &ProductDelegate::goon);
        }
        else if (offset > shiftY)
        {
            offset = shiftY;
        }
    }
    else
    {
        m_previousIndex = QModelIndex();
    }

    const QFontMetrics fm(option.widget->font());
    const QRect shiftedTextRect = textRect.adjusted(0, -offset, 0, -offset);

    // The pixmap.
    if (offset < shiftY)
    {
        QPixmap pm = index.data(ProductModel::ItemImageRole).value<QPixmap>();
        QRect inner(x + 11, y, ProductModel::defaultImageSize().width(),
                    ProductModel::defaultImageSize().height());
        QRect pixmapRect = inner;
        if (!pm.isNull())
        {
            painter->setPen(foregroundColor2);

            QPoint pixmapPos = pixmapRect.center();
            pixmapPos.rx() -= pm.width() / pm.devicePixelRatio() / 2;
            pixmapPos.ry() -= pm.height() / pm.devicePixelRatio() / 2;
            painter->drawPixmap(pixmapPos, pm);
        }
        else
        {
            // The description text as fallback.
            painter->setPen(foregroundColor2);
            painter->setFont(sizedFont(11, option.widget));
            painter->drawText(pixmapRect.adjusted(6, 10, -6, -10), item->description, wrapped);
        }
        qDrawPlainRect(painter, pixmapRect.translated(-1, -1), foregroundColor1);
    }

    // The description background rect
    if (offset)
    {
        QRect backgroundRect = shiftedTextRect.adjusted(0, -16, 0, 0);
        painter->fillRect(backgroundRect, backgroundColor);
    }

    // The title of the example.
    painter->setPen(foregroundColor1);
    painter->setFont(sizedFont(13, option.widget));
    QRectF nameRect;
    if (offset)
    {
        nameRect = painter->boundingRect(shiftedTextRect, item->name, wrapped);
        painter->drawText(nameRect, item->name, wrapped);
    }
    else
    {
        nameRect = QRect(x, y + nameY, x + w, y + nameY + 20);
        QString elidedName = fm.elidedText(item->name, Qt::ElideRight, w - 20);
        painter->drawText(nameRect, elidedName);
    }

    // The separator line below the example title.
    if (offset)
    {
        int ll = nameRect.bottom() + 5;
        painter->setPen(lightColor);
        painter->setOpacity(animationProgress); // "fade in" separator line and description
        painter->drawLine(x, ll, x + w, ll);
    }

    // The description text.
    if (offset)
    {
        int dd = nameRect.height() + 10;
        QRect descRect = shiftedTextRect.adjusted(0, dd, 0, dd);
        painter->setPen(foregroundColor2);
        painter->setFont(sizedFont(11, option.widget));
        painter->drawText(descRect, item->description, wrapped);
        painter->setOpacity(1);
    }

    // Separator line between text and 'Tags:' section
    painter->setPen(lightColor);
    painter->drawLine(x, y + TagsSeparatorY,
                      x + w, y + TagsSeparatorY);

    // The 'Tags:' section
    const int tagsHeight = h - tagsBase;
    const QFont tagsFont = sizedFont(10, option.widget);
    const QFontMetrics tagsFontMetrics(tagsFont);
    QRect tagsLabelRect = QRect(x, y + tagsBase, 30, tagsHeight - 2);
    painter->setPen(foregroundColor2);
    painter->setFont(tagsFont);
    painter->drawText(tagsLabelRect, tr("Tags:"));

    painter->setPen(Qt::green);
    m_currentTagRects.clear();
    int xx = 0;
    int yy = y + tagsBase;
    for (const QString &tag : item->tags)
    {
        const int ww = tagsFontMetrics.horizontalAdvance(tag) + 5;
        if (xx + ww > w - 30)
        {
            yy += 15;
            xx = 0;
        }
        const QRect tagRect(xx + x + 30, yy, ww, 15);
        painter->drawText(tagRect, tag);
        m_currentTagRects.append({ tag, tagRect });
        xx += ww;
    }

    // Box it when hovered.
    if (hovered)
        qDrawPlainRect(painter, rc, lightColor);
}

QSize ProductDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const ProductItem *item = index.data(ProductModel::ItemRole).value<ProductItem *>();

    // "empty" items (last row of a section)
    if (!item)
        return QStyledItemDelegate::sizeHint(option, index);

    return QSize(230 + 10,
                 230 + 10);
}

bool ProductDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        const ProductItem *item = index.data(ProductModel::ItemRole).value<ProductItem *>();
        if (!item)
            return false;

        auto mev = static_cast<QMouseEvent *>(event);
        if (mev->button() != Qt::LeftButton)
            return false;

        if (index.isValid())
        {
            clickAction(item);
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ProductDelegate::clickAction(const ProductItem *item)
{
    auto productItem = static_cast<const ProductItem *>(item);
    const QUrl url(QString("https://marketplace.qt.io/products/"));
    QDesktopServices::openUrl(url);
}

void ProductDelegate::goon()
{
    if (m_currentWidget)
    {
        m_currentWidget->viewport()->update(m_currentArea);
    }
}
