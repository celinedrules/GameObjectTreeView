#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "GameObject.h"
#include "TreeItem.h"

#include <QStandardItemModel>


class TreeModel : public QStandardItemModel {
public:
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


//    void removeRow(int row) {
//        GameObject* gameObject = item(row)->data().value<GameObject*>();

//        // Check if the GameObject has already been deleted
//        if (gameObject) {
//            // If the GameObject has a parent, it's a child
//            if (gameObject->parent()) {
//                // Remove the GameObject from its parent's list of children
//                gameObject->parent()->children().removeOne(gameObject);

//                // Delete the associated data
//                delete gameObject;

//                // Set the associated data to nullptr to mark it as deleted
//                item(row)->setData(QVariant());

//                // Remove the row from the model
//                QStandardItemModel::removeRow(row);
//            } else {
//                // The GameObject is not a child, handle this case as needed
//                qDebug() << "The GameObject is not a child.";
//            }
//        }
//    }



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



private:
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
#endif // TREEMODEL_H
