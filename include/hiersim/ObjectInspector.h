#pragma once
#include <QWidget>
#include <QTreeWidget>
namespace hiersim {
class ObjectInspector : public QWidget {
    Q_OBJECT
public:
    explicit ObjectInspector(QWidget* parent = nullptr);
    void inspect(void* obj, const std::string& type);
private:
    QTreeWidget* tree_;
};
}
