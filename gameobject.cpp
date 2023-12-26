#include "gameobject.h"

GameObject::GameObject(){}

GameObject::GameObject(const QString &name, int x, int y, GameObject *parent)
    : name_(name), x_(x), y_(y), parent_(parent) {
    guid_ = QUuid::createUuid().toString();                                         // Generate a unique id and store it as a string
    visibileIcon_ = QIcon(":/resources/icons/visible.png");                         // Set the default visibily icon to visible
    visible_ = true;                                                                // Set the default visibility to visible

    if(parent != nullptr) {                                                         // Check if this GameObject has a parent specified
        parent->addChild(this);                                                     // and if so make it a child of the parent
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
    for (GameObject* child : children_) {                                           // Loop through all the children for the GameObject
        if (child->name() == name) {                                                // and check if the name matches the name we are looking for.
            return child;                                                           // If so, return that GameObject
        }
    }

    return nullptr;                                                                 // No child GameObject was found with the specified name
}

QList<GameObject *> GameObject::children() const { return children_; }

