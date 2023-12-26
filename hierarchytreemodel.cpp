#include "hierarchytreemodel.h"


HierarchyTreeModel::HierarchyTreeModel(QList<GameObject *> &gameObjects, QObject *parent)
    : QStandardItemModel(parent), gameObjects(gameObjects) {}

Qt::ItemFlags HierarchyTreeModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

    // If it's the icon column
    if (index.column() == 1) {
        // Remove the editable flag
        return defaultFlags & ~Qt::ItemIsEditable;
    }

    // Return the default flags for other columns
    return defaultFlags;
}

void HierarchyTreeModel::appendRow(QStandardItem *item) {
    // Get the GameObject from the item
    GameObject* data = item->data().value<GameObject*>();

    // Check if the GameObject has a parent
    if (data->parent()) {
        // Find the parent item
        QStandardItem* parentItem = findItem(data->parent());


        if (parentItem) {
            // Append the item to the parent item
            parentItem->appendRow(item);
        }
    } else {
        // If the GameObject does not have a parent, append the item to the model
        QStandardItemModel::appendRow(item);
    }
}

void HierarchyTreeModel::appendRow(QList<QStandardItem *> items) {
    // Append the list of items to the model
    QStandardItemModel::appendRow(items);
}

void HierarchyTreeModel::removeRow(QString guid) {
    // Traverse the model to find the item with the matching QUuid
    for(int i = 0; i < rowCount(); ++i) {
        // Get the item at the current index
        QStandardItem* item = this->item(i);
        // Get the GameObject from the item
        GameObject* gameObject = item->data().value<GameObject*>();

        // Check if the GameObject exists and its GUID matches the input GUID
        if (gameObject && gameObject->guid() == guid) {
            // Check yf the GameObject has a parent
            if (gameObject->parent()) {
                // Remove it from the parent's list of children
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

QModelIndex HierarchyTreeModel::indexFromItem(const GameObject *gameObject, const QModelIndex &parent) const
{
    // Traverse the rows of the model
    for (int row = 0; row < rowCount(parent); ++row) {
        // Get the model index at the current row
        QModelIndex index = this->index(row, 0, parent);

        // Check if the GameObject of the item at the current index matches the input GameObject
        if (itemFromIndex(index)->data().value<GameObject*>() == gameObject) {
            // Return the current index
            return index;
        }

        // Recursively check the children of the current item
        QModelIndex childIndex = indexFromItem(gameObject, index);

        // If the child index is valid
        if (childIndex.isValid()) {
            // Return the child index
            return childIndex;
        }
    }

    // Return an invalid QModelIndex if the GameObject was not found
    return QModelIndex();
}

QStringList HierarchyTreeModel::mimeTypes() const {
    // Return a QStringList with the supported MIME type
    return QStringList() << "application/vnd.treeviewdragdrop.list";
}

QMimeData *HierarchyTreeModel::mimeData(const QModelIndexList &indexes) const {
    // Create a new QMimeData object
    QMimeData *mimeData = new QMimeData();
    // Create a QByteArray to hold the encoded data
    QByteArray encodedData;
    // Create a QDataStream to write to the QByteArray
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    // Loop through each model index in the list of indexes
    for (const QModelIndex &index : indexes) {
        // Check if the model index is valid
        if (index.isValid()) {
            // Get the display text of the data at the model index
            QString text = data(index, Qt::DisplayRole).toString();
            // Write the text to the stream
            stream << text;
        }
    }
    // Set the data of the QMimeData object with the encoded data
    mimeData->setData("application/vnd.treeviewdragdrop.list", encodedData);

    // Return the QMimeData object
    return mimeData;
}

bool HierarchyTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) {
    // If the action is IgnoreAction, return true
    if (action == Qt::IgnoreAction)
        return true;

    // If the data does not have the correct format, return false
    if (!data->hasFormat("application/vnd.treeviewdragdrop.list"))
        return false;

     // If the column is greater than 0, return false
    if (column > 0)
        return false;

    // Get the data from the mimeData
    QByteArray encodedData = data->data("application/vnd.treeviewdragdrop.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString gameObjectName;
    // Read the name of the GameObject from the stream
    stream >> gameObjectName;

    // Find the GameObject being moved
    GameObject* movedGameObject = nullptr;

    // Traverse the list of GameObjects
    for (GameObject* gameObject : gameObjects) {
        // Check if the name of the GameObject matches the name from the stream
        if (gameObject->name() == gameObjectName) {
            // Set the movedGameObject to the current GameObject
            movedGameObject = gameObject;
            break;
        }
    }

    // Check if the movedGameObject exists
    if (movedGameObject) {
        // If the GameObject is dropped onto another GameObject, set the target GameObject as the parent
        if (parent.isValid()) {
            // Get the parent item from the parent index
            QStandardItem* parentItem = itemFromIndex(parent);

            if(parentItem) {
                // Get the new parent GameObject from the parent item
                GameObject* newParent = parentItem->data().value<GameObject*>();
                // Set the parent of the moved GameObject to the new parent
                movedGameObject->setParent(newParent);
            }
        } else {
            // If it's not dropped onto another GameObject, set the parent to null
            movedGameObject->setParent(nullptr);
        }

        // Emit the gameObjectMoved signal
        emit gameObjectMoved();
    }

    // Return true to indicate that the drop was handled
    return true;
}

QStandardItem *HierarchyTreeModel::findItem(GameObject *gameObject, QStandardItem *parent) {
     // Get the row count of the parent item if it exists, otherwise get the row count of the model
    int rowCount = (parent ? parent->rowCount() : this->rowCount());

    // Traverse the rows
    for (int i = 0; i < rowCount; ++i) {
        // Get the item at the current index
        QStandardItem* item = (parent ? parent->child(i) : this->item(i));

        // Check if the GameObject of the item at the current index matches the input GameObject
        if (item->data().value<GameObject*>() == gameObject) {
            // Return the item
            return item;
        }

        // Recursively check the children of the current item
        QStandardItem* childItem = findItem(gameObject, item);

        // Check if the child item exists
        if (childItem) {
            // Return the child item
            return childItem;
        }
    }

    // Return nullptr if the GameObject was not found
    return nullptr;
}
