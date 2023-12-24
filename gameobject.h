#include <QStandardItem>
#include <QUuid>

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject
{
public:
    GameObject();
    GameObject(const QString &name, int x = 0, int y = 0, GameObject* parent = nullptr);
    ~GameObject();

    QString guid() const;
    QString name() const;
    int x() const;
    int y() const;
    bool visible();
    GameObject* parent() const;
    QIcon icon();

    void setParent(GameObject* parent);
    void setName(QString name);
    void setVisible(bool visible);
    void setIcon(const QIcon &icon);
    void addChild(GameObject* child);
    GameObject* findChild(const QString& name) const;
    QList<GameObject*> children() const;
public slots:
    void clicked();

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
