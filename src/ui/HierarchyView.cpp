#include "hiersim/HierarchyView.h"
#include "hiersim/Organization.h"
#include <QVBoxLayout>
#include <QLabel>

namespace hiersim {
HierarchyView::HierarchyView(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Organization Hierarchy"), this));
}
void HierarchyView::setOrganization(const Organization* org) {}
void HierarchyView::update() {}
}
