#ifndef HIERARCHYTREEMODEL_H
#define HIERARCHYTREEMODEL_H

#include "gameobject.h"

#include <QIODevice>
#include <QMimeData>
#include <QStandardItemModel>


class HierarchyTreeModel : public QStandardItemModel {
    Q_OBJECT
public:
    HierarchyTreeModel(QList<GameObject*>& gameObjects, QObject *parent = nullptr)
        : QStandardItemModel(parent), gameObjects(gameObjects) {}

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

        if (index.column() == 1) { // If it's the icon column
            return defaultFlags & ~Qt::ItemIsEditable; // Remove the editable flag
        }

        return defaultFlags;
    }

    void appendRow(QStandardItem* item) {
        GameObject* data = item->data().value<GameObject*>();
        if (data->parent()) {
            QStandardItem* parentItem = findItem(data->parent());
            if (parentItem) {
                parentItem->appendRow(item);
            }
        } else {
            QStandardItemModel::appendRow(item);
        }
    }

    void appendRow(QList<QStandardItem*> items){
        QStandardItemModel::appendRow(items);
    }

    void removeRow(QString guid) {
        qDebug() << guid;

        // Traverse the model to find the item with the matching QUuid
        for(int i = 0; i < rowCount(); ++i) {
            QStandardItem* item = this->item(i);
            GameObject* gameObject = item->data().value<GameObject*>();
            if (gameObject && gameObject->guid() == guid) {
                // If the GameObject has a parent, remove it from the parent's list of children
                if (gameObject->parent()) {
                    gameObject->parent()->children().removeOne(gameObject);
                }

                // Delete the associated data
                delete gameObject;

                // Set the associated data to nullptr to mark it as deleted
                item->setData(QVariant());

                // Remove the row from the model
                QStandardItemModel::removeRow(i);
                break;
            }
        }
    }

    QModelIndex indexFromItem(const GameObject* gameObject, const QModelIndex& parent) const
    {
        for (int row = 0; row < rowCount(parent); ++row) {
            QModelIndex index = this->index(row, 0, parent);
            if (itemFromIndex(index)->data().value<GameObject*>() == gameObject) {
                return index;
            }
            // Recursively check the children of the current item
            QModelIndex childIndex = indexFromItem(gameObject, index);
            if (childIndex.isValid()) {
                return childIndex;
            }
        }
        // Return an invalid QModelIndex if the GameObject was not found
        return QModelIndex();
    }

    QStringList mimeTypes() const override {
        return QStringList() << "application/vnd.treeviewdragdrop.list";
    }

    QMimeData* mimeData(const QModelIndexList &indexes) const override {
        QMimeData *mimeData = new QMimeData();
        QByteArray encodedData;

        QDataStream stream(&encodedData, QIODevice::WriteOnly);

        for (const QModelIndex &index : indexes) {
            if (index.isValid()) {
                QString text = data(index, Qt::DisplayRole).toString();
                stream << text;
            }
        }

        mimeData->setData("application/vnd.treeviewdragdrop.list", encodedData);
        return mimeData;
    }

    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override {
        if (action == Qt::IgnoreAction)
            return true;

        if (!data->hasFormat("application/vnd.treeviewdragdrop.list"))
            return false;

        if (column > 0)
            return false;

        // Get the data from the mimeData
        QByteArray encodedData = data->data("application/vnd.treeviewdragdrop.list");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QString gameObjectName;
        stream >> gameObjectName;

        // Find the GameObject being moved
        GameObject* movedGameObject = nullptr;
        for (GameObject* gameObject : gameObjects) {
            if (gameObject->name() == gameObjectName) {
                movedGameObject = gameObject;
                qDebug() << "MOVED: " << movedGameObject->name();
                break;
            }
        }

        if (movedGameObject) {
            // If the GameObject is dropped onto another GameObject, set the target GameObject as the parent
            if (parent.isValid()) {
                QStandardItem* parentItem = itemFromIndex(parent);
                if(parentItem){
                    //qDebug() << "HERE";
                    GameObject* newParent = parentItem->data().value<GameObject*>();
                    //qDebug() << "NEW PARENT: " << newParent->name();
                    movedGameObject->setParent(newParent);
                }
            } else {
                // If it's not dropped onto another GameObject, set the parent to null
                movedGameObject->setParent(nullptr);
            }

            emit gameObjectMoved();
        }

        return true;
    }

signals:
    void gameObjectMoved();

private:
    QList<GameObject*>& gameObjects;

    QStandardItem* findItem(GameObject* gameObject, QStandardItem* parent = nullptr) {
        int rowCount = (parent ? parent->rowCount() : this->rowCount());
        for (int i = 0; i < rowCount; ++i) {
            QStandardItem* item = (parent ? parent->child(i) : this->item(i));
            if (item->data().value<GameObject*>() == gameObject) {
                return item;
            }
            QStandardItem* childItem = findItem(gameObject, item);
            if (childItem) {
                return childItem;
            }
        }
        return nullptr;
    }
};
#endif // HIERARCHYTREEMODEL_H
