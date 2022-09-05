#pragma once

#include <QPointer>
#include <QElapsedTimer>
#include <QAbstractItemView>
#include <QStyledItemDelegate>
#include "ProductModel.h"


/**
 * @brief 产品委托
 */
class ProductDelegate : public QStyledItemDelegate
{
public:
    explicit ProductDelegate(QObject *parent = nullptr);
    ~ProductDelegate() override;

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

    void clickAction(const ProductItem *item);
    void goon();

private:
    QColor lightColor;
    QColor backgroundColor;
    QColor foregroundColor1;
    QColor foregroundColor2;

private:
    mutable QPersistentModelIndex m_previousIndex;
    mutable QElapsedTimer m_startTime;
    mutable QRect m_currentArea;
    mutable QPointer<QAbstractItemView> m_currentWidget;
    mutable QVector<QPair<QString, QRect>> m_currentTagRects;
};


