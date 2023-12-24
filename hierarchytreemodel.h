#ifndef HIERARCHYTREEMODEL_H
#define HIERARCHYTREEMODEL_H

#include "gameobject.h"

#include <QIODevice>
#include <QMimeData>
#include <QStandardItemModel>


class HierarchyTreeModel : public QStandardItemModel {
    Q_OBJECT
public:
    HierarchyTreeModel(QList<GameObject*>& gameObjects, QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void appendRow(QStandardItem* item);
    void appendRow(QList<QStandardItem*> items);
    void removeRow(QString guid);
    QModelIndex indexFromItem(const GameObject* gameObject, const QModelIndex& parent) const;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

signals:
    void gameObjectMoved();

private:
    QList<GameObject*>& gameObjects;

    QStandardItem* findItem(GameObject* gameObject, QStandardItem* parent = nullptr);
};
#endif // HIERARCHYTREEMODEL_H
