#pragma once
#include <QWidget>
namespace hiersim {
class Organization;
class HierarchyView : public QWidget {
    Q_OBJECT
public:
    explicit HierarchyView(QWidget* parent = nullptr);
    void setOrganization(const Organization* org);
    void update();
};
}
