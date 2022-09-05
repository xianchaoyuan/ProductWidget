#pragma once

#include <QAbstractListModel>

/**
 * @brief 产品项
 */
class ProductItem
{
public:
    virtual ~ProductItem() {}
    QString name;
    QString description;
    QString imageUrl;
    QStringList tags;
};
Q_DECLARE_METATYPE(ProductItem *)


/**
 * @brief 产品模型
 */
class ProductModel : public QAbstractListModel
{
public:
    enum ProductDataRole {
        ItemRole = Qt::UserRole,
        ItemImageRole,
        ItemTagsRole
    };

    explicit ProductModel(QObject *parent);
    ~ProductModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! 添加 item
    void appendItem(ProductItem *item);
    //! 添加 items
    void appendItems(const QList<ProductItem *> &items);

    //! 默认图片大小
    static const QSize defaultImageSize();

private:
    QList<ProductItem *> m_items_;
};

