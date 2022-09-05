#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "ProductModel.h"
#include "ProductDelegate.h"

/**
 * @brief 产品列表
 */
class ProductWidget:public QWidget
{
public:
    explicit ProductWidget(QWidget* parent=nullptr);
    ~ProductWidget();

    ProductDelegate  *m_product_delegate_{};
    ProductModel *m_product_model_{};
};

#endif // PRODUCTWIDGET_H
