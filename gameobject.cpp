#include "gameobject.h"

GameObject::GameObject(){}

GameObject::GameObject(const QString &name, int x, int y, GameObject *parent)
    : name_(name), x_(x), y_(y), parent_(parent) {
    // Generate a unique GUID for the GameObject
    guid_ = QUuid::createUuid().toString();
    // Set the visibility icon of the GameObject
    visibileIcon_ = QIcon(":/resources/icons/visible.png");
    // Set the visibility status of the GameObject to true
    visible_ = true;

    // If a parent GameObject is provided, add this GameObject as a child of the parent
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
QIcon GameObject::getVisibleIcon() { return visibileIcon_; }
void GameObject::setParent(GameObject *parent) { parent_ = parent; }
void GameObject::setName(QString name) { name_ = name;}
void GameObject::setVisible(bool visible) { visible_ = visible; }
void GameObject::setVisibleIcon(const QIcon &icon) { visibileIcon_= icon; }

void GameObject::addChild(GameObject *child) { children_.append(child); }

GameObject *GameObject::findChild(const QString &name) const {
    // Loop through each child GameObject
    for (GameObject* child : children_) {
        // Check if the name of the child GameObject matches the provided name
        if (child->name() == name) {
            // If a match is found, return the child GameObject
            return child;
        }
    }

    // If no match is found, return nullptr
    return nullptr;
}

QList<GameObject *> GameObject::children() const { return children_; }

