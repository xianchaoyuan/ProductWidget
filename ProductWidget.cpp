#include "ProductWidget.h"

#include <QHBoxLayout>
#include <QListView>
#include <QScrollArea>

ProductWidget::ProductWidget(QWidget* parent)
    :QWidget(parent)
{
    auto listView = new QListView(this);
//    listView->setWrapping(true);
//    listView->setFlow(QListView::LeftToRight);		//从左到右布局
    listView->setViewMode(QListView::IconMode);
    listView->setResizeMode(QListView::Adjust);

    m_product_model_ = new ProductModel(this);

    QList<ProductItem*> listItems;
    for(int i=0; i<10; i++)
    {
        auto *product = new ProductItem();
        product->name = "1234567890";
        product->description = "qwertyuiopasdfghjkl";
        product->imageUrl = "123456789";
        product->tags = QStringList{"qwertyuiop", "asdfghjk"};

        listItems.append(product);
    }
    m_product_model_->appendItems(listItems);
    m_product_delegate_ = new ProductDelegate();

    listView->setModel(m_product_model_);
    listView->setItemDelegate(m_product_delegate_);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(listView);
}

ProductWidget::~ProductWidget()
{

}
