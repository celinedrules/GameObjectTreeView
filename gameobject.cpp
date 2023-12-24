#include "gameobject.h"

GameObject::GameObject(){}

GameObject::GameObject(const QString &name, int x, int y, GameObject *parent)
    : name_(name), x_(x), y_(y), parent_(parent) {
    guid_ = QUuid::createUuid().toString();
    icon_ = QIcon(":/resources/icons/visible.png");
    visible_ = true;

    if(parent != nullptr) {
        parent->addChild(this);
    }
}

GameObject::~GameObject(){}

QString GameObject::guid() const { return guid_;}
QString GameObject::name() const { return name_; }
int GameObject::x() const { return x_; }
int GameObject::y() const { return y_; }
bool GameObject::visible() { return visible_; }
GameObject *GameObject::parent() const { return parent_; }
QIcon GameObject::icon() { return icon_; }
void GameObject::setParent(GameObject *parent) { parent_ = parent; }
void GameObject::setName(QString name) { name_ = name;}
void GameObject::setVisible(bool visible) { visible_ = visible; }
void GameObject::setIcon(const QIcon &icon) { icon_= icon; }

void GameObject::addChild(GameObject *child) { children_.append(child); }

GameObject *GameObject::findChild(const QString &name) const {
    for (GameObject* child : children_) {
        if (child->name() == name) {
            return child;
        }
    }
    return nullptr;
}

QList<GameObject *> GameObject::children() const { return children_; }
void GameObject::clicked(){}
