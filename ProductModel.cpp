#include <QPixmap>
#include "ProductModel.h"

ProductModel::ProductModel(QObject *parent)
{

}

ProductModel::~ProductModel()
{

}

int ProductModel::rowCount(const QModelIndex &parent) const
{
    return m_items_.size();
}

QVariant ProductModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= m_items_.size())
    {
        return QVariant{};
    }
    auto *item = m_items_.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
        return QString(item->name + " " + item->tags.join(' '));
    case ItemRole:
        return QVariant::fromValue(item);
    case ItemImageRole:
    {
        QPixmap pixmap(":/Resources/curriculum.png");
        return pixmap;
    }
    case ItemTagsRole:
        return item->tags;
    default:
        break;
    }

    return QVariant{};
}

void ProductModel::appendItem(ProductItem *item)
{
    beginInsertRows(QModelIndex(), m_items_.size(), m_items_.size() + 1);
    m_items_.append(item);
    endInsertRows();
}

void ProductModel::appendItems(const QList<ProductItem *> &items)
{
    beginInsertRows(QModelIndex(), m_items_.size(), m_items_.size() + items.size());
    m_items_.append(items);
    endInsertRows();
}

const QSize ProductModel::defaultImageSize()
{
    return QSize(200, 150);
}
