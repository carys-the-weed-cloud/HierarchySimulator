#include "hiersim/ObjectInspector.h"
#include <QVBoxLayout>

namespace hiersim {
ObjectInspector::ObjectInspector(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    tree_ = new QTreeWidget(this);
    tree_->setHeaderLabels({"Property", "Value"});
    layout->addWidget(tree_);
}
void ObjectInspector::inspect(void* obj, const std::string& type) {
    tree_->clear();
    auto* root = new QTreeWidgetItem(tree_);
    root->setText(0, QString::fromStdString(type));
    root->setText(1, "Object");
}
}
