#pragma once

#include <QStandardItem>
#include <QUuid>

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
public:
    GameObject(){}
    GameObject(const QString &name, int x = 0, int y = 0, GameObject* parent = nullptr)
        : name_(name), x_(x), y_(y), parent_(parent) {
        guid_ = QUuid::createUuid().toString();
        icon_ = QIcon(":/resources/icons/visible.png");
        visible_ = true;

        if(parent != nullptr) {
            parent->addChild(this);
        }
    }
    ~GameObject(){}

    QString guid() const { return guid_;}
    QString name() const { return name_; }
    int x() const { return x_; }
    int y() const { return y_; }
    bool visible() { return visible_; }
    GameObject* parent() const { return parent_; }
    QIcon icon() { return icon_; }

    void setParent(GameObject* parent) { parent_ = parent; }
    void setName(QString name) { name_ = name;}
    void setVisible(bool visible) { visible_ = visible; }
    void setIcon(const QIcon &icon) {icon_= icon;}
    void addChild(GameObject* child) {
        children_.append(child);
    }
    GameObject* findChild(const QString& name) const
    {
        for (GameObject* child : children_) {
            if (child->name() == name) {
                return child;
            }
        }
        return nullptr;
    }


    QList<GameObject*> children() const {
        return children_;
    }
public slots:
    void clicked(){}

private:
    QString guid_;
    QString name_;
    int x_;
    int y_;
    bool visible_;
    GameObject* parent_;
    QIcon icon_;
    QList<GameObject*> children_;
};

#endif // GAMEOBJECT_H
